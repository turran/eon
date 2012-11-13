/* EON - List and Toolkit library
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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_bin.h"
#include "eon_list.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"

#include "eon_private_keyboard_proxy.h"
#include "eon_private_keyboard_proxy_navigation.h"

#include "eon_private_layout.h"
#include "eon_private_layout_list.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_LIST_ORIENTATION;

typedef struct _Eon_List_Child
{
	Ender_Element *ender;
} Eon_List_Child;

typedef struct _Eon_List
{
	/* properties */
	Eon_Orientation orientation;
	/* private */
	Eon_Input_Proxy *proxy;
	/* container related data */
	Eina_List *children;
	/* layout related data */
	int children_count;
} Eon_List;

static inline Eon_List * _eon_list_get(Eon_Element *ee)
{
	Eon_List *thiz;

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
static Ender_Element * _eon_list_state_element_get(Ender_Element *e, double x, double y)
{
	Eon_List *thiz;
	Eon_List_Child *ech;
	Ender_Element *child = NULL;
	Eon_Element *ee;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_list_get(ee);

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Eon_Geometry g;
		Eon_Element *child_e;

		child_e = ender_element_object_get(ech->ender);
		eon_element_geometry_get(child_e, &g);
		if ((x < g.x + g.width) && (x >= g.x) && (y < g.y + g.height) && (y >= g.y))
			child = ech->ender;
	}
	printf("list returning %p at %g %g\n", child, x, y);

	return child;
}

static Ender_Element * _eon_list_state_element_next(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Ender_Element * _eon_list_state_element_prev(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Eon_Input_State_Descriptor _list_proxy_descriptor = {
	/* .element_get 	= */ _eon_list_state_element_get,
	/* .element_next 	= */ _eon_list_state_element_next,
	/* .element_prev 	= */ _eon_list_state_element_prev,
};
/*----------------------------------------------------------------------------*
 *                    The Keyboard Proxy Navigation interface                 *
 *----------------------------------------------------------------------------*/
static Ender_Element * _list_navigation_tab(void *data,
		Ender_Element *current)
{
	Eon_List *thiz = data;
	Eon_List_Child *child;
	Eina_List *l;

	printf("list tab %p\n", current);
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

static Ender_Element * _list_navigation_reverse_tab(void *data,
		Ender_Element *current)
{
	Eon_List *thiz = data;
	Eon_List_Child *child;
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

static Eon_Keyboard_Proxy_Navigation_Descriptor _list_navigation_descriptor = {
	/* .tab 	= */ _list_navigation_tab,
	/* .reverse_tab = */ _list_navigation_reverse_tab,
	/* .up 		= */ NULL,
	/* .down 	= */ NULL,
	/* .left 	= */ NULL,
	/* .right 	= */ NULL
};
/*----------------------------------------------------------------------------*
 *                        The main layout descriptors                         *
 *----------------------------------------------------------------------------*/
static void _list_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_List_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_geometry_set(e, g);
}

static void _list_layout_child_count_set(void *ref, int count)
{
	Eon_List *thiz = ref;
	thiz->children_count = count;
}

static int _list_layout_child_count_get(void *ref)
{
	Eon_List *thiz = ref;
	return thiz->children_count;
}

static void _list_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_List_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_hints_get(e, hints);
}

static void _list_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_List *thiz = ref;
	Eon_List_Child *thiz_child;
	Eina_List *l;

	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ref, thiz_child, data);
	}
}

static Eon_Orientation _list_layout_orientation_get(void *ref)
{
	Eon_List *thiz = ref;
	return thiz->orientation;
}

