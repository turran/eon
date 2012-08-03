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
#include "eon_container.h"
#include "eon_bin.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
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
	Eon_Element_Setup setup;
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
	Eina_Bool enabled;

	ee = ender_element_object_get(e);

	eon_widget_enabled_get(e, &enabled);
	if (!enabled) return;
	eon_widget_state_set(ee, "mouse_down", EINA_FALSE);
}

static void _button_base_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Element *ee;
	Eina_Bool enabled;

	ee = ender_element_object_get(e);

	eon_widget_enabled_get(e, &enabled);
	if (!enabled) return;
	eon_widget_state_set(ee, "mouse_up", EINA_FALSE);
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

/* TODO if we allow that the button base elements (radio, checkbox, button, whatever)
 * can be bigger than its contents maximum size, then we need to add another function
 * beside the margin. Something like what we have before, a position_get or something
 * like that that should receive also the size of the content, this way we can center
 * left-align or whatever the content itself when it the bin area is bigger
 * than the content. Note that, for some cases we might need also to say that the
 * button_base should or should not scale its content if the size of the content
 * is set to -1. Right now we always use the preferred size, but it might not be
 * the case with this flag
 */

static double _eon_button_base_min_preferred_width_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Eon_Margin margin;
	Enesim_Renderer *theme_r;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_button_base_get(ee);

	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_button_base_margin_get(theme_r, &margin);

	return cmv + margin.left + margin.right;
}

static double _eon_button_base_min_preferred_height_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Eon_Margin margin;
	Enesim_Renderer *theme_r;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_button_base_get(ee);

	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_button_base_margin_get(theme_r, &margin);

	return cmv + margin.top + margin.bottom;
}

static double _eon_button_base_max_width_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static double _eon_button_base_max_height_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static Eina_Bool _eon_button_base_content_relayout(Ender_Element *content,
		Enesim_Renderer *theme_r,
		double ax,
		double ay,
		double aw,
		double ah,
		Enesim_Surface *s,
		Enesim_Error **err)
{
	Eon_Element *content_e;
	Eon_Margin margin;
	Eon_Size size;
	Eon_Position position;

	content_e = ender_element_object_get(content);
	eon_theme_button_base_margin_get(theme_r, &margin);
	size.width = aw - margin.left - margin.right;
	size.height = ah - margin.top - margin.bottom;

	eon_element_real_relative_size_get(content, &size, &size);
	eon_element_actual_size_set(content_e, size.width, size.height);
	eon_theme_button_base_position_get(theme_r, &size, &position);
	eon_element_actual_position_set(content_e, ax + position.x, ay + position.y);
	if (!eon_element_setup(content, s, err))
	{
		printf("impossible to setup the content\n");
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static Eina_Bool _eon_button_base_setup(Ender_Element *e,
		const Eon_Element_State *state,
		const Eon_Bin_State *cstate,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Button_Base *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_button_base_get(ee);
	if (cstate->content)
	{
		Enesim_Renderer *theme_r;
		Eina_Bool ret;

		theme_r = eon_widget_theme_renderer_get(ee);
		ret =_eon_button_base_content_relayout(cstate->content, theme_r,
				state->actual_position.x,
				state->actual_position.y,
				state->actual_size.width,
				state->actual_size.height,
				s,
				err);
		if (!ret)
		{
			return ret;
		}

	}
	if (thiz->setup)
		return thiz->setup(e, state, s, err);

	return EINA_TRUE;
}

static void _eon_button_base_free(Eon_Element *ee)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(ee);
	if (thiz->free)
		thiz->free(ee);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_button_base_data_get(Eon_Element *ee)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(ee);
	if (!thiz) return NULL;
	return thiz->data;
}

Eon_Element * eon_button_base_new(Eon_Button_Base_Descriptor *descriptor, void *data)
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
	pdescriptor.free = _eon_button_base_free;
	pdescriptor.setup = _eon_button_base_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.min_width_get = _eon_button_base_min_preferred_width_get;
	pdescriptor.max_width_get = _eon_button_base_max_width_get;
	pdescriptor.min_height_get = _eon_button_base_min_preferred_height_get;
	pdescriptor.max_height_get = _eon_button_base_max_height_get;
	pdescriptor.preferred_width_get = _eon_button_base_min_preferred_width_get;
	pdescriptor.preferred_height_get = _eon_button_base_min_preferred_height_get;

	ee = eon_bin_new(&pdescriptor, thiz);
	if (!ee) goto renderer_err;

	kp = eon_keyboard_proxy_focus_new();
        eon_element_keyboard_proxy_set(ee, kp);

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

#define _eon_button_base_delete NULL
#include "eon_generated_button_base.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
