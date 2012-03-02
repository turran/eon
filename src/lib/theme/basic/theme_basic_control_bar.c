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
typedef struct _Eon_Basic_Bar Eon_Basic_Bar;

struct _Eon_Basic_Bar
{
	/* properties */
	double radius;
	Eon_Orientation orientation;
	Enesim_Color border_color;
	/* private */
	Enesim_Renderer *bar;
	Enesim_Renderer *bar_background;
	Enesim_Renderer *bar_background_fill;
	Enesim_Renderer *bar_shape;
};

static const double _border_weight = 2.0;

/*----------------------------------------------------------------------------*
 *                   The Eon's theme bar interface                    *
 *----------------------------------------------------------------------------*/
static double _basic_bar_min_width_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_bar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return 24;
	else
		return 16;
}

static double _basic_bar_max_width_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_bar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return DBL_MAX;
	else
		return 16;
}

static double _basic_bar_min_height_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_bar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return 16;
	else
		return 24;
}

static double _basic_bar_max_height_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_bar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_VERTICAL)
		return DBL_MAX;
	else
		return 16;
}

static Eina_Bool _basic_bar_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Eon_Basic_Bar *thiz;
	Eon_Orientation orientation;
	Enesim_Matrix matrix;
	Enesim_Matrix bf_matrix;
	Enesim_Matrix bbf_matrix;
	double ox, oy;
	double width, height;
	double lx, ly, lw, lh;

	thiz = _bar_get(r);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	/* the size */
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	/* the background fill */
	eon_theme_bar_orientation_get(r, &orientation);
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
	/* the bar shape */
	/* the bar background fill */
	enesim_renderer_transformation_set(thiz->bar_background_fill, &bbf_matrix);
	/* the bar background */
	/* the bar */
	/* the composition */

	return EINA_TRUE;
}

static void _basic_bar_free(Enesim_Renderer *r)
{
	Eon_Basic_Bar *thiz;

	thiz = _bar_get(r);

	thiz = enesim_renderer_data_get(r);

	if (thiz->bar)
		enesim_renderer_unref(thiz->bar);
	if (thiz->bar_shape)
		enesim_renderer_unref(thiz->bar_shape);
	if (thiz->bar_background)
		enesim_renderer_unref(thiz->bar_background);
	if (thiz->bar_background_fill)
		enesim_renderer_unref(thiz->bar_background_fill);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
Eon_Basic_Bar * eon_basic_bar_new(void)
{
	Eon_Basic_Bar *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Bar));
	if (!thiz) return NULL;
	thiz->radius = 4;
	thiz->border_color = 0xff555555;

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
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	return thiz;

compound_err:
	enesim_renderer_unref(thiz->bar);
bar_err:
	enesim_renderer_unref(thiz->bar_shape);
bar_shape_err:
	enesim_renderer_unref(thiz->bar_background);
bar_background_err:
	enesim_renderer_unref(thiz->bar_background_fill);
bar_background_fill_err:

	free(thiz);
	return NULL;
}

static void _basic_bar_thumb_size_set(Enesim_Renderer *r, double size)
{
	Eon_Basic_Bar *thiz;
	Eon_Orientation orientation;
	double bw, bh;

	thiz = _bar_get(r);
	eon_theme_bar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_VERTICAL)
	{
		bw = 14;
		bh = size;
	}
	else
	{
		bw = size;
		bh = 14;
	}
	enesim_renderer_rectangle_size_set(thiz->bar_shape, bw, bh);
	enesim_renderer_rectangle_size_set(thiz->bar_background, bw - 6, bh - 6);
	enesim_renderer_stripes_even_thickness_set(thiz->bar_background_fill, (size - 6) / 2);
	enesim_renderer_stripes_odd_thickness_set(thiz->bar_background_fill, (size - 6) / 2);
}

static double _basic_bar_thumb_max_size_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _basic_bar_thumb_min_size_get(Enesim_Renderer *r)
{
	return 20;
}

static Enesim_Renderer * _basic_bar_renderer_get(Enesim_Renderer *r)
{
	Eon_Basic_Bar *thiz;

	thiz = _bar_get(r);
	return thiz->bar;
}


