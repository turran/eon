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
#include "eon_checkbox.h"
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
#include "theme/eon_theme_container.h"
#include "theme/eon_theme_bin.h"
#include "theme/eon_theme_checkbox.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_checkbox_log

static int _eon_checkbox_log = -1;

static Ender_Property *EON_CHECKBOX_SELECTED;

typedef struct _Eon_Checkbox
{
	/* properties */
	Eina_Bool selected;
	/* private */
	Ender_Element *e;
	/* our own checkbox control */
	Eon_Theme_Instance *control;
	/* layout */
	double min_length;
} Eon_Checkbox;

static inline Eon_Checkbox * _eon_checkbox_get(Eon_Element *ee)
{
	Eon_Checkbox *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                             The checkbox layout                               *
 *----------------------------------------------------------------------------*/
static void _checkbox_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 10;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _checkbox_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Checkbox *thiz;
	Eon_Element *e = ref;

	thiz = _eon_checkbox_get(e);
	eon_theme_instance_property_set(thiz->control, "x", g->x, NULL);
	eon_theme_instance_property_set(thiz->control, "y", g->y, NULL);
	eon_theme_instance_property_set(thiz->control, "width", g->width, NULL);
	eon_theme_instance_property_set(thiz->control, "height", g->height, NULL);
}

static void _checkbox_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Checkbox *thiz;
	Eon_Element *e = ref;
	Eon_Size size;

	thiz = _eon_checkbox_get(e);
	eon_theme_control_checkbox_size_get(thiz->control->object, &size);
	eon_hints_sizes_values_set(hints, size.width, size.height,
			size.width, size.height,
			size.width, size.height);
}

static void _checkbox_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	cb(ref, NULL, data);
}

static Eon_Layout_Frame_Descriptor _checkbox_layout = {
	/* .child_padding_get 	= */ _checkbox_layout_child_padding_get,
	/* .child_foreach 	= */ _checkbox_layout_child_foreach,
	/* .child_hints_get 	= */ _checkbox_layout_child_hints_get,
	/* .child_geometry_set 	= */ _checkbox_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                           The bin child layout                             *
 *----------------------------------------------------------------------------*/
static void _child_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 10;
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
static void _checkbox_stack_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Layout_Child_Data *lchild = child;
	eon_layout_geometry_set(lchild->layout, lchild->descriptor, ref, g);
}

static void _checkbox_stack_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Layout_Child_Data *lchild = child;
	eon_layout_hints_get(lchild->layout, lchild->descriptor, ref, hints);
}

static void _checkbox_stack_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Layout_Child_Data lchild;

	/* the only children of this main stack are the two frames */
	lchild.layout = &eon_layout_frame;
	lchild.descriptor = &_checkbox_layout;
	cb(ref, &lchild, data);

	lchild.layout = &eon_layout_frame;
	lchild.descriptor = &_child_layout;
	cb(ref, &lchild, data);
}

static Eon_Orientation _checkbox_stack_layout_orientation_get(void *ref)
{
	return EON_ORIENTATION_HORIZONTAL;
}

static void _checkbox_stack_layout_min_length_get(void *ref, double *min)
{
	Eon_Element *e = ref;
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(e);
	*min = thiz->min_length;
}

static void _checkbox_stack_layout_min_length_set(void *ref, double min)
{
	Eon_Element *e = ref;
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(e);
	thiz->min_length = min;
}

static int _checkbox_stack_layout_child_weight_get(void *ref, void *child)
{
	Eon_Layout_Child_Data *lchild = child;
	

	/* the child frame always has priority */
	if (lchild->descriptor == &_child_layout)
		return 1;
	else
		return 0;
}

static Eon_Layout_Stack_Descriptor _stack_layout = {
	/* .orientation_get 	= */ _checkbox_stack_layout_orientation_get,
	/* .min_length_get 	= */ _checkbox_stack_layout_min_length_get,
	/* .min_length_get 	= */ _checkbox_stack_layout_min_length_set,
	/* .child_weight_get 	= */ _checkbox_stack_layout_child_weight_get,
	/* .child_foreach 	= */ _checkbox_stack_layout_child_foreach,
	/* .child_hints_get 	= */ _checkbox_stack_layout_child_hints_get,
	/* .child_geometry_set 	= */ _checkbox_stack_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                     The main frame layout descriptors                      *
 *----------------------------------------------------------------------------*/
static void _main_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 10;
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
static void _eon_checkbox_mouse_click(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eina_Bool selected;

	eon_checkbox_selected_get(e, &selected);
	eon_checkbox_selected_set(e, !selected);
}

static void _eon_checkbox_key_up(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eon_Event_Key_Up *ev = event_data;
	Eon_Navigation_Key nkey;
	Eina_Bool enabled;
	Eina_Bool selected;

	/* check if the key is an enter key */
	if (!eon_input_navigation_key_get(ev->input, ev->key, &nkey))
		return;

	if (nkey != EON_NAVIGATION_KEY_OK)
		return;

	eon_widget_enabled_get(e, &enabled);
	if (!enabled)
		return;

	eon_checkbox_selected_get(e, &selected);
	eon_checkbox_selected_set(e, !selected);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's button_base interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_checkbox_initialize(Ender_Element *e)
{
	Eon_Checkbox *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_checkbox_get(ee);
	thiz->e = e;
	/* register every needed callback */
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
			_eon_checkbox_mouse_click, NULL);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_UP],
			_eon_checkbox_key_up, NULL);
}

