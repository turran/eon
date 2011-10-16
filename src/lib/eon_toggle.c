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
typedef struct _Eon_Toggle
{
	/* properties */
	Eina_Bool active;
} Eon_Toggle;

static inline Eon_Toggle * _eon_toggle_get(Enesim_Renderer *r)
{
	Eon_Toggle *thiz;

	thiz = eon_button_base_data_get(r);
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
Enesim_Renderer * _eon_toggle_new(void)
{
	Eon_Toggle *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Toggle));
	if (!thiz) return NULL;

	r = eon_button_base_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_toggle_active_get(Enesim_Renderer *r, Eina_Bool *active)
{
	Eon_Toggle *thiz;

	thiz = _eon_toggle_get(r);
	*active = thiz->active;
}

static void _eon_toggle_active_set(Enesim_Renderer *r, Eina_Bool active)
{
	Eon_Toggle *thiz;
	Eon_Event_Toggled toggle_event;
	Escen_Ender *theme;
	Escen_State *new_state;
	Ender_Element *e;

	thiz = _eon_toggle_get(r);
	/* first set the property internally */
	thiz->active = active;
	/* now set the state */
	theme = eon_widget_theme_ender_get(r);
	if (active)
		new_state = escen_ender_state_get(theme, "active");
	else
		new_state = escen_ender_state_get(theme, "inactive");
	if (new_state)
	{
		Escen_Instance *eei;
		eei = eon_widget_theme_instance_get(r);
		escen_instance_state_set(eei, new_state);
	}
	/* trigger the active event */
	e = ender_element_renderer_from(r);
	toggle_event.active = active;
	ender_event_dispatch(e, "Toggled", &toggle_event);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
	return ender_element_new_with_namespace("toggle", "eon");
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
