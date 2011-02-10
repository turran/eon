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
typedef struct _Checkbox
{
	Enesim_Renderer *box;
	Enesim_Renderer *check;
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill;
	unsigned int size;
} Checkbox;

static inline Checkbox * _checkbox_get(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = enesim_renderer_data_get(r);
	return thiz;
}

static void _checkbox_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	thiz->fill(thiz->box, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _checkbox_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	if (!enesim_renderer_sw_setup(thiz->box)) return EINA_FALSE;
	thiz->fill = enesim_renderer_sw_fill_get(thiz->box);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _checkbox_draw;

	return EINA_TRUE;
}

static void _checkbox_cleanup(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	enesim_renderer_sw_cleanup(thiz->box);
}

static void _checkbox_free(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	if (thiz->box)
		enesim_renderer_delete(thiz->box);
	free(thiz);
}

static void _checkbox_boundings(Enesim_Renderer *r, Eina_Rectangle *boundings)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	enesim_renderer_boundings(thiz->box, boundings);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _checkbox_setup,
	.sw_cleanup = _checkbox_cleanup,
	.boundings = _checkbox_boundings,
	.free = _checkbox_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_checkbox_new(void)
{
	Enesim_Renderer *r;
	Enesim_Renderer_Flag flags;
	Checkbox *thiz;

	thiz = calloc(1, sizeof(Checkbox));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_path_new();
	if (!r) goto path_err;
	thiz->check = r;
	enesim_renderer_shape_fill_color_set(r, 0xff000000);

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->box = r;
	enesim_renderer_shape_outline_weight_set(r, 2);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_outline_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_renderer_set(r, thiz->check);

	r = enesim_renderer_new(&_descriptor, flags, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_delete(thiz->box);
rectangle_err:
	enesim_renderer_delete(thiz->check);
path_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_checkbox_size_set(Enesim_Renderer *r, unsigned int size)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	thiz->size = size;

	enesim_renderer_rectangle_size_set(thiz->box, size, size);
	/* create the vertices */
	enesim_renderer_path_clear(thiz->check);
	enesim_renderer_path_move_to(thiz->check, size * 0.1, size * 0.45);
	enesim_renderer_path_line_to(thiz->check, size * 0.2, size * 0.35);
	enesim_renderer_path_line_to(thiz->check, size * 0.4, size * 0.6);
	enesim_renderer_path_line_to(thiz->check, size * 0.8, size * 0.15);
	enesim_renderer_path_line_to(thiz->check, size * 0.9, size * 0.25);
	enesim_renderer_path_line_to(thiz->check, size * 0.4, size * 0.85);
	enesim_renderer_path_line_to(thiz->check, size * 0.1, size * 0.45);
}

