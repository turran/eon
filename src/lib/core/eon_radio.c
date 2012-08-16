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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_bin.h"
#include "eon_radio.h"
#include "eon_label.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"
#include "eon_private_bin.h"
#include "eon_private_button_base.h"

#include "eon_private_layout.h"
#include "eon_private_layout_stack.h"
#include "eon_private_layout_frame.h"

#include "theme/eon_theme_widget.h"
#include "theme/eon_theme_bin.h"
#include "theme/eon_theme_radio.h"
#include "theme/eon_theme_control_radio.h"
/* The radio should be composed of a stack layout.
 * Inside the stack there should be two frame layouts, one for the
 * radio control and another for the bin contents
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_RADIO_GROUP_NAME;
static Ender_Property *EON_RADIO_SELECTED;

typedef struct _Eon_Radio
{
	/* properties */
	Eina_Bool selected;
	char *group_name;
	/* private */
	Ender_Element *e;
	/* our own radio control */
	Eon_Theme_Instance *control;
	/* layout */
	double min_length;
} Eon_Radio;

static Eina_Hash *_groups = NULL;

static inline Eon_Radio * _eon_radio_get(Eon_Element *ee)
{
	Eon_Radio *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                             The radio layout                               *
 *----------------------------------------------------------------------------*/
static void _radio_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = 4;
	margin->top = 2;
	margin->bottom = 2;
	margin->right = 4;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _radio_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Radio *thiz;
	Eon_Element *e = ref;
	Eon_Size size;
	double x;
	double y;

	thiz = _eon_radio_get(e);
	eon_theme_control_radio_size_get(thiz->control->object, &size);
	/* always center it */
	//printf("control %g %g %g %g\n", g->x, g->y, g->width, g->height);
	x = (g->width / 2) - (size.width / 2);
	y = (g->height / 2) - (size.height);
	eon_theme_instance_property_set(thiz->control, "x", g->x + x, NULL);
	eon_theme_instance_property_set(thiz->control, "y", g->y + y, NULL);
	eon_theme_instance_property_set(thiz->control, "width", size.width, NULL);
	eon_theme_instance_property_set(thiz->control, "height", size.height, NULL);
}

static void _radio_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Radio *thiz;
	Eon_Element *e = ref;
	Eon_Size size;

	thiz = _eon_radio_get(e);
	eon_theme_control_radio_size_get(thiz->control->object, &size);
	eon_hints_values_set(hints, size.width, size.height,
			size.width, size.height,
			size.width, size.height);
}

static void _radio_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	cb(ref, NULL, data);
}

static Eon_Layout_Frame_Descriptor _radio_layout = {
	/* .child_padding_get 	= */ _radio_layout_child_padding_get,
	/* .child_foreach 	= */ _radio_layout_child_foreach,
	/* .child_hints_get 	= */ _radio_layout_child_hints_get,
	/* .child_geometry_set 	= */ _radio_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                           The bin child layout                             *
 *----------------------------------------------------------------------------*/
static void _child_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = 2;
	margin->top = 2;
	margin->bottom = 2;
	margin->right = 2;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _child_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Element *e = child;
	eon_element_geometry_set(e, g);
}

static void _child_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Element *e = child;
	eon_element_hints_get(e, hints);
}

static void _child_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Element *e = ref;
	Ender_Element *child;

	child = eon_bin_internal_child_get(e);
	if (!child) return;

	e = ender_element_object_get(child);
	cb(ref, e, data);
}

static Eon_Layout_Frame_Descriptor _child_layout = {
	/* .child_padding_get 	= */ _child_layout_child_padding_get,
	/* .child_foreach 	= */ _child_layout_child_foreach,
	/* .child_hints_get 	= */ _child_layout_child_hints_get,
	/* .child_geometry_set 	= */ _child_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                     The main stack layout descriptors                      *
 *----------------------------------------------------------------------------*/
static void _radio_stack_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Layout_Child_Data *lchild = child;
	eon_layout_geometry_set(lchild->layout, lchild->descriptor, ref, g);
}

static void _radio_stack_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Layout_Child_Data *lchild = child;
	eon_layout_hints_get(lchild->layout, lchild->descriptor, ref, hints);
}

static void _radio_stack_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Layout_Child_Data lchild;

	/* the only children of this main stack are the two frames */
	lchild.layout = &eon_layout_frame;
	lchild.descriptor = &_radio_layout;
	cb(ref, &lchild, data);

	lchild.layout = &eon_layout_frame;
	lchild.descriptor = &_child_layout;
	cb(ref, &lchild, data);
}

static Eina_Bool _radio_stack_layout_is_homogeneous(void *ref)
{
	return EINA_FALSE;
}

