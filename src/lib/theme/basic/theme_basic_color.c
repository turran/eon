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
#include "Eon_Theme.h"
#include "Eon_Basic.h"
#include "eon_theme_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Basic_Color
{
	Ender_Element *content;
	Enesim_Renderer *content_fill;
	Enesim_Renderer *content_fill_background;
	Enesim_Renderer *content_fill_color;
} Basic_Color;

static inline Basic_Color * _color_get(Enesim_Renderer *r)
{
	Basic_Color *thiz;

	thiz = eon_theme_color_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Button theme interface                         *
 *----------------------------------------------------------------------------*/
static Ender_Element * _color_content_element_get(Enesim_Renderer *r)
{
	Basic_Color *thiz;

	thiz = _color_get(r);
	return thiz->content;
}

static void _color_content_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Color *thiz;

	thiz = _color_get(r);
	enesim_renderer_background_color_set(thiz->content_fill_color, color);
}

static Eina_Bool _color_setup(Enesim_Renderer *r,
		const Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		const Eon_Theme_Container_State *cstates[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Basic_Color *thiz;

	thiz = _color_get(r);
	return EINA_TRUE;
}

static void _color_free(Enesim_Renderer *r)
{
	Basic_Color *thiz;

	thiz = _color_get(r);
	free(thiz);
}

static Eon_Theme_Color_Descriptor _descriptor = {
	.content_element_get = _color_content_element_get,
	.content_color_set = _color_content_color_set,
	.setup = _color_setup,
	.free = _color_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_basic_color_new(void)
{
	Enesim_Renderer *r;
	Ender_Element *e;
	Basic_Color *thiz;

	thiz = calloc(1, sizeof(Basic_Color));
	if (!thiz) return NULL;

	r = enesim_renderer_checker_new();
	enesim_renderer_checker_width_set(r, 4);
	enesim_renderer_checker_height_set(r, 4);
	enesim_renderer_checker_even_color_set(r, 0xff444444);
	enesim_renderer_checker_even_color_set(r, 0xffffffff);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	thiz->content_fill_background = r;

	r = enesim_renderer_background_new();
	enesim_renderer_background_color_set(r, 0xaaaa0000);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->content_fill_color = r;

	r = enesim_renderer_compound_new();
	enesim_renderer_compound_layer_add(r, thiz->content_fill_background);
	enesim_renderer_compound_layer_add(r, thiz->content_fill_color);
	thiz->content_fill = r;

	e = ender_element_new_with_namespace("rectangle", "enesim", 0);
	r = ender_element_object_get(e);
	enesim_renderer_rectangle_size_set(r, 20.0, 20.0);
	enesim_renderer_rectangle_corner_radius_set(r, 3);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_shape_fill_renderer_set(r, thiz->content_fill);
	thiz->content = e;

	r = eon_theme_color_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	free(thiz);
	return NULL;
}
