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
#include "Eon_Drawer.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Drawer_Button
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
	double border_weight;
	/* private */
	Enesim_Renderer *background_fill;
	Enesim_Renderer *inner_button_fill;
	Enesim_Renderer *inner_button;
	Enesim_Renderer *compound;
	Enesim_Renderer *shape;
	Enesim_Renderer *proxy;
	Enesim_Renderer *proxy_default;
	/* last properties set */
	double y;
	double height;
} Eon_Basic_Drawer_Button;

static inline void _set_gradient_stops(Eon_Basic_Drawer_Button *thiz)
{
	Enesim_Renderer_Gradient_Stop stop;
	Enesim_Renderer *r;

	r = thiz->inner_button_fill;
	enesim_renderer_gradient_stop_clear(r);
	stop.argb = thiz->start_bevel;
	stop.pos = 0;
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = thiz->fill_color;
	stop.pos = (2.0 + 3) / (thiz->height - thiz->vertical_padding);
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = thiz->fill_color;
	stop.pos = 1 - stop.pos;
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = thiz->end_bevel;
	stop.pos = 1;
	enesim_renderer_gradient_stop_add(r, &stop);
}


/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_basic_drawer_button_content_set(Eon_Drawer_Widget *w,
		void *data, Enesim_Renderer *r)
{

}

static void _eon_basic_drawer_button_min_size_get(Eon_Drawer_Widget *w,
		void *data, Eon_Size *min)
{

}

static Enesim_Renderer * _eon_basic_drawer_button_renderer_get(
		Eon_Drawer_Widget *w, void *data)
{

}

static void _eon_basic_drawer_button_geometry_set(Eon_Drawer_Widget *w,
		void *data, Eina_Rectangle *geom)
{

}

static void _eon_basic_drawer_button_free(Eon_Drawer_Widget *w, void *data)
{

}

Eon_Drawer_Button_Descriptor _descriptor = {
	/* .content_set		= */ _eon_basic_drawer_button_content_set,
	/* .min_size_get 	= */ _eon_basic_drawer_button_min_size_get,
	/* .renderer_get 	= */ _eon_basic_drawer_button_renderer_get,
	/* .geometry_set 	= */ _eon_basic_drawer_button_geometry_set,
	/* .free 		= */ _eon_basic_drawer_button_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Basic_Drawer_Button * eon_basic_drawer_button_new(void)
{
	Eon_Basic_Drawer_Button *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Basic_Drawer_Button));
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
	thiz->border_weight = 1.0;

	/* FIXME the proxy default is for now, until we fix enesim to make
	 * a renderer "active"
	 */
	r = enesim_renderer_background_new();
	if (!r) goto proxy_default_err;
	enesim_renderer_color_set(r, 0x00000000);
	thiz->proxy_default = r;

	r = enesim_renderer_proxy_new();
	if (!r) goto content_proxy_err;
	enesim_renderer_proxy_proxied_set(r, thiz->proxy_default);
	thiz->proxy = r;

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
	enesim_renderer_shape_stroke_weight_set(r, thiz->border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_fill_renderer_set(r, thiz->inner_button_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
	thiz->inner_button = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, thiz->background_fill);
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(r, l);

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, thiz->inner_button);
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(r, l);

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, thiz->proxy);
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(r, l);
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
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
	enesim_renderer_unref(thiz->proxy);
content_proxy_err:
	enesim_renderer_unref(thiz->proxy_default);
proxy_default_err:
	free(thiz);
	return NULL;
}

void eon_basic_drawer_button_fill_color_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	thiz->fill_color = color;
	_set_gradient_stops(thiz);
}

void eon_basic_drawer_button_border_color_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;
	thiz->border_color = color;

	r = thiz->inner_button;
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
}

void eon_basic_drawer_button_horizontal_padding_set(Eon_Basic_Drawer_Button *thiz, double padding)
{
	thiz->horizontal_padding = padding;
}