static Eon_Orientation _radio_stack_layout_orientation_get(void *ref)
{
	return EON_ORIENTATION_HORIZONTAL;
}

static void _radio_stack_layout_min_length_get(void *ref, double *min)
{
	Eon_Element *e = ref;
	Eon_Radio *thiz;

	thiz = _eon_radio_get(e);
	*min = thiz->min_length;
}

static void _radio_stack_layout_min_length_set(void *ref, double min)
{
	Eon_Element *e = ref;
	Eon_Radio *thiz;

	thiz = _eon_radio_get(e);
	thiz->min_length = min;
}

static int _radio_stack_layout_child_gravity_get(void *ref, void *child)
{
	Eon_Layout_Child_Data *lchild = child;
	

	/* the child frame always has priority */
	if (lchild->descriptor == &_child_layout)
		return 1;
	else
		return 0;
}

static Eon_Layout_Stack_Descriptor _stack_layout = {
	/* .is_homogeneous 	= */ _radio_stack_layout_is_homogeneous,
	/* .orientation_get 	= */ _radio_stack_layout_orientation_get,
	/* .min_length_get 	= */ _radio_stack_layout_min_length_get,
	/* .min_length_get 	= */ _radio_stack_layout_min_length_set,
	/* .child_gravity_get 	= */ _radio_stack_layout_child_gravity_get,
	/* .child_count_get 	= */ NULL,
	/* .child_count_set 	= */ NULL,
	/* .child_foreach 	= */ _radio_stack_layout_child_foreach,
	/* .child_hints_get 	= */ _radio_stack_layout_child_hints_get,
	/* .child_geometry_set 	= */ _radio_stack_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                     The main frame layout descriptors                      *
 *----------------------------------------------------------------------------*/
static void _main_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 0;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _main_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	eon_layout_geometry_set(&eon_layout_stack, &_stack_layout, ref, g);
}

static void _main_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	eon_layout_hints_get(&eon_layout_stack, &_stack_layout, ref, hints);
}

static void _main_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	cb(ref, NULL, data);
}

static Eon_Layout_Frame_Descriptor _main_layout = {
	/* .child_padding_get 	= */ _main_layout_child_padding_get,
	/* .child_foreach 	= */ _main_layout_child_foreach,
	/* .child_hints_get 	= */ _main_layout_child_hints_get,
	/* .child_geometry_set 	= */ _main_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                                Event handlers                              *
 *----------------------------------------------------------------------------*/
static void _eon_radio_mouse_click(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eina_Bool enabled;

	eon_widget_enabled_get(e, &enabled);
	if (!enabled)
		return;

	eon_radio_selected_set(e, EINA_TRUE);
	/* TODO trigger the selected event */
}

static void _eon_radio_key_up(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eon_Event_Key_Up *ev = event_data;
	Eon_Navigation_Key nkey;
	Eon_Theme_Instance *theme;
	Eina_Bool enabled;

	/* check if the key is an enter key */
	if (!eon_input_navigation_key_get(ev->input, ev->key, &nkey))
		return;

	if (nkey != EON_NAVIGATION_KEY_OK)
		return;

	eon_widget_enabled_get(e, &enabled);
	if (!enabled)
		return;

	eon_radio_selected_set(e, EINA_TRUE);
	/* TODO trigger the selected event */
}
/*----------------------------------------------------------------------------*
 *                     The Eon's button_base interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_radio_initialize(Ender_Element *e)
{
	Eon_Radio *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_radio_get(ee);
	thiz->e = e;
	/* register every needed callback */
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
			_eon_radio_mouse_click, NULL);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_UP],
			_eon_radio_key_up, NULL);
}

static void _eon_radio_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	eon_layout_geometry_set(&eon_layout_frame, &_main_layout, e, g);
}

static void _eon_radio_free(Eon_Element *e)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(e);
	free(thiz);
}

static void _eon_radio_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	eon_layout_hints_get(&eon_layout_frame, &_main_layout, e, hints);
	hints->max.width = DBL_MAX;
	hints->max.height = DBL_MAX;
}

static Eon_Button_Base_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_radio_initialize,
	/* .setup 		= */ NULL,
	/* .needs_setup 	= */ NULL,
	/* .geometry_set 	= */ _eon_radio_geometry_set,
	/* .free		= */ _eon_radio_free,
	/* .name 		= */ "radio",
	/* .hints_get 		= */ _eon_radio_hints_get,
	/* .child_at 		= */ NULL,
	/* .pass_events		= */ EINA_FALSE,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_radio_new(void)
{
	Eon_Radio *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;
	Eon_Theme_Instance *control;

	theme = eon_theme_instance_new("radio", EINA_TRUE);
	if (!theme) return NULL;

	control = eon_theme_instance_new("control_radio", EINA_TRUE);
	if (!control) goto radio_control_err;

	thiz = calloc(1, sizeof(Eon_Radio));
	thiz->control = control;
	eon_theme_radio_control_set(theme->object, control->renderer);

	ee = eon_button_base_new(theme, &_descriptor, thiz);
	if (!ee) goto base_err;

	return ee;

base_err:
	free(thiz);
	//eon_theme_instance_free(control);
radio_control_err:
	//eon_theme_instance_free(theme);
	return NULL;
}

