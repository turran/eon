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
#include "eon_widget.h"

#include "eon_private_input.h"
#include "eon_private_keyboard_proxy.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"

#include "theme/eon_theme_widget.h"
/* TODO
 * - Add a notification whenever the theme has changed
 * - A widget should have several default states:
 *  - focus in/focus out/enabled/disabled/etc
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_WIDGET_MAGIC 0xe0400003
#define EON_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_WIDGET_MAGIC)
#define EON_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_WIDGET_MAGIC, ret)

static Ender_Property *EON_WIDGET_THEME;
static Ender_Property *EON_WIDGET_ENABLED;

typedef struct _Eon_Widget_Descriptor_Internal
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Free free;
	Eon_Element_Geometry_Set geometry_set;
	Eon_Widget_Hints_Get hints_get;
} Eon_Widget_Descriptor_Internal;

typedef struct _Eon_Widget
{
	EINA_MAGIC
	/* properties */
	Eina_Bool enabled;
	/* the theme data */
	Eon_Theme_Instance *theme;
	Escen_State *disabled;
	/* interface */
	Eon_Widget_Descriptor_Internal descriptor;
	/* private */
	void *data;
	Ender_Element *e;
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Eon_Element *ee)
{
	Eon_Widget *thiz;

	if (!ee) return NULL;
	thiz = eon_element_data_get(ee);
	EON_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_widget_theme_changed(Ender_Element *e)
{
	printf("the theme changed!!!\n");
}

static void _widget_focus_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	eon_theme_instance_state_set(thiz->theme, "focus_in", EINA_FALSE);
}

static void _widget_focus_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	eon_theme_instance_state_set(thiz->theme, "focus_out", EINA_TRUE);
}

static void _widget_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	if (!thiz->enabled) return;
	eon_theme_instance_state_set(thiz->theme, "mouse_in", EINA_FALSE);
}

static void _widget_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	if (!thiz->enabled) return;
	eon_theme_instance_state_set(thiz->theme, "mouse_out", EINA_TRUE);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_widget_free(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	if (thiz->descriptor.free)
		thiz->descriptor.free(ee);
	free(thiz);
}

static void _eon_widget_initialize(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	thiz->e = e;

	ender_event_listener_add(e, "FocusIn", _widget_focus_in, NULL);
	ender_event_listener_add(e, "FocusOut", _widget_focus_out, NULL);
	ender_event_listener_add(e, "MouseIn", _widget_mouse_in, NULL);
	ender_event_listener_add(e, "MouseOut", _widget_mouse_out, NULL);

	/* register every needed callback */
	if (thiz->descriptor.initialize)
		thiz->descriptor.initialize(e);
}

static void _eon_widget_hints_get(Eon_Element *e, Eon_Hints *hints)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(e);
	if (thiz->descriptor.hints_get)
	{
		thiz->descriptor.hints_get(e, thiz->theme, hints);
	}
}

#if 0
static Eina_Bool _eon_widget_needs_setup(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	Eina_Bool ret = EINA_FALSE;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	/* first check if the theme associated with this widget needs a setup */
	ret = eon_theme_widget_informs_setup(thiz->theme->renderer);
	if (ret) return ret;

	/* call the change interface */
	if (thiz->descriptor.needs_setup)
		return thiz->descriptor.needs_setup(e);
	return ret;
}

static Eina_Bool _eon_widget_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	enesim_renderer_rop_set(thiz->theme->renderer, ENESIM_BLEND);
	if (thiz->descriptor.setup)
		thiz->descriptor.setup(e, state, s, err);
	eon_theme_widget_setup(thiz->theme->renderer, err);

	return EINA_TRUE;
}
#endif

static Enesim_Renderer * _eon_widget_renderer_get(Ender_Element *ender)
{
	Eon_Element *ee;
	Eon_Widget *thiz;

	ee = ender_element_object_get(ender);
	thiz = _eon_widget_get(ee);
	return thiz->theme->renderer;
}

