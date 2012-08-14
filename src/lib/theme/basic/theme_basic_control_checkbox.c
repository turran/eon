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
#include "eon_theme_control_checkbox.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Control_Checkbox
{
	/* properties */
	unsigned int size;
	/* private */
	double x;
	double y;

	Enesim_Renderer *background;
	Enesim_Renderer *compound;
	Enesim_Renderer *box;
	Enesim_Renderer *check;
} Eon_Basic_Control_Checkbox;

EAPI void eon_basic_control_checkbox_size_set(Eon_Theme_Widget *t, unsigned int size);

static inline Eon_Basic_Control_Checkbox * _checkbox_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Checkbox *thiz;

	thiz = eon_theme_control_checkbox_data_get(t);
	return thiz;
}

static void _basic_control_checkbox_update(Eon_Basic_Control_Checkbox *thiz)
{
	Enesim_Renderer *r;

	r = thiz->box;
	enesim_renderer_rectangle_size_set(r, thiz->size, thiz->size);
	r = thiz->check;
	enesim_renderer_path_move_to(r, thiz->size * 0.1, thiz->size * 0.45);
	enesim_renderer_path_line_to(r, thiz->size * 0.2, thiz->size * 0.35);
	enesim_renderer_path_line_to(r, thiz->size * 0.4, thiz->size * 0.6);
	enesim_renderer_path_line_to(r, thiz->size * 0.8, thiz->size * 0.15);
	enesim_renderer_path_line_to(r, thiz->size * 0.9, thiz->size * 0.25);
	enesim_renderer_path_line_to(r, thiz->size * 0.4, thiz->size * 0.85);
	enesim_renderer_path_line_to(r, thiz->size * 0.1, thiz->size * 0.45);
}
/*----------------------------------------------------------------------------*
 *                 The Eon_Control_Checkbox theme interface                   *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _basic_control_checkbox_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Checkbox *thiz;

	thiz = _checkbox_get(t);
	return thiz->box;
}

static void _basic_control_checkbox_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Control_Checkbox *thiz;
	Enesim_Matrix m;

	thiz = _checkbox_get(t);
	enesim_renderer_rectangle_x_set(thiz->box, x);
	thiz->x = x;
	enesim_matrix_translate(&m, thiz->x, thiz->y);
	enesim_renderer_geometry_transformation_set(thiz->check, &m);
}

static void _basic_control_checkbox_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Control_Checkbox *thiz;
	Enesim_Matrix m;

	thiz = _checkbox_get(t);
	enesim_renderer_rectangle_y_set(thiz->box, y);
	thiz->y = y;
	enesim_matrix_translate(&m, thiz->x, thiz->y);
	enesim_renderer_geometry_transformation_set(thiz->check, &m);
}

static void _basic_control_checkbox_size_get(Eon_Theme_Widget *t, Eon_Size *size)
{
	Eon_Basic_Control_Checkbox *thiz;

	thiz = _checkbox_get(t);
	size->width = thiz->size;
	size->height = thiz->size;
}

static void _basic_control_checkbox_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Checkbox *thiz;

	thiz = _checkbox_get(t);
	if (thiz->box)
		enesim_renderer_unref(thiz->box);
	if (thiz->check)
		enesim_renderer_unref(thiz->check);
	free(thiz);
}

static Eon_Theme_Control_Checkbox_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_control_checkbox_renderer_get,
	/* .x_set 		= */ _basic_control_checkbox_x_set,
	/* .y_set 		= */ _basic_control_checkbox_y_set,
	/* .free 		= */ _basic_control_checkbox_free,
	/* .size_get 		= */ _basic_control_checkbox_size_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_basic_control_checkbox_new(void)
{
	Eon_Theme_Widget *t;
	Eon_Basic_Control_Checkbox *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Checkbox));
	if (!thiz) return NULL;
	thiz->size = 16;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_rop_set(r, ENESIM_FILL);
	enesim_renderer_background_color_set(r, 0xffff0000);
	thiz->background = r;

	r = enesim_renderer_path_new();
	if (!r) goto path_err;
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_fill_color_set(r, 0xff000000);
	thiz->check = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_compound_layer_add(r, thiz->background);
	thiz->compound = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->box = r;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);

	_basic_control_checkbox_update(thiz);

	t = eon_theme_control_checkbox_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	enesim_renderer_unref(thiz->box);
rectangle_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->check);
path_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_control_checkbox_selected_set(Eon_Theme_Widget *t, Eina_Bool selected)
{
	Eon_Basic_Control_Checkbox *thiz;

	thiz = _checkbox_get(t);
	if (selected)
	{
		enesim_renderer_compound_layer_add(thiz->compound, thiz->check);
	}
	else
	{
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->check);
	}
}

EAPI void eon_basic_control_checkbox_size_set(Eon_Theme_Widget *t, unsigned int size)
{
	Eon_Basic_Control_Checkbox *thiz;

	thiz = _checkbox_get(t);
	if (thiz->size == size) return;
	thiz->size = size;

	_basic_control_checkbox_update(thiz);
	/* TODO inform about the new size upstream */

}
