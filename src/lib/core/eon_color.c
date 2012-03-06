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
/* This widget is kind of tricky, not by its complexity but by its nature.
 * Should this be an *instance* of a button or it should *inherit* from a button
 * i.e the "has a", "is a" issue
 * so far we have implemented as an inheritance, but maybe we should do the
 * other way around even if that needs some code duplication
 */
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
} Eon_Color;

static inline Eon_Color * _eon_color_get(Enesim_Renderer *r)
{
	Eon_Color *thiz;

	thiz = eon_button_base_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_color_free(Enesim_Renderer *r)
{
	Eon_Color *thiz;

	thiz = _eon_color_get(r);
	//ender_element_delete(thiz->wrapper);

	free(thiz);
}

static void _eon_color_initialize(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Ender_Element *content_e;

	r = ender_element_object_get(e);
	thiz = _eon_color_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	eon_container_content_set(e, thiz->wrapper);
	content_e = eon_theme_color_content_element_get(theme_r);
	eon_wrapper_wrapped_set(thiz->wrapper, content_e);
}

static Eon_Button_Base_Descriptor _descriptor = {
	.initialize = _eon_color_initialize,
	.free = _eon_color_free,
	.name = "color",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_color_new(void)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Color));
	if (!thiz) return NULL;

	r = eon_button_base_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	thiz->wrapper = eon_wrapper_new();

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_color_value_set(Enesim_Renderer *r, Enesim_Argb argb)
{
	Eon_Color *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_color_get(r);

	thiz->argb = argb;
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_color_content_color_set(theme_r, argb);
}

static void _eon_color_value_get(Enesim_Renderer *r, Enesim_Argb *argb)
{
	Eon_Color *thiz;

	thiz = _eon_color_get(r);
	*argb = thiz->argb;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
