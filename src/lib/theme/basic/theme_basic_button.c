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
	Enesim_Color start_shadow;
	Enesim_Color end_shadow;
	Enesim_Color start_bevel;
	Enesim_Color end_bevel;
	uint8_t highlight;
	uint8_t shadow;
	double horizontal_padding;
	double vertical_padding;
	double radius;
	Enesim_Color border_color;
	Enesim_Color fill_color;
	/* private */
	Enesim_Renderer *background_fill;
	Enesim_Renderer *background;
	Enesim_Renderer *inner_button_fill;
	Enesim_Renderer *inner_button;
	Enesim_Renderer *compound;
	Enesim_Renderer *content;
	Enesim_Renderer_Sw_Fill fill;
} Button;

static const double _border_weight = 2.0;

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
	Enesim_Matrix m;
	Enesim_Renderer_Gradient_Stop stop;
	double width, height;

	thiz = _button_get(r);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	r = thiz->background_fill;
	m.xx = 0; m.xy = 0; m.xz = 0;
	m.yx = 0; m.yy = 1.0/height; m.yz = 0;
	m.zx = 0; m.zy = 0; m.zz = 1;
	enesim_renderer_transformation_set(r, &m);

	r = thiz->background;
	enesim_renderer_rectangle_width_set(r, width);
	enesim_renderer_rectangle_height_set(r, height);

	r = thiz->inner_button_fill;
	enesim_renderer_gradient_stop_clear(r);
	enesim_renderer_gradient_linear_x0_set(r, 0);
	enesim_renderer_gradient_linear_y0_set(r, 0);
	enesim_renderer_gradient_linear_x1_set(r, 0);
	enesim_renderer_gradient_linear_y1_set(r, height - thiz->radius);
	stop.argb = thiz->start_bevel;
	stop.pos = 0;
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = thiz->fill_color;
	stop.pos = (2.0 + 3) /(height - thiz->vertical_padding);
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = thiz->fill_color;
	stop.pos = 1 - stop.pos;
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = thiz->end_bevel;
	stop.pos = 1;
	enesim_renderer_gradient_stop_add(r, &stop);

	r = thiz->inner_button;
	enesim_renderer_rectangle_width_set(r, width - (thiz->horizontal_padding * 2));
	enesim_renderer_rectangle_height_set(r, height - (thiz->vertical_padding * 2));
}
/*----------------------------------------------------------------------------*
 *                         The Button theme interface                         *
 *----------------------------------------------------------------------------*/
static void _button_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Button *thiz;
	Enesim_Renderer *content;

	thiz = _button_get(r);
	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		margin->left = thiz->horizontal_padding + 15;;
		margin->right =thiz->horizontal_padding + 15;
		margin->top = thiz->vertical_padding + 15;
		margin->bottom = thiz->vertical_padding + 15;

	}
	else
	{
		margin->left = _border_weight + thiz->horizontal_padding + thiz->radius;
		margin->right = _border_weight + thiz->horizontal_padding + thiz->radius;
		margin->top = _border_weight + thiz->vertical_padding + thiz->radius;
		margin->bottom = _border_weight + thiz->vertical_padding + thiz->radius;
	}
}

static void _button_position_get(Enesim_Renderer *r, Eon_Size *size,
		Eon_Position *position)
{
	Button *thiz;

	thiz = _button_get(r);
	position->x = thiz->horizontal_padding + _border_weight + thiz->radius;
	position->y = thiz->vertical_padding + _border_weight + thiz->radius;
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
		enesim_renderer_compound_layer_add(thiz->compound, thiz->background);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->inner_button);
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
	thiz->start_shadow = 0x77000000;
	thiz->end_shadow = 0x22000000;
	thiz->radius = 4;
	thiz->horizontal_padding = 5;
	thiz->vertical_padding = 2;
	thiz->highlight = 0xff;
	thiz->shadow = 0x00;
	thiz->border_color = 0xff555555;
	thiz->fill_color = 0xffcccccc;
	thiz->start_bevel = 0xfffffff;
	thiz->end_bevel = 0xff000000;

	r = enesim_renderer_stripes_new();
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);
	thiz->background_fill = r;

	r = enesim_renderer_rectangle_new();
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->background_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	thiz->background = r;

	r = enesim_renderer_gradient_linear_new();
	thiz->inner_button_fill = r;

	r = enesim_renderer_rectangle_new();
	enesim_renderer_origin_set(r, thiz->horizontal_padding, thiz->vertical_padding);
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_stroke_weight_set(r, _border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_fill_renderer_set(r, thiz->inner_button_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->inner_button = r;

	r = enesim_renderer_compound_new();
	thiz->compound = r;

	r = eon_theme_button_new(&_descriptor, thiz);
	return r;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_fill_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->fill_color = color;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_border_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;
	Enesim_Renderer *rr;

	thiz = _button_get(r);
	thiz->border_color = color;

	rr = thiz->inner_button;
	enesim_renderer_shape_stroke_color_set(rr, thiz->border_color);
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

EAPI void eon_basic_button_start_shadow_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;
	Enesim_Renderer *rr;

	thiz = _button_get(r);
	thiz->start_shadow = color;

	rr = thiz->background_fill;
	enesim_renderer_stripes_odd_color_set(rr, thiz->start_shadow);
}

EAPI void eon_basic_button_end_shadow_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;
	Enesim_Renderer *rr;

	thiz = _button_get(r);
	thiz->end_shadow = color;

	rr = thiz->background_fill;
	enesim_renderer_stripes_even_color_set(rr, thiz->end_shadow);
}

EAPI void eon_basic_button_start_bevel_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->start_bevel = color;
}

EAPI void eon_basic_button_end_bevel_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->end_bevel = color;
}

