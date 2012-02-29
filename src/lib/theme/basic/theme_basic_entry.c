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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Basic_Entry
{
	/* properties */
	double radius;
	Enesim_Color border_color;
	/* private */
	Enesim_Renderer *text;
	Enesim_Renderer *shape;
	Enesim_Renderer *shape_fill;
	Enesim_Renderer *background;
} Basic_Entry;

static inline Basic_Entry * _entry_get(Enesim_Renderer *r)
{
	Basic_Entry *thiz;

	thiz = eon_theme_entry_data_get(r);
	return thiz;
}

static const double _border_weight = 2.0;
/*----------------------------------------------------------------------------*
 *                          The Entry theme interface                         *
 *----------------------------------------------------------------------------*/
static void _entry_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	margin->left = margin->right = margin->top = margin->bottom = 4;
}

static Enesim_Renderer * _entry_renderer_get(Enesim_Renderer *r)
{
	Basic_Entry *thiz;

	thiz = _entry_get(r);
	return thiz->shape;
}

static Eina_Bool _entry_setup(Enesim_Renderer *r, Enesim_Renderer *text,
		Eon_Horizontal_Alignment alignment, Enesim_Error **error)
{
	Basic_Entry *thiz;
	Enesim_Rectangle boundings;
	double tx, ty;
	double ox, oy;
	double width, height;

	thiz = _entry_get(r);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->shape, ox, oy);

	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	//printf("width = %g height = %g\n", width, height);
	enesim_renderer_rectangle_size_set(thiz->shape, width, height);
	if (!thiz->text)
	{
		thiz->text = text;
		enesim_renderer_compound_layer_add(thiz->shape_fill, text);
		enesim_renderer_rop_set(text, ENESIM_BLEND);
		enesim_renderer_color_set(text, 0xff000000);
	}
	/* chech the alignment */
	enesim_renderer_boundings(text, &boundings);
	switch (alignment)
	{
		case EON_HORIZONTAL_ALIGNMENT_LEFT:
		tx = 4.0;
		ty = 4.0;
		break;

		case EON_HORIZONTAL_ALIGNMENT_CENTER:
		tx = (width - boundings.w) / 2.0;
		ty = 4.0;
		break;

		case EON_HORIZONTAL_ALIGNMENT_RIGHT:
		tx = width - boundings.w - 4.0;
		ty = 4.0;
		break;
	}
	enesim_renderer_origin_set(text, tx, ty);

	return EINA_TRUE;
}

static void _entry_free(Enesim_Renderer *r)
{
	Basic_Entry *thiz;

	thiz = _entry_get(r);
	free(thiz);
}

static Eon_Theme_Entry_Descriptor _descriptor = {
	.renderer_get = _entry_renderer_get,
	.setup = _entry_setup,
	.free = _entry_free,
	.margin_get = _entry_margin_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_entry_new(void)
{
	Enesim_Renderer *r;
	Basic_Entry *thiz;

	thiz = calloc(1, sizeof(Basic_Entry));
	if (!thiz) return NULL;
	thiz->radius = 4;
	thiz->border_color = 0xff555555;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_background_color_set(r, 0xffffffff);
	enesim_renderer_rop_set(r, ENESIM_FILL);

	r = enesim_renderer_compound_new();
	if (!r) goto shape_fill_err;
	thiz->shape_fill = r;
	enesim_renderer_compound_layer_add(r, thiz->background);

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	thiz->shape = r;
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_stroke_weight_set(r, _border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_fill_renderer_set(r, thiz->shape_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_entry_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	enesim_renderer_unref(thiz->shape);
shape_err:
	enesim_renderer_unref(thiz->shape_fill);
shape_fill_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}