void eon_basic_drawer_button_vertical_padding_set(Eon_Basic_Drawer_Button *thiz, double padding)
{
	thiz->vertical_padding = padding;
}

void eon_basic_drawer_button_radius_set(Eon_Basic_Drawer_Button *thiz, double radius)
{
	thiz->radius = radius;
}

void eon_basic_drawer_button_start_shadow_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;

	thiz->start_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
}

void eon_basic_drawer_button_end_shadow_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;

	thiz->end_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
}

void eon_basic_drawer_button_start_bevel_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	thiz->start_bevel = color;
	_set_gradient_stops(thiz);
}

void eon_basic_drawer_button_end_bevel_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	thiz->end_bevel = color;
	_set_gradient_stops(thiz);
}

void eon_basic_drawer_button_x_set(Eon_Basic_Drawer_Button *thiz,
		double x)
{
	enesim_renderer_x_origin_set(thiz->background_fill, x);
	enesim_renderer_gradient_linear_x0_set(thiz->inner_button_fill, x);
	enesim_renderer_gradient_linear_x1_set(thiz->inner_button_fill, x);
	enesim_renderer_rectangle_x_set(thiz->inner_button, x + thiz->horizontal_padding);
	enesim_renderer_rectangle_x_set(thiz->shape, x);
}

void eon_basic_drawer_button_y_set(Eon_Basic_Drawer_Button *thiz,
		double y)
{
	enesim_renderer_y_origin_set(thiz->background_fill, y);
	enesim_renderer_gradient_linear_y0_set(thiz->inner_button_fill, y);
	enesim_renderer_rectangle_y_set(thiz->inner_button, y + thiz->vertical_padding);
	enesim_renderer_rectangle_y_set(thiz->shape, y);
	thiz->y = y;
}

void eon_basic_drawer_button_width_set(Eon_Basic_Drawer_Button *thiz,
		double width)
{
	enesim_renderer_rectangle_width_set(thiz->inner_button, width - (thiz->horizontal_padding * 2));
	enesim_renderer_rectangle_width_set(thiz->shape, width);
}

void eon_basic_drawer_button_height_set(Eon_Basic_Drawer_Button *thiz,
		double height)
{
	Enesim_Matrix m;
	Enesim_Renderer *r;

	/* store the height */
	thiz->height = height;

	r = thiz->background_fill;
	m.xx = 0; m.xy = 0; m.xz = 0;
	m.yx = 0; m.yy = 1.0/height; m.yz = 0;
	m.zx = 0; m.zy = 0; m.zz = 1;
	enesim_renderer_transformation_set(r, &m);
	/* the gradient stops */
	r = thiz->inner_button_fill;
	enesim_renderer_gradient_linear_y1_set(r, thiz->y + (height - (thiz->radius - 1)));
	_set_gradient_stops(thiz);

	r = thiz->inner_button;
	enesim_renderer_rectangle_height_set(r, height - (thiz->vertical_padding * 2));
	r = thiz->shape;
	enesim_renderer_rectangle_height_set(r, height);
}

void eon_basic_drawer_button_renderer_content_set(Eon_Basic_Drawer_Button *thiz,
		Enesim_Renderer *content)
{
	enesim_renderer_proxy_proxied_set(thiz->proxy, content);
	if (!content)
		enesim_renderer_proxy_proxied_set(thiz->proxy, thiz->proxy_default);
}

#if 0
void eon_basic_drawer_button_renderer_content_get(Eon_Basic_Drawer_Button *thiz,
		Enesim_Renderer **content)
{
	enesim_renderer_proxy_proxied_get(thiz->proxy, content);
	if (*content == thiz->proxy_default) *content = NULL;
}
#endif

Enesim_Renderer * eon_basic_drawer_button_renderer_get(Eon_Basic_Drawer_Button *thiz)
{
	return thiz->shape;
}

void eon_basic_drawer_button_free(Eon_Basic_Drawer_Button *thiz)
{
	enesim_renderer_unref(thiz->proxy);
	enesim_renderer_unref(thiz->proxy_default);
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

