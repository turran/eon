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
/*
 * This is a wrapper of Escen, a simple interface to interact with the
 * underlying theme system
 */

/* TODO
 * Add a notification whenever the theme has changed
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Widget_Theme Eon_Widget_Theme;

typedef struct _Eon_Widget_Theme
{
	/* the theme data */
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Widget_Theme * eon_widget_theme_new(const char *name)
{
	Eon_Widget_Theme *thiz;
	Escen *theme_escen;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;

	theme_escen = eon_theme_get();
	if (!theme_escen) {
		printf("no theme\n");
		goto err_theme;
	}

	theme_ender = escen_ender_get(theme_escen, name);
	if (!theme_ender)
	{
		printf("no ender %s\n", name);
		goto err_theme;
	}

	theme_instance = escen_instance_new(theme_ender);
	if (!theme_instance)
	{
		printf("no instance %s\n", name);
		goto err_instance;
	}

	theme_element = escen_instance_ender_get(theme_instance);
	theme_renderer = ender_element_object_get(theme_element);

	thiz = calloc(1, sizeof(Eon_Widget_Theme));
	thiz->theme_ender = theme_ender;
	thiz->theme_instance = theme_instance;
	thiz->theme_element = theme_element;
	thiz->theme_renderer = theme_renderer;

	return thiz;

err_instance:
	// escen_ender_delete (theme_ender);
err_ender:
err_theme:
	return NULL;
}

static void _eon_widget_theme_theme_set(Enesim_Renderer *r, const char *file)
{
	Eon_Widget_Theme *thiz;
	Ender_Element *e;
	Ender_Element *old_element;
	Escen_Instance *old_instance;
	Escen *theme_escen;

	thiz = _eon_widget_theme_get(r);
	e = thiz->e;
	theme_escen = eon_theme_get_from_file(file);
	if (!theme_escen)
	{
		printf("no valid file %s\n", file);
		return;
	}

	old_element = thiz->theme_element;
	old_instance = thiz->theme_instance;

	_eon_widget_theme_theme_setup(thiz, e, theme_escen);
	/* FIXME delete the old instance/element */

	thiz->theme = strdup(file);
}

static void _eon_widget_theme_theme_get(Enesim_Renderer *r, const char **file)
{
	Eon_Widget_Theme *thiz;

	thiz = _eon_widget_theme_get(r);
	*file = thiz->theme;
}

void * eon_widget_theme_data_get(Enesim_Renderer *r)
{
	Eon_Widget_Theme *e;

	e = _eon_widget_theme_get(r);
	if (!e) return NULL;

	return e->data;
}

void eon_widget_theme_property_set(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget_Theme *thiz;
	va_list va_args;

	thiz = _eon_widget_theme_get(r);
	va_start(va_args, name);
	ender_element_value_set_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_theme_property_get(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget_Theme *thiz;
	va_list va_args;

	thiz = _eon_widget_theme_get(r);
	va_start(va_args, name);
	ender_element_value_get_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_theme_property_add(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget_Theme *thiz;
	va_list va_args;

	thiz = _eon_widget_theme_get(r);
	va_start(va_args, name);
	ender_element_value_add_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_theme_property_remove(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget_Theme *thiz;
	va_list va_args;

	thiz = _eon_widget_theme_get(r);
	va_start(va_args, name);
	ender_element_value_remove_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_widget_theme_property_clear(Enesim_Renderer *r, const char *name)
{
	Eon_Widget_Theme *thiz;

	thiz = _eon_widget_theme_get(r);
	ender_element_value_clear(thiz->theme_element, name);
}

void eon_widget_theme_state_set(Enesim_Renderer *r, const char *name, Eina_Bool be_finalized)
{
	Eon_Widget_Theme *thiz;
	Escen_State *new_state;

	thiz = _eon_widget_theme_get(r);
	if (be_finalized && !escen_instance_current_state_finalized(thiz->theme_instance))
	{
		printf("state didnt finalize, not setting a new one\n");
		return;
	}
	new_state = escen_ender_state_get(thiz->theme_ender, name);
	if (!new_state) return;

	escen_instance_state_set(thiz->theme_instance, new_state);
}

Escen_Ender * eon_widget_theme_theme_ender_get(Enesim_Renderer *r)
{
	Eon_Widget_Theme *thiz;

	thiz = _eon_widget_theme_get(r);
	return thiz->theme_ender;
}

Enesim_Renderer * eon_widget_theme_theme_renderer_get(Enesim_Renderer *r)
{
	Eon_Widget_Theme *thiz;

	thiz = _eon_widget_theme_get(r);
	return thiz->theme_renderer;
}

Ender_Element * eon_widget_theme_theme_element_get(Enesim_Renderer *r)
{
	Eon_Widget_Theme *thiz;

	thiz = _eon_widget_theme_get(r);
	return thiz->theme_element;
}

Escen_Instance * eon_widget_theme_theme_instance_get(Enesim_Renderer *r)
{
	Eon_Widget_Theme *thiz;

	thiz = _eon_widget_theme_get(r);
	return thiz->theme_instance;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
