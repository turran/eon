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
typedef struct _Checkbox
{
	/* properties */
	unsigned int size;
	Eina_Bool selected;
	Eina_Bool was_selected;
	/* private */
	Enesim_Renderer *content;
	Enesim_Renderer *background;
	Enesim_Renderer *box;
	Enesim_Renderer *check;
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill;
} Checkbox;

const static int checkbox_to_content_padding = 10;

static inline Checkbox * _checkbox_get(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = eon_theme_checkbox_data_get(r);
	return thiz;
}

static void _checkbox_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                       The Checkbox theme interface                         *
 *----------------------------------------------------------------------------*/
static void _checkbox_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	margin->left = thiz->size + checkbox_to_content_padding;
	margin->right = 0;
	margin->top = thiz->size / 2;
	margin->bottom = thiz->size / 2;
}

static Eina_Bool _checkbox_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Checkbox *thiz;
	Enesim_Renderer *content;
	Eina_Bool restack = EINA_FALSE;
	double ox, oy;
	double width, height;

	thiz = _checkbox_get(r);
	/* set the common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	/* setup the layers now */
	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		printf("checkbox no content\n");
		return EINA_FALSE;
	}
	if (thiz->content != content)
	{
		/* FIXME at the cleanup we should restore this */
		enesim_renderer_rop_set(content, ENESIM_BLEND);
		thiz->content = content;
		restack = EINA_TRUE;
	}
	if (thiz->was_selected != thiz->selected)
	{
		thiz->was_selected = thiz->selected;
		restack = EINA_TRUE;
	}
	if (restack)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->background);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->box);
		if (thiz->selected)
		{
			enesim_renderer_compound_layer_add(thiz->compound, thiz->check);
		}
		enesim_renderer_compound_layer_add(thiz->compound, content);
	}
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	enesim_renderer_y_origin_set(thiz->box, height/2 - thiz->size/2);
	enesim_renderer_y_origin_set(thiz->check, height/2 - thiz->size/2);
	enesim_renderer_rectangle_width_set(thiz->background, width);
	enesim_renderer_rectangle_height_set(thiz->background, height);

	if (!enesim_renderer_sw_setup(thiz->compound)) return EINA_FALSE;
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _checkbox_draw;

	return EINA_TRUE;
}

static void _checkbox_cleanup(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _checkbox_free(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static Eon_Theme_Checkbox_Descriptor _descriptor = {
	.margin_get = _checkbox_margin_get,
	.sw_setup = _checkbox_setup,
	.sw_cleanup = _checkbox_cleanup,
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
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->box = r;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_color_set(r, 0xffffffff);
	
	r = enesim_renderer_rectangle_new();
	if (!r) goto background_err;
	thiz->background = r;
	/* FIXME we are using the circle_radius from the radiobutton we must share such variable */
	enesim_renderer_rectangle_corner_radius_set(r, 8);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_color_set(r, 0xffeeeeee);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);

	r = eon_theme_checkbox_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->background);
background_err:
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

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_checkbox_selected_set(Enesim_Renderer *r, Eina_Bool selected)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	if (thiz->selected == selected)
		return;

	thiz->selected = selected;
}
