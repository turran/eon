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
	double radius;
	Enesim_Color border_color;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *line;
	Enesim_Renderer *background;
	Enesim_Renderer *background_fill;
	Enesim_Renderer *bar;
	Enesim_Renderer *bar_background;
	Enesim_Renderer *bar_background_fill;
	Enesim_Renderer *bar_shape;
	Enesim_Renderer_Sw_Fill fill;
} Basic_Splitter;

static const double _border_weight = 2.0;

static inline Basic_Splitter * _splitter_get(Enesim_Renderer *r)
{
	Basic_Splitter *thiz;

	thiz = eon_theme_splitter_data_get(r);
	return thiz;
}

static void _draw(Enesim_Renderer *r, int x, int y, unsigned int len, void *dst)
{
	Basic_Splitter *thiz;

	thiz = _splitter_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}

/*----------------------------------------------------------------------------*
 *                   The Eon's theme splitter interface                    *
 *----------------------------------------------------------------------------*/
static double _basic_splitter_min_width_get(Enesim_Renderer *r)
{
	return 16;
}

static double _basic_splitter_max_width_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_splitter_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return DBL_MAX;
	else
		return 16;
}

static double _basic_splitter_min_height_get(Enesim_Renderer *r)
{
	return 16;
}

static double _basic_splitter_max_height_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_splitter_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_VERTICAL)
		return DBL_MAX;
	else
		return 16;
}

