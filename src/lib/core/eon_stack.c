/* EON - Stack and Toolkit library
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
static Ender_Property *EON_STACK_DIRECTION;
static Ender_Property *EON_STACK_HOMOGENEOUS;
static Ender_Property *EON_STACK_CHILD_GRAVITY;

typedef struct _Eon_Stack_Child
{
	Ender_Element *ender;
	double old_x;
	double old_y;
	double curr_x;
	double curr_y;
	int gravity;
} Eon_Stack_Child;

typedef struct _Eon_Stack
{
	/* properties */
	Eon_Direction direction;
	Eina_Bool homogeneous;
	/* private */
	/* container related data */
	Eina_List *children;
	/* layout related data */
	int children_count;
	double min_length;

	/* TODO remove ... */
	Eina_Bool needs_setup : 1;
} Eon_Stack;

typedef struct _Eon_Stack_Damage_Data
{
	double x;
	double y;
	Enesim_Renderer_Damage_Cb real_cb;
	void *real_data;
} Eon_Stack_Damage_Data;

static inline Eon_Stack * _eon_stack_get(Eon_Element *ee)
{
	Eon_Stack *thiz;

	thiz = eon_container_data_get(ee);
	return thiz;
}
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
 *                       The Eon's layout descriptors                         *
 *----------------------------------------------------------------------------*/
static void _stack_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Stack_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_geometry_set(e, g);
}

static void _stack_layout_child_count_set(void *ref, int count)
{
	Eon_Stack *thiz = ref;
	thiz->children_count = count;
}

static int _stack_layout_child_count_get(void *ref)
{
	Eon_Stack *thiz = ref;
	return thiz->children_count;
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

static Eina_Bool _stack_layout_is_homogeneous(void *ref)
{
	Eon_Stack *thiz = ref;
	return thiz->homogeneous;
}

static Eon_Direction _stack_layout_direction_get(void *ref)
{
	Eon_Stack *thiz = ref;
	return thiz->direction;
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

static int _stack_layout_child_gravity_get(void *ref, void *child)
{
	Eon_Stack_Child *thiz_child = child;

	return thiz_child->gravity;
}

static Eon_Layout_Stack_Descriptor _stack_layout = {
	/* .is_homogeneous 	= */ _stack_layout_is_homogeneous,
	/* .direction_get 	= */ _stack_layout_direction_get,
	/* .min_length_get 	= */ _stack_layout_min_length_get,
	/* .min_length_get 	= */ _stack_layout_min_length_set,
	/* .child_gravity_get 	= */ _stack_layout_child_gravity_get,
	/* .child_count_get 	= */ _stack_layout_child_count_get,
	/* .child_count_set 	= */ _stack_layout_child_count_set,
	/* .child_foreach 	= */ _stack_layout_child_foreach,
	/* .child_hints_get 	= */ _stack_layout_child_hints_get,
	/* .child_geometry_set 	= */ _stack_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_stack_free(Eon_Element *ee)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(ee);
	free(thiz);
}

#if 0
static Eina_Bool _eon_stack_needs_setup(Ender_Element *e)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Eon_Element *ee;
	Eina_List *l;
	Eina_Bool ret;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);

	ret = thiz->needs_setup;
	if (ret) return EINA_TRUE;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		ret = eon_element_needs_setup(ech->ender);
		if (ret) break;
	}
	return ret;
}

static Eina_Bool _eon_stack_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Stack *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	/* TODO we should only relayout whenever our properties have changed or a child
	 * has changed properties, if not just leave it as is. Also we should not
	 * check *every* property
	 */
	/* if the actual size has changed, then relayout
	 * if some child preferred/min/max size has changed then call the relayout
	 */
	return _stack_relayout(e, thiz, &state->actual_position, &state->actual_size, s, err);
}

static double _eon_stack_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Stack *thiz;
	Eon_Element *ee;
	double min_width;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	if (!thiz) return 0;

	if (thiz->direction == EON_DIRECTION_HORIZONTAL)
		min_width = _stack_horizontal_min_width(thiz);
	else
		min_width = _stack_vertical_min_width(thiz);

	return min_width;
}

static double _eon_stack_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Stack *thiz;
	Eon_Element *ee;
	double min_height;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	if (!thiz) return 0;

	if (thiz->direction == EON_DIRECTION_HORIZONTAL)
		min_height = _stack_horizontal_min_height(thiz);
	else
		min_height = _stack_vertical_min_height(thiz);

	return min_height;
}

static double _eon_stack_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Stack *thiz;
	Eon_Element *ee;
	double preferred_width = -1;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	if (!thiz) return preferred_width;

	if (thiz->direction == EON_DIRECTION_HORIZONTAL)
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		preferred_width = 0;
		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double pw;

			eon_element_preferred_width_get(ech->ender, &pw);
			if (pw < 0) continue;
			preferred_width += pw;
		}
	}
	else
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double pw;

			eon_element_preferred_width_get(ech->ender, &pw);
			preferred_width = MAX(pw, preferred_width);
		}

	}
	return preferred_width;
}

