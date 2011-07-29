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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Frame
{
	/* properties */
	/* private */
	Enesim_Renderer *rectangle;
	Enesim_Renderer *compound;
	Enesim_Renderer *content;
	Enesim_Renderer_Sw_Fill fill;
} Frame;

static const int horizontal_padding = 10;
static const int vertical_padding = 3;
static const int rectangle_radius = 8;

static inline Frame * _frame_get(Enesim_Renderer *r)
{
	Frame *thiz;

	thiz = eon_theme_frame_data_get(r);
	return thiz;
}

static void _frame_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Frame *thiz;

	thiz = _frame_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}

static void _frame_update_rectangle(Enesim_Renderer *r)
{
	Frame *thiz;
	double width, height;

	thiz = _frame_get(r);
	/* add 6px of padding to the text */
	/* set the size of the rectangle based on the size of the string */
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	enesim_renderer_rectangle_width_set(thiz->rectangle, width);
	enesim_renderer_rectangle_height_set(thiz->rectangle, height);
	/* always center */
	enesim_renderer_origin_set(thiz->content, horizontal_padding,
			vertical_padding);
}
/*----------------------------------------------------------------------------*
 *                         The Frame theme interface                         *
 *----------------------------------------------------------------------------*/
static double _frame_decoration_width_get(Enesim_Renderer *r)
{
	return horizontal_padding * 2;
}

static double _frame_decoration_height_get(Enesim_Renderer *r)
{
	return vertical_padding * 2;
}

static void _frame_content_position_get(Enesim_Renderer *r, Enesim_Renderer *content,
		double *x, double *y)
{
	*x = 0;
	*y = 0;
}

static Eina_Bool _frame_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Frame *thiz;
	Enesim_Renderer *content;
	double ox, oy;

	thiz = _frame_get(r);
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
	if (thiz->content != content)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->rectangle);
		enesim_renderer_compound_layer_add(thiz->compound, content);
		/* FIXME at the cleanup we should restore this */
		enesim_renderer_rop_set(content, ENESIM_BLEND);
		thiz->content = content;
	}
	_frame_update_rectangle(r);

	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("not available to setup yet\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _frame_draw;

	return EINA_TRUE;
}

static void _frame_cleanup(Enesim_Renderer *r)
{
	Frame *thiz;

	thiz = _frame_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _frame_free(Enesim_Renderer *r)
{
	Frame *thiz;

	thiz = _frame_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static Eon_Theme_Frame_Descriptor _descriptor = {
	.content_position_get = _frame_content_position_get,
	.decoration_width_get = _frame_decoration_width_get,
	.decoration_height_get = _frame_decoration_height_get,
	.sw_setup = _frame_setup,
	.sw_cleanup = _frame_cleanup,
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
	enesim_renderer_shape_stroke_weight_set(thiz->rectangle, 1);
	enesim_renderer_rectangle_corner_radius_set(r, rectangle_radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_draw_mode_set(thiz->rectangle, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_frame_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->rectangle);
rectangle_err:
	enesim_renderer_delete(thiz->compound);
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

