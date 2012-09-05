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
static Ender_Property *EON_CANVAS_CHILD_Y;
static Ender_Property *EON_CANVAS_CHILD_X;

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
	Eina_Bool needs_setup : 1;
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
	Eina_Bool needs_setup : 1;
	Eon_Canvas_State old, curr;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Canvas;

static inline Eon_Canvas * _eon_canvas_get(Eon_Element *ee)
{
	Eon_Canvas *thiz;

	thiz = eon_layout_data_get(ee);
	return thiz;
}

static Eina_Bool _canvas_child_is_inside(Eon_Canvas_Child *ech, double x, double y)
{
	Eon_Size child_size;
	Eon_Element *child_e;
	double child_x, child_y;

	child_x = x - ech->current.x;
	if (child_x < 0) return EINA_FALSE;
	child_y = y - ech->current.y;
	if (child_y < 0) return EINA_FALSE;

	/* TODO still need the width and height */
	child_e = ender_element_object_get(ech->ender);
	eon_element_actual_size_get(child_e, &child_size);
	if (child_x <= child_size.width && child_y <= child_size.height)
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

static Ender_Element * _canvas_child_up_at(Ender_Element *e, Ender_Element *rel, double x, double y)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Ender_Element *current;
	Ender_Element *ret = NULL;
	Eon_Element *ee;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_canvas_get(ee);
	if (!thiz) return NULL;
	current = NULL;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Ender_Element *child = NULL;

		if (!_canvas_child_is_inside(ech, x, y))
			continue;

		child = ech->ender;
		if ((current == rel) && (child))
		{
			ret = child;
			break;
		}
		current = child;
	}

	return ret;;
}

static Ender_Element * _canvas_child_down_at(Ender_Element *e, Ender_Element *rel, double x, double y)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Ender_Element *current;
	Ender_Element *ret = NULL;
	Eon_Element *ee;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_canvas_get(ee);
	if (!thiz) return NULL;
	current = NULL;

	EINA_LIST_REVERSE_FOREACH (thiz->children, l, ech)
	{
		Ender_Element *child = NULL;

		if (!_canvas_child_is_inside(ech, x, y))
			continue;

		child = ech->ender;

		if ((current == rel) && (child))
		{
			ret = child;
			break;
		}
		current = child;
	}

	return ret;;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_canvas_free(Eon_Element *ee)
{
	Eon_Canvas *thiz;

	thiz = _eon_canvas_get(ee);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's layout interface                         *
 *----------------------------------------------------------------------------*/
/* FIXME same code as the stack we cannot share so easily given that the list
 * is part of the private data
 */
static Ender_Element * _eon_canvas_child_at(Ender_Element *e, double x, double y)
{
	Ender_Element * child;

	child = _canvas_child_down_at(e, NULL, x, y);
	return child;
}

static Eina_Bool _eon_canvas_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;

	thiz = _eon_canvas_get(ee);
	ech = calloc(1, sizeof(Eon_Canvas_Child));
	ech->ender = child;
	thiz->children = eina_list_append(thiz->children, ech);
	thiz->needs_setup = EINA_TRUE;
	return EINA_TRUE;
}

static Eina_Bool _eon_canvas_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l, *l_next;
	Eina_Bool found = EINA_FALSE;

	thiz = _eon_canvas_get(ee);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, ech)
	{
		if (ech->ender == child)
		{
			thiz->children = eina_list_remove_list(thiz->children, l);
			found = EINA_TRUE;
			break;
		}
	}
	if (found)
	{
		thiz->needs_setup = EINA_TRUE;
		return EINA_TRUE;
	}

	return EINA_FALSE;
}

static Eon_Layout_Descriptor _descriptor = {
	.child_at = _eon_canvas_child_at,
	.child_add = _eon_canvas_child_add,
	.child_remove = _eon_canvas_child_remove,
	.free = _eon_canvas_free,
	.name = "canvas",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_canvas_new(void)
{
	Eon_Canvas *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Canvas));
	if (!thiz) return NULL;

	ee = eon_layout_new(&_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_canvas_child_x_set(Eon_Element *ee, Ender_Element *child, double x)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(ee);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->current.x = x;
			ech->needs_setup = EINA_TRUE;
		}
	}
}

static void _eon_canvas_child_y_set(Eon_Element *ee, Ender_Element *child, double y)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(ee);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->current.y = y;
			ech->needs_setup = EINA_TRUE;
		}
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_canvas_delete NULL
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
	return EON_ELEMENT_NEW("canvas");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_x_set(Ender_Element *e, Ender_Element *child, double x)
{
	ender_element_property_value_set(child, EON_CANVAS_CHILD_X, x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_y_set(Ender_Element *e, Ender_Element *child, double y)
{
	ender_element_property_value_set(child, EON_CANVAS_CHILD_Y, y, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_canvas_child_at_down_relative(Ender_Element *e, Ender_Element *rel, double x, double y)
{
	Ender_Element *child;

	child = _canvas_child_down_at(e, rel, x, y);
	return child;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_canvas_child_at_up_relative(Ender_Element *e, Ender_Element *rel, double x, double y)
{
	Ender_Element *child;

	child = _canvas_child_up_at(e, rel, x, y);
	return child;
}

