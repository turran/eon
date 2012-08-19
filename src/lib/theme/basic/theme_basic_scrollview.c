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
#include "eon_theme_container.h"
#include "eon_theme_bin.h"
#include "eon_theme_scrollview.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Scrollview
{
	/* properties */
	Eina_Bool over;
	double bar_width;
	double bar_radius;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *background;
	Enesim_Renderer *shape;
	Enesim_Renderer *hbar;
	Enesim_Renderer *vbar;
	Enesim_Renderer *child;
} Eon_Basic_Scrollview;

static inline Eon_Basic_Scrollview * _scrollview_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Scrollview *thiz;

	thiz = eon_theme_scrollview_data_get(t);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                          The Scrollview theme interface                    *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _basic_scrollview_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	return thiz->shape;
}

static void _basic_scrollview_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	enesim_renderer_rectangle_x_set(thiz->shape, x);
}

static void _basic_scrollview_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	enesim_renderer_rectangle_y_set(thiz->shape, y);
}

static void _basic_scrollview_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	enesim_renderer_rectangle_width_set(thiz->shape, width);
}

static void _basic_scrollview_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	enesim_renderer_rectangle_height_set(thiz->shape, height);
}

static void _basic_scrollview_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	
	free(thiz);
}

static void _basic_scrollview_child_set(Eon_Theme_Widget *t, Enesim_Renderer *child)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	if (thiz->child)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->child);
	enesim_renderer_compound_layer_add(thiz->compound, child);
	thiz->child = child;
}

static void _basic_scrollview_child_get(Eon_Theme_Widget *t, Enesim_Renderer **child)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	*child = thiz->child;
}

static void _basic_scrollview_hbar_set(Eon_Theme_Widget *t, Enesim_Renderer *hbar)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	if (thiz->hbar)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->hbar);
	enesim_renderer_compound_layer_add(thiz->compound, hbar);
	thiz->hbar = hbar;
}

static void _basic_scrollview_vbar_set(Eon_Theme_Widget *t, Enesim_Renderer *vbar)
{
	Eon_Basic_Scrollview *thiz;

	thiz = _scrollview_get(t);
	if (thiz->vbar)	
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->vbar);
	enesim_renderer_compound_layer_add(thiz->compound, vbar);
	thiz->vbar = vbar;
}

static Eon_Theme_Scrollview_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_scrollview_renderer_get,
	/* .x_set 		= */ _basic_scrollview_x_set,
	/* .y_set 		= */ _basic_scrollview_y_set,
	/* .width_set 		= */ _basic_scrollview_width_set,
	/* .height_set 		= */ _basic_scrollview_height_set,
	/* .free 		= */ _basic_scrollview_free,
	/* .child_set 		= */ _basic_scrollview_child_set,
	/* .child_get 		= */ _basic_scrollview_child_get,
	/* .hbar_set 		= */ _basic_scrollview_hbar_set,
	/* .vbar_set 		= */ _basic_scrollview_vbar_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_basic_scrollview_new(void)
{
	Eon_Theme_Widget *t;
	Eon_Basic_Scrollview *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Scrollview));
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

	t = eon_theme_scrollview_new(&_descriptor, thiz);
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
