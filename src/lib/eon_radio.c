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
typedef struct _Eon_Radio
{
	/* properties */
	char *group_name;
} Eon_Radio;

static Eina_Hash *_groups = NULL;

static inline Eon_Radio * _eon_radio_get(Enesim_Renderer *r)
{
	Eon_Radio *thiz;

	thiz = eon_button_base_data_get(r);
	return thiz;
}

static void _radio_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Escen_Ender *theme;
	Escen_State *new_state;
	Enesim_Renderer *r;

	/* TODO Whenever the mouse is clicked, we should know if by clicking on x, y
	 * we actually need to change the state of the radio button
	 */
	r = ender_element_renderer_get(e);
	theme = eon_widget_theme_ender_get(r);
	new_state = escen_ender_state_get(theme, "selected");
	if (new_state)
	{
		Escen_Instance *eei;
		eei = eon_widget_theme_instance_get(r);
		escen_instance_state_set(eei, new_state);
	}
	eon_radio_selected_set(e, EINA_TRUE);
	/* TODO trigger the selected event */
}
/*----------------------------------------------------------------------------*
 *                     The Eon's button_base interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_radio_initialize(Ender_Element *ender)
{
	/* register every needed callback */
	ender_event_listener_add(ender, "MouseClick", _radio_mouse_click, NULL);
}

static Eon_Button_Base_Descriptor _descriptor = {
	.initialize = _eon_radio_initialize,
	.name = "radio",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_radio_new(void)
{
	Eon_Radio *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Radio));
	if (!thiz) return NULL;

	r = eon_button_base_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_radio_group_name_set(Enesim_Renderer *r, const char *group)
{
	Eon_Radio *thiz;
	Eina_List *radios;
	Eina_List *l;

	thiz = _eon_radio_get(r);
	if (!thiz) return;

	if (!_groups)
	{
		_groups = eina_hash_string_superfast_new(NULL);
	}
	/* remove from the previous group */
	if (thiz->group_name)
	{
		radios = l = eina_hash_find(_groups, thiz->group_name);
		radios = eina_list_remove(radios, r);
		if (!radios)
			eina_hash_del(_groups, thiz->group_name, l);
		
	}
	/* append to the new group */
	thiz->group_name = strdup(group);
	radios = l = eina_hash_find(_groups, thiz->group_name);
	radios = eina_list_append(radios, r);
	if (!l)
	{
		eina_hash_add(_groups, thiz->group_name, radios);	
	}
}

static void _eon_radio_group_name_get(Enesim_Renderer *r, const char **group)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(r);
	if (!thiz) return;

	*group = thiz->group_name;
}

static void _eon_radio_selected_get(Enesim_Renderer *r, Eina_Bool *selected)
{
	*selected = EINA_TRUE;
	//eon_widget_property_get(r, "selected", selected, NULL);
}


static void _eon_radio_selected_set(Enesim_Renderer *r, Eina_Bool selected)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "selected", selected, NULL);
	if (selected)
	{
		Enesim_Renderer *other_radio;
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

			if (other_radio == r) continue;

			_eon_radio_selected_get(other_radio, &other_selected);
			if (other_selected)
			{
				Ender_Element *other_radio_ender;

				other_radio_ender = ender_element_renderer_from(other_radio);
				eon_radio_selected_set(other_radio_ender, EINA_FALSE);	
			}
		}
	}
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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

