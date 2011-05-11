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
	/* private */
	Enesim_Renderer *content;
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
 *                      The Eon's theme widget interface                      *
 *----------------------------------------------------------------------------*/
static double _checkbox_min_width_get(Enesim_Renderer *r)
{
	Checkbox *thiz;
	Enesim_Renderer *content;
	double min_width;

	thiz = _checkbox_get(r);
	min_width = thiz->size;
	eon_theme_container_content_get(r, &content);
	if (!content)
		goto end;

	/* the padding */
	min_width += checkbox_to_content_padding;
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

static double _checkbox_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _checkbox_min_height_get(Enesim_Renderer *r)
{
	Checkbox *thiz;
	Enesim_Renderer *content;
	double min_height;

	thiz = _checkbox_get(r);
	min_height = thiz->size;
	eon_theme_container_content_get(r, &content);
	if (!content)
		goto end;
	if (eon_is_element(content))
	{
		double cmin_height;

		eon_element_min_height_get(content, &cmin_height);
		min_height += cmin_height;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(content, &boundings);
		min_height += boundings.h;
	}
end:
	return min_height;
}

static double _checkbox_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eon_Theme_Widget_Descriptor _twdescriptor = {
	.max_width_get = _checkbox_max_width_get,
	.max_height_get = _checkbox_max_height_get,
	.min_width_get = _checkbox_min_width_get,
	.min_height_get = _checkbox_min_height_get,
};
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _checkbox_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Checkbox *thiz;
	Enesim_Renderer *content;
	double ox, oy;

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
		enesim_renderer_compound_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->box);
		if (thiz->selected)
		{
			enesim_renderer_compound_layer_add(thiz->compound, thiz->check);
		}
		enesim_renderer_compound_layer_add(thiz->compound, content);
		/* FIXME at the cleanup we should restore this */
		enesim_renderer_rop_set(content, ENESIM_BLEND);
		thiz->content = content;
	}
	enesim_renderer_origin_set(thiz->content, thiz->size +
			checkbox_to_content_padding, 0);

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

static void _checkbox_boundings(Enesim_Renderer *r, Enesim_Rectangle *boundings)
{
	Checkbox *thiz;

	thiz = _checkbox_get(r);
	enesim_renderer_boundings(thiz->compound, boundings);
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
	enesim_renderer_shape_outline_weight_set(r, 2);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_outline_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_color_set(r, 0xffffffff);

	r = eon_theme_checkbox_new(&_twdescriptor, &_descriptor, thiz);
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
