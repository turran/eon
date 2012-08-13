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
#include "eon_input.h"
#include "eon_element.h"
#include "eon_bin.h"
#include "eon_button.h"
#include "eon_widget.h"
#include "eon_label.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"
#include "eon_private_bin.h"
#include "eon_private_button_base.h"

#include "eon_private_layout.h"
#include "eon_private_layout_frame.h"
/* A button should be composed of a frame layout. We use the 'bin' interface
 * to allow only a single child.
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Button
{
	/* properties */
	Eon_Margin padding;
	Ender_Property *p_padding;
} Eon_Button;

static inline Eon_Button * _eon_button_get(Eon_Element *ee)
{
	Eon_Button *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                           The theme extension                              *
 *----------------------------------------------------------------------------*/
static void _button_padding_set(Ender_Element *ee, Ender_Property *ep,
		Ender_Value *v, void *data)
{
	Eon_Element *e = data;
	Eon_Button *thiz;
	Eon_Margin *padding;

	thiz = _eon_button_get(e);
	padding = ender_value_struct_get(v);
	thiz->padding = *padding;

	eon_element_inform_change(e);
}
/*----------------------------------------------------------------------------*
 *                       The Eon's layout descriptors                         *
 *----------------------------------------------------------------------------*/
static void _main_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	Eon_Element *e;
	Eon_Button *thiz;

	e = ref;
	thiz = _eon_button_get(e);

	*margin = thiz->padding;
}

static void _main_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Element *e = child;
	eon_element_geometry_set(e, g);
}

static void _main_layout_child_hints_get(void *ref, void *child, Eon_Hints *hints)
{
	Eon_Element *e = child;
	eon_element_hints_get(e, hints);
}

static void _main_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Element *e = ref;
	Ender_Element *child;

	child = eon_bin_internal_child_get(e);
	if (!child) return;

	e = ender_element_object_get(child);
	cb(ref, e, data);
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
static void _eon_button_key_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
	Eon_Event_Key_Down *ev = event_data;
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

	ee = ender_element_object_get(e);
	theme = eon_widget_theme_instance_get(ee);
	eon_theme_instance_state_set(theme, "mouse_down", EINA_FALSE);
}

static void _eon_button_key_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
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

	ee = ender_element_object_get(e);
	theme = eon_widget_theme_instance_get(ee);
	eon_theme_instance_state_set(theme, "mouse_up", EINA_FALSE);
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_button_initialize(Ender_Element *e)
{
	/* the events */
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_UP],
			_eon_button_key_up, NULL);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_DOWN],
			_eon_button_key_down, NULL);
}

static void _eon_button_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	eon_layout_geometry_set(&eon_layout_frame, &_main_layout, e, g);
}

static void _eon_button_free(Eon_Element *e)
{
	Eon_Button *thiz;

	thiz = _eon_button_get(e);
	free(thiz);
}

static void _eon_button_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	eon_layout_hints_get(&eon_layout_frame, &_main_layout, e, hints);
	hints->max.width = DBL_MAX;
	hints->max.height = DBL_MAX;
}

static Eon_Button_Base_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_button_initialize,
	/* .setup 		= */ NULL,
	/* .needs_setup 	= */ NULL,
	/* .geometry_set 	= */ _eon_button_geometry_set,
	/* .free		= */ _eon_button_free,
	/* .name 		= */ "button",
	/* .hints_get 		= */ _eon_button_hints_get,
	/* .child_at 		= */ NULL,
	/* .pass_events		= */ EINA_FALSE,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_button_new(void)
{
	Eon_Button *thiz;
	Eon_Element *e;
	Eon_Theme_Instance *theme;
	Ender_Container *ec;

	/* get the needed containers */
	ec = ender_container_find("area");
	if (!ec) return NULL;

	theme = eon_theme_instance_new("button", EINA_FALSE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Button));
	e = eon_button_base_new(theme, &_descriptor, thiz);
	if (!e) goto base_err;

	/* register our own theme properties */
	thiz->p_padding = ender_element_property_add(theme->element, "padding",
			ec, NULL, _button_padding_set, 	NULL, NULL, NULL,
			NULL, EINA_FALSE, e);
	/* now set the default theme */
	eon_theme_instance_state_set(theme, "default", EINA_FALSE);

	return e;

base_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_button_delete NULL
#include "eon_generated_button.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_button_new(void)
{
	return EON_ELEMENT_NEW("button");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_button_new_with_label(const char *text)
{
	Ender_Element *e;
	Ender_Element *label;

	e = eon_button_new();
	if (!e) return NULL;

	label = eon_label_new_with_text(text);
	if (!label)
	{
		return NULL;
	}
	eon_bin_child_set(e, label);
	return e;
}
