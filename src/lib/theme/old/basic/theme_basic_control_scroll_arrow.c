/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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

#include <math.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Control_Scroll_Arrow
{
	/* private */
	Eon_Direction direction;

	Enesim_Renderer *shape;
	Enesim_Renderer *arrow;

	Enesim_Matrix *m;

	double x;
	double y;
} Eon_Basic_Control_Scroll_Arrow;

static Enesim_Matrix _ms[EON_DIRECTIONS];

static void _eon_theme_basic_control_scroll_arrow_init(void)
{
	Enesim_Matrix m1, m2;

	/* set the initial transform to the top corner of the triangle */
	/* top */
	enesim_matrix_identity(&_ms[EON_DIRECTION_TOP]);
	/* left */
	enesim_matrix_translate(&m2, -6, 0);
	enesim_matrix_rotate(&m1, -M_PI_2);
	enesim_matrix_compose(&m1, &m2, &_ms[EON_DIRECTION_LEFT]);
	/* bottom */
	enesim_matrix_translate(&m1, 6, 4);
	enesim_matrix_rotate(&m2, M_PI);
	enesim_matrix_compose(&m1, &m2, &_ms[EON_DIRECTION_BOTTOM]);
	/* right */
	enesim_matrix_translate(&m1, 4, 0);
	enesim_matrix_rotate(&m2, M_PI_2);
	enesim_matrix_compose(&m1, &m2, &_ms[EON_DIRECTION_RIGHT]);
}

static inline Eon_Basic_Control_Scroll_Arrow * _eon_theme_basic_control_scroll_arrow_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	thiz = eon_theme_control_scroll_arrow_data_get(t);
	return thiz;
}

static Enesim_Renderer * _basic_control_scroll_arrow_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	return thiz->shape;
}

static void _basic_control_scroll_arrow_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	Enesim_Matrix m;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	thiz->x = x;
	enesim_matrix_translate(&m, thiz->x, thiz->y);
	enesim_matrix_compose(&m, thiz->m, &m);
	enesim_renderer_geometry_transformation_set(thiz->shape, &m);
}

static void _basic_control_scroll_arrow_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	Enesim_Matrix m;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	thiz->y = y;
	enesim_matrix_translate(&m, thiz->x, thiz->y);
	enesim_matrix_compose(&m, thiz->m, &m);
	enesim_renderer_geometry_transformation_set(thiz->shape, &m);
}

static void _basic_control_scroll_arrow_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	free(thiz);
}

static void _basic_control_scroll_arrow_size_get(Eon_Theme_Widget *t, Eon_Size *size)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	if (thiz->direction == EON_DIRECTION_TOP || thiz->direction == EON_DIRECTION_BOTTOM)
	{
		size->width = 8;
		size->height = 6;
	}
	else
	{
		size->width = 6;
		size->height = 8;
	}
}

static void _basic_control_scroll_arrow_direction_set(Eon_Theme_Widget *t, Eon_Direction d)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	thiz->direction = d;
	thiz->m = &_ms[d];
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

Eon_Theme_Widget * eon_theme_basic_control_scroll_arrow_new(void)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;
	Enesim_Color fill_color = 0xff000000;
	Enesim_Renderer *r;
	static int init = 0;

	if (!init)
	{
		_eon_theme_basic_control_scroll_arrow_init();
		init++;
	}

	thiz = calloc(1, sizeof(Eon_Basic_Control_Scroll_Arrow));
	if (!thiz) return NULL;
	thiz->direction = EON_DIRECTION_TOP;

	r = enesim_renderer_path_new();
	enesim_renderer_shape_fill_color_set(r, fill_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	/* create the arrow */
	/* we define it on a 4x4 box */
	enesim_renderer_path_move_to(r, 4, 0);
	enesim_renderer_path_line_to(r, 6, 4);
	enesim_renderer_path_line_to(r, 0, 4);
	enesim_renderer_path_close(r, EINA_TRUE);
	thiz->shape = r;
	thiz->m = &_ms[thiz->direction];

	return eon_theme_control_scroll_arrow_new(&_descriptor, thiz);
}

EAPI void eon_theme_basic_control_scroll_arrow_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Control_Scroll_Arrow *thiz;

	thiz = _eon_theme_basic_control_scroll_arrow_get(t);
	enesim_renderer_shape_fill_color_set(thiz->shape, color);
}
