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
#include "eon_theme_control_scroll_thumb_area.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Control_Scroll_Thumb_Area
{
	/* properties */
	/* private */
	Eon_Orientation orientation;
	Enesim_Renderer *in;
	Enesim_Renderer *out;

	Enesim_Renderer *compound;
} Eon_Basic_Control_Scroll_Thumb_Area;

static inline Eon_Basic_Control_Scroll_Thumb_Area * _eon_theme_basic_control_scroll_thumb_area_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;
	thiz = eon_theme_control_scroll_thumb_area_data_get(t);
	return thiz;
}

static Enesim_Renderer * _basic_control_scroll_thumb_area_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	return thiz->compound;
}

static void _basic_control_scroll_thumb_area_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		enesim_renderer_rectangle_x_set(thiz->out, x);
		enesim_renderer_rectangle_x_set(thiz->in, x + 1);
	}
	else
	{
		enesim_renderer_rectangle_x_set(thiz->out, x);
		enesim_renderer_rectangle_x_set(thiz->in, x + 1);
	}
}

static void _basic_control_scroll_thumb_area_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		enesim_renderer_rectangle_y_set(thiz->out, y);
		enesim_renderer_rectangle_y_set(thiz->in, y + 1);
	}
	else
	{
		enesim_renderer_rectangle_y_set(thiz->out, y);
		enesim_renderer_rectangle_y_set(thiz->in, y + 1);
	}
}

static void _basic_control_scroll_thumb_area_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		enesim_renderer_rectangle_width_set(thiz->out, 2);
		enesim_renderer_rectangle_width_set(thiz->in, 1);
	}
	else
	{
		enesim_renderer_rectangle_width_set(thiz->out, width);
		enesim_renderer_rectangle_width_set(thiz->in, width - 2);
	}
}

static void _basic_control_scroll_thumb_area_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		enesim_renderer_rectangle_height_set(thiz->out, height);
		enesim_renderer_rectangle_height_set(thiz->in, height - 2);
	}
	else
	{
		enesim_renderer_rectangle_height_set(thiz->out, 2);
		enesim_renderer_rectangle_height_set(thiz->in, 1);
	}
}

static void _basic_control_scroll_thumb_area_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	if (thiz->in)
		enesim_renderer_unref(thiz->in);
	if (thiz->out)
		enesim_renderer_unref(thiz->out);
	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	free(thiz);
}

static void _basic_control_scroll_thumb_area_sizes_get(Eon_Theme_Widget *t, Eon_Size *min, Eon_Size *max)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		min->width = 2;
		max->width = 2;
		min->height = 3;
		max->height = DBL_MAX;
	}
	else
	{
		min->width = 3;
		max->width = DBL_MAX;
		min->height = 2;
		max->height = 2;
	}
}

static void _basic_control_scroll_thumb_area_orientation_set(Eon_Theme_Widget *t, Eon_Orientation orientation)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	thiz->orientation = orientation;
}

static Eon_Theme_Control_Scroll_Thumb_Area_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_control_scroll_thumb_area_renderer_get,
	/* .x_set 		= */ _basic_control_scroll_thumb_area_x_set,
	/* .y_set 		= */ _basic_control_scroll_thumb_area_y_set,
	/* .width_set 		= */ _basic_control_scroll_thumb_area_width_set,
	/* .height_set 		= */ _basic_control_scroll_thumb_area_height_set,
	/* .free 		= */ _basic_control_scroll_thumb_area_free,
	/* .sizes_get 		= */ _basic_control_scroll_thumb_area_sizes_get,
	/* .orientation_set	= */ _basic_control_scroll_thumb_area_orientation_set,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_theme_basic_control_scroll_thumb_area_new(void)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;
	Enesim_Renderer *r;
	Enesim_Color in_color = 0xffffffff;
	Enesim_Color out_color = 0xff000000;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Scroll_Thumb_Area));

	r = enesim_renderer_rectangle_new();
	if (!r) goto out_err;
	enesim_renderer_shape_fill_color_set(r, out_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	thiz->out = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto in_err;
	enesim_renderer_shape_fill_color_set(r, in_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	thiz->in = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_compound_layer_add(r, thiz->out);
	enesim_renderer_compound_layer_add(r, thiz->in);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->compound = r;

	return eon_theme_control_scroll_thumb_area_new(&_descriptor, thiz);

compound_err:
	enesim_renderer_unref(thiz->in);
in_err:
	enesim_renderer_unref(thiz->out);
out_err:
	free(thiz);
	return NULL;
}

EAPI void eon_theme_basic_control_scroll_thumb_area_out_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	enesim_renderer_shape_fill_color_set(thiz->out, color);
}

EAPI void eon_theme_basic_control_scroll_thumb_area_in_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Scroll_Thumb_Area *thiz;

	thiz = _eon_theme_basic_control_scroll_thumb_area_get(t);
	enesim_renderer_shape_fill_color_set(thiz->in, color);
}