static Eon_Layout_List_Descriptor _list_layout = {
	/* .orientation_get 	= */ _list_layout_orientation_get,
	/* .child_count_get 	= */ _list_layout_child_count_get,
	/* .child_count_set 	= */ _list_layout_child_count_set,
	/* .child_foreach 	= */ _list_layout_child_foreach,
	/* .child_hints_get 	= */ _list_layout_child_hints_get,
	/* .child_geometry_set 	= */ _list_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_list_free(Eon_Element *ee)
{
	Eon_List *thiz;

	thiz = _eon_list_get(ee);
	free(thiz);
}

static void _eon_list_initialize(Ender_Element *e)
{
	Eon_List *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_list_get(ee);
	thiz->proxy = eon_input_proxy_new(e, &_list_proxy_descriptor);
}

static Eina_Bool _eon_list_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_List *thiz;
	Eon_List_Child *thiz_child;

	thiz = _eon_list_get(ee);
	thiz_child = calloc(1, sizeof(Eon_List_Child));
	thiz_child->ender = child;
	thiz->children = eina_list_append(thiz->children, thiz_child);

	return EINA_TRUE;
}

static Eina_Bool _eon_list_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_List *thiz;
	Eon_List_Child *thiz_child;
	Eina_List *l, *l_next;
	Eina_Bool found = EINA_FALSE;

	thiz = _eon_list_get(ee);
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

static void _eon_list_child_foreach(Eon_Element *ee, Eon_Container_Child_Foreach_Cb cb, void *user_data)
{
	Eon_List *thiz;
	Eon_List_Child *thiz_child;
	Eina_List *l;

	thiz = _eon_list_get(ee);
	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ee, thiz_child->ender, user_data);
	}
}

static void _eon_list_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_List *thiz;

	thiz = _eon_list_get(e);
	eon_layout_geometry_set(&eon_layout_list, &_list_layout, thiz, g);
}

static void _eon_list_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_List *thiz;

	thiz = _eon_list_get(e);
	eon_layout_hints_get(&eon_layout_list, &_list_layout, thiz, hints);
}

static Eon_Container_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_list_initialize,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .geometry_set 	= */ _eon_list_geometry_set,
	/* .free	 	= */ _eon_list_free,
	/* .name 		= */ "list",
	/* .hints_get	 	= */ _eon_list_hints_get,
	/* .child_add 		= */ _eon_list_child_add,
	/* .child_remove 	= */ _eon_list_child_remove,
	/* .child_foreach 	= */ _eon_list_child_foreach,
	/* .child_at 		= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_list_new(void)
{
	Eon_List *thiz;
	Eon_Element *ee;
	Eon_Keyboard_Proxy *kp;
	Eon_Theme_Instance *theme;

	theme = eon_theme_instance_new("list", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_List));
	if (!thiz) return NULL;

	ee = eon_container_new(theme, &_descriptor, thiz);
	if (!ee) goto base_err;

	kp = eon_keyboard_proxy_navigation_new(&_list_navigation_descriptor, thiz);
	eon_element_keyboard_proxy_set(ee, kp);

	return ee;

base_err:
	free(thiz);
	return NULL;
}

static void _eon_list_orientation_set(Eon_Element *e, Eon_Orientation orientation)
{
	Eon_List *thiz;

	thiz = _eon_list_get(e);
	thiz->orientation = orientation;
	eon_element_inform_change(e);
}

static void _eon_list_orientation_get(Eon_Element *e, Eon_Orientation *orientation)
{
	Eon_List *thiz;

	thiz = _eon_list_get(e);
	if (orientation) *orientation = thiz->orientation;
}

#define _eon_list_delete NULL
#include "eon_generated_list.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_list_init(void)
{
	_eon_list_init();
}

void eon_list_shutdown(void)
{
	_eon_list_shutdown();
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_list_new(void)
{
	return EON_ELEMENT_NEW("list");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_hlist_new(void)
{
	Ender_Element *e;

	e = EON_ELEMENT_NEW("list");
	eon_list_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_vlist_new(void)
{
	Ender_Element *e;

	e = EON_ELEMENT_NEW("list");
	eon_list_orientation_set(e, EON_ORIENTATION_VERTICAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_list_orientation_set(Ender_Element *e, Eon_Orientation orientation)
{
	ender_element_property_value_set(e, EON_LIST_ORIENTATION, orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_list_orientation_get(Ender_Element *e, Eon_Orientation *orientation)
{
	Eon_List *thiz;
	Eon_Element *ee;

	if (!orientation) return;
	ee = ender_element_object_get(e);
	thiz = _eon_list_get(ee);
	*orientation = thiz->orientation;
}
