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
#include "eon_basic.h"
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


#if 0
static void eon_basic_drawer_button_horizontal_padding_set(Eon_Basic_Drawer_Button *thiz, double padding)
{
	thiz->horizontal_padding = padding;
}

static void eon_basic_drawer_button_vertical_padding_set(Eon_Basic_Drawer_Button *thiz, double padding)
{
	thiz->vertical_padding = padding;
}

static void eon_basic_drawer_button_radius_set(Eon_Basic_Drawer_Button *thiz, double radius)
{
	thiz->radius = radius;
}

#endif
/*----------------------------------------------------------------------------*
 *                         Attribute setters/getters                           *
 *----------------------------------------------------------------------------*/
static void _eon_basic_drawer_button_border_color_set(Eon_Drawer_Widget *w, Enesim_Color color)
{
	Eon_Basic_Drawer_Button *thiz;
	Enesim_Renderer *r;

	thiz = eon_drawer_button_data_get(w);
	thiz->border_color = color;
	r = thiz->inner_button;
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
}

static Enesim_Color _eon_basic_drawer_button_border_color_get(Eon_Drawer_Widget *w)
{
	Eon_Basic_Drawer_Button *thiz;

	thiz = eon_drawer_button_data_get(w);
	return thiz->border_color;
}

static void _eon_basic_drawer_button_fill_color_set(Eon_Drawer_Widget *w, Enesim_Color color)
{
	Eon_Basic_Drawer_Button *thiz;

	thiz = eon_drawer_button_data_get(w);
	thiz->fill_color = color;
	_set_gradient_stops(thiz);
}

static Enesim_Color _eon_basic_drawer_button_fill_color_get(Eon_Drawer_Widget *w)
{
	Eon_Basic_Drawer_Button *thiz;

	thiz = eon_drawer_button_data_get(w);
	return thiz->fill_color;
}

static void eon_basic_drawer_button_start_shadow_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
{
	Enesim_Renderer *r;

	thiz->start_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
}

static void eon_basic_drawer_button_end_shadow_set(Eon_Basic_Drawer_Button *thiz, Enesim_Color color)
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
/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_basic_drawer_button_content_set(Eon_Drawer_Widget *w,
		void *data, Enesim_Renderer *r)
{
	Eon_Basic_Drawer_Button *thiz = data;

	if (r)
		enesim_renderer_proxy_proxied_set(thiz->proxy, r);
	else
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->proxy_default));
}

static void _eon_basic_drawer_button_padding_get(Eon_Drawer_Widget *w,
		void *data, Eon_Box *padding)
{
	Eon_Basic_Drawer_Button *thiz = data;

	printf("[basic] padding get\n");
	padding->top = thiz->vertical_padding + thiz->radius;
	padding->bottom = thiz->vertical_padding + thiz->radius;
	padding->left = thiz->horizontal_padding + thiz->radius;
	padding->right = thiz->horizontal_padding + thiz->radius;
}

static Enesim_Renderer * _eon_basic_drawer_button_renderer_get(
		Eon_Drawer_Widget *w, void *data)
{
	Eon_Basic_Drawer_Button *thiz = data;
	printf("[basic] renderer get\n");
	return enesim_renderer_ref(thiz->shape);
}

