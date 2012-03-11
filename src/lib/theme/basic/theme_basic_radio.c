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
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Radio
{
	/* properties */
	Eina_Bool selected;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *content;
	Enesim_Renderer *background;
	Enesim_Renderer *outter_circle;
	Enesim_Renderer *inner_circle;
	Enesim_Renderer *shape;
} Radio;

const static int radio_to_content_padding = 10;
const static int circle_radius = 8;

static inline Radio * _radio_get(Enesim_Renderer *r)
{
	Radio *thiz;

	thiz = eon_theme_radio_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                          The Radio theme interface                         *
 *----------------------------------------------------------------------------*/
static void _radio_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	margin->left = circle_radius + 2 + radio_to_content_padding;
	margin->right = 2;
	margin->top = circle_radius;
	margin->bottom = circle_radius;
}
static Enesim_Renderer * _radio_renderer_get(Enesim_Renderer *r)
{
	Radio *thiz;

	thiz = _radio_get(r);
	return thiz->shape;
}

static Eina_Bool _radio_setup(Enesim_Renderer *r,
		const Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		const Eon_Theme_Container_State *cstates[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Radio *thiz;
	const Eon_Theme_Container_State *ccs = cstates[ENESIM_STATE_CURRENT];
	const Eon_Theme_Container_State *pcs = cstates[ENESIM_STATE_PAST];
	const Eon_Theme_Widget_State *cs = states[ENESIM_STATE_CURRENT];
	const Eon_Theme_Widget_State *ps = states[ENESIM_STATE_PAST];

	thiz = _radio_get(r);

	/* add the compound layers */
	if (ccs->content != pcs->content)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->background);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->outter_circle);
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
		double x = cs->x;
		double y = cs->y;
		double width = cs->width;
		double height = cs->height;

		/* set the needed properties */
		enesim_renderer_rectangle_position_set(thiz->shape, x, y);
		enesim_renderer_rectangle_width_set(thiz->shape, width);
		enesim_renderer_rectangle_height_set(thiz->shape, height);
		enesim_renderer_origin_set(thiz->inner_circle, x + circle_radius + 2, y + height/2);
		enesim_renderer_origin_set(thiz->outter_circle, x + circle_radius + 2, y + height/2);
	}

	return EINA_TRUE;
}

static void _radio_free(Enesim_Renderer *r)
{
	Radio *rad;

	rad = _radio_get(r);
	if (rad->compound)
		enesim_renderer_unref(rad->compound);
	if (rad->outter_circle)
		enesim_renderer_unref(rad->outter_circle);
	if (rad->inner_circle)
		enesim_renderer_unref(rad->inner_circle);
	free(rad);
}

static Eon_Theme_Radio_Descriptor _descriptor = {
	.margin_get = _radio_margin_get,
	.renderer_get = _radio_renderer_get,
	.setup = _radio_setup,
	.free = _radio_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Enesim_Renderer * eon_basic_radio_new(void)
{
	Enesim_Renderer *r;
	Radio *thiz;

	thiz = calloc(1, sizeof(Radio));
	if (!thiz) return NULL;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_background_color_set(r, 0xffeeeeee);

	r = enesim_renderer_circle_new();
	if (!r) goto outter_err;
	thiz->outter_circle = r;
	enesim_renderer_circle_radius_set(r, circle_radius);
	enesim_renderer_shape_fill_color_set(r, 0xffffffff);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_circle_new();
	if (!r) goto inner_err;
	thiz->inner_circle = r;
	enesim_renderer_circle_radius_set(r, circle_radius);
	enesim_renderer_shape_stroke_weight_set(r, 3);
	enesim_renderer_shape_stroke_color_set(r, 0xffffffff);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	thiz->shape = r;
	enesim_renderer_rectangle_corner_radius_set(r, circle_radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);

	r = eon_theme_radio_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_unref(thiz->shape);
shape_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->inner_circle);
inner_err:
	enesim_renderer_unref(thiz->outter_circle);
outter_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_radio_selected_set(Enesim_Renderer *r, Eina_Bool selected)
{
	Radio *thiz;

	thiz = _radio_get(r);
	thiz->selected = selected;
	if (selected)
	{
		enesim_renderer_shape_fill_renderer_set(thiz->outter_circle, thiz->inner_circle);
	}
	else
	{
		enesim_renderer_shape_fill_renderer_set(thiz->outter_circle, NULL);
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_radio_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Radio *thiz;

	thiz = _radio_get(r);
	enesim_renderer_shape_stroke_color_set(thiz->outter_circle, color);
	enesim_renderer_shape_fill_color_set(thiz->inner_circle, color);
}
