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
static Ender_Property *EON_TOGGLE_ACTIVE;

typedef struct _Eon_Toggle
{
	/* properties */
	Eina_Bool active;
	/* private */
	Ender_Element *e;
} Eon_Toggle;

static inline Eon_Toggle * _eon_toggle_get(Eon_Element *ee)
{
	Eon_Toggle *thiz;

	thiz = eon_button_base_data_get(ee);
	return thiz;
}

static void _toggle_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eina_Bool active;

	eon_toggle_active_get(e, &active);
	eon_toggle_active_set(e, !active);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's button_base interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_toggle_initialize(Ender_Element *ender)
{
	Eon_Toggle *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(ender);
	thiz = _eon_toggle_get(ee);
	thiz->e = ender;
	/* register every needed callback */
	ender_event_listener_add(ender, "MouseClick", _toggle_mouse_click, NULL);
}

static Eon_Button_Base_Descriptor _descriptor = {
	.initialize = _eon_toggle_initialize,
	.name = "toggle",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_toggle_new(void)
{
	Eon_Toggle *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Toggle));
	if (!thiz) return NULL;

	ee = eon_button_base_new(&_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_toggle_active_get(Eon_Element *ee, Eina_Bool *active)
{
	Eon_Toggle *thiz;

	thiz = _eon_toggle_get(ee);
	*active = thiz->active;
}

static void _eon_toggle_active_set(Eon_Element *ee, Eina_Bool active)
{
	Eon_Toggle *thiz;
	Eon_Event_Toggled toggle_event;
	Escen_Ender *theme;
	Escen_State *new_state;
	Ender_Element *e;

	thiz = _eon_toggle_get(ee);
	/* first set the property internally */
	thiz->active = active;
	/* now set the state */
	theme = eon_widget_theme_ender_get(ee);
	if (active)
		new_state = escen_ender_state_get(theme, "active");
	else
		new_state = escen_ender_state_get(theme, "inactive");
	if (new_state)
	{
		Escen_Instance *eei;
		eei = eon_widget_theme_instance_get(ee);
		escen_instance_state_set(eei, new_state);
	}
	/* trigger the active event */
	e = thiz->e;
	toggle_event.active = active;
	ender_event_dispatch(e, "Toggled", &toggle_event);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_toggle_delete NULL
#include "eon_generated_toggle.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_toggle_new(void)
{
	return EON_ELEMENT_NEW("toggle");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_toggle_new_with_label(const char *text)
{
	Ender_Element *e;
	Ender_Element *label;

	e = eon_toggle_new();
	if (!e) return NULL;

	label = eon_label_new_with_text(text);
	if (!label)
	{
		return NULL;
	}
	eon_container_content_set(e, label);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_toggle_active_set(Ender_Element *e, Eina_Bool active)
{
	ender_element_value_set(e, "active", active, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_toggle_active_get(Ender_Element *e, Eina_Bool *active)
{
	ender_element_value_get(e, "active", active, NULL);
}