static void _eon_widget_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(e);
	eon_theme_instance_property_set(thiz->theme, "x", g->x, NULL);
	eon_theme_instance_property_set(thiz->theme, "y", g->y, NULL);
	eon_theme_instance_property_set(thiz->theme, "width", g->width, NULL);
	eon_theme_instance_property_set(thiz->theme, "height", g->height, NULL);
	if (thiz->descriptor.geometry_set)
		thiz->descriptor.geometry_set(e, g);
}

static Eina_Bool _eon_widget_is_focusable(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	/* TODO maybe the widget might need to fine tune this? */
	return thiz->enabled;
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_widget_enabled_set(Eon_Element *ee, Eina_Bool enabled)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	if (thiz->enabled == enabled)
		return;

	thiz->enabled = enabled;
	if (enabled)
	{
		eon_theme_instance_state_set(thiz->theme, "enabled", EINA_FALSE);
	}
	else
	{
		eon_theme_instance_state_set(thiz->theme, "disabled", EINA_FALSE);

	}
}

static void _eon_widget_enabled_get(Eon_Element *ee, Eina_Bool *enabled)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	*enabled = thiz->enabled;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_widget_delete NULL
#define _eon_widget_theme_set NULL
#define _eon_widget_theme_get NULL
#include "eon_generated_widget.c"

Eon_Element * eon_widget_new(Eon_Theme_Instance *theme,
		Eon_Widget_Descriptor *descriptor, void *data)
{
	Eon_Widget *thiz;
	Eon_Element_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;

	if (!descriptor || !theme)
		return NULL;

	thiz = calloc(1, sizeof(Eon_Widget));
	EINA_MAGIC_SET(thiz, EON_WIDGET_MAGIC);
	/* default values */
	thiz->enabled = EINA_TRUE;
	thiz->theme = theme;
	/* the interface */
	thiz->data = data;
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.initialize = descriptor->initialize;
	thiz->descriptor.setup = descriptor->setup;
	thiz->descriptor.needs_setup = descriptor->needs_setup;
	thiz->descriptor.geometry_set = descriptor->geometry_set;
	thiz->descriptor.hints_get = descriptor->hints_get;

	pdescriptor.initialize = _eon_widget_initialize;
	pdescriptor.backend_added = descriptor->backend_added;
	pdescriptor.backend_removed = descriptor->backend_removed;
	pdescriptor.setup = NULL;
	pdescriptor.renderer_get = _eon_widget_renderer_get;
	pdescriptor.needs_setup = NULL;
	pdescriptor.hints_get = _eon_widget_hints_get;
	pdescriptor.geometry_set = _eon_widget_geometry_set;
	pdescriptor.is_focusable = _eon_widget_is_focusable;
	pdescriptor.free = _eon_widget_free;
	pdescriptor.name = descriptor->name;

	ee = eon_element_new(&pdescriptor, thiz);
	if (!ee) goto base_err;

	/* set the theme notification callback */
	eon_theme_widget_size_changed_cb_set(thiz->theme->object,
		EON_THEME_WIDGET_SIZE_CHANGED_CB(_eon_widget_theme_changed),
		ee);

	return ee;

base_err:
	free(thiz);
	return NULL;
}

void * eon_widget_data_get(Eon_Element *e)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(e);
	return thiz->data;
}

/* TODO in case we want to add support for the _set variant
 * we need a way to inform every implementation that a new
 * theme has been set and thus reset aany property from the old
 * instance to the new one
 */
Eon_Theme_Instance * eon_widget_theme_instance_get(Eon_Element *e)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(e);
	return thiz->theme;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_widget(Ender_Element *e)
{
	Eon_Element *ee;
	Eon_Widget *thiz;

	ee = ender_element_object_get(e);
	thiz = eon_element_data_get(ee);
	if (!EINA_MAGIC_CHECK(thiz, EON_WIDGET_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_theme_set(Ender_Element *e, const char *file)
{
	ender_element_property_value_set(e, EON_WIDGET_THEME, file, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_enabled_set(Ender_Element *e, Eina_Bool enabled)
{
	ender_element_property_value_set(e, EON_WIDGET_ENABLED, enabled, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_enabled_get(Ender_Element *e, Eina_Bool *enabled)
{
	ender_element_property_value_get(e, EON_WIDGET_ENABLED, enabled, NULL);
}

