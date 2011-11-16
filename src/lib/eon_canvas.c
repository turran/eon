/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Canvas_Child_State
{
	double x;
	double y;
	double width;
	double height;
} Eon_Canvas_Child_State;

typedef struct _Eon_Canvas_Child
{
	Ender_Element *ender;
	Eon_Canvas_Child_State past;
	Eon_Canvas_Child_State current;
	Eina_Bool changed : 1;
} Eon_Canvas_Child;

typedef struct _Eon_Canvas_State
{
	Enesim_Color color;
} Eon_Canvas_State;

typedef struct _Eon_Canvas_Damage_Data
{
	double x;
	double y;
	Enesim_Renderer_Damage_Cb real_cb;
	void *real_data;
} Eon_Canvas_Damage_Data;

typedef struct _Eon_Canvas
{
	Eina_List *children;
	Eina_Bool changed : 1;
	Eon_Canvas_State old, curr;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Canvas;

static inline Eon_Canvas * _eon_canvas_get(Enesim_Renderer *r)
{
	Eon_Canvas *thiz;

	thiz = eon_layout_data_get(r);
	return thiz;
}

static Eina_Bool _canvas_damage_cb(Enesim_Renderer *child_r, Enesim_Rectangle *rect, Eina_Bool past, void *data)
{
	Eon_Canvas_Damage_Data *ddata = data;

	/* TODO the previous data should be added with the previous x, y */
	rect->x += ddata->x;
	rect->y += ddata->y;

	ddata->real_cb(child_r, rect, past, ddata->real_data);
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_canvas_free(Enesim_Renderer *r)
{
	Eon_Canvas *thiz;

	thiz = _eon_canvas_get(r);
	free(thiz);
}

static Eina_Bool _eon_canvas_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Enesim_Renderer *r;
	Eina_List *l;

	r = ender_element_renderer_get(e);
	eon_widget_property_clear(r, "child");

	thiz = _eon_canvas_get(r);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *child_r;
		Enesim_Renderer *child_rr;

		child_r = ender_element_renderer_get(ech->ender);
		child_rr = eon_element_renderer_get(ech->ender);

		eon_element_real_width_get(ech->ender, &ech->current.width);
		eon_element_real_height_get(ech->ender, &ech->current.height);

		eon_element_actual_size_set(child_r, ech->current.width, ech->current.height);
		eon_element_actual_position_set(child_r, ech->current.x, ech->current.y);
		/* now add the renderer associated with the widget into the theme */
		eon_widget_property_add(r, "child", child_rr, NULL);
		eon_element_setup(ech->ender, s, err);
	}

	return EINA_TRUE;
}

static void _eon_canvas_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Enesim_Renderer *r;
	Eina_List *l;

	r = ender_element_renderer_get(e);
	thiz = _eon_canvas_get(r);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		eon_element_cleanup(ech->ender, s);
		ech->past = ech->current;
		ech->changed = EINA_FALSE;
	}
	thiz->changed = EINA_FALSE;
}

static void _eon_canvas_damage(Ender_Element *e, Enesim_Renderer_Damage_Cb cb, void *data)
{
	Eon_Canvas *thiz;
	Enesim_Renderer *r;
	double x;
	double y;

	r = ender_element_renderer_get(e);
	thiz = _eon_canvas_get(r);

	eon_element_actual_position_get(r, &x, &y);
	/* if we have changed then just return our size */
	if (thiz->changed)
	{
		Enesim_Rectangle area;

		area.x = x;
		area.y = y;
		eon_element_actual_width_get(e, &area.w);
		eon_element_actual_height_get(e, &area.h);

		/* FIXME we should pass the previous and current */
		cb(r, &area, EINA_FALSE, data);
		return;
	}
	/* if not, return the children's */
	else
	{
		Eon_Canvas_Child *ech;
		Eina_List *l;

		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			/* in case the child changed the x, y (FIXME or the w, h) */
			if (ech->changed)
			{
				Enesim_Renderer *child_r;
				Enesim_Rectangle area;


				child_r = ender_element_renderer_get(ech->ender);
				/* the past */
				area.x = ech->past.x;
				area.y = ech->past.y;
				area.w = ech->past.width;
				area.h = ech->past.height;
				cb(child_r, &area, EINA_TRUE, data);
				/* the current */
				area.x = ech->past.x;
				area.y = ech->past.y;
				eon_element_real_width_get(ech->ender, &area.w);
				eon_element_real_height_get(ech->ender, &area.h);
				cb(child_r, &area, EINA_FALSE, data);
			}
			else
			{
				Eon_Canvas_Damage_Data ddata;
				ddata.x = x;
				ddata.y = y;
				ddata.real_cb = cb;
				ddata.real_data = data;

				eon_element_damages_get(ech->ender, _canvas_damage_cb, &ddata);
			}
		}
	}
}

