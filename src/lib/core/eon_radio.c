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
static Ender_Property *EON_RADIO_GROUP_NAME;
static Ender_Property *EON_RADIO_SELECTED;

typedef struct _Eon_Radio
{
	/* properties */
	char *group_name;
	/* private */
	Ender_Element *e;
} Eon_Radio;

static Eina_Hash *_groups = NULL;

static inline Eon_Radio * _eon_radio_get(Eon_Element *ee)
{
	Eon_Radio *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}

static void _eon_radio_mouse_click(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eon_Element *ee;
	Eina_Bool enabled;

	eon_widget_enabled_get(e, &enabled);
	if (!enabled)
		return;
	/* TODO Whenever the mouse is clicked, we should know if by clicking on x, y
	 * we actually need to change the state of the radio button
	 */
	ee = ender_element_object_get(e);
	eon_widget_state_set(ee, "selected", EINA_FALSE);
	eon_radio_selected_set(e, EINA_TRUE);
	/* TODO trigger the selected event */
}

static void _eon_radio_key_up(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
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

	eon_widget_state_set(ee, "selected", EINA_FALSE);
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

static Eon_Button_Base_Descriptor _descriptor = {
	.initialize = _eon_radio_initialize,
	.name = "radio",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_radio_new(void)
{
	Eon_Radio *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Radio));
	if (!thiz) return NULL;

	ee = eon_button_base_new(&_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_radio_group_name_set(Eon_Element *ee, const char *group)
{
	Eon_Radio *thiz;
	Eina_List *radios;
	Eina_List *l;

	thiz = _eon_radio_get(ee);
	if (!thiz) return;

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
	if (!thiz) return;

	*group = thiz->group_name;
}

static void _eon_radio_selected_get(Eon_Element *ee, Eina_Bool *selected)
{
	*selected = EINA_TRUE;
	//eon_widget_property_get(ee, "selected", selected, NULL);
}

static void _eon_radio_selected_set(Eon_Element *ee, Eina_Bool selected)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(ee);
	if (!thiz) return;

	eon_widget_property_set(ee, "selected", selected, NULL);
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
	return ender_element_new_with_namespace("radio", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_group_name_set(Ender_Element *e, const char *group)
{
	ender_element_value_set(e, "group_name", group, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_group_name_get(Ender_Element *e, const char **group)
{
	ender_element_value_get(e, "group_name", group, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_selected_set(Ender_Element *e, Eina_Bool selected)
{
	ender_element_value_set(e, "selected", selected, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_selected_get(Ender_Element *e, Eina_Bool *selected)
{
	ender_element_value_get(e, "selected", selected, NULL);
}

