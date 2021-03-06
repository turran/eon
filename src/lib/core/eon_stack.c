/* EON - Stack and Toolkit library
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
#include "eon_stack.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"

#include "eon_private_keyboard_proxy.h"
#include "eon_private_keyboard_proxy_navigation.h"

#include "eon_private_layout.h"
#include "eon_private_layout_stack.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_stack_log

static int _eon_stack_log = -1;

static Ender_Property *EON_STACK_ORIENTATION;
static Ender_Property *EON_STACK_CHILD_WEIGHT;

typedef struct _Eon_Stack_Child
{
	Ender_Element *ender;
	int weight;
} Eon_Stack_Child;

typedef struct _Eon_Stack
{
	/* properties */
	Eon_Orientation orientation;
	/* private */
	Eon_Input_Proxy *proxy;
	/* container related data */
	Eina_List *children;
	/* layout related data */
	double min_length;
} Eon_Stack;

static inline Eon_Stack * _eon_stack_get(Eon_Element *ee)
{
	Eon_Stack *thiz;

	thiz = eon_container_data_get(ee);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                       The Eon Input State interface                        *
 *----------------------------------------------------------------------------*/
/* FIXME we need to decide how are we going to get find an element on the scene
 * so far we were intersecting the input coordinates with the object layout
 * geometry, for things like comboboxes we might need to intersect against
 * the displayed geometry, given that a combobox display somehting outside
 * the layout tree but the user should interact with it normally
 */
static Ender_Element * _eon_stack_state_element_get(Ender_Element *e, double x, double y)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Ender_Element *child = NULL;
	Eon_Element *ee;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Eon_Geometry g;
		Eon_Element *child_e;

		child_e = ender_element_object_get(ech->ender);
		eon_element_geometry_get(child_e, &g);
		if ((x < g.x + g.width) && (x >= g.x) && (y < g.y + g.height) && (y >= g.y))
			child = ech->ender;
	}
	printf("stack returning %p at %g %g\n", child, x, y);

	return child;
}

static Ender_Element * _eon_stack_state_element_next(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Ender_Element * _eon_stack_state_element_prev(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Eon_Input_State_Descriptor _stack_proxy_descriptor = {
	/* .element_get 	= */ _eon_stack_state_element_get,
	/* .element_next 	= */ _eon_stack_state_element_next,
	/* .element_prev 	= */ _eon_stack_state_element_prev,
};
/*----------------------------------------------------------------------------*
 *                    The Keyboard Proxy Navigation interface                 *
 *----------------------------------------------------------------------------*/
static Ender_Element * _stack_navigation_tab(void *data,
		Ender_Element *current)
{
	Eon_Stack *thiz = data;
	Eon_Stack_Child *child;
	Eina_List *l;

	printf("stack tab %p\n", current);
	if (!current)
	{
		child = eina_list_data_get(thiz->children);
		if (!child) return NULL;

		return child->ender;
	}
	else
	{
		/* TODO We need to implement the siblings */
		EINA_LIST_FOREACH(thiz->children, l, child)
		{
			if (child->ender == current)
			{
				l = eina_list_next(l);
				if (!l) return NULL;
				child = eina_list_data_get(l);
				return child->ender;
			}
		}
		return NULL;
	}
}

static Ender_Element * _stack_navigation_reverse_tab(void *data,
		Ender_Element *current)
{
	Eon_Stack *thiz = data;
	Eon_Stack_Child *child;
	Eina_List *l;

	if (!current)
	{
		l = eina_list_last(thiz->children);
		if (!l) return NULL;

		child = eina_list_data_get(l);
		return child->ender;
	}
	else
	{
		/* TODO We need to implement the siblings */
		EINA_LIST_FOREACH(thiz->children, l, child)
		{
			if (child->ender == current)
			{
				l = eina_list_prev(l);
				if (!l) return NULL;

				child = eina_list_data_get(l);
				return child->ender;
			}
		}
		return NULL;
	}
}

static Eon_Keyboard_Proxy_Navigation_Descriptor _stack_navigation_descriptor = {
	/* .tab 	= */ _stack_navigation_tab,
	/* .reverse_tab = */ _stack_navigation_reverse_tab,
	/* .up 		= */ NULL,
	/* .down 	= */ NULL,
	/* .left 	= */ NULL,
	/* .right 	= */ NULL
};
/*----------------------------------------------------------------------------*
 *                        The main layout descriptors                         *
 *----------------------------------------------------------------------------*/
static void _stack_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Stack_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_geometry_set(e, g);
}

static void _stack_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Stack_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_hints_get(e, hints);
}

static void _stack_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Stack *thiz = ref;
	Eon_Stack_Child *thiz_child;
	Eina_List *l;

	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ref, thiz_child, data);
	}
}

static Eon_Orientation _stack_layout_orientation_get(void *ref)
{
	Eon_Stack *thiz = ref;
	return thiz->orientation;
}

static void _stack_layout_min_length_get(void *ref, double *min)
{
	Eon_Stack *thiz = ref;
	*min = thiz->min_length;
}

static void _stack_layout_min_length_set(void *ref, double min)
{
	Eon_Stack *thiz = ref;
	thiz->min_length = min;
}

static int _stack_layout_child_weight_get(void *ref, void *child)
{
	Eon_Stack_Child *thiz_child = child;

	return thiz_child->weight;
}

