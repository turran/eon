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
typedef struct _Frame
{
	/* properties */
	/* private */
	Enesim_Renderer *description_area;
	Enesim_Renderer *rectangle;
	Enesim_Renderer *compound;
	Enesim_Renderer *content;
	Enesim_Renderer *description;
} Frame;

static const int rectangle_radius = 8;
static const int description_area_left_margin = 5;

static inline Frame * _frame_get(Enesim_Renderer *r)
{
	Frame *thiz;

	thiz = eon_theme_frame_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Frame theme interface                         *
 *----------------------------------------------------------------------------*/
static void _frame_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Frame *thiz;
	Enesim_Rectangle description_boundings;
	const char *description;

	thiz = _frame_get(r);
	eon_theme_frame_description_get(r, &description);
	etex_span_text_set(thiz->description, description);
	enesim_renderer_boundings(thiz->description, &description_boundings);
	margin->left = description_area_left_margin;
	margin->right = 0;
	margin->top = description_boundings.h + 3;
	margin->bottom = 0;
}

static Enesim_Renderer * _frame_renderer_get(Enesim_Renderer *r)
{
	Frame *thiz;

	thiz = _frame_get(r);
	return thiz->compound;
}

static Enesim_Renderer * _frame_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Frame *thiz;
	Enesim_Renderer *content;
	Enesim_Rectangle description_boundings;
	const char *description;
	double width, height;
	double ox, oy;

	thiz = _frame_get(r);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	/* setup the layers now */
	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		printf("frame no content\n");
		return EINA_FALSE;
	}
	eon_theme_frame_description_get(r, &description);
	etex_span_text_set(thiz->description, description);
	enesim_renderer_boundings(thiz->description, &description_boundings);
	if (thiz->content != content)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->rectangle);
		enesim_renderer_compound_layer_add(thiz->compound, content);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->description_area);
		/* FIXME at the cleanup we should restore this */
		enesim_renderer_rop_set(content, ENESIM_BLEND);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->description);
		thiz->content = content;
	}
	enesim_renderer_origin_set(thiz->rectangle, 0, description_boundings.h / 2);
	enesim_renderer_rectangle_width_set(thiz->rectangle, width);
	enesim_renderer_rectangle_height_set(thiz->rectangle, height - (description_boundings.h / 2));

	enesim_renderer_rectangle_width_set(thiz->description_area, width - description_area_left_margin);
	enesim_renderer_rectangle_height_set(thiz->description_area, description_boundings.h + 2);
	enesim_renderer_origin_set(thiz->description_area, description_area_left_margin, 0);

	enesim_renderer_origin_set(thiz->description, 10, 1);

	return EINA_TRUE;
}

static void _frame_free(Enesim_Renderer *r)
{
	Frame *thiz;

	thiz = _frame_get(r);
	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	free(thiz);
}

static Eon_Theme_Frame_Descriptor _descriptor = {
	.margin_get = _frame_margin_get,
	.renderer_get = _frame_renderer_get,
	.setup = _frame_setup,
	.free = _frame_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_frame_new(void)
{
	Enesim_Renderer *r;
	Frame *thiz;

	thiz = calloc(1, sizeof(Frame));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->rectangle = r;
	/* setup the initial state */
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_color_set(r, 0xffffffff);
	enesim_renderer_shape_stroke_weight_set(r, 1);
	enesim_renderer_rectangle_corner_radius_set(r, rectangle_radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = etex_span_new();
	if (!r) goto description_err;
	thiz->description = r;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_color_set(r, 0xff000000);

	r = enesim_renderer_rectangle_new();
	if (!r) goto description_area_err;
	thiz->description_area = r;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_fill_color_set(r, 0xffaaaaaa);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_stroke_weight_set(r, 1);
	enesim_renderer_rectangle_corner_radius_set(r, rectangle_radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_frame_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->description_area);
description_area_err:
	enesim_renderer_unref(thiz->description);
description_err:
	enesim_renderer_unref(thiz->rectangle);
rectangle_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_frame_foreground_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Frame *thiz;

	thiz = _frame_get(r);
	enesim_renderer_shape_fill_color_set(thiz->rectangle, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_frame_border_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Frame *thiz;

	thiz = _frame_get(r);
	enesim_renderer_shape_stroke_color_set(thiz->rectangle, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_frame_description_font_set(Enesim_Renderer *r, const char *font)
{
	Frame *thiz;

	thiz = _frame_get(r);
	etex_base_font_name_set(thiz->description, font);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_frame_description_size_set(Enesim_Renderer *r, int size)
{
	Frame *thiz;

	thiz = _frame_get(r);
	etex_base_size_set(thiz->description, size);

}


