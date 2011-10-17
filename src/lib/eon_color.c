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
typedef struct _Eon_Color
{
	Ender_Element *button;
	Ender_Element *wrapper;
} Eon_Color;

static inline Eon_Color * _eon_color_get(Enesim_Renderer *r)
{
	Eon_Color *thiz;

	thiz = eon_color_base_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static double _eon_color_min_width_get(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);

	eon_element_min_width_get(thiz->button, &v);

	return v;
}

static double _eon_color_max_width_get(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);

	eon_theme_color_max_width_get(thiz->button, &v);

	return v;
}

static double _eon_color_min_height_get(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);

	eon_element_min_height_get(thiz->button, &v);

	return v;
}

static double _eon_color_max_height_get(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);

	eon_element_max_height_get(thiz->button, &v);

	return v;
}

static double _eon_color_preferred_width_get(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);

	eon_element_preferred_width_get(thiz->button, &v);

	return v;
}

static double _eon_color_preferred_height_get(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);

	eon_element_preferred_height_get(thiz->button, &v);

	return v;
}

static void _eon_color_free(Enesim_Renderer *r)
{
	Eon_Color *thiz;

	thiz = _eon_color_get(r);
	//ender_element_delete(thiz->button);
	//ender_element_delete(thiz->wrapper);

	free(thiz);
}

static void _eon_color_initialize(Ender_Element *e)
{
	Eon_Color *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;

	r = ender_element_renderer_get(e);
	thiz = _eon_color_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	//eon_theme_color_renderer_get(theme_r, 
}

static Eon_Widget_Descriptor _descriptor = {
	.initialize = _eon_color_initialize,
	.name = "color",
	.min_width_get = _eon_color_min_width_get,
	.max_width_get = _eon_color_max_width_get,
	.min_height_get = _eon_color_min_height_get,
	.max_height_get = _eon_color_max_height_get,
	.preferred_width_get = _eon_color_preferred_width_get,
	.preferred_height_get = _eon_color_preferred_height_get,
	.free = _eon_color_free,
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

	r = eon_widget_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	thiz->button = eon_button_new();
	thiz->wrapper = eon_wrapper_new();
	eon_container_content_set(thiz->button, thiz->wrapper);

	return r;

renderer_err:
	free(thiz);
	return NULL;
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
EAPI void eon_color_value_set(Ender_Element *e, Enesim_Color value)
{
	ender_element_value_set(e, "value", value, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_color_value_get(Ender_Element *e, Enesim_Color *value)
{
	ender_element_value_get(e, "value", value, NULL);
}