static void _eon_checkbox_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	eon_layout_geometry_set(&eon_layout_frame, &_main_layout, e, g);
}

static void _eon_checkbox_free(Eon_Element *e)
{
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(e);
	free(thiz);
}

static void _eon_checkbox_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	eon_layout_hints_get(&eon_layout_frame, &_main_layout, e, hints);
	hints->max.width = DBL_MAX;
	hints->max.height = DBL_MAX;
}


static Eon_Button_Base_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_checkbox_initialize,
	/* .geometry_set 	= */ _eon_checkbox_geometry_set,
	/* .free		= */ _eon_checkbox_free,
	/* .name 		= */ "checkbox",
	/* .hints_get 		= */ _eon_checkbox_hints_get,
	/* .child_at 		= */ NULL,
	/* .pass_events		= */ EINA_FALSE,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_checkbox_new(void)
{
	Eon_Checkbox *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;
	Eon_Theme_Instance *control;

	theme = eon_theme_instance_new("checkbox", EINA_TRUE);
	if (!theme) return NULL;

	control = eon_theme_instance_new("control_checkbox", EINA_TRUE);
	if (!control) goto checkbox_control_err;

	thiz = calloc(1, sizeof(Eon_Checkbox));
	thiz->control = control;
	eon_theme_checkbox_control_set(theme->object, control->renderer);

	ee = eon_button_base_new(theme, &_descriptor, thiz);
	if (!ee) goto base_err;

	return ee;

base_err:
	free(thiz);
	//eon_theme_instance_free(control);
checkbox_control_err:
	//eon_theme_instance_free(theme);
	return NULL;
}

static void _eon_checkbox_selected_get(Eon_Element *ee, Eina_Bool *selected)
{
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(ee);
	*selected = thiz->selected;
}

static void _eon_checkbox_selected_set(Eon_Element *ee, Eina_Bool selected)
{
	Eon_Checkbox *thiz;
	Eon_Event_Selected selected_event;
	Ender_Element *e;
	Eon_Theme_Instance *theme;

	thiz = _eon_checkbox_get(ee);
	if (thiz->selected == selected) return;
	/* first set the property internally */
	thiz->selected = selected;

	theme = eon_widget_theme_instance_get(ee);
	if (selected)
	{
		eon_theme_instance_state_set(theme, "selected", EINA_FALSE);
		eon_theme_instance_state_set(thiz->control, "selected", EINA_FALSE);
	}
	else
	{
		eon_theme_instance_state_set(theme, "unselected", EINA_FALSE);
		eon_theme_instance_state_set(thiz->control, "unselected", EINA_FALSE);
	}

	/* trigger the selected event */
	e = thiz->e;
	selected_event.selected = selected;
	ender_event_dispatch(e, "Selected", &selected_event);
}

#define _eon_checkbox_delete NULL
#include "eon_generated_checkbox.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_checkbox_init(void)
{
	_eon_checkbox_log = eina_log_domain_register("eon_checkbox", EON_LOG_COLOR_DEFAULT);
	if (_eon_checkbox_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_checkbox_init();
}

void eon_checkbox_shutdown(void)
{
	if (_eon_checkbox_log < 0)
		return;
	_eon_checkbox_shutdown();
	eina_log_domain_unregister(_eon_checkbox_log);
	_eon_checkbox_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_checkbox_new(void)
{
	return EON_ELEMENT_NEW("checkbox");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_checkbox_new_with_label(const char *text)
{
	Ender_Element *e;
	Ender_Element *label;

	e = eon_checkbox_new();
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
EAPI void eon_checkbox_selected_set(Ender_Element *e, Eina_Bool selected)
{
	ender_element_property_value_set(e, EON_CHECKBOX_SELECTED, selected, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_checkbox_selected_get(Ender_Element *e, Eina_Bool *selected)
{
	ender_element_property_value_get(e, EON_CHECKBOX_SELECTED, selected, NULL);
}