static Eina_Bool _basic_splitter_setup(Enesim_Renderer *r, Enesim_Surface *s,
		Enesim_Renderer_Sw_Fill *fill, Enesim_Error **error)
{
	Basic_Splitter *thiz;
	Eon_Orientation orientation;
	Enesim_Matrix matrix;
	Enesim_Matrix bf_matrix;
	Enesim_Matrix bbf_matrix;
	double ox, oy;
	double width, height;
	double lx, ly, lw, lh;

	thiz = _splitter_get(r);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	/* the size */
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	/* the background fill */
	eon_theme_splitter_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_VERTICAL)
	{
		lx = width / 2;
		ly = thiz->radius;
		lw = 1;
		lh = height - (thiz->radius * 2);

		enesim_matrix_scale(&matrix, 0.6, 1.0);

		enesim_matrix_identity(&bbf_matrix);
		bf_matrix.xx = 0; bf_matrix.xy = -1; bf_matrix.xz = 0;
		bf_matrix.yx = 1; bf_matrix.yy = 0; bf_matrix.yz = 0;
		bf_matrix.zx = 0; bf_matrix.zy = 0; bf_matrix.zz = 1;
	}
	else
	{
		lx = thiz->radius;
		ly = height / 2;
		lw = width - (thiz->radius * 2);
		lh = 1;

		enesim_matrix_scale(&matrix, 1.0, 0.6);

		bbf_matrix.xx = 0; bbf_matrix.xy = -1; bbf_matrix.xz = 0;
		bbf_matrix.yx = 1; bbf_matrix.yy = 0; bbf_matrix.yz = 0;
		bbf_matrix.zx = 0; bbf_matrix.zy = 0; bbf_matrix.zz = 1;
		enesim_matrix_identity(&bf_matrix);
	}
	enesim_renderer_stripes_even_thickness_set(thiz->background_fill, 8);
	enesim_renderer_stripes_odd_thickness_set(thiz->background_fill, 8);
	enesim_renderer_transformation_set(thiz->background_fill, &bf_matrix);
	
	/* the background */
	enesim_renderer_rectangle_width_set(thiz->background, width);
	enesim_renderer_rectangle_height_set(thiz->background, height);
	/* the line */
	enesim_renderer_origin_set(thiz->line, lx, ly);
	enesim_renderer_rectangle_width_set(thiz->line, lw);
	enesim_renderer_rectangle_height_set(thiz->line, lh);
	enesim_renderer_transformation_set(thiz->line, &matrix);
	/* the bar shape */
	/* the bar background fill */
	enesim_renderer_transformation_set(thiz->bar_background_fill, &bbf_matrix);
	/* the bar background */
	/* the bar */
	/* the composition */
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	if (!enesim_renderer_sw_setup(thiz->compound, s, error))
	{
		enesim_error_dump(*error);
		printf("failed 1\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _draw;

	return EINA_TRUE;
}

static void _basic_splitter_cleanup(Enesim_Renderer *r)
{
	Basic_Splitter *thiz;

	thiz = _splitter_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _basic_splitter_free(Enesim_Renderer *r)
{
	Basic_Splitter *thiz;

	thiz = _splitter_get(r);

	thiz = enesim_renderer_data_get(r);

	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	if (thiz->bar)
		enesim_renderer_unref(thiz->bar);
	if (thiz->bar_shape)
		enesim_renderer_unref(thiz->bar_shape);
	if (thiz->bar_background)
		enesim_renderer_unref(thiz->bar_background);
	if (thiz->bar_background_fill)
		enesim_renderer_unref(thiz->bar_background_fill);
	if (thiz->line)
		enesim_renderer_unref(thiz->line);
	if (thiz->background)
		enesim_renderer_unref(thiz->background);
	if (thiz->background_fill)
		enesim_renderer_unref(thiz->background_fill);
	free(thiz);
}

static Eon_Theme_Splitter_Descriptor _descriptor = {
	.max_width_get = _basic_splitter_max_width_get,
	.max_height_get = _basic_splitter_max_height_get,
	.min_width_get = _basic_splitter_min_width_get,
	.min_height_get = _basic_splitter_min_height_get,
	.sw_setup = _basic_splitter_setup,
	.sw_cleanup = _basic_splitter_cleanup,
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
	thiz->radius = 4;
	thiz->border_color = 0xff555555;

	r = enesim_renderer_stripes_new();
	enesim_renderer_stripes_odd_color_set(r, 0x11000000);
	enesim_renderer_stripes_even_color_set(r, 0x33000000);
	if (!r) goto background_fill_err;
	thiz->background_fill = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->background_fill);
	//enesim_renderer_shape_fill_color_set(r, 0xffff0000);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_FILL);

	r = enesim_renderer_rectangle_new();
	if (!r) goto line_err;
	thiz->line = r;
	enesim_renderer_shape_fill_color_set(r, 0xff888888);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_stripes_new();
	if (!r) goto bar_background_fill_err;
	thiz->bar_background_fill = r;
	enesim_renderer_stripes_odd_color_set(r, 0x22000000);
	enesim_renderer_stripes_even_color_set(r, 0x33000000);
	enesim_renderer_origin_set(r, -1.0, -1.0);

	r = enesim_renderer_rectangle_new();
	if (!r) goto bar_background_err;
	thiz->bar_background = r;
	enesim_renderer_rectangle_corner_radius_set(r, 16);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->bar_background_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_origin_set(r, 3.0, 3.0);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_rectangle_new();
	if (!r) goto bar_shape_err;
	thiz->bar_shape = r;
	enesim_renderer_rectangle_corner_radius_set(r, 16);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_fill_color_set(r, 0xffcccccc);
	enesim_renderer_shape_stroke_weight_set(r, _border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = enesim_renderer_compound_new();
	if (!r) goto bar_err;
	thiz->bar = r;
	enesim_renderer_compound_layer_add(r, thiz->bar_shape);
	enesim_renderer_compound_layer_add(r, thiz->bar_background);

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->background);
	enesim_renderer_compound_layer_add(r, thiz->line);
	enesim_renderer_compound_layer_add(r, thiz->bar);

	r = eon_theme_splitter_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->bar);
bar_err:
	enesim_renderer_unref(thiz->bar_shape);
bar_shape_err:
	enesim_renderer_unref(thiz->bar_background);
bar_background_err:
	enesim_renderer_unref(thiz->bar_background_fill);
bar_background_fill_err:
	enesim_renderer_unref(thiz->line);
line_err:
	enesim_renderer_unref(thiz->background);
background_err:
	enesim_renderer_unref(thiz->background_fill);
background_fill_err:
	free(thiz);
	return NULL;
}

