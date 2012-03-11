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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Basic_Control_Arrow
{
	Enesim_Renderer *shape;
	Enesim_Renderer *arrow;
	Enesim_Color border_color;
	Enesim_Color fill_color;
	double border_weight;
};

void eon_basic_control_arrow_setup(Eon_Basic_Control_Arrow *thiz,
		Eon_Position *position, Eon_Size *size, Eon_Basic_Control_Arrow_Direction direction)
{
	Enesim_Renderer *r;

	r = thiz->shape;
	enesim_renderer_rectangle_size_set(r, size->width , size->height);
	switch (direction)
	{
		case EON_BASIC_CONTROL_ARROW_DIRECTION_LEFT:
		enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_FALSE, EINA_TRUE, EINA_FALSE);
		break;

		case EON_BASIC_CONTROL_ARROW_DIRECTION_RIGHT:
		enesim_renderer_rectangle_corners_set(r, EINA_FALSE, EINA_TRUE, EINA_FALSE, EINA_TRUE);
		break;

		case EON_BASIC_CONTROL_ARROW_DIRECTION_TOP:
		enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_FALSE, EINA_FALSE);
		break;

		case EON_BASIC_CONTROL_ARROW_DIRECTION_BOTTOM:
		enesim_renderer_rectangle_corners_set(r, EINA_FALSE, EINA_FALSE, EINA_TRUE, EINA_TRUE);
		break;

		default:
		break;
	}
	enesim_renderer_origin_set(r, position->x, position->y);
	enesim_renderer_rectangle_corner_radius_set(r, size->width / 2.0);
}

Enesim_Renderer * eon_basic_control_arrow_renderer_get(Eon_Basic_Control_Arrow *thiz)
{
	return thiz->shape;
}

Eon_Basic_Control_Arrow * eon_basic_control_arrow_new(void)
{
	Eon_Basic_Control_Arrow *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Control_Arrow));
	if (!thiz) return NULL;
	thiz->border_color = 0xff555555;
	thiz->fill_color = 0xffcccccc;
	thiz->border_weight = 2.0;

	r = enesim_renderer_rectangle_new();
	enesim_renderer_shape_fill_color_set(r, thiz->fill_color);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(r, thiz->border_weight);
	enesim_renderer_shape_stroke_color_set(r, thiz->border_color);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	thiz->shape = r;

	return thiz;
}
