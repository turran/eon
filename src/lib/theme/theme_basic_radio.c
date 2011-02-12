/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
#include "Eon_Basic.h"
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Radio
{
	Enesim_Renderer *outter_circle;
	Enesim_Renderer *inner_circle;
	Enesim_Renderer_Sw_Fill fill;
	Eina_Bool selected;
} Radio;

static inline Radio * _radio_get(Enesim_Renderer *r)
{
	Radio *rad;

	rad = enesim_renderer_data_get(r);
	return rad;
}

static void _radio_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Radio *rad;

	rad = _radio_get(r);
	rad->fill(rad->outter_circle, x, y, len, dst);
}

static Eina_Bool _radio_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Radio *thiz;
	double ox, oy;

	thiz = _radio_get(r);
	/* set the common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->outter_circle, ox, oy);
	/* get the fill function */
	if (!enesim_renderer_sw_setup(thiz->outter_circle))
	{
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->outter_circle);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _radio_draw;

	return EINA_TRUE;
}

static void _radio_cleanup(Enesim_Renderer *r)
{
	Radio *thiz;

	thiz = _radio_get(r);
	enesim_renderer_sw_cleanup(thiz->outter_circle);
}

static void _radio_boundings(Enesim_Renderer *r, Eina_Rectangle *bounds)
{
	Radio *thiz;

	thiz = _radio_get(r);
	enesim_renderer_boundings(thiz->outter_circle, bounds);
}

static void _radio_free(Enesim_Renderer *r)
{
	Radio *rad;

	rad = _radio_get(r);
	if (rad->outter_circle)
		enesim_renderer_delete(rad->outter_circle);
	if (rad->inner_circle)
		enesim_renderer_delete(rad->inner_circle);
	free(rad);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _radio_setup,
	.sw_cleanup = _radio_cleanup,
	.boundings = _radio_boundings,
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

	r = enesim_renderer_circle_new();
	if (!r) goto outter_err;
	thiz->outter_circle = r;
	enesim_renderer_circle_radius_set(r, 8);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_outline_weight_set(r, 2);

	r = enesim_renderer_circle_new();
	if (!r) goto inner_err;
	thiz->inner_circle = r;
	enesim_renderer_circle_radius_set(r, 8);
	enesim_renderer_shape_outline_weight_set(r, 3);
	enesim_renderer_shape_outline_color_set(r, 0xffffffff);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	/* TODO set the initial state calling the function */
	//printf("inner %p outter %p\n", thiz->inner_circle, thiz->outter_circle);

	r = enesim_renderer_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_delete(thiz->inner_circle);
inner_err:
	enesim_renderer_delete(thiz->outter_circle);
outter_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_radio_selected_set(Enesim_Renderer *r, int selected)
{
	Radio *thiz;

	thiz = _radio_get(r);
	thiz->selected = selected;
	if (selected)
	{
		enesim_renderer_shape_fill_renderer_set(thiz->outter_circle, thiz->inner_circle);
		enesim_renderer_shape_fill_color_set(thiz->outter_circle, 0xffffffff);
	}
	else
	{
		enesim_renderer_shape_fill_renderer_set(thiz->outter_circle, NULL);
		enesim_renderer_shape_fill_color_set(thiz->outter_circle, 0xffffffff);
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
	printf("setting the color\n");
	enesim_renderer_shape_outline_color_set(thiz->outter_circle, color);
	enesim_renderer_shape_fill_color_set(thiz->inner_circle, color);
}
