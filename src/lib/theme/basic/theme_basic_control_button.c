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
#include "theme/eon_theme_widget.h"
#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Basic_Control_Button
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
	Enesim_Renderer *inner_button_fill;
	Enesim_Renderer *inner_button;
	Enesim_Renderer *compound;
	Enesim_Renderer *shape;
	Enesim_Renderer *proxy;
	Enesim_Renderer *proxy_default;
	Eina_Bool changed : 1;
};

static const double _border_weight = 2.0;

#if 0
static void _button_propagate(Eon_Basic_Control_Button *thiz,
		const Eon_Theme_Widget_State *current,
		Eina_Bool geometry_changed)
{
	Enesim_Renderer *r;
	Enesim_Matrix m;
	Enesim_Renderer_Gradient_Stop stop;
	double x = current->x;
	double y = current->y;
	double width = current->width;
	double height = current->height;

	if (geometry_changed)
	{
		/* setup the background fill */
		r = thiz->background_fill;
		m.xx = 0; m.xy = 0; m.xz = 0;
		m.yx = 0; m.yy = 1.0/height; m.yz = 0;
		m.zx = 0; m.zy = 0; m.zz = 1;
		enesim_renderer_transformation_set(r, &m);
		enesim_renderer_origin_set(r, x, y);

		/* setup the background */
		r = thiz->shape;
		enesim_renderer_rectangle_width_set(r, width);
		enesim_renderer_rectangle_height_set(r, height);
		enesim_renderer_rectangle_position_set(r, x, y);
	}

	if (geometry_changed || thiz->changed)
	{
		/* setup the inner button fill */
		r = thiz->inner_button_fill;
		enesim_renderer_origin_set(r, x, y);
		enesim_renderer_gradient_linear_x0_set(r, 0);
		enesim_renderer_gradient_linear_y0_set(r, 0);
		enesim_renderer_gradient_linear_x1_set(r, 0);
		enesim_renderer_gradient_linear_y1_set(r, height - (thiz->radius - 1));
		/* the gradient stops */
		enesim_renderer_gradient_stop_clear(r);
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
		/* setup the inner button */
		r = thiz->inner_button;
		enesim_renderer_rectangle_width_set(r, width - (thiz->horizontal_padding * 2));
		enesim_renderer_rectangle_height_set(r, height - (thiz->vertical_padding * 2));
		enesim_renderer_rectangle_position_set(r, x + thiz->horizontal_padding, y + thiz->vertical_padding);

		thiz->changed = EINA_FALSE;
	}
}
#endif
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Basic_Control_Button * eon_basic_control_button_new(void)
{
	Eon_Basic_Control_Button *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Button));
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
	if (!r) goto background_fill_err;
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);
	thiz->background_fill = r;

	r = enesim_renderer_gradient_linear_new();
	if (!r) goto inner_button_fill_err;
	thiz->inner_button_fill = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto inner_button_err;
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_stroke_weight_set(r, _border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	//enesim_renderer_shape_fill_color_set(r, 0xff00ffff);
	enesim_renderer_shape_fill_renderer_set(r, thiz->inner_button_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->inner_button = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_compound_layer_add(r, thiz->background_fill);
	enesim_renderer_compound_layer_add(r, thiz->inner_button);
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	thiz->shape = r;

	return thiz;

shape_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->inner_button);
inner_button_err:
	enesim_renderer_unref(thiz->inner_button_fill);
inner_button_fill_err:
	enesim_renderer_unref(thiz->background_fill);
background_fill_err:
	free(thiz);
	return NULL;
}

