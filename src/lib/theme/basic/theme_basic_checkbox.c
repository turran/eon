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
typedef struct _Checkbox
{
	/* properties */
	unsigned int size;
	Eina_Bool selected;
	Eina_Bool was_selected;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *content;
	Enesim_Renderer *background;
	Enesim_Renderer *box;
	Enesim_Renderer *check;
	Enesim_Renderer *shape;
} Checkbox;

const static int checkbox_to_content_padding = 10;

static inline Checkbox * _checkbox_get(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = eon_theme_checkbox_data_get(r);
	return thiz;
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

static Enesim_Renderer * _checkbox_renderer_get(Enesim_Renderer *r)
{
	Checkbox *thiz;
	thiz = _checkbox_get(r);

	return thiz->shape;
}

static Eina_Bool  _checkbox_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Checkbox *thiz;
	Enesim_Renderer *content;
	Eina_Bool restack = EINA_FALSE;
	double ox, oy;
	double width, height;

	thiz = _checkbox_get(r);
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
	/* FIXME we should use the correct widget x, y */
	enesim_renderer_origin_get(r, &ox, &oy);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	/* set the needed properties */
	enesim_renderer_rectangle_position_set(thiz->shape, ox, oy);
	enesim_renderer_rectangle_width_set(thiz->shape, width);
	enesim_renderer_rectangle_height_set(thiz->shape, height);
	/* FIXME we need to avoid as many origin_get/set as we can */
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	enesim_renderer_y_origin_set(thiz->box, height/2 - thiz->size/2);
	enesim_renderer_y_origin_set(thiz->check, height/2 - thiz->size/2);

	return EINA_TRUE;
}

static void _checkbox_free(Enesim_Renderer *r)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	free(thiz);
}

static Eon_Theme_Checkbox_Descriptor _descriptor = {
	.margin_get = _checkbox_margin_get,
	.renderer_get = _checkbox_renderer_get,
	.setup = _checkbox_setup,
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

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_background_color_set(r, 0xffeeeeee);

	r = enesim_renderer_path_new();
	if (!r) goto path_err;
	thiz->check = r;
	enesim_renderer_shape_fill_color_set(r, 0xffff0000);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->box = r;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_color_set(r, 0xffffffff);

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	thiz->shape = r;
	/* FIXME we are using the circle_radius from the radiobutton we must share such variable */
	enesim_renderer_rectangle_corner_radius_set(r, 8);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);

	r = eon_theme_checkbox_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->shape);
shape_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->box);
rectangle_err:
	enesim_renderer_unref(thiz->check);
path_err:
	enesim_renderer_unref(thiz->shape);
background_err:
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
	enesim_renderer_path_command_clear(thiz->check);
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