static double _eon_stack_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Stack *thiz;
	Eon_Element *ee;
	double preferred_height = -1;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	if (!thiz) return preferred_height;

	if (thiz->direction == EON_DIRECTION_VERTICAL)
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		preferred_height = 0;
		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double ph;

			eon_element_preferred_height_get(ech->ender, &ph);
			if (ph < 0) continue;
			preferred_height += ph;
		}
	}
	else
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double ph;

			eon_element_preferred_height_get(ech->ender, &ph);
			preferred_height = MAX(ph, preferred_height);
		}
		//printf("preferred height %g\n", preferred_height);
	}

	return preferred_height;
}
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's layout interface                         *
 *----------------------------------------------------------------------------*/
/* FIXME we need to decide how are we going to get find an element on the scene
 * so far we were intersecting the input coordinates with the object layout
 * geometry, for things like comboboxes we might need to intersect against
 * the displayed geometry, given that a combobox display somehting outside
 * the layout tree but the user should interact with it normally
 */
static Ender_Element * _eon_stack_child_at(Ender_Element *e, double x, double y)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Ender_Element *child = NULL;
	Eon_Element *ee;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	if (!thiz) return NULL;
#if 0
	{
		char *name;
		enesim_renderer_name_get(r, &name);
		printf("stack %s child at %g %g \n", name, x, y);
	}
#endif

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Eon_Element *child_e;
		Eon_Size child_size;
		double child_x, child_y;

		child_x = x - ech->curr_x;
		if (child_x < 0) continue;
		child_y = y - ech->curr_y;
		if (child_y < 0) continue;

		/* TODO still need the width and height */
		child_e = ender_element_object_get(ech->ender);
		//eon_element_actual_size_get(child_e, &child_size);
		if (child_x <= child_size.width && child_y <= child_size.height)
		{
			child = ech->ender;
		}
	}
	//printf("returning %p\n", child);

	return child;
}

static Eina_Bool _eon_stack_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;

	thiz = _eon_stack_get(ee);
	thiz_child = calloc(1, sizeof(Eon_Stack_Child));
	thiz_child->ender = child;
	thiz->children = eina_list_append(thiz->children, thiz_child);
	thiz->needs_setup = EINA_TRUE;

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
		thiz->needs_setup = EINA_TRUE;
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
	/* .initialize 		= */ NULL,
	/* .setup 		= */ NULL,
	/* .needs_setup 	= */ NULL,
	/* .geometry_set 	= */ _eon_stack_geometry_set,
	/* .free	 	= */ _eon_stack_free,
	/* .name 		= */ "stack",
	/* .hints_get	 	= */ _eon_stack_hints_get,
	/* .child_add 		= */ _eon_stack_child_add,
	/* .child_remove 	= */ _eon_stack_child_remove,
	/* .child_foreach 	= */ _eon_stack_child_foreach,
	/* .child_at 		= */ _eon_stack_child_at,
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

static void _eon_stack_direction_set(Eon_Element *ee, Eon_Direction direction)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(ee);
	thiz->direction = direction;
	thiz->needs_setup = EINA_TRUE;
}

static void _eon_stack_direction_get(Eon_Element *ee, Eon_Direction *direction)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(ee);
	if (direction) *direction = thiz->direction;

}

static void _eon_stack_child_gravity_set(Eon_Element *ee, Ender_Element *child,
		int gravity)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Eina_List *l;

	thiz = _eon_stack_get(ee);
	printf(">>> setting gravity %d\n", gravity);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->gravity = gravity;
			thiz->needs_setup = EINA_TRUE;
		}
	}
}

static void _eon_stack_homogeneous_set(Eon_Element *ee, Eina_Bool homogeneous)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(ee);
	thiz->homogeneous = homogeneous;
	thiz->needs_setup = EINA_TRUE;
}

static void _eon_stack_homogeneous_get(Eon_Element *ee, Eina_Bool *homogeneous)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(ee);
	*homogeneous = thiz->homogeneous;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_stack_delete NULL
#define _eon_stack_child_gravity_get NULL
#include "eon_generated_stack.c"
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
	eon_stack_direction_set(e, EON_DIRECTION_HORIZONTAL);
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
	eon_stack_direction_set(e, EON_DIRECTION_VERTICAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_direction_set(Ender_Element *e, Eon_Direction direction)
{
	ender_element_property_value_set(e, EON_STACK_DIRECTION, direction, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_direction_get(Ender_Element *e, Eon_Direction *direction)
{
	Eon_Stack *thiz;
	Eon_Element *ee;

	if (!direction) return;
	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	*direction = thiz->direction;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_homogeneous_set(Ender_Element *e, Eina_Bool homogeneous)
{
	ender_element_property_value_set(e, EON_STACK_HOMOGENEOUS, homogeneous, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_homogeneous_get(Ender_Element *e, Eina_Bool *homogeneous)
{
	Eon_Stack *thiz;
	Eon_Element *ee;

	if (!homogeneous) return;
	ee = ender_element_object_get(e);
	thiz = _eon_stack_get(ee);
	*homogeneous = thiz->homogeneous;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_gravity_set(Ender_Element *e, Ender_Element *child,
		int gravity)
{
	ender_element_property_value_set(child, EON_STACK_CHILD_GRAVITY, gravity, NULL);
}

