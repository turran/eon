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
static Ender_Property *EON_CHECKBOX_SELECTED;

typedef struct _Eon_Checkbox
{
	/* properties */
	Eina_Bool selected;
	/* private */
	Ender_Element *e;
} Eon_Checkbox;

static inline Eon_Checkbox * _eon_checkbox_get(Eon_Element *ee)
{
	Eon_Checkbox *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}

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

static Eon_Button_Base_Descriptor _descriptor = {
	.initialize = _eon_checkbox_initialize,
	.name = "checkbox",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_checkbox_new(void)
{
	Eon_Checkbox *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Checkbox));
	if (!thiz) return NULL;

	ee = eon_button_base_new(&_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
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

	thiz = _eon_checkbox_get(ee);
	/* first set the property internally */
	thiz->selected = selected;

	if (selected)
		eon_theme_instance_state_set(ee, "selected", EINA_FALSE);
	else
		eon_theme_instance_state_set(ee, "unselected", EINA_FALSE);

	/* trigger the selected event */
	e = thiz->e;
	selected_event.selected = selected;
	ender_event_dispatch(e, "Selected", &selected_event);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_checkbox_delete NULL
#include "eon_generated_checkbox.c"
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


