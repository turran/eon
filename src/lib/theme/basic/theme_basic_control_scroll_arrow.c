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
#include "eon_theme_control_scroll_arrow.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Control_Scroll_Arrow
{
	Enesim_Renderer *shape;
	Enesim_Renderer *arrow;
	Enesim_Color border_color;
	Enesim_Color fill_color;
	double border_weight;

	Enesim_Matrix ms[EON_DIRECTIONS];
	Enesim_Matrix *m;

	double x;
	double y;
} Eon_Basic_Control_Scroll_Arrow;

static inline Eon_Basic_Control_Scroll_Arrow * _eon_basic_control_scroll_arrow_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	thiz = eon_theme_control_scroll_arrow_data_get(t);
	return thiz;
}

static Enesim_Renderer * _basic_control_scroll_arrow_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_basic_control_scroll_arrow_get(t);
	return thiz->shape;
}

static void _basic_control_scroll_arrow_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	Enesim_Matrix m;

	thiz = _eon_basic_control_scroll_arrow_get(t);
	enesim_matrix_translate(&m, thiz->x, thiz->y);
	enesim_matrix_compose(thiz->m, &m, &m);
	enesim_renderer_geometry_transformation_set(thiz->shape, &m);
}

static void _basic_control_scroll_arrow_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	Enesim_Matrix m;

	thiz = _eon_basic_control_scroll_arrow_get(t);
	enesim_matrix_translate(&m, thiz->x, thiz->y);
	enesim_matrix_compose(thiz->m, &m, &m);
	enesim_renderer_geometry_transformation_set(thiz->shape, &m);
}

static void _basic_control_scroll_arrow_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_basic_control_scroll_arrow_get(t);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	free(thiz);
}

static void _basic_control_scroll_arrow_size_get(Eon_Theme_Widget *t, Eon_Size *size)
{
	size->width = size->height = 4;
}

static void _basic_control_scroll_arrow_direction_set(Eon_Theme_Widget *t, Eon_Direction d)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_basic_control_scroll_arrow_get(t);
	printf("setting direction to %d of %d\n", d, EON_DIRECTIONS);
	thiz->m = &thiz->ms[d];
}

static Eon_Theme_Control_Scroll_Arrow_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_control_scroll_arrow_renderer_get,
	/* .x_set 		= */ _basic_control_scroll_arrow_x_set,
	/* .y_set 		= */ _basic_control_scroll_arrow_y_set,
	/* .free 		= */ _basic_control_scroll_arrow_free,
	/* .size_get 		= */ _basic_control_scroll_arrow_size_get,
	/* .direction_set 	= */ _basic_control_scroll_arrow_direction_set,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

Eon_Theme_Widget * eon_basic_control_scroll_arrow_new(void)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	Enesim_Renderer *r;
	Enesim_Matrix ms[EON_DIRECTIONS];
	Enesim_Matrix tx1;
	Enesim_Matrix tx2;
	int i;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Scroll_Arrow));
	if (!thiz) return NULL;
	thiz->border_color = 0xff555555;
	thiz->fill_color = 0xffcccccc;
	thiz->border_weight = 2.0;

	r = enesim_renderer_path_new();
	enesim_renderer_shape_fill_color_set(r, thiz->fill_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(r, thiz->border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	/* create the arrow */
	/* we define it on a 4x4 box */
	enesim_renderer_path_move_to(r, 2, 2);
	enesim_renderer_path_line_to(r, 4, 4);
	enesim_renderer_path_line_to(r, 4, 4);
	enesim_renderer_path_close(r, EINA_TRUE);
	thiz->shape = r;
	/* create every transformation */
	enesim_matrix_identity(&ms[EON_DIRECTION_LEFT]);
	enesim_matrix_identity(&ms[EON_DIRECTION_TOP]);
	enesim_matrix_identity(&ms[EON_DIRECTION_RIGHT]);
	enesim_matrix_identity(&ms[EON_DIRECTION_BOTTOM]);

	enesim_matrix_translate(&tx1, 2, 2);
	enesim_matrix_translate(&tx1, -2, -2);
	for (i = 0; i < EON_DIRECTIONS; i++)
	{
		enesim_matrix_compose(&tx1, &ms[i], &thiz->ms[i]);
		enesim_matrix_compose(&thiz->ms[i], &tx2, &thiz->ms[i]);
	}
	thiz->m = &thiz->ms[0];

	return eon_theme_control_scroll_arrow_new(&_descriptor, thiz);
}
