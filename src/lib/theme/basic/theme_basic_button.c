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
typedef struct _Button
{
	/* properties */
	double horizontal_padding;
	double vertical_padding;
	double radius;
	/* private */
	Enesim_Renderer *rectangle;
	Enesim_Renderer *compound;
	Enesim_Renderer *content;
	Enesim_Renderer_Sw_Fill fill;
} Button;

static inline Button * _button_get(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = eon_theme_button_data_get(r);
	return thiz;
}

static void _button_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}

static void _button_update_rectangle(Enesim_Renderer *r)
{
	Button *thiz;
	double width, height;

	thiz = _button_get(r);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	enesim_renderer_rectangle_width_set(thiz->rectangle, width);
	enesim_renderer_rectangle_height_set(thiz->rectangle, height);
}
/*----------------------------------------------------------------------------*
 *                         The Button theme interface                         *
 *----------------------------------------------------------------------------*/
static void _button_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Button *thiz;

	thiz = _button_get(r);
	margin->left = thiz->radius + thiz->horizontal_padding;
	margin->right = thiz->radius + thiz->horizontal_padding;
	margin->top = thiz->radius + thiz->vertical_padding;
	margin->bottom = thiz->radius + thiz->vertical_padding;
}

static void _button_position_get(Enesim_Renderer *r, Eon_Size *size,
		Eon_Position *position)
{
	Button *thiz;

	thiz = _button_get(r);
	position->x = thiz->radius + thiz->horizontal_padding;
	position->y = thiz->radius + thiz->vertical_padding;
}

static Eina_Bool _button_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Button *thiz;
	Enesim_Renderer *content;
	double ox, oy;

	thiz = _button_get(r);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	/* setup the layers now */
	eon_theme_container_content_get(r, &content);
	if (thiz->content != content || !content)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->rectangle);
		if (content)
		{
			enesim_renderer_compound_layer_add(thiz->compound, content);
			/* FIXME at the cleanup we should restore this */
			enesim_renderer_rop_set(content, ENESIM_BLEND);
			thiz->content = content;
		}
	}
	_button_update_rectangle(r);

	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("not available to setup yet\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _button_draw;

	return EINA_TRUE;
}

static void _button_cleanup(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = _button_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _button_free(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = _button_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static Eon_Theme_Button_Descriptor _descriptor = {
	.margin_get = _button_margin_get,
	.position_get = _button_position_get,
	.sw_setup = _button_setup,
	.sw_cleanup = _button_cleanup,
	.free = _button_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_button_new(void)
{
	Enesim_Renderer *r;
	Button *thiz;

	thiz = calloc(1, sizeof(Button));
	if (!thiz) return NULL;
	thiz->horizontal_padding = 0;
	thiz->vertical_padding = 0;
	thiz->radius = 8;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->rectangle = r;
	/* setup the initial state */
	enesim_renderer_shape_stroke_weight_set(thiz->rectangle, 1);
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_draw_mode_set(thiz->rectangle, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_button_new(&_descriptor, thiz);
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
EAPI void eon_basic_button_foreground_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_shape_fill_color_set(thiz->rectangle, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_border_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_shape_stroke_color_set(thiz->rectangle, color);
}

EAPI void eon_basic_button_horizontal_padding_set(Enesim_Renderer *r, double padding)
{

	Button *thiz;

	thiz = _button_get(r);
	thiz->horizontal_padding = padding;
}

EAPI void eon_basic_button_vertical_padding_set(Enesim_Renderer *r, double padding)
{

	Button *thiz;

	thiz = _button_get(r);
	thiz->vertical_padding = padding;
}

EAPI void eon_basic_button_radius_set(Enesim_Renderer *r, double radius)
{

	Button *thiz;

	thiz = _button_get(r);
	thiz->radius = radius;
}