static Eon_Layout_Stack_Descriptor _stack_layout = {
	/* .orientation_get 	= */ _stack_layout_orientation_get,
	/* .min_length_get 	= */ _stack_layout_min_length_get,
	/* .min_length_set 	= */ _stack_layout_min_length_set,
	/* .child_weight_get 	= */ _stack_layout_child_weight_get,
	/* .child_foreach 	= */ _stack_layout_child_foreach,
	/* .child_hints_get 	= */ _stack_layout_child_hints_get,
	/* .child_geometry_set 	= */ _stack_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_stack_free(Eon_Element *ee)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(ee);
	free(thiz);
}

static void _eon_stack_initialize(Ender_Element *e)
{
	Eon_Stack *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	thiz->proxy = eon_input_proxy_new(e, &_stack_proxy_descriptor);
}

static Eina_Bool _eon_stack_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;

	thiz = _eon_stack_get(ee);
	thiz_child = calloc(1, sizeof(Eon_Stack_Child));
	thiz_child->ender = child;
	thiz->children = eina_list_append(thiz->children, thiz_child);

	return EINA_TRUE;
}

static Eina_Bool _eon_stack_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;
	Eina_List *l, *l_next;
	Eina_Bool found = EINA_FALSE;

	thiz = _eon_stack_get(ee);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, thiz_child)
	{
		if (thiz_child->ender == child)
		{
			thiz->children = eina_list_remove_list(thiz->children, l);
			found = EINA_TRUE;
			break;
		}
	}
	if (found)
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

static void _eon_stack_child_foreach(Eon_Element *ee, Eon_Container_Child_Foreach_Cb cb, void *user_data)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;
	Eina_List *l;

	thiz = _eon_stack_get(ee);
	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ee, thiz_child->ender, user_data);
	}
}

static void _eon_stack_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(e);
	eon_layout_geometry_set(&eon_layout_stack, &_stack_layout, thiz, g);
}

static void _eon_stack_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(e);
	eon_layout_hints_get(&eon_layout_stack, &_stack_layout, thiz, hints);
}

static Eon_Container_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_stack_initialize,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .geometry_set 	= */ _eon_stack_geometry_set,
	/* .free	 	= */ _eon_stack_free,
	/* .name 		= */ "stack",
	/* .hints_get	 	= */ _eon_stack_hints_get,
	/* .child_add 		= */ _eon_stack_child_add,
	/* .child_remove 	= */ _eon_stack_child_remove,
	/* .child_foreach 	= */ _eon_stack_child_foreach,
	/* .child_at 		= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_stack_new(void)
{
	Eon_Stack *thiz;
	Eon_Element *ee;
	Eon_Keyboard_Proxy *kp;
	Eon_Theme_Instance *theme;

	theme = eon_theme_instance_new("stack", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Stack));
	if (!thiz) return NULL;

	ee = eon_container_new(theme, &_descriptor, thiz);
	if (!ee) goto base_err;

	kp = eon_keyboard_proxy_navigation_new(&_stack_navigation_descriptor, thiz);
	eon_element_keyboard_proxy_set(ee, kp);

	return ee;

base_err:
	free(thiz);
	return NULL;
}

static void _eon_stack_orientation_set(Eon_Element *e, Eon_Orientation orientation)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(e);
	thiz->orientation = orientation;
	eon_element_inform_change(e);
}

static void _eon_stack_orientation_get(Eon_Element *e, Eon_Orientation *orientation)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(e);
	if (orientation) *orientation = thiz->orientation;
}

static void _eon_stack_child_weight_set(Eon_Element *e, Ender_Element *child,
		int weight)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Eina_List *l;

	thiz = _eon_stack_get(e);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->weight = weight;
			eon_element_inform_change(e);
		}
	}
}

#define _eon_stack_delete NULL
#define _eon_stack_child_weight_get NULL
#include "eon_generated_stack.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_stack_init(void)
{
	_eon_stack_log = eina_log_domain_register("eon_stack", EON_LOG_COLOR_DEFAULT);
	if (_eon_stack_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_stack_init();
}

void eon_stack_shutdown(void)
{
	if (_eon_stack_log < 0)
		return;
	_eon_stack_shutdown();
	eina_log_domain_unregister(_eon_stack_log);
	_eon_stack_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_stack_new(void)
{
	return EON_ELEMENT_NEW("stack");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_hstack_new(void)
{
	Ender_Element *e;

	e = EON_ELEMENT_NEW("stack");
	eon_stack_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_vstack_new(void)
{
	Ender_Element *e;

	e = EON_ELEMENT_NEW("stack");
	eon_stack_orientation_set(e, EON_ORIENTATION_VERTICAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_orientation_set(Ender_Element *e, Eon_Orientation orientation)
{
	ender_element_property_value_set(e, EON_STACK_ORIENTATION, orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_orientation_get(Ender_Element *e, Eon_Orientation *orientation)
{
	Eon_Stack *thiz;
	Eon_Element *ee;

	if (!orientation) return;
	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	*orientation = thiz->orientation;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_weight_set(Ender_Element *e, Ender_Element *child,
		int weight)
{
	ender_element_property_value_set(child, EON_STACK_CHILD_WEIGHT, weight, NULL);
}

