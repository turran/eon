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
typedef struct _Basic_Spin
{
	/* properties */
	Enesim_Color start_shadow;
	Enesim_Color end_shadow;
	double radius;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *arrows;
	Enesim_Renderer *arrows_background;
	Enesim_Renderer *arrows_background_shadow;
} Basic_Spin;

static inline Basic_Spin * _spin_get(Enesim_Renderer *r)
{
	Basic_Spin *thiz;

	thiz = eon_theme_spin_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Spin theme interface                         *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _spin_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Enesim_Renderer *rr;
	Basic_Spin *thiz;
	Enesim_Matrix m;
	double ox, oy;
	double width, height;

	thiz = _spin_get(r);

	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	rr = thiz->arrows_background_shadow;
	m.xx = 0; m.xy = 0; m.xz = 0;
	m.yx = 0; m.yy = 1.0/height; m.yz = 0;
	m.zx = 0; m.zy = 0; m.zz = 1;
	enesim_renderer_transformation_set(rr, &m);

	rr = thiz->arrows_background;
	enesim_renderer_rectangle_width_set(rr, 12);
	enesim_renderer_rectangle_height_set(rr, height - 2);

	rr = thiz->arrows;
	enesim_renderer_origin_set(rr, width - 13, 1);

	return thiz->compound;
}

static void _spin_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	margin->left = 2;
	margin->right = 14;
	margin->top = 2;
	margin->bottom = 2;
}

static void _spin_entry_set(Enesim_Renderer *r, Enesim_Renderer *entry, Enesim_Renderer *old_entry)
{
	Basic_Spin *thiz;

	thiz = _spin_get(r);
	if (old_entry)
	{
		enesim_renderer_compound_layer_remove(thiz->compound, old_entry);
	}

	if (entry)
	{
		enesim_renderer_compound_layer_add(thiz->compound, entry);
		enesim_renderer_rop_set(entry, ENESIM_BLEND);
	}
}

static void _spin_free(Enesim_Renderer *r)
{
	Basic_Spin *thiz;

	thiz = _spin_get(r);
	free(thiz);
}

static Eon_Theme_Spin_Descriptor _descriptor = {
	.entry_set = _spin_entry_set,
	.margin_get = _spin_margin_get,
	.setup = _spin_setup,
	.free = _spin_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_spin_new(void)
{
	Enesim_Renderer *r;
	Basic_Spin *thiz;

	thiz = calloc(1, sizeof(Basic_Spin));
	if (!thiz) return NULL;
	thiz->start_shadow = 0x77000000;
	thiz->end_shadow = 0x22000000;
	thiz->radius = 4;

	r = enesim_renderer_stripes_new();
	enesim_renderer_stripes_odd_color_set(r, thiz->start_shadow);
	enesim_renderer_stripes_even_color_set(r, thiz->end_shadow);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	thiz->arrows_background_shadow = r;

	r = enesim_renderer_rectangle_new();
	enesim_renderer_rectangle_corner_radius_set(r, thiz->radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->arrows_background_shadow);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	thiz->arrows_background = r;

	r = enesim_renderer_compound_new();
	enesim_renderer_compound_layer_add(r, thiz->arrows_background);
	thiz->arrows = r;

	r = enesim_renderer_compound_new();
	enesim_renderer_compound_layer_add(r, thiz->arrows);
	thiz->compound = r;

	r = eon_theme_spin_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	free(thiz);
	return NULL;
}
