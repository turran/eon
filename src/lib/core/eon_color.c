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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_COLOR_VALUE;

typedef struct _Eon_Color
{
	/* properties */
	Enesim_Argb argb;
	/* private */
	Ender_Element *wrapper;
	Ender_Element *button;
} Eon_Color;

static inline Eon_Color * _eon_color_get(Eon_Element *ee)
{
	Eon_Color *thiz;

	thiz = eon_compound_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_color_free(Eon_Element *ee)
{
	Eon_Color *thiz;

	thiz = _eon_color_get(ee);
	//ender_element_delete(thiz->wrapper);

	free(thiz);
}

static void _eon_color_initialize(Ender_Element *e)
{
	Eon_Color *thiz;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;
	Ender_Element *content_e;

	ee = ender_element_object_get(e);
	thiz = _eon_color_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);

	eon_container_content_set(thiz->button, thiz->wrapper);
	content_e = eon_theme_color_content_element_get(theme_r);
	eon_wrapper_wrapped_set(thiz->wrapper, content_e);
}

static Eon_Compound_Descriptor _descriptor = {
	/* .initialize 	= */ _eon_color_initialize,
	/* .free 	= */ _eon_color_free,
	/* .name 	= */ "color",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_color_new(void)
{
	Eon_Color *thiz;
	Eon_Element *ee;
	Eon_Keyboard_Proxy *kp;

	thiz = calloc(1, sizeof(Eon_Color));
	if (!thiz) return NULL;

	thiz->wrapper = eon_wrapper_new();
	thiz->button = eon_button_new();

	ee = eon_compound_new(&_descriptor, thiz->button, thiz);

	kp = eon_keyboard_proxy_focus_new();
        eon_element_keyboard_proxy_set(ee, kp);
	return ee;
}

static void _eon_color_value_set(Eon_Element *ee, Enesim_Argb argb)
{
	Eon_Color *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_color_get(ee);

	thiz->argb = argb;
	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_color_content_color_set(theme_r, argb);
}

static void _eon_color_value_get(Eon_Element *ee, Enesim_Argb *argb)
{
	Eon_Color *thiz;

	thiz = _eon_color_get(ee);
	*argb = thiz->argb;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_color_delete NULL
#include "eon_generated_color.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_color_new(void)
{
	return ender_element_new_with_namespace("color", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_color_value_set(Ender_Element *e, Enesim_Argb value)
{
	ender_element_property_value_set(e, EON_COLOR_VALUE, value, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_color_value_get(Ender_Element *e, Enesim_Argb *value)
{
	ender_element_property_value_get(e, EON_COLOR_VALUE, value, NULL);
}
