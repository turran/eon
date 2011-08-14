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
typedef struct _Eon_Button_Base
{
	/* properties */
	Eina_Bool enabled;
	/* private */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	void *data;
} Eon_Button_Base;

static inline Eon_Button_Base * _eon_button_base_get(Enesim_Renderer *r)
{
	Eon_Button_Base *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static void _button_base_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	if (!thiz->enabled) return;
	eon_widget_state_set(r, "mouse_in", EINA_FALSE);
}

static void _button_base_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	if (!thiz->enabled) return;
	eon_widget_state_set(r, "mouse_out", EINA_TRUE);
}

static void _button_base_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	if (!thiz->enabled) return;
	eon_widget_state_set(r, "mouse_down", EINA_FALSE);
}

static void _button_base_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	if (!thiz->enabled) return;
	eon_widget_state_set(r, "mouse_up", EINA_FALSE);
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
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);
	ender_event_listener_add(e, "MouseIn", _button_base_mouse_in, NULL);
	ender_event_listener_add(e, "MouseOut", _button_base_mouse_out, NULL);
	ender_event_listener_add(e, "MouseDown", _button_base_mouse_down, NULL);
	ender_event_listener_add(e, "MouseUp", _button_base_mouse_up, NULL);
	if (thiz->initialize)
		thiz->initialize(e);
}

/* TODO if we allow that the button base elements (radio, checkbox, button, whatever)
 * can be bigger than its contents maximum size, then we need to add another function
 * beside the margin. Something like what we have before, a position_get or something
 * like that that should receive also the size of the content, this way we can center
 * left-align or whatever the content itself when it the container area is bigger
 * than the content. Note that, for some cases we might need also to say that the
 * button_base should or should not scale its content if the size of the content
 * is set to -1. Right now we always use the preferred size, but it might not be
 * the case with this flag
 */

static double _eon_button_base_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Eon_Margin margin;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_button_base_margin_get(theme_r, &margin);

	return cmv + margin.top + margin.bottom;
}

static double _eon_button_base_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Eon_Margin margin;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
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

static Eina_Bool _eon_button_base_setup(Ender_Element *e)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;
	Ender_Element *content;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);
	eon_container_content_get(e, &content);
	if (content)
	{
		Eon_Margin margin;
		Eon_Position position;
		Eon_Size size;
		Enesim_Renderer *content_r;
		Enesim_Renderer *theme_r;
		double aw, ah;
		double ax, ay;

		content_r = ender_element_renderer_get(content);
		theme_r = eon_widget_theme_renderer_get(r);
		/* set the size and position of the content */
		eon_element_actual_width_get(e, &aw);
		eon_element_actual_height_get(e, &ah);
		eon_element_actual_position_get(r, &ax, &ay);

		eon_theme_button_base_margin_get(theme_r, &margin);
		size.width = aw - margin.left - margin.right;
		size.height = ah - margin.top - margin.bottom;

		eon_element_real_relative_size_get(content, &size, &size);
		eon_element_actual_size_set(content_r, size.width, size.height);
		eon_theme_button_base_position_get(theme_r, &size, &position);
		//eon_element_actual_position_set(content_r, ax + margin.left, ay + margin.top);
		eon_element_actual_position_set(content_r, position.x, position.y);
		if (!eon_element_setup(content))
		{
			printf("impossible to setup the content\n");
			return EINA_FALSE;
		}
	}
	if (thiz->setup)
		return thiz->setup(e);

	return EINA_TRUE;
}

static void _eon_button_base_free(Enesim_Renderer *r)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_button_base_enabled_set(Enesim_Renderer *r, Eina_Bool enabled)
{
	Eon_Button_Base *thiz;
	Ender_Element *e;

	thiz = _eon_button_base_get(r);
	if (thiz->enabled == enabled) return;

	thiz->enabled = enabled;
	e = ender_element_renderer_from(r);
	if (enabled)
	{
		eon_widget_state_set(r, "enabled", EINA_FALSE);
	}
	else
	{
		eon_widget_state_set(r, "disabled", EINA_FALSE);

	}
}

static void _eon_button_base_enabled_get(Enesim_Renderer *r, Eina_Bool *enabled)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(r);
	*enabled = thiz->enabled;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_button_base_data_get(Enesim_Renderer *r)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(r);
	if (!thiz) return NULL;
	return thiz->data;
}

Enesim_Renderer * eon_button_base_new(Eon_Button_Base_Descriptor *descriptor, void *data)
{
	Eon_Button_Base *thiz;
	Eon_Container_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Button_Base));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;
	thiz->enabled = EINA_TRUE;

	pdescriptor.initialize = _eon_button_base_initialize;
	pdescriptor.free = _eon_button_base_free;
	pdescriptor.setup = _eon_button_base_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.min_width_get = _eon_button_base_min_width_get;
	pdescriptor.max_width_get = _eon_button_base_max_width_get;
	pdescriptor.min_height_get = _eon_button_base_min_height_get;
	pdescriptor.max_height_get = _eon_button_base_max_height_get;

	r = eon_container_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

#include "eon_generated_button_base.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_button_base_enabled_set(Ender_Element *e, Eina_Bool enabled)
{
	ender_element_value_set(e, "enabled", enabled, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_button_base_enabled_get(Ender_Element *e, Eina_Bool *enabled)
{
	ender_element_value_get(e, "enabled", enabled, NULL);
}