static void _eon_radio_group_name_set(Eon_Element *ee, const char *group)
{
	Eon_Radio *thiz;
	Eina_List *radios;
	Eina_List *l;

	thiz = _eon_radio_get(ee);

	if (!_groups)
	{
		_groups = eina_hash_string_superfast_new(NULL);
	}
	/* remove from the previous group */
	if (thiz->group_name)
	{
		radios = l = eina_hash_find(_groups, thiz->group_name);
		radios = eina_list_remove(radios, ee);
		if (!radios)
			eina_hash_del(_groups, thiz->group_name, l);
	}
	/* append to the new group */
	thiz->group_name = strdup(group);
	radios = l = eina_hash_find(_groups, thiz->group_name);
	radios = eina_list_append(radios, ee);
	if (!l)
	{
		eina_hash_add(_groups, thiz->group_name, radios);
	}
}

static void _eon_radio_group_name_get(Eon_Element *ee, const char **group)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(ee);
	*group = thiz->group_name;
}

static void _eon_radio_selected_get(Eon_Element *ee, Eina_Bool *selected)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(ee);
	*selected = thiz->selected;
}

static void _eon_radio_selected_set(Eon_Element *ee, Eina_Bool selected)
{
	Eon_Radio *thiz;
	Eon_Theme_Instance *theme;

	thiz = _eon_radio_get(ee);
	if (thiz->selected == selected) return;

	thiz->selected = selected;
	theme = eon_widget_theme_instance_get(ee);
	if (selected)
	{
		Eon_Element *other_radio;
		Eina_List *radios;
		Eina_List *l;

		radios = eina_hash_find(_groups, thiz->group_name);
		if (!radios) return;

		/* TODO we could optimize this to not iterate over
		 * the list trying to find the current selected
		 * just store it
		 */
		EINA_LIST_FOREACH(radios, l, other_radio)
		{
			Eina_Bool other_selected;

			if (other_radio == ee) continue;

			_eon_radio_selected_get(other_radio, &other_selected);
			if (other_selected)
			{
				Eon_Radio *other;
				Ender_Element *other_radio_ender;

				other = _eon_radio_get(other_radio);
				other_radio_ender = other->e;
				eon_radio_selected_set(other_radio_ender, EINA_FALSE);
			}
		}
		eon_theme_instance_state_set(theme, "selected", EINA_FALSE);
		eon_theme_instance_state_set(thiz->control, "selected", EINA_FALSE);
	}
	else
	{
		eon_theme_instance_state_set(theme, "unselected", EINA_FALSE);
		eon_theme_instance_state_set(thiz->control, "unselected", EINA_FALSE);
	}
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_radio_delete NULL
#include "eon_generated_radio.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_radio_new(void)
{
	return EON_ELEMENT_NEW("radio");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_radio_new_with_label(const char *text)
{
	Ender_Element *e;
	Ender_Element *label;

	e = eon_radio_new();
	if (!e) return NULL;

	label = eon_label_new_with_text(text);
	if (!label)
	{
		return NULL;
	}
	eon_bin_child_set(e, label);
	eon_element_vertical_alignment_set(label, EON_VERTICAL_ALIGNMENT_CENTER);
	eon_element_horizontal_alignment_set(label, EON_HORIZONTAL_ALIGNMENT_LEFT);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_group_name_set(Ender_Element *e, const char *group)
{
	ender_element_property_value_set(e, EON_RADIO_GROUP_NAME, group, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_group_name_get(Ender_Element *e, const char **group)
{
	Eon_Radio *thiz;
	Eon_Element *ee;

	if (!group) return;
	ee = ender_element_object_get(e);
	thiz = _eon_radio_get(ee);
	*group = thiz->group_name;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_selected_set(Ender_Element *e, Eina_Bool selected)
{
	ender_element_property_value_set(e, EON_RADIO_SELECTED, selected, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_selected_get(Ender_Element *e, Eina_Bool *selected)
{
	Eon_Radio *thiz;
	Eon_Element *ee;

	if (!selected) return;
	ee = ender_element_object_get(e);
	thiz = _eon_radio_get(ee);
	*selected = thiz->selected;
}