static void _eon_basic_drawer_button_geometry_set(Eon_Drawer_Widget *w,
		void *data, Eina_Rectangle *geom)
{
	Eon_Basic_Drawer_Button *thiz = data;
	Enesim_Matrix m;
	Enesim_Renderer *r;
	int x, y, width, height;

	printf("[basic] geometry set\n");
	x = geom->x;
	y = geom->y;
	width = geom->w;
	height = geom->h;

	/* the x */
	enesim_renderer_x_origin_set(thiz->background_fill, x);
	enesim_renderer_gradient_linear_x0_set(thiz->inner_button_fill, x);
	enesim_renderer_gradient_linear_x1_set(thiz->inner_button_fill, x);
	enesim_renderer_rectangle_x_set(thiz->inner_button, x + thiz->horizontal_padding);
	enesim_renderer_rectangle_x_set(thiz->shape, x);

	/* the y */
	enesim_renderer_y_origin_set(thiz->background_fill, y);
	enesim_renderer_gradient_linear_y0_set(thiz->inner_button_fill, y);
	enesim_renderer_rectangle_y_set(thiz->inner_button, y + thiz->vertical_padding);
	enesim_renderer_rectangle_y_set(thiz->shape, y);
	thiz->y = y;

	/* the width */
	enesim_renderer_rectangle_width_set(thiz->inner_button, width - (thiz->horizontal_padding * 2));
	enesim_renderer_rectangle_width_set(thiz->shape, width);
	/* the height */
	/* store the height */
	thiz->height = height;

	printf("height = %d\n", height);
	r = thiz->background_fill;
	m.xx = 1; m.xy = 0; m.xz = 0;
	m.yx = 0; m.yy = height; m.yz = 0;
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

static void _eon_basic_drawer_button_ender_populate(Eon_Drawer_Widget *w,
		void *data, Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *attr;

	/* add our own attributes */
	printf("[basic] ender populate\n");
	attr = ender_attr_int_new("border-color",
			ENDER_ATTR_INT_GET(_eon_basic_drawer_button_border_color_get),
			ENDER_ATTR_INT_SET(_eon_basic_drawer_button_border_color_set));
	egueb_dom_element_attribute_add(n, attr, NULL);
	attr = ender_attr_int_new("fill-color",
			ENDER_ATTR_INT_GET(_eon_basic_drawer_button_fill_color_get),
			ENDER_ATTR_INT_SET(_eon_basic_drawer_button_fill_color_set));
	egueb_dom_element_attribute_add(n, attr, NULL);
}

static Eina_Bool _eon_basic_drawer_button_ender_process(Eon_Drawer_Widget *w,
		void *data, Egueb_Dom_Node *n)
{
	printf("[basic] ender process\n");
}

static void _eon_basic_drawer_button_free(Eon_Drawer_Widget *w, void *data)
{
	Eon_Basic_Drawer_Button *thiz = data;

	enesim_renderer_unref(thiz->proxy);
	enesim_renderer_unref(thiz->proxy_default);
	enesim_renderer_unref(thiz->shape);
	enesim_renderer_unref(thiz->compound);
	enesim_renderer_unref(thiz->inner_button);
	enesim_renderer_unref(thiz->inner_button_fill);
	enesim_renderer_unref(thiz->background_fill);
	free(thiz);

}

static Eon_Drawer_Button_Descriptor _descriptor = {
	/* .content_set		= */ _eon_basic_drawer_button_content_set,
	/* .padding_get 	= */ _eon_basic_drawer_button_padding_get,
	/* .renderer_get 	= */ _eon_basic_drawer_button_renderer_get,
	/* .geometry_set 	= */ _eon_basic_drawer_button_geometry_set,
	/* .ender_populate	= */ _eon_basic_drawer_button_ender_populate,
	/* .ender_process	= */ _eon_basic_drawer_button_ender_process,
	/* .free 		= */ _eon_basic_drawer_button_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Drawer_Widget * eon_basic_drawer_button_new(void)
{
	Eon_Basic_Drawer_Button *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Basic_Drawer_Button));
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
	enesim_renderer_color_set(r, 0x00000000);
	thiz->proxy_default = r;

	r = enesim_renderer_proxy_new();
	enesim_renderer_proxy_proxied_set(r, enesim_renderer_ref(thiz->proxy_default));
	thiz->proxy = r;

	r = enesim_renderer_stripes_new();
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);
	thiz->background_fill = r;

	r = enesim_renderer_gradient_linear_new();
	thiz->inner_button_fill = r;

	r = enesim_renderer_rectangle_new();
	enesim_renderer_rectangle_corner_radii_set(r, thiz->radius, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_stroke_weight_set(r, thiz->border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_shape_fill_renderer_set(r, enesim_renderer_ref(thiz->inner_button_fill));
	enesim_renderer_shape_draw_mode_set(r, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
	thiz->inner_button = r;

	r = enesim_renderer_compound_new();
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->background_fill));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(r, l);

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->inner_button));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(r, l);

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->proxy));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(r, l);
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	enesim_renderer_rectangle_corner_radii_set(r, thiz->radius, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, enesim_renderer_ref(thiz->compound));
	enesim_renderer_shape_draw_mode_set(r, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	thiz->shape = r;

	return eon_drawer_button_new(&_descriptor, thiz);
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

