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

#include "eon_theme_widget.h"
#include "eon_theme_control_scroll_thumb.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Control_Scroll_Thumb
{
	/* properties */
	double radius;
	double border_weight;
	Enesim_Color border_color;
	/* private */
	Eon_Orientation orientation;

	Enesim_Renderer *bar;
	Enesim_Renderer *bar_background;
	Enesim_Renderer *bar_background_fill;
	Enesim_Renderer *bar_shape;
} Eon_Basic_Control_Scroll_Thumb;

static inline Eon_Basic_Control_Scroll_Thumb * _eon_theme_basic_control_scroll_thumb_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;
	thiz = eon_theme_control_scroll_thumb_data_get(t);
	return thiz;
}

static Enesim_Renderer * _basic_control_scroll_thumb_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	return thiz->bar;
}

static void _basic_control_scroll_thumb_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;
	double bbfx;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	printf("thumb setting x %g\n", x);
	enesim_renderer_rectangle_x_set(thiz->bar_shape, x);
	enesim_renderer_rectangle_x_set(thiz->bar_background, x + 3);
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		enesim_renderer_y_origin_set(thiz->bar_background_fill, x + 3);
}

static void _basic_control_scroll_thumb_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;
	double bbfy;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	printf("thumb setting y %g\n", y);
	enesim_renderer_rectangle_y_set(thiz->bar_shape, y);
	enesim_renderer_rectangle_y_set(thiz->bar_background, y + 3);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
		enesim_renderer_y_origin_set(thiz->bar_background_fill, y + 3);
}

static void _basic_control_scroll_thumb_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;
	double bw;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	printf("thumb setting width %g\n", width);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		bw = 14;
	}
	else
	{
		Enesim_Matrix bbf_matrix;
		bw = width;

		bbf_matrix.xx = 0; bbf_matrix.xy = -1; bbf_matrix.xz = 0;
		bbf_matrix.yx = 1; bbf_matrix.yy = 0; bbf_matrix.yz = 0;
		bbf_matrix.zx = 0; bbf_matrix.zy = 0; bbf_matrix.zz = 1;
		enesim_renderer_stripes_even_thickness_set(thiz->bar_background_fill, (bw - 6) / 2);
		enesim_renderer_stripes_odd_thickness_set(thiz->bar_background_fill, (bw - 6) / 2);
		enesim_renderer_transformation_set(thiz->bar_background_fill, &bbf_matrix);
	}
	enesim_renderer_rectangle_width_set(thiz->bar_shape, bw);
	enesim_renderer_rectangle_width_set(thiz->bar_background, bw - 6);
}

static void _basic_control_scroll_thumb_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;
	double bw, bh;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	printf("thumb setting height %g\n", height);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		Enesim_Matrix bbf_matrix;
		bh = height;

		enesim_matrix_identity(&bbf_matrix);
		enesim_renderer_stripes_even_thickness_set(thiz->bar_background_fill, (bh - 6) / 2);
		enesim_renderer_stripes_odd_thickness_set(thiz->bar_background_fill, (bh - 6) / 2);
		enesim_renderer_transformation_set(thiz->bar_background_fill, &bbf_matrix);
	}
	else
	{
		bh = 14;
	}
	enesim_renderer_rectangle_height_set(thiz->bar_shape, bh);
	enesim_renderer_rectangle_height_set(thiz->bar_background, bh - 6);
}

static void _basic_control_scroll_thumb_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
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

static void _basic_control_scroll_thumb_sizes_get(Eon_Theme_Widget *t, Eon_Size *min, Eon_Size *max)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		min->width = 14;
		max->width = 14;
		min->height = 14;
		max->height = DBL_MAX;
	}
	else
	{
		min->width = 14;
		max->width = DBL_MAX;
		min->height = 14;
		max->height = 14;
	}
}

static void _basic_control_scroll_thumb_orientation_set(Eon_Theme_Widget *t, Eon_Orientation orientation)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_get(t);
	thiz->orientation = orientation;
}

static Eon_Theme_Control_Scroll_Thumb_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_control_scroll_thumb_renderer_get,
	/* .x_set 		= */ _basic_control_scroll_thumb_x_set,
	/* .y_set 		= */ _basic_control_scroll_thumb_y_set,
	/* .width_set 		= */ _basic_control_scroll_thumb_width_set,
	/* .height_set 		= */ _basic_control_scroll_thumb_height_set,
	/* .free 		= */ _basic_control_scroll_thumb_free,
	/* .sizes_get 		= */ _basic_control_scroll_thumb_sizes_get,
	/* .orientation_set	= */ _basic_control_scroll_thumb_orientation_set,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_theme_basic_control_scroll_thumb_new(void)
{
	Eon_Basic_Control_Scroll_Thumb *thiz;
	Enesim_Renderer *r;
	Enesim_Matrix m;
	Enesim_Renderer_Gradient_Stop stop;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Scroll_Thumb));
	thiz->radius = 4;
	thiz->border_color = 0xff555555;
	thiz->border_weight = 1.2;

	r = enesim_renderer_stripes_new();
	if (!r) goto bar_background_fill_err;
	thiz->bar_background_fill = r;
	enesim_renderer_stripes_odd_color_set(r, 0x22000000);
	enesim_renderer_stripes_even_color_set(r, 0x33000000);

	r = enesim_renderer_rectangle_new();
	if (!r) goto bar_background_err;
	thiz->bar_background = r;
	enesim_renderer_rectangle_corner_radius_set(r, 16);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->bar_background_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_rectangle_new();
	if (!r) goto bar_shape_err;
	thiz->bar_shape = r;
	enesim_renderer_rectangle_corner_radius_set(r, 16);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_fill_color_set(r, 0xffcccccc);
	enesim_renderer_shape_stroke_weight_set(r, thiz->border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = enesim_renderer_compound_new();
	if (!r) goto bar_err;
	thiz->bar = r;
	enesim_renderer_compound_layer_add(r, thiz->bar_shape);
	enesim_renderer_compound_layer_add(r, thiz->bar_background);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	
	return eon_theme_control_scroll_thumb_new(&_descriptor, thiz);

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
