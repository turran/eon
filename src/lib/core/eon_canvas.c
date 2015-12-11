/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_bin.h"
#include "eon_canvas.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"

#include "eon_private_keyboard_proxy.h"
#include "eon_private_keyboard_proxy_navigation.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_canvas_log

static int _eon_canvas_log = -1;

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
	Eon_Input_Proxy *proxy;
	Eina_Bool needs_setup : 1;
	Eon_Canvas_State old, curr;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Canvas;

static inline Eon_Canvas * _eon_canvas_get(Eon_Element *ee)
{
	Eon_Canvas *thiz;

	thiz = eon_container_data_get(ee);
	return thiz;
}

static Eina_Bool _canvas_child_is_inside(Eon_Canvas_Child *ech, double x, double y)
{
	Eon_Size child_size;
	Eon_Element *child_e;
	Eon_Geometry g;

	child_e = ender_element_object_get(ech->ender);
	eon_element_geometry_get(child_e, &g);
	if ((x < g.x + g.width) && (x >= g.x) && (y < g.y + g.height) && (y >= g.y))
		return EINA_TRUE;
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
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_canvas_initialize(Ender_Element *e)
{
	Eon_Canvas *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_canvas_get(ee);
	//thiz->proxy = eon_input_proxy_new(e, &_canvas_proxy_descriptor);
}

static void _eon_canvas_free(Eon_Element *ee)
{
	Eon_Canvas *thiz;

	thiz = _eon_canvas_get(ee);
	free(thiz);
}

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

static void _eon_canvas_child_foreach(Eon_Element *ee, Eon_Container_Child_Foreach_Cb cb, void *user_data)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *thiz_child;
	Eina_List *l;

	thiz = _eon_canvas_get(ee);
	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ee, thiz_child->ender, user_data);
	}
}

static void _eon_canvas_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *child_thiz;
	Eina_List *l;

	thiz = _eon_canvas_get(e);
	EINA_LIST_FOREACH (thiz->children, l, child_thiz)
	{
		Eon_Element *child_e;
		Eon_Hints hints;
		Eon_Geometry cg;

		child_e = ender_element_object_get(child_thiz->ender);
		eon_element_hints_get(child_e, &hints);
		/* FIXME handle the dynamic case */
		cg.width = hints.preferred.width;
		if (cg.width < 0)
			cg.width = hints.min.width;
		cg.height = hints.preferred.height;
		if (cg.height < 0)
			cg.height = hints.min.height;
		cg.x = g->x + child_thiz->current.x;
		cg.y = g->y + child_thiz->current.y;
		eon_element_geometry_set(child_e, &cg);
	}
}

static void _eon_canvas_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_Canvas *thiz;

	thiz = _eon_canvas_get(e);
	/* TODO get every child hint and then move them by x, y
	 * and return the max bounding rectangle
	 */
}

static Eon_Container_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_canvas_initialize,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .geometry_set 	= */ _eon_canvas_geometry_set,
	/* .free	 	= */ _eon_canvas_free,
	/* .name 		= */ "canvas",
	/* .hints_get	 	= */ _eon_canvas_hints_get,
	/* .child_add 		= */ _eon_canvas_child_add,
	/* .child_remove 	= */ _eon_canvas_child_remove,
	/* .child_foreach 	= */ _eon_canvas_child_foreach,
	/* .child_at 		= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_canvas_new(void)
{
	Eon_Canvas *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;

	theme = eon_theme_instance_new("grid", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Canvas));
	if (!thiz) return NULL;

	ee = eon_container_new(theme, &_descriptor, thiz);
	if (!ee) goto base_err;

	return ee;

base_err:
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

#define _eon_canvas_delete NULL
#define _eon_canvas_child_x_get NULL
#define _eon_canvas_child_y_get NULL
#include "eon_generated_canvas.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/void eon_canvas_init(void)
{
	_eon_canvas_log = eina_log_domain_register("eon_canvas", EON_LOG_COLOR_DEFAULT);
	if (_eon_canvas_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_canvas_init();
}

void eon_canvas_shutdown(void)
{
	if (_eon_canvas_log < 0)
		return;
	_eon_canvas_shutdown();
	eina_log_domain_unregister(_eon_canvas_log);
	_eon_canvas_log = -1;
}
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

