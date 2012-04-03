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
#include "Eon_Theme.h"
#include "eon_theme_private.h"
/* TODO
 * - Add a notification whenever the theme has changed
 * - A widget should have several default states:
 *  - focus in/focus out/enabled/disabled/etc
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_WIDGET_MAGIC)
#define EON_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_WIDGET_MAGIC, ret)

static Ender_Property *EON_WIDGET_THEME;
static Ender_Property *EON_WIDGET_ENABLED;

typedef struct _Eon_Widget_Extend
{
	Ender_Element *e;
	/* FIXME this should be a list */
	Ender_Property *property;
	Ender_Element *other;
	/* FIXME this should be a list */
	Ender_Property *o_property;
} Eon_Widget_Extend;

/*
 * This is a wrapper of Escen, a simple interface to interact with the
 * underlying theme system
 */
typedef struct _Eon_Widget_Theme
{
	Escen_Ender *ender;
	Escen_Instance *instance;
	Ender_Element *element;
	Enesim_Renderer *renderer;
} Eon_Widget_Theme;

typedef struct _Eon_Widget
{
	EINA_MAGIC
	/* properties */
	char *theme_file;
	Eina_Bool enabled;
	/* private */
	/* the theme data */
	Eon_Widget_Theme theme;
	Escen_State *disabled;

	/* interface */
	Eon_Widget_Min_Width_Get min_width_get;
	Eon_Widget_Max_Width_Get max_width_get;
	Eon_Widget_Min_Height_Get min_height_get;
	Eon_Widget_Max_Height_Get max_height_get;
	Eon_Widget_Preferred_Width_Get preferred_width_get;
	Eon_Widget_Preferred_Height_Get preferred_height_get;

	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Free free;
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

static Eina_Bool _eon_widget_theme_setup(Eon_Widget *thiz, Ender_Element *e, Escen *theme_escen)
{
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
	const char *name;

	/* remove the theme already associated with the element
	 * and set this, get the correct escen_ender and set
	 * the current state
	 */
	name = ender_element_name_get(e);
	theme_ender = escen_ender_get(theme_escen, name);
	if (!theme_ender)
	{
		printf("no ender %s\n", name);
		return EINA_FALSE;
	}

	theme_instance = escen_instance_new(theme_ender);
	if (!theme_instance)
	{
		printf("no instance %s\n", name);
		return EINA_FALSE;
	}

	theme_element = escen_instance_ender_get(theme_instance);
	theme_renderer = ender_element_object_get(theme_element);

	thiz->theme.ender = theme_ender;
	thiz->theme.instance = theme_instance;
	thiz->theme.element = theme_element;
	thiz->theme.renderer = theme_renderer;

	return EINA_TRUE;
}

static void _widget_focus_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	eon_widget_state_set(ee, "focus_in", EINA_FALSE);
}

static void _widget_focus_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	eon_widget_state_set(ee, "focus_out", EINA_TRUE);
}

static void _widget_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	if (!thiz->enabled) return;
	eon_widget_state_set(ee, "mouse_in", EINA_FALSE);
}

static void _widget_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Widget *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	if (!thiz->enabled) return;
	eon_widget_state_set(ee, "mouse_out", EINA_TRUE);
}
/*----------------------------------------------------------------------------*
 *                        The Theme extension functions                       *
 *----------------------------------------------------------------------------*/
/* whenever we register the new properties, we register this function dummy */
static void _eon_widget_extend_accessor(Ender_Element *e, Ender_Property *prop,
		Ender_Value *v, void *data)
{
	Eon_Widget_Extend *extend = data;

	/* FIXME use the property directly */
	printf("=>>>> setting property %s\n", ender_property_name_get(prop));
	ender_element_value_set_simple(extend->other,
			ender_property_name_get(prop),
			v);
}

static void _eon_widget_extend_properties(Ender_Property *prop, void *data)
{
	Eon_Widget_Extend *extend = data;
	Ender_Container *c;

	printf("new property %s\n", ender_property_name_get(prop));
	c = ender_property_container_get(prop);
	/* duplicate theme on thiz */
	extend->property = ender_element_property_add(extend->e, ender_property_name_get(prop),
			c,
			NULL,
			_eon_widget_extend_accessor,
			_eon_widget_extend_accessor,
			_eon_widget_extend_accessor,
			NULL,
			EINA_FALSE,
			extend);
}

/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_widget_free(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	if (thiz->free)
		thiz->free(ee);
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
	if (thiz->initialize)
		thiz->initialize(e);
}

