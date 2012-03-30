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
typedef struct _Eon_Button
{
} Eon_Button;

static inline Eon_Button * _eon_button_get(Eon_Element *ee)
{
	Eon_Button *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}

static void _eon_button_key_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
	Eon_Event_Key_Down *ev = event_data;
	Eon_Navigation_Key nkey;
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
	eon_widget_state_set(ee, "mouse_down", EINA_FALSE);
}

static void _eon_button_key_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
	Eon_Event_Key_Up *ev = event_data;
	Eon_Navigation_Key nkey;
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
	eon_widget_state_set(ee, "mouse_up", EINA_FALSE);
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_button_free(Eon_Element *ee)
{
	Eon_Button *thiz;

	thiz = _eon_button_get(ee);
	free(thiz);
}

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

static Eon_Button_Base_Descriptor _descriptor = {
	.initialize = _eon_button_initialize,
	.free = _eon_button_free,
	.name = "button",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_button_new(void)
{
	Eon_Button *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Button));
	if (!thiz) return NULL;

	ee = eon_button_base_new(&_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
	return ender_element_new_with_namespace("button", "eon");
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
	eon_container_content_set(e, label);
	return e;
}
