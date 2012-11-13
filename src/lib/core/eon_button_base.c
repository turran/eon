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
#include "eon_bin.h"
#include "eon_button.h"
#include "eon_widget.h"
#include "eon_label.h"
#include "eon_container.h"
#include "eon_bin.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"
#include "eon_private_bin.h"
#include "eon_private_button_base.h"
#include "eon_private_keyboard_proxy.h"
#include "eon_private_keyboard_proxy_focus.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Button_Base
{
	/* private */
	Eon_Element_Initialize initialize;
	Eon_Element_Free free;
	void *data;
} Eon_Button_Base;

static inline Eon_Button_Base * _eon_button_base_get(Eon_Element *ee)
{
	Eon_Button_Base *thiz;

	thiz = eon_bin_data_get(ee);
	return thiz;
}

static void _button_base_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
	Eon_Theme_Instance *theme;
	Eina_Bool enabled;

	ee = ender_element_object_get(e);

	eon_widget_enabled_get(e, &enabled);
	if (!enabled) return;
	theme = eon_widget_theme_instance_get(ee);
	eon_theme_instance_state_set(theme, "mouse_down", EINA_FALSE);
}

static void _button_base_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
	Eon_Theme_Instance *theme;
	Eina_Bool enabled;

	ee = ender_element_object_get(e);

	eon_widget_enabled_get(e, &enabled);
	if (!enabled) return;
	theme = eon_widget_theme_instance_get(ee);
	eon_theme_instance_state_set(theme, "mouse_up", EINA_FALSE);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
/* FIXME add the click event here instead of the button itself, as every
 * inherited eleement of this type must handle this
 */
static void _eon_button_base_initialize(Ender_Element *e)
{
	Eon_Button_Base *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_button_base_get(ee);
	ender_event_listener_add(e, "MouseDown", _button_base_mouse_down, NULL);
	ender_event_listener_add(e, "MouseUp", _button_base_mouse_up, NULL);
	if (thiz->initialize)
		thiz->initialize(e);
}

static void _eon_button_base_free(Eon_Element *ee)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(ee);
	if (thiz->free)
		thiz->free(ee);
	free(thiz);
}


#define _eon_button_base_delete NULL
#include "eon_generated_button_base.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_button_base_init(void)
{
	_eon_button_base_init();
}

void eon_button_base_shutdown(void)
{
	_eon_button_base_shutdown();
}

void * eon_button_base_data_get(Eon_Element *ee)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(ee);
	if (!thiz) return NULL;
	return thiz->data;
}

Eon_Element * eon_button_base_new(Eon_Theme_Instance *theme,
		Eon_Button_Base_Descriptor *descriptor, void *data)
{
	Eon_Button_Base *thiz;
	Eon_Bin_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;
	Eon_Keyboard_Proxy *kp;

	thiz = calloc(1, sizeof(Eon_Button_Base));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;

	pdescriptor.initialize = _eon_button_base_initialize;
	pdescriptor.geometry_set = descriptor->geometry_set;
	pdescriptor.free = _eon_button_base_free;
	pdescriptor.name = descriptor->name;
	pdescriptor.hints_get = descriptor->hints_get;
	pdescriptor.child_at = descriptor->child_at;
	pdescriptor.pass_events = descriptor->pass_events;

	ee = eon_bin_new(theme, &pdescriptor, thiz);
	if (!ee) goto renderer_err;

	kp = eon_keyboard_proxy_focus_new();
        eon_element_keyboard_proxy_set(ee, kp);

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
