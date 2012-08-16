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
#include "eon_theme_control_radio.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Control_Radio
{
	/* properties */
	Enesim_Color start_shadow;
	Enesim_Color end_shadow;
	int circle_radius;
	/* private */
	Enesim_Renderer *compound;

	Enesim_Renderer *background_fill;
	Enesim_Renderer *background_circle;

	Enesim_Renderer *outter_circle;
	Enesim_Renderer *outter_fill;

	Enesim_Renderer *inner_circle;
} Eon_Basic_Control_Radio;

static inline Eon_Basic_Control_Radio * _eon_basic_control_radio_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Radio *thiz;
	thiz = eon_theme_control_radio_data_get(t);
	return thiz;
}

static Enesim_Renderer * _basic_control_radio_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);
	return thiz->compound;
}

static void _basic_control_radio_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);

	enesim_renderer_circle_x_set(thiz->background_circle, x + thiz->circle_radius);

	enesim_renderer_gradient_radial_center_x_set(thiz->outter_fill, x + thiz->circle_radius);
	enesim_renderer_gradient_radial_focus_x_set(thiz->outter_fill, x + thiz->circle_radius);
	enesim_renderer_circle_x_set(thiz->outter_circle, x + thiz->circle_radius);

	enesim_renderer_circle_x_set(thiz->inner_circle, x + thiz->circle_radius);

	//enesim_renderer_x_origin_set(thiz->background_fill, x);
}

static void _basic_control_radio_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);

	enesim_renderer_circle_y_set(thiz->background_circle, y + thiz->circle_radius);

	enesim_renderer_gradient_radial_center_y_set(thiz->outter_fill, y + thiz->circle_radius);
	enesim_renderer_gradient_radial_focus_y_set(thiz->outter_fill, y + thiz->circle_radius);
	enesim_renderer_circle_y_set(thiz->outter_circle, y + thiz->circle_radius);

	enesim_renderer_circle_y_set(thiz->inner_circle, y + thiz->circle_radius);

	//enesim_renderer_y_origin_set(thiz->background_fill, y);
}

static void _basic_control_radio_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);
	if (thiz->outter_circle)
		enesim_renderer_unref(thiz->outter_circle);
	if (thiz->inner_circle)
		enesim_renderer_unref(thiz->inner_circle);
	free(thiz);
}

static void _basic_control_radio_size_get(Eon_Theme_Widget *t, Eon_Size *size)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);
	size->width = size->height = thiz->circle_radius;
}

static Eon_Theme_Control_Radio_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_control_radio_renderer_get,
	/* .x_set 		= */ _basic_control_radio_x_set,
	/* .y_set 		= */ _basic_control_radio_y_set,
	/* .free 		= */ _basic_control_radio_free,
	/* .size_get 		= */ _basic_control_radio_size_get,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_basic_control_radio_new(void)
{
	Eon_Basic_Control_Radio *thiz;
	Enesim_Renderer *r;
	Enesim_Matrix m;
	Enesim_Renderer_Gradient_Stop stop;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Radio));
	if (!thiz) return NULL;
	thiz->circle_radius = 8;
	thiz->start_shadow = 0x77000000;
	thiz->end_shadow = 0x22000000;

	r = enesim_renderer_stripes_new();
	if (!r) goto background_fill_err;
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);
	m.xx = 0; m.xy = 0; m.xz = 0;
	m.yx = 0; m.yy = 1.0/thiz->circle_radius; m.yz = 0;
	m.zx = 0; m.zy = 0; m.zz = 1;
	enesim_renderer_transformation_set(r, &m);
	thiz->background_fill = r;

	r = enesim_renderer_circle_new();
	if (!r) goto background_err;
	enesim_renderer_circle_radius_set(r, thiz->circle_radius);
	enesim_renderer_shape_fill_renderer_set(r, thiz->background_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	thiz->background_circle = r;

	r = enesim_renderer_gradient_radial_new();
	if (!r) goto outter_fill_err;
	stop.argb = 0xffdddddd;
	stop.pos = 0;
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = 0xffa8a8a8;
	stop.pos = 0.9;
	enesim_renderer_gradient_stop_add(r, &stop);
	stop.argb = 0xffa8a8a8;
	stop.pos = 1;
	enesim_renderer_gradient_stop_add(r, &stop);
	enesim_renderer_gradient_radial_radius_set(r, thiz->circle_radius - 2);
	thiz->outter_fill = r;

	r = enesim_renderer_circle_new();
	if (!r) goto outter_err;
	enesim_renderer_circle_radius_set(r, thiz->circle_radius - 2);
	enesim_renderer_shape_fill_renderer_set(r, thiz->outter_fill);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_color_set(r, 0xff555555);
	enesim_renderer_shape_stroke_weight_set(r, 1);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->outter_circle = r;

	r = enesim_renderer_circle_new();
	if (!r) goto inner_err;
	enesim_renderer_circle_radius_set(r, 3);
	enesim_renderer_shape_fill_color_set(r, 0xff000000);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->inner_circle = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_compound_layer_add(r, thiz->background_circle);
	enesim_renderer_compound_layer_add(r, thiz->outter_circle);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->compound = r;

	return eon_theme_control_radio_new(&_descriptor, thiz);

compound_err:
	enesim_renderer_unref(thiz->inner_circle);
inner_err:
	enesim_renderer_unref(thiz->outter_circle);
outter_err:
	enesim_renderer_unref(thiz->outter_fill);
outter_fill_err:
	enesim_renderer_unref(thiz->background_circle);
background_err:
	enesim_renderer_unref(thiz->background_fill);
background_fill_err:
	free(thiz);
	return NULL;
}

void eon_basic_control_radio_start_shadow_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Radio *thiz;
	Enesim_Renderer *r;

	thiz = _eon_basic_control_radio_get(t);
	thiz->start_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
}

void eon_basic_control_radio_end_shadow_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Radio *thiz;
	Enesim_Renderer *r;

	thiz = _eon_basic_control_radio_get(t);
	thiz->end_shadow = color;
	r = thiz->background_fill;
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
}

EAPI void eon_basic_control_radio_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);
	enesim_renderer_shape_stroke_color_set(thiz->outter_circle, color);
}

EAPI void eon_basic_control_radio_selected_set(Eon_Theme_Widget *t, Eina_Bool selected)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);
	if (selected)
	{
		enesim_renderer_compound_layer_add(thiz->compound, thiz->inner_circle);
	}
	else
	{
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->inner_circle);
	}
}

EAPI void eon_basic_control_radio_selected_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Radio *thiz;

	thiz = _eon_basic_control_radio_get(t);
	enesim_renderer_shape_fill_color_set(thiz->inner_circle, color);
}