static Eina_Bool _eon_widget_needs_setup(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	Eina_Bool ret = EINA_FALSE;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);

	/* first check if the theme associated with this widget needs a setup */
	ret = eon_theme_widget_informs_setup(thiz->theme.renderer);
	if (ret) return ret;

	/* call the change interface */
	if (thiz->needs_setup)
		return thiz->needs_setup(e);
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
	enesim_renderer_rop_set(thiz->theme.renderer, ENESIM_BLEND);
	if (thiz->setup)
		thiz->setup(e, state, s, err);
	eon_theme_widget_setup(thiz->theme.renderer, err);

	return EINA_TRUE;
}

static Enesim_Renderer * _eon_widget_renderer_get(Ender_Element *ender)
{
	Eon_Element *ee;
	Eon_Widget *thiz;

	ee = ender_element_object_get(ender);
	thiz = _eon_widget_get(ee);
	return thiz->theme.renderer;
}

static void _eon_widget_actual_width_set(Eon_Element *ee, double width)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	ender_element_value_set(thiz->theme.element, "width", width, NULL);
}

static void _eon_widget_actual_height_set(Eon_Element *ee, double height)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	ender_element_value_set(thiz->theme.element, "height", height, NULL);
}

static void _eon_widget_actual_y_set(Eon_Element *ee, double y)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	ender_element_value_set(thiz->theme.element, "y", y, NULL);
}

static void _eon_widget_actual_x_set(Eon_Element *ee, double x)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	ender_element_value_set(thiz->theme.element, "x", x, NULL);
}

static double _eon_widget_min_width_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	if (thiz->min_width_get)
		v = thiz->min_width_get(e, thiz->theme.renderer);
	return v;
}

static double _eon_widget_min_height_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	if (thiz->min_height_get)
		v = thiz->min_height_get(e, thiz->theme.renderer);
	return v;
}

static double _eon_widget_max_width_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	double v = DBL_MAX;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	if (thiz->max_width_get)
		v = thiz->max_width_get(e, thiz->theme.renderer);
	return v;
}

static double _eon_widget_max_height_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	double v = DBL_MAX;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	if (thiz->max_height_get)
		v = thiz->max_height_get(e, thiz->theme.renderer);
	return v;
}

static double _eon_widget_preferred_width_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	double v = -1;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	if (thiz->preferred_width_get)
		v = thiz->preferred_width_get(e, thiz->theme.renderer);
	return v;
}

static double _eon_widget_preferred_height_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Eon_Element *ee;
	double v = -1;

	ee = ender_element_object_get(e);
	thiz = _eon_widget_get(ee);
	if (thiz->preferred_height_get)
		v = thiz->preferred_height_get(e, thiz->theme.renderer);
	return v;
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
static void _eon_widget_theme_set(Eon_Element *ee, const char *file)
{
	Eon_Widget *thiz;
	Ender_Element *e;
	Ender_Element *old_element;
	Escen_Instance *old_instance;
	Escen *theme_escen;

	thiz = _eon_widget_get(ee);
	e = thiz->e;
	theme_escen = eon_theme_get_from_file(file);
	if (!theme_escen)
	{
		printf("no valid file %s\n", file);
		return;
	}

	old_element = thiz->theme.element;
	old_instance = thiz->theme.instance;

	_eon_widget_theme_setup(thiz, e, theme_escen);
	/* FIXME delete the old instance/element */

	thiz->theme_file = strdup(file);
}

static void _eon_widget_theme_get(Eon_Element *ee, const char **file)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	*file = thiz->theme_file;
}

static void _eon_widget_enabled_set(Eon_Element *ee, Eina_Bool enabled)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	if (thiz->enabled == enabled)
		return;

	thiz->enabled = enabled;
	if (enabled)
	{
		eon_widget_state_set(ee, "enabled", EINA_FALSE);
	}
	else
	{
		eon_widget_state_set(ee, "disabled", EINA_FALSE);

	}
}

static void _eon_widget_enabled_get(Eon_Element *ee, Eina_Bool *enabled)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	*enabled = thiz->enabled;
}

