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
typedef struct _Basic_Scrollbar
{
	/* properties */
	double radius;
	Enesim_Color border_color;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *line;
	Enesim_Renderer *background;
	Enesim_Renderer *background_fill;
	Eon_Basic_Bar *bar;
} Basic_Scrollbar;

static const double _border_weight = 2.0;

static inline Basic_Scrollbar * _scrollbar_get(Enesim_Renderer *r)
{
	Basic_Scrollbar *thiz;

	thiz = eon_theme_scrollbar_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                   The Eon's theme scrollbar interface                    *
 *----------------------------------------------------------------------------*/
static double _basic_scrollbar_min_width_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_scrollbar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return 24;
	else
		return 16;
}

static double _basic_scrollbar_max_width_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_scrollbar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return DBL_MAX;
	else
		return 16;
}

static double _basic_scrollbar_min_height_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_scrollbar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
		return 16;
	else
		return 24;
}

static double _basic_scrollbar_max_height_get(Enesim_Renderer *r)
{
	Eon_Orientation orientation;

	eon_theme_scrollbar_orientation_get(r, &orientation);
	if (orientation == EON_ORIENTATION_VERTICAL)
		return DBL_MAX;
	else
		return 16;
}

static void _basic_scrollbar_thumb_percent_set(Enesim_Renderer *r, double percent)
{
	Basic_Scrollbar *thiz;
	Eon_Orientation orientation;
	double ofx = 0;
	double ofy = 0;
	double ox, oy;

	thiz = _scrollbar_get(r);
	eon_theme_scrollbar_orientation_get(r, &orientation);
	ox = oy = _border_weight / 2;
	/* given that we dont have arrow, just move it relative to the set widht/height */
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double w;

		eon_theme_widget_width_get(r, &w);
		ofx = w * percent;
	}
	else
	{
		double h;

		eon_theme_widget_height_get(r, &h);
		ofy = h * percent;
	}
	eon_basic_bar_position_set(thiz->bar, ox + ofx, oy + ofy);	
}

static void _basic_scrollbar_thumb_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry)
{
	Basic_Scrollbar *thiz;
	Enesim_Renderer *bar_r;

	thiz = _scrollbar_get(r);
	bar_r = eon_basic_bar_renderer_get(thiz->bar);
	enesim_renderer_boundings(bar_r, geometry);
	enesim_renderer_origin_get(bar_r, &geometry->x, &geometry->y);
}

static void _basic_scrollbar_thumb_size_set(Enesim_Renderer *r, double size)
{
	Basic_Scrollbar *thiz;
	Eon_Orientation orientation;

	thiz = _scrollbar_get(r);
	eon_theme_scrollbar_orientation_get(r, &orientation);
	eon_basic_bar_size_set(thiz->bar, size, orientation);
}

static double _basic_scrollbar_thumb_max_size_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _basic_scrollbar_thumb_min_size_get(Enesim_Renderer *r)
{
	return 20;
}

static Enesim_Renderer * _basic_scrollbar_renderer_get(Enesim_Renderer *r)
{
	Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(r);
	return thiz->compound;
}

static Eina_Bool _basic_scrollbar_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Basic_Scrollbar *thiz;
	Eon_Orientation orientation;
	Enesim_Matrix matrix;
	Enesim_Matrix bf_matrix;
	Enesim_Matrix bbf_matrix;
	double ox, oy;
	double width, height;
	double lx, ly, lw, lh;

	thiz = _scrollbar_get(r);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	/* the size */
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	/* the background fill */
	eon_theme_scrollbar_orientation_get(r, &orientation);
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
	enesim_renderer_rectangle_position_set(thiz->line, lx, ly);
	enesim_renderer_rectangle_size_set(thiz->line, lw, lh);
	enesim_renderer_transformation_set(thiz->line, &matrix);
	/* the composition */
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	return EINA_TRUE;
}

static void _basic_scrollbar_free(Enesim_Renderer *r)
{
	Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(r);

	thiz = enesim_renderer_data_get(r);

	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	if (thiz->bar)
		eon_basic_bar_free(thiz->bar);
	if (thiz->line)
		enesim_renderer_unref(thiz->line);
	if (thiz->background)
		enesim_renderer_unref(thiz->background);
	if (thiz->background_fill)
		enesim_renderer_unref(thiz->background_fill);
	free(thiz);
}

static Eon_Theme_Scrollbar_Descriptor _descriptor = {
	.max_width_get = _basic_scrollbar_max_width_get,
	.max_height_get = _basic_scrollbar_max_height_get,
	.min_width_get = _basic_scrollbar_min_width_get,
	.min_height_get = _basic_scrollbar_min_height_get,
	.thumb_percent_set = _basic_scrollbar_thumb_percent_set,
	.thumb_size_set = _basic_scrollbar_thumb_size_set,
	.thumb_max_size_get = _basic_scrollbar_thumb_max_size_get,
	.thumb_min_size_get = _basic_scrollbar_thumb_min_size_get,
	.thumb_geometry_get = _basic_scrollbar_thumb_geometry_get,
	.renderer_get = _basic_scrollbar_renderer_get,
	.setup = _basic_scrollbar_setup,
	.free = _basic_scrollbar_free,
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
Enesim_Renderer * eon_basic_scrollbar_new(void)
{
	Basic_Scrollbar *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *bar_r;

	thiz = calloc(1, sizeof(Basic_Scrollbar));
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

	thiz->bar = eon_basic_bar_new();
	bar_r = eon_basic_bar_renderer_get(thiz->bar);

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->background);
	enesim_renderer_compound_layer_add(r, thiz->line);
	enesim_renderer_compound_layer_add(r, bar_r);

	r = eon_theme_scrollbar_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->line);
line_err:
	enesim_renderer_unref(thiz->background);
background_err:
	enesim_renderer_unref(thiz->background_fill);
background_fill_err:
	free(thiz);
	return NULL;
}
