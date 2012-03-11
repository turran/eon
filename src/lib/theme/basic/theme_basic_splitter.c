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
#include "eon_basic_private.h"
#include <math.h>
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Splitter
{
	/* properties */
	Enesim_Color border_color;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *splitter;
	Enesim_Renderer *background;
} Basic_Splitter;

static const double _border_weight = 2.0;

static inline Basic_Splitter * _splitter_get(Enesim_Renderer *r)
{
	Basic_Splitter *thiz;

	thiz = eon_theme_splitter_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                   The Eon's theme splitter interface                    *
 *----------------------------------------------------------------------------*/
static double _basic_min_length_get(Enesim_Renderer *r)
{
	return 16;
}

static double _basic_splitter_thickness_get(Enesim_Renderer *r)
{
	return 10;
}

static Enesim_Renderer * _basic_splitter_renderer_get(Enesim_Renderer *r)
{
	Basic_Splitter *thiz;

	thiz = _splitter_get(r);
	return thiz->compound;
}

static Eina_Bool _basic_splitter_setup(Enesim_Renderer *r,
		const Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		const Eon_Theme_Container_State *cstates[ENESIM_RENDERER_STATES],
		const Eon_Theme_Splitter_State *sstates[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Basic_Splitter *thiz;
	Eon_Orientation orientation;
	Enesim_Renderer *content;
	Enesim_Renderer *second_content;
	double ox, oy;
	double width, height;
	double position;

	thiz = _splitter_get(r);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	/* the size */
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	/* the contents */
	eon_theme_container_content_get(r, &content);
	eon_theme_splitter_second_content_get(r, &second_content);

	/* the separator */
	eon_theme_splitter_orientation_get(r, &orientation);
	eon_theme_splitter_position_get(r, &position);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		enesim_renderer_rectangle_size_set(thiz->splitter, 10, height);
		enesim_renderer_origin_set(thiz->splitter, position, 0);
	}
	else
	{
		enesim_renderer_rectangle_size_set(thiz->splitter, width, 10);
		enesim_renderer_origin_set(thiz->splitter, 0, position);
	}
	/* the background */
	/* the composition */
	enesim_renderer_compound_layer_clear(thiz->compound);
	enesim_renderer_compound_layer_add(thiz->compound, thiz->background);
	enesim_renderer_compound_layer_add(thiz->compound, content);
	enesim_renderer_compound_layer_add(thiz->compound, second_content);
	enesim_renderer_compound_layer_add(thiz->compound, thiz->splitter);
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	return EINA_TRUE;
}

static void _basic_splitter_free(Enesim_Renderer *r)
{
	Basic_Splitter *thiz;

	thiz = _splitter_get(r);

	thiz = enesim_renderer_data_get(r);

	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	if (thiz->background)
		enesim_renderer_unref(thiz->background);
	free(thiz);
}

static Eon_Theme_Splitter_Descriptor _descriptor = {
	.thickness_get = _basic_splitter_thickness_get,
	.min_length_get = _basic_min_length_get,
	.renderer_get = _basic_splitter_renderer_get,
	.setup = _basic_splitter_setup,
	.free = _basic_splitter_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
Enesim_Renderer * eon_basic_splitter_new(void)
{
	Basic_Splitter *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Basic_Splitter));
	if (!thiz) return NULL;
	thiz->border_color = 0xff555555;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_background_color_set(r, 0xffffffff);
	enesim_renderer_rop_set(r, ENESIM_FILL);

	r = enesim_renderer_rectangle_new();
	if (!r) goto splitter_err;
	thiz->splitter = r;
	enesim_renderer_shape_fill_color_set(r, 0xffcccccc);
	enesim_renderer_shape_stroke_weight_set(r, _border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = eon_theme_splitter_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->splitter);
splitter_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}

