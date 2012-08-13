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
#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Basic_Control_Radio
{
	/* properties */
	int circle_radius;
	/* private */
	Enesim_Renderer *background;
	Enesim_Renderer *outter_circle;
	Enesim_Renderer *inner_circle;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Basic_Control_Radio * eon_basic_control_radio_new(void)
{
	Enesim_Renderer *r;
	Eon_Basic_Control_Radio *thiz;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Radio));
	if (!thiz) return NULL;
	thiz->circle_radius = 8;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_background_color_set(r, 0xffeeeeee);

	r = enesim_renderer_circle_new();
	if (!r) goto outter_err;
	thiz->outter_circle = r;
	enesim_renderer_circle_radius_set(r, thiz->circle_radius);
	enesim_renderer_shape_fill_color_set(r, 0xffffffff);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_circle_new();
	if (!r) goto inner_err;
	thiz->inner_circle = r;
	enesim_renderer_circle_radius_set(r, thiz->circle_radius);
	enesim_renderer_shape_stroke_weight_set(r, 3);
	enesim_renderer_shape_stroke_color_set(r, 0xffffffff);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	return thiz;

inner_err:
	enesim_renderer_unref(thiz->outter_circle);
outter_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}

Enesim_Renderer * eon_basic_control_radio_renderer_get(Eon_Basic_Control_Radio *thiz)
{
	return thiz->outter_circle;
}

void eon_basic_control_radio_x_set(Eon_Basic_Control_Radio *thiz, double x)
{
	enesim_renderer_circle_x_set(thiz->inner_circle, x);
	enesim_renderer_circle_x_set(thiz->outter_circle, x + thiz->circle_radius + 2);
}

void eon_basic_control_radio_y_set(Eon_Basic_Control_Radio *thiz, double y)
{
	enesim_renderer_circle_y_set(thiz->outter_circle, y);
	enesim_renderer_circle_y_set(thiz->inner_circle, y + thiz->circle_radius + 2);
}

void eon_basic_control_radio_free(Eon_Basic_Control_Radio *thiz)
{
	if (thiz->outter_circle)
		enesim_renderer_unref(thiz->outter_circle);
	if (thiz->inner_circle)
		enesim_renderer_unref(thiz->inner_circle);
	free(thiz);
}

void eon_basic_control_radio_selected_set(Eon_Basic_Control_Radio *thiz, Eina_Bool selected)
{
	if (selected)
	{
		enesim_renderer_shape_fill_renderer_set(thiz->outter_circle, thiz->inner_circle);
	}
	else
	{
		enesim_renderer_shape_fill_renderer_set(thiz->outter_circle, NULL);
	}
}

void eon_basic_control_radio_background_color_set(Eon_Basic_Control_Radio *thiz, Enesim_Color background_color)
{
	enesim_renderer_background_color_set(thiz->background, background_color);
}

void eon_basic_control_radio_color_set(Eon_Basic_Control_Radio *thiz, Enesim_Color color)
{
	enesim_renderer_shape_stroke_color_set(thiz->outter_circle, color);
	enesim_renderer_shape_fill_color_set(thiz->inner_circle, color);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

