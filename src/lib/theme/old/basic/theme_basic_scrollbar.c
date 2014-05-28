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
#include "eon_theme_scrollbar.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Scrollbar
{
	/* properties */
	/* private */
	Enesim_Renderer *arrow_increment;
	Enesim_Renderer *arrow_decrement;
	Enesim_Renderer *thumb;
	Enesim_Renderer *thumb_area;

	Enesim_Renderer *shape;
	Enesim_Renderer *compound;
	Enesim_Renderer *background;
} Eon_Basic_Scrollbar;

static inline Eon_Basic_Scrollbar * _scrollbar_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = eon_theme_scrollbar_data_get(t);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                          The Scrollbar theme interface                     *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _basic_scrollbar_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	return thiz->shape;
}

static void _basic_scrollbar_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	enesim_renderer_rectangle_x_set(thiz->shape, x);
}

static void _basic_scrollbar_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	enesim_renderer_rectangle_y_set(thiz->shape, y);
}

static void _basic_scrollbar_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	enesim_renderer_rectangle_width_set(thiz->shape, width);
}

static void _basic_scrollbar_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	enesim_renderer_rectangle_height_set(thiz->shape, height);
}

static void _basic_scrollbar_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	
	free(thiz);
}
	
static void _basic_scrollbar_thumb_set(Eon_Theme_Widget *t, Enesim_Renderer *thumb)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	if (thiz->thumb)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->thumb);
	enesim_renderer_compound_layer_add(thiz->compound, thumb);
	thiz->thumb = thumb;
}

static void _basic_scrollbar_thumb_area_set(Eon_Theme_Widget *t, Enesim_Renderer *thumb_area)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	if (thiz->thumb_area)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->thumb_area);
	enesim_renderer_compound_layer_add(thiz->compound, thumb_area);
	thiz->thumb_area = thumb_area;
}

static void _basic_scrollbar_arrow_increment_set(Eon_Theme_Widget *t, Enesim_Renderer *arrow_increment)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	if (thiz->arrow_increment)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->arrow_increment);
	enesim_renderer_compound_layer_add(thiz->compound, arrow_increment);
	thiz->arrow_increment = arrow_increment;
}

static void _basic_scrollbar_arrow_decrement_set(Eon_Theme_Widget *t, Enesim_Renderer *arrow_decrement)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	if (thiz->arrow_decrement)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->arrow_decrement);
	enesim_renderer_compound_layer_add(thiz->compound, arrow_decrement);
	thiz->arrow_decrement = arrow_decrement;
}

static Eon_Theme_Scrollbar_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_scrollbar_renderer_get,
	/* .x_set 		= */ _basic_scrollbar_x_set,
	/* .y_set 		= */ _basic_scrollbar_y_set,
	/* .width_set 		= */ _basic_scrollbar_width_set,
	/* .height_set 		= */ _basic_scrollbar_height_set,
	/* .free 		= */ _basic_scrollbar_free,
	/* .thumb_set 		= */ _basic_scrollbar_thumb_set,
	/* .thumb_area_set 	= */ _basic_scrollbar_thumb_area_set,
	/* .arrow_increment_set	= */ _basic_scrollbar_arrow_increment_set,
	/* .arrow_decrement_set	= */ _basic_scrollbar_arrow_decrement_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_theme_basic_scrollbar_new(void)
{
	Eon_Theme_Widget *t;
	Eon_Basic_Scrollbar *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Scrollbar));
	if (!thiz) return NULL;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_background_color_set(r, 0xffff0000);
	thiz->background = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->background);

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	thiz->shape = r;
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);
	enesim_renderer_shape_stroke_weight_set(r, 1.0);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	t = eon_theme_scrollbar_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;
base_err:
	enesim_renderer_unref(thiz->shape);
shape_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}

EAPI void eon_theme_basic_scrollbar_background_color_set(Eon_Theme_Widget *t, Enesim_Color c)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	enesim_renderer_background_color_set(thiz->background, c);
}

EAPI void eon_theme_basic_scrollbar_border_color_set(Eon_Theme_Widget *t, Enesim_Color c)
{
	Eon_Basic_Scrollbar *thiz;

	thiz = _scrollbar_get(t);
	enesim_renderer_shape_stroke_color_set(thiz->shape, c);
}