Eina_Bool _eon_canvas_needs_setup(Ender_Element *e)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Enesim_Renderer *r;
	Eina_List *l;
	Eina_Bool ret = EINA_FALSE;


	r = ender_element_renderer_get(e);
	thiz = _eon_canvas_get(r);

	if (thiz->changed) return EINA_TRUE;


	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->changed) return EINA_TRUE;
		ret = eon_element_has_changed(ech->ender);
		if (ret) break;
	}
	return ret;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's layout interface                         *
 *----------------------------------------------------------------------------*/
/* FIXME same code as the stack we cannot share so easily given that the list
 * is part of the private data
 */
static Ender_Element * _eon_canvas_child_at(Ender_Element *e, double x, double y)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Ender_Element *child = NULL;
	Enesim_Renderer *r;
	Eina_List *l;

	r = ender_element_renderer_get(e);
	thiz = _eon_canvas_get(r);
	if (!thiz) return NULL;
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *rchild;
		double child_x, child_y;
		Eon_Size child_size;

		child_x = x - ech->current.x;
		if (child_x < 0) continue;
		child_y = y - ech->current.y;
		if (child_y < 0) continue;

		/* TODO still need the width and height */
		rchild = ender_element_renderer_get(ech->ender);
		if (eon_is_element(rchild))
		{
			eon_element_actual_size_get(rchild, &child_size);
		}
		else
		{
			Enesim_Rectangle bounds;
			enesim_renderer_boundings(rchild, &bounds);
			child_size.width = bounds.w;
			child_size.height = bounds.h;
		}
		if (child_x <= child_size.width && child_y <= child_size.height)
		{
			if (enesim_renderer_is_inside(rchild, child_x, child_y))
			{
				child = ech->ender;
				break;
			}
		}
	}
	//printf("returning %p\n", child);

	return child;
}

static void _eon_canvas_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;

	thiz = _eon_canvas_get(r);
	ech = calloc(1, sizeof(Eon_Canvas_Child));
	ech->ender = child;
	thiz->children = eina_list_append(thiz->children, ech);
	thiz->changed = EINA_TRUE;
}

static void _eon_canvas_child_remove(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l, *l_next;

	thiz = _eon_canvas_get(r);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, ech)
	{
		if (ech->ender == child)
		{
			thiz->children = eina_list_remove_list(thiz->children, l);
			break;
		}
	}
	thiz->changed = EINA_TRUE;
}

static Eon_Layout_Descriptor _descriptor = {
	.child_at = _eon_canvas_child_at,
	.child_add = _eon_canvas_child_add,
	.child_remove = _eon_canvas_child_remove,
	.free = _eon_canvas_free,
	.damage = _eon_canvas_damage,
	.cleanup = _eon_canvas_cleanup,
	.needs_setup = _eon_canvas_needs_setup,
	.setup = _eon_canvas_setup,
	.name = "canvas",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_canvas_new(void)
{
	Eon_Canvas *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Canvas));
	if (!thiz) return NULL;

	r = eon_layout_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_canvas_child_x_set(Enesim_Renderer *r, Ender_Element *child, double x)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(r);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->current.x = x;
			ech->changed = EINA_TRUE;
		}
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
static void _eon_canvas_child_y_set(Enesim_Renderer *r, Ender_Element *child, double y)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(r);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->current.y = y;
			ech->changed = EINA_TRUE;
		}
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_canvas_child_x_get NULL
#define _eon_canvas_child_y_get NULL
#include "eon_generated_canvas.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_canvas_new(void)
{
	return ender_element_new_with_namespace("canvas", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_x_set(Ender_Element *e, Ender_Element *child, double x)
{
	ender_element_value_set(child, "child_x", x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_y_set(Ender_Element *e, Ender_Element *child, double y)
{
	ender_element_value_set(child, "child_y", y, NULL);
}
