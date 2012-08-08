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

#include "eon_private_theme.h"

#include "theme/eon_theme_widget.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Bool _init = EINA_FALSE;
static Eon_Theme_File *_theme = NULL;
#if 0
/*----------------------------------------------------------------------------*
 *                        The Theme extension functions                       *
 *----------------------------------------------------------------------------*/
typedef struct _Eon_Widget_Extend
{
	Ender_Element *e;
	/* FIXME this should be a list */
	Ender_Property *property;
	Ender_Element *other;
	/* FIXME this should be a list */
	Ender_Property *o_property;
} Eon_Widget_Extend;

/* whenever we register the new properties, we register this function dummy */
static void _eon_theme_instance_extend_accessor(Ender_Element *e, Ender_Property *prop,
		Ender_Value *v, void *data)
{
	Eon_Widget_Extend *extend = data;

	/* FIXME use the property directly */
	printf("=>>>> setting property %s\n", ender_property_name_get(prop));
	ender_element_value_set_simple(extend->other,
			ender_property_name_get(prop),
			v);
}

static void _eon_theme_instance_extend_properties(Ender_Property *prop, void *data)
{
	Eon_Widget_Extend *extend = data;
	Ender_Container *c;

	printf("new property %s\n", ender_property_name_get(prop));
	c = ender_property_container_get(prop);
	/* duplicate theme on thiz */
	extend->property = ender_element_property_add(extend->e, ender_property_name_get(prop),
			c,
			NULL,
			_eon_theme_instance_extend_accessor,
			_eon_theme_instance_extend_accessor,
			_eon_theme_instance_extend_accessor,
			NULL,
			NULL,
			EINA_FALSE,
			extend);
}
#endif
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool eon_theme_init(void)
{
	if (!_init)
	{
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, PACKAGE_DATA_DIR "/themes/basic.escen");
		_theme = eon_theme_file_get_from_file(path);
		if (!_theme) return EINA_FALSE;

		/* TODO once we have the theme loaded, append the datadir to the paths */
		_init = EINA_TRUE;
	}
	return _init;
}

void eon_theme_shutdown(void)
{
	if (_init)
	{
		_init = EINA_FALSE;
	}
}

Eon_Theme_File * eon_theme_file_get(void)
{
	return _theme;
}

Eon_Theme_File * eon_theme_file_get_from_file(const char *file)
{
	Eon_Theme_File *thiz;
	Escen *escen;

	/* TODO check if the file is absolute, if it is, use directly that name
	 * otherwise prepend the data_dir
	 */
	escen = escen_parser_load(file);
	if (!escen) return NULL;

	thiz = malloc(sizeof(Eon_Theme_File));
	thiz->file = strdup(file);
	thiz->escen = escen;

	return thiz;
}
Eon_Theme_Instance * eon_theme_instance_new(const char *name, Eina_Bool def)
{
	return eon_theme_instance_new_from_file(_theme, name, def);
}

Eon_Theme_Instance * eon_theme_instance_new_from_file(Eon_Theme_File *file, const char *name, Eina_Bool def)
{
	Eon_Theme_Instance *thiz;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	void *theme_object;
	Enesim_Renderer *theme_renderer;

	theme_ender = escen_ender_get(file->escen, name);
	if (!theme_ender)
	{
		printf("no ender '%s'\n", name);
		return NULL;
	}

	theme_instance = escen_instance_new(theme_ender, def);
	if (!theme_instance)
	{
		printf("no instance '%s'\n", name);
		return NULL;
	}

	theme_element = escen_instance_ender_get(theme_instance);
	theme_object = ender_element_object_get(theme_element);
	eon_theme_widget_renderer_get(theme_object, &theme_renderer);

	thiz = calloc(1, sizeof(Eon_Theme_Instance));
	thiz->object = theme_object;
	thiz->ender = theme_ender;
	thiz->instance = theme_instance;
	thiz->element = theme_element;
	thiz->renderer = theme_renderer;
	thiz->theme = file;

	return thiz;
}

void eon_theme_instance_property_set(Eon_Theme_Instance *thiz, const char *name, ...)
{
	va_list va_args;

	va_start(va_args, name);
	ender_element_value_set_valist(thiz->element, name, va_args);
	va_end(va_args);
}

void eon_theme_instance_property_get(Eon_Theme_Instance *thiz, const char *name, ...)
{
	va_list va_args;

	va_start(va_args, name);
	ender_element_value_get_valist(thiz->element, name, va_args);
	va_end(va_args);
}

void eon_theme_instance_property_add(Eon_Theme_Instance *thiz, const char *name, ...)
{
	va_list va_args;

	va_start(va_args, name);
	ender_element_value_add_valist(thiz->element, name, va_args);
	va_end(va_args);
}

void eon_theme_instance_property_remove(Eon_Theme_Instance *thiz, const char *name, ...)
{
	va_list va_args;

	va_start(va_args, name);
	ender_element_value_remove_valist(thiz->element, name, va_args);
	va_end(va_args);
}

void eon_theme_instance_property_clear(Eon_Theme_Instance *thiz, const char *name)
{
	ender_element_value_clear(thiz->element, name);
}

void eon_theme_instance_state_set(Eon_Theme_Instance *thiz, const char *name, Eina_Bool be_finalized)
{
	Escen_State *new_state;

	if (be_finalized && !escen_instance_current_state_finalized(thiz->instance))
	{
		printf("state didnt finalize, not setting a new one\n");
		return;
	}
	new_state = escen_ender_state_get(thiz->ender, name);
	if (!new_state) return;

	escen_instance_state_set(thiz->instance, new_state);
}

Escen_State * eon_theme_instance_state_get(Eon_Theme_Instance *thiz, const char *name)
{
	Escen_State *state;

	state = escen_ender_state_get(thiz->ender, name);
	return state;
}

Escen_Ender * eon_theme_instance_ender_get(Eon_Theme_Instance *thiz)
{
	return thiz->ender;
}

Enesim_Renderer * eon_theme_instance_renderer_get(Eon_Theme_Instance *thiz)
{
	return thiz->renderer;
}

Ender_Element * eon_theme_instance_element_get(Eon_Theme_Instance *thiz)
{
	return thiz->element;
}

Escen_Instance * eon_theme_instance_instance_get(Eon_Theme_Instance *thiz)
{
	return thiz->instance;
}

#if 0
void eon_theme_instance_extend(Eon_Theme_Instance *ee, Ender_Element *e)
{
	Eon_Widget_Extend *extend;
	Eon_Theme_Instance *other_e;
	Ender_Descriptor *d;

	if (!eon_is_widget(e))
		return;

	thiz = _eon_theme_instance_get(ee);
	other_e = ender_element_object_get(e);
	other = _eon_theme_instance_get(other_e);

	d = ender_element_descriptor_get(other->theme.element);
	/* iterate over the list of properties to add a new property on theme element */
	extend = calloc(1, sizeof(Eon_Widget_Extend));
	extend->e = thiz->element;
	extend->other = other->theme.element;
	ender_descriptor_property_list(d, _eon_theme_instance_extend_properties, extend);
	/* and register a mutation event that will send the properties to the correct theme element */
	/* TODO store the extend information */
}
#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

