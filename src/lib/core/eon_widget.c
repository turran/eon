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
 * Add a notification whenever the theme has changed
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_WIDGET_MAGIC)
#define EON_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_WIDGET_MAGIC, ret)

static Ender_Property *EON_WIDGET_THEME;

typedef struct _Eon_Widget
{
	EINA_MAGIC
	/* properties */
	char *theme;
	/* private */
	/* the theme data */
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;

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
	Enesim_Renderer_Delete free;
	void *data;
	Ender_Element *e;
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	if (!r) return NULL;
	thiz = eon_element_data_get(r);
	EON_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_widget_free(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

static void _eon_widget_initialize(Ender_Element *ender)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(ender);
	thiz = _eon_widget_get(r);
	thiz->e = ender;
	/* register every needed callback */
	if (thiz->initialize)
		thiz->initialize(ender);
}

static Eina_Bool _eon_widget_needs_setup(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	Eina_Bool ret = EINA_FALSE;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);

	/* first check if the theme associated with this widget needs a setup */
	ret = eon_theme_widget_informs_setup(thiz->theme_renderer);
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
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	enesim_renderer_rop_set(thiz->theme_renderer, ENESIM_BLEND);
	if (thiz->setup)
		thiz->setup(e, state, s, err);
	return EINA_TRUE;
}

static Enesim_Renderer * _eon_widget_renderer_get(Ender_Element *ender)
{
	Enesim_Renderer *r;
	Eon_Widget *thiz;

	r = ender_element_object_get(ender);
	thiz = _eon_widget_get(r);
	return thiz->theme_renderer;
}

static void _eon_widget_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	ender_element_value_set(thiz->theme_element, "width", width, NULL);
}

static void _eon_widget_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	ender_element_value_set(thiz->theme_element, "height", height, NULL);
}

static void _eon_widget_actual_y_set(Enesim_Renderer *r, double y)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	ender_element_value_set(thiz->theme_element, "y", y, NULL);
}

static void _eon_widget_actual_x_set(Enesim_Renderer *r, double x)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	ender_element_value_set(thiz->theme_element, "x", x, NULL);
}

static double _eon_widget_min_width_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	if (thiz->min_width_get)
		v = thiz->min_width_get(e, thiz->theme_renderer);
	return v;
}

static double _eon_widget_min_height_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	if (thiz->min_height_get)
		v = thiz->min_height_get(e, thiz->theme_renderer);
	return v;
}

static double _eon_widget_max_width_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	double v = DBL_MAX;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	if (thiz->max_width_get)
		v = thiz->max_width_get(e, thiz->theme_renderer);
	return v;
}

static double _eon_widget_max_height_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	double v = DBL_MAX;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	if (thiz->max_height_get)
		v = thiz->max_height_get(e, thiz->theme_renderer);
	return v;
}

static double _eon_widget_preferred_width_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	double v = -1;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	if (thiz->preferred_width_get)
		v = thiz->preferred_width_get(e, thiz->theme_renderer);
	return v;
}

static double _eon_widget_preferred_height_get(Ender_Element *e)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;
	double v = -1;

	r = ender_element_object_get(e);
	thiz = _eon_widget_get(r);
	if (thiz->preferred_height_get)
		v = thiz->preferred_height_get(e, thiz->theme_renderer);
	return v;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_widget_new(Eon_Widget_Descriptor *descriptor, void *data)
{
	Eon_Widget *thiz;
	Eon_Element_Descriptor pdescriptor = { 0 };
	Escen *theme_escen;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
	Enesim_Renderer *r;
	Enesim_Color color;

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
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;
	thiz->theme_ender = theme_ender;
	thiz->theme_instance = theme_instance;
	thiz->theme_element = theme_element;
	thiz->theme_renderer = theme_renderer;
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
	pdescriptor.cleanup = descriptor->cleanup;
	pdescriptor.renderer_get = _eon_widget_renderer_get;
	pdescriptor.damage = descriptor->damage;
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

	pdescriptor.free = _eon_widget_free;
	pdescriptor.name = descriptor->name;

	r = eon_element_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	/* FIXME Set the default properties from the state */
	enesim_renderer_color_get(theme_renderer, &color);
	enesim_renderer_color_set(r, color);

	return r;

renderer_err:
	free(thiz);
	return NULL;
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

	thiz->theme_ender = theme_ender;
	thiz->theme_instance = theme_instance;
	thiz->theme_element = theme_element;
	thiz->theme_renderer = theme_renderer;

	return EINA_TRUE;
}

static void _eon_widget_theme_set(Enesim_Renderer *r, const char *file)
{
	Eon_Widget *thiz;
	Ender_Element *e;
	Ender_Element *old_element;
	Escen_Instance *old_instance;
	Escen *theme_escen;

	thiz = _eon_widget_get(r);
	e = thiz->e;
	theme_escen = eon_theme_get_from_file(file);
	if (!theme_escen)
	{
		printf("no valid file %s\n", file);
		return;
	}

	old_element = thiz->theme_element;
	old_instance = thiz->theme_instance;

	_eon_widget_theme_setup(thiz, e, theme_escen);
	/* FIXME delete the old instance/element */

	thiz->theme = strdup(file);
}

static void _eon_widget_theme_get(Enesim_Renderer *r, const char **file)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	*file = thiz->theme;
}

#include "eon_generated_widget.c"

void * eon_widget_data_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = _eon_widget_get(r);
	if (!e) return NULL;

	return e->data;
}

void eon_widget_property_set(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(r);
	va_start(va_args, name);
	ender_element_value_set_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_get(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(r);
	va_start(va_args, name);
	ender_element_value_get_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_add(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(r);
	va_start(va_args, name);
	ender_element_value_add_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_remove(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	va_list va_args;

	thiz = _eon_widget_get(r);
	va_start(va_args, name);
	ender_element_value_remove_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_property_clear(Enesim_Renderer *r, const char *name)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	ender_element_value_clear(thiz->theme_element, name);
}

void eon_widget_state_set(Enesim_Renderer *r, const char *name, Eina_Bool be_finalized)
{
	Eon_Widget *thiz;
	Escen_State *new_state;

	thiz = _eon_widget_get(r);
	if (be_finalized && !escen_instance_current_state_finalized(thiz->theme_instance))
	{
		printf("state didnt finalize, not setting a new one\n");
		return;
	}
	new_state = escen_ender_state_get(thiz->theme_ender, name);
	if (!new_state) return;

	escen_instance_state_set(thiz->theme_instance, new_state);
}

Escen_Ender * eon_widget_theme_ender_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	return thiz->theme_ender;
}

Enesim_Renderer * eon_widget_theme_renderer_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	return thiz->theme_renderer;
}

Ender_Element * eon_widget_theme_element_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	return thiz->theme_element;
}

Escen_Instance * eon_widget_theme_instance_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	return thiz->theme_instance;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_widget(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = eon_element_data_get(r);
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
	ender_element_value_set(e, "theme", file, NULL);
}