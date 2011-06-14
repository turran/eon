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
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Radio
{
	/* properties */
	Eina_Bool selected;
	/* private */
	Enesim_Renderer *content;
	Enesim_Renderer *outter_circle;
	Enesim_Renderer *inner_circle;
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill;
} Radio;

const static int radio_to_content_padding = 10;
const static int circle_radius = 8;

static inline Radio * _radio_get(Enesim_Renderer *r)
{
	Radio *thiz;

	thiz = eon_theme_radio_data_get(r);
	return thiz;
}

static void _radio_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Radio *rad;

	rad = _radio_get(r);
	rad->fill(rad->compound, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme widget interface                      *
 *----------------------------------------------------------------------------*/
static double _radio_min_width_get(Enesim_Renderer *r)
{
	Radio *thiz;
	Enesim_Renderer *content;
	double min_width = circle_radius * 2 + 2;

	thiz = _radio_get(r);
	eon_theme_container_content_get(r, &content);
	if (!content)
		goto end;

	/* the padding */
	min_width += radio_to_content_padding;
	if (eon_is_element(content))
	{
		double cmin_width;

		eon_element_min_width_get(content, &cmin_width);
		min_width += cmin_width;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(content, &boundings);
		min_width += boundings.w;
	}
end:
	return min_width;
}

static double _radio_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _radio_min_height_get(Enesim_Renderer *r)
{
	Radio *thiz;
	Enesim_Renderer *content;
	double min_height = circle_radius * 2;

	thiz = _radio_get(r);
	eon_theme_container_content_get(r, &content);
	if (!content)
		goto end;
	if (eon_is_element(content))
	{
		double cmin_height;

		eon_element_min_height_get(content, &cmin_height);
		if (cmin_height > min_height)
			min_height = cmin_height;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(content, &boundings);
		if (boundings.w > min_height)
			min_height = boundings.h;
	}
end:
	return min_height;
}

static double _radio_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eon_Theme_Widget_Descriptor _twdescriptor = {
	.max_width_get = _radio_max_width_get,
	.max_height_get = _radio_max_height_get,
	.min_width_get = _radio_min_width_get,
	.min_height_get = _radio_min_height_get,
};
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _radio_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Radio *thiz;
	Enesim_Renderer *content;
	double ox, oy;

	thiz = _radio_get(r);
	/* set the common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	/* setup the layers now */
	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		return EINA_FALSE;
	}
	if (thiz->content != content)
	{
		enesim_renderer_compound_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->outter_circle);
		enesim_renderer_compound_layer_add(thiz->compound, content);
		/* FIXME at the cleanup we should restore this */
		enesim_renderer_rop_set(content, ENESIM_BLEND);
		thiz->content = content;
	}
	enesim_renderer_origin_set(thiz->content, circle_radius * 2 +
			radio_to_content_padding, 0);
	/* get the fill function */
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill)
	{
		return EINA_FALSE;
	}

	*fill = _radio_draw;

	return EINA_TRUE;
}

static void _radio_cleanup(Enesim_Renderer *r)
{
	Radio *thiz;

	thiz = _radio_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _radio_boundings(Enesim_Renderer *r, Enesim_Rectangle *bounds)
{
	Radio *thiz;

	thiz = _radio_get(r);
	enesim_renderer_boundings(thiz->compound, bounds);
}

static void _radio_free(Enesim_Renderer *r)
{
	Radio *rad;

	rad = _radio_get(r);
	if (rad->compound)
		enesim_renderer_delete(rad->compound);
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

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_circle_new();
	if (!r) goto outter_err;
	thiz->outter_circle = r;
	enesim_renderer_origin_set(r, circle_radius, circle_radius);
	enesim_renderer_circle_radius_set(r, circle_radius);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(r, 2);

	r = enesim_renderer_circle_new();
	if (!r) goto inner_err;
	thiz->inner_circle = r;
	enesim_renderer_circle_radius_set(r, circle_radius);
	enesim_renderer_shape_stroke_weight_set(r, 3);
	enesim_renderer_shape_stroke_color_set(r, 0xffffffff);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_radio_new(&_twdescriptor, &_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_delete(thiz->inner_circle);
inner_err:
	enesim_renderer_delete(thiz->outter_circle);
outter_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
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
	enesim_renderer_shape_stroke_color_set(thiz->outter_circle, color);
	enesim_renderer_shape_fill_color_set(thiz->inner_circle, color);
}