#include "eon_generated_widget.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Element * eon_widget_new(Eon_Widget_Descriptor *descriptor, void *data)
{
	Eon_Widget *thiz;
	Eon_Element_Descriptor pdescriptor = { 0 };
	Escen *theme_escen;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
	Eon_Element *ee;

	if (!descriptor)
		return NULL;

	theme_escen = eon_theme_get();
	if (!theme_escen) {
		printf("no theme\n");
		return NULL;
	}

	theme_ender = escen_ender_get(theme_escen, descriptor->name);
	if (!theme_ender)
	{
		printf("no ender %s\n", descriptor->name);
		return NULL;
	}

	theme_instance = escen_instance_new(theme_ender);
	if (!theme_instance)
	{
		printf("no instance %s\n", descriptor->name);
		return NULL;
	}

	theme_element = escen_instance_ender_get(theme_instance);
	theme_renderer = ender_element_object_get(theme_element);

	thiz = calloc(1, sizeof(Eon_Widget));
	EINA_MAGIC_SET(thiz, EON_WIDGET_MAGIC);
	/* default values */
	thiz->enabled = EINA_TRUE;

	/* the interface */
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;
	thiz->theme.ender = theme_ender;
	thiz->theme.instance = theme_instance;
	thiz->theme.element = theme_element;
	thiz->theme.renderer = theme_renderer;
	thiz->setup = descriptor->setup;
	thiz->needs_setup = descriptor->needs_setup;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->max_width_get = descriptor->max_width_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->max_height_get = descriptor->max_height_get;
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;

	pdescriptor.initialize = _eon_widget_initialize;
	pdescriptor.setup = _eon_widget_setup;
	pdescriptor.renderer_get = _eon_widget_renderer_get;
	pdescriptor.needs_setup = _eon_widget_needs_setup;
	pdescriptor.min_width_get = _eon_widget_min_width_get;
	pdescriptor.max_width_get = _eon_widget_max_width_get;
	pdescriptor.min_height_get = _eon_widget_min_height_get;
	pdescriptor.max_height_get = _eon_widget_max_height_get;
	pdescriptor.preferred_width_get = _eon_widget_preferred_width_get;
	pdescriptor.preferred_height_get = _eon_widget_preferred_height_get;
	pdescriptor.actual_x_set = _eon_widget_actual_x_set;
	pdescriptor.actual_y_set = _eon_widget_actual_y_set;
	pdescriptor.actual_width_set = _eon_widget_actual_width_set;
	pdescriptor.actual_height_set = _eon_widget_actual_height_set;
	pdescriptor.is_focusable = _eon_widget_is_focusable;

	pdescriptor.free = _eon_widget_free;
	pdescriptor.name = descriptor->name;

	ee = eon_element_new(&pdescriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_widget_data_get(Eon_Element *ee)
{
	Eon_Widget *e;

	e = _eon_widget_get(ee);
	if (!e) return NULL;

	return e->data;
}

void eon_widget_property_set(Eon_Element *ee, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(ee);
	va_start(va_args, name);
	ender_element_value_set_valist(thiz->theme.element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_get(Eon_Element *ee, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(ee);
	va_start(va_args, name);
	ender_element_value_get_valist(thiz->theme.element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_add(Eon_Element *ee, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(ee);
	va_start(va_args, name);
	ender_element_value_add_valist(thiz->theme.element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_remove(Eon_Element *ee, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(ee);
	va_start(va_args, name);
	ender_element_value_remove_valist(thiz->theme.element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_clear(Eon_Element *ee, const char *name)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	ender_element_value_clear(thiz->theme.element, name);
}

void eon_widget_state_set(Eon_Element *ee, const char *name, Eina_Bool be_finalized)
{
	Eon_Widget *thiz;
	Escen_State *new_state;

	thiz = _eon_widget_get(ee);
	if (be_finalized && !escen_instance_current_state_finalized(thiz->theme.instance))
	{
		printf("state didnt finalize, not setting a new one\n");
		return;
	}
	new_state = escen_ender_state_get(thiz->theme.ender, name);
	if (!new_state) return;

	escen_instance_state_set(thiz->theme.instance, new_state);
}

Escen_State * eon_widget_state_get(Eon_Element *ee, const char *name)
{
	Eon_Widget *thiz;
	Escen_State *state;

	thiz = _eon_widget_get(ee);
	state = escen_ender_state_get(thiz->theme.ender, name);
	return state;
}

Escen_Ender * eon_widget_theme_ender_get(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	return thiz->theme.ender;
}

Enesim_Renderer * eon_widget_theme_renderer_get(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	return thiz->theme.renderer;
}

Ender_Element * eon_widget_theme_element_get(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	return thiz->theme.element;
}

Escen_Instance * eon_widget_theme_instance_get(Eon_Element *ee)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(ee);
	return thiz->theme.instance;
}

void eon_widget_theme_extend(Eon_Element *ee, Ender_Element *e)
{
	Eon_Widget_Extend *extend;
	Eon_Widget *thiz;
	Eon_Widget *other;
	Eon_Element *other_e;
	Ender_Descriptor *d;

	if (!eon_is_widget(e))
		return;

	thiz = _eon_widget_get(ee);
	other_e = ender_element_object_get(e);
	other = _eon_widget_get(other_e);

	d = ender_element_descriptor_get(other->theme.element);
	/* iterate over the list of properties to add a new property on theme element */
	extend = calloc(1, sizeof(Eon_Widget_Extend));
	extend->e = thiz->theme.element;
	extend->other = other->theme.element;
	ender_descriptor_property_list(d, _eon_widget_extend_properties, extend);
	/* and register a mutation event that will send the properties to the correct theme element */
	/* TODO store the extend information */
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