void eon_basic_control_button_fill_color_set(Eon_Basic_Control_Button *thiz, Enesim_Color color)
{
	thiz->fill_color = color;
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_border_color_set(Eon_Basic_Control_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;
	thiz->border_color = color;

	r = thiz->inner_button;
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_horizontal_padding_set(Eon_Basic_Control_Button *thiz, double padding)
{
	thiz->horizontal_padding = padding;
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_vertical_padding_set(Eon_Basic_Control_Button *thiz, double padding)
{
	thiz->vertical_padding = padding;
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_radius_set(Eon_Basic_Control_Button *thiz, double radius)
{
	thiz->radius = radius;
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_start_shadow_set(Eon_Basic_Control_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;

	thiz->start_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_end_shadow_set(Eon_Basic_Control_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;

	thiz->end_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_start_bevel_set(Eon_Basic_Control_Button *thiz, Enesim_Color color)
{
	thiz->start_bevel = color;
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_end_bevel_set(Eon_Basic_Control_Button *thiz, Enesim_Color color)
{
	thiz->end_bevel = color;
	thiz->changed = EINA_TRUE;
}

void eon_basic_control_button_margin_get(Eon_Basic_Control_Button *thiz,
		Enesim_Renderer *content, Eon_Margin *margin)
{
	if (!content)
	{
		margin->left = thiz->horizontal_padding + 15;;
		margin->right = thiz->horizontal_padding + 15;
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

#if 0
Eina_Bool eon_basic_control_button_setup(Eon_Basic_Control_Button *thiz,
		const Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		const Eon_Theme_Container_State *cstates[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Eina_Bool geometry_changed = EINA_FALSE;
	const Eon_Theme_Container_State *ccs = cstates[ENESIM_STATE_CURRENT];
	const Eon_Theme_Container_State *pcs = cstates[ENESIM_STATE_PAST];
	const Eon_Theme_Widget_State *cs = states[ENESIM_STATE_CURRENT];
	const Eon_Theme_Widget_State *ps = states[ENESIM_STATE_PAST];

	/* add the compound layers */
	if (ccs->content != pcs->content)
	{
		if (pcs->content)
		{
			enesim_renderer_compound_layer_remove(thiz->compound, pcs->content);
		}
		if (ccs->content)
		{
			enesim_renderer_compound_layer_add(thiz->compound, ccs->content);
		}
	}
	if ((cs->x != ps->x) ||
			(cs->y != ps->y) ||
			(cs->width != ps->width) ||
			(cs->height != ps->height))
	{
		geometry_changed = EINA_TRUE;
	}
	_button_propagate(thiz, cs, geometry_changed);

	return EINA_TRUE;
}
#endif

void eon_basic_control_button_x_set(Eon_Basic_Control_Button *thiz,
		double x)
{
	enesim_renderer_gradient_linear_x0_set(thiz->inner_button_fill, x);
	enesim_renderer_gradient_linear_x1_set(thiz->inner_button_fill, x);
}

void eon_basic_control_button_y_set(Eon_Basic_Control_Button *thiz,
		double y)
{
	enesim_renderer_gradient_linear_y0_set(thiz->inner_button_fill, y);
}

void eon_basic_control_button_width_set(Eon_Basic_Control_Button *thiz,
		double width)
{

}

void eon_basic_control_button_height_set(Eon_Basic_Control_Button *thiz,
		double height)
{
	enesim_renderer_gradient_linear_y1_set(thiz->inner_button_fill, height - (thiz->radius - 1));
#if 0
	enesim_renderer_gradient_stop_clear(r);
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
#endif
}

void eon_basic_control_button_renderer_content_set(Eon_Basic_Control_Button *thiz,
		Enesim_Renderer *content)
{
	enesim_renderer_proxy_proxied_set(thiz->proxy, content);
	if (!content)
		enesim_renderer_proxy_proxied_set(thiz->proxy, thiz->proxy_default);
}

Enesim_Renderer * eon_basic_control_button_renderer_get(Eon_Basic_Control_Button *thiz)
{
	return thiz->shape;
}

void eon_basic_control_button_free(Eon_Basic_Control_Button *thiz)
{
	enesim_renderer_unref(thiz->shape);
	enesim_renderer_unref(thiz->compound);
	enesim_renderer_unref(thiz->inner_button);
	enesim_renderer_unref(thiz->inner_button_fill);
	enesim_renderer_unref(thiz->background_fill);
	free(thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

