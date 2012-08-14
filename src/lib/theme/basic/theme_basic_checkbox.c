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
#include "eon_theme_bin.h"
#include "eon_theme_checkbox.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Checkbox
{
	/* private */
	Enesim_Renderer *control;

	Enesim_Renderer *shape;
	Enesim_Renderer *compound;
	Enesim_Renderer *background;
	Enesim_Renderer *content;

	Enesim_Renderer *proxy;
	Enesim_Renderer *proxy_default;
} Eon_Basic_Checkbox;

const static int checkbox_to_content_padding = 10;

static inline Eon_Basic_Checkbox * _checkbox_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Checkbox *thiz;

	thiz = eon_theme_checkbox_data_get(t);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       The Checkbox theme interface                         *
 *----------------------------------------------------------------------------*/
#if 0
static void _checkbox_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(r);
	margin->left = thiz->size + checkbox_to_content_padding;
	margin->right = 0;
	margin->top = thiz->size / 2;
	margin->bottom = thiz->size / 2;
}

static Enesim_Renderer * _checkbox_renderer_get(Enesim_Renderer *r)
{
	Eon_Basic_Checkbox *thiz;
	thiz = _checkbox_get(r);

	return thiz->shape;
}

static Eina_Bool  _checkbox_setup(Enesim_Renderer *r,
		const Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		const Eon_Theme_Container_State *cstates[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Eon_Basic_Checkbox *thiz;
	Enesim_Renderer *content;
	Eina_Bool restack = EINA_FALSE;
	const Eon_Theme_Container_State *ccs = cstates[ENESIM_STATE_CURRENT];
	const Eon_Theme_Container_State *pcs = cstates[ENESIM_STATE_PAST];
	const Eon_Theme_Widget_State *cs = states[ENESIM_STATE_CURRENT];
	const Eon_Theme_Widget_State *ps = states[ENESIM_STATE_PAST];

	thiz = _checkbox_get(r);
	/* setup the layers now */
	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		printf("checkbox no content\n");
		return EINA_FALSE;
	}
	if (ccs->content != pcs->content)
	{
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
		enesim_renderer_compound_layer_add(thiz->compound, ccs->content);
	}
	if ((cs->x != ps->x) ||
			(cs->y != ps->y) ||
			(cs->width != ps->width) ||
			(cs->height != ps->height))
	{
		double x = cs->x;
		double y = cs->y;
		double width = cs->width;
		double height = cs->height;

		/* set the needed properties */
		enesim_renderer_rectangle_position_set(thiz->shape, x, y);
		enesim_renderer_rectangle_width_set(thiz->shape, width);
		enesim_renderer_rectangle_height_set(thiz->shape, height);
		enesim_renderer_origin_set(thiz->box, x, y + height/2 - thiz->size/2);
		enesim_renderer_origin_set(thiz->check, x, y + height/2 - thiz->size/2);
	}

	return EINA_TRUE;
}

static void _checkbox_free(Enesim_Renderer *r)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(r);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	free(thiz);
}
#endif

static Enesim_Renderer * _basic_checkbox_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	return thiz->shape;
}

static void _basic_checkbox_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_rectangle_x_set(thiz->shape, x);
}

static void _basic_checkbox_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_rectangle_y_set(thiz->shape, y);
}

static void _basic_checkbox_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_rectangle_width_set(thiz->shape, width);
}

static void _basic_checkbox_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_rectangle_height_set(thiz->shape, height);
}

static void _basic_checkbox_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	if (thiz->shape)
		enesim_renderer_unref(thiz->shape);
	
	free(thiz);
}
	
static void _basic_checkbox_child_set(Eon_Theme_Widget *t, Enesim_Renderer *child)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_proxy_proxied_set(thiz->proxy, child);
	if (!child)
		enesim_renderer_proxy_proxied_set(thiz->proxy, thiz->proxy_default);
}

static void _basic_checkbox_child_get(Eon_Theme_Widget *t, Enesim_Renderer **child)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_proxy_proxied_get(thiz->proxy, child);
	if (*child == thiz->proxy_default) *child = NULL;
}

static void _basic_checkbox_control_set(Eon_Theme_Widget *t, Enesim_Renderer *control)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	if (thiz->control)
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->control);
	enesim_renderer_compound_layer_add(thiz->compound, control);
	thiz->control = control;
}


static Eon_Theme_Checkbox_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_checkbox_renderer_get,
	/* .x_set 		= */ _basic_checkbox_x_set,
	/* .y_set 		= */ _basic_checkbox_y_set,
	/* .width_set 		= */ _basic_checkbox_width_set,
	/* .height_set 		= */ _basic_checkbox_height_set,
	/* .free 		= */ _basic_checkbox_free,
	/* .child_set 		= */ _basic_checkbox_child_set,
	/* .child_get 		= */ _basic_checkbox_child_get,
	/* .control_set 	= */ _basic_checkbox_control_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_basic_checkbox_new(void)
{
	Eon_Theme_Widget *t;
	Eon_Basic_Checkbox *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Checkbox));
	if (!thiz) return NULL;

	/* FIXME the proxy default is for now, until we fix enesim to make
	 * a renderer "active"
	 */
	r = enesim_renderer_background_new();
	if (!r) goto proxy_default_err;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_color_set(r, 0x00000000);
	thiz->proxy_default = r;

	r = enesim_renderer_proxy_new();
	if (!r) goto content_proxy_err;
	enesim_renderer_proxy_proxied_set(r, thiz->proxy_default);
	thiz->proxy = r;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_rop_set(r, ENESIM_FILL);
	enesim_renderer_background_color_set(r, 0xffd7d7d7);
	thiz->background = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->background);
	enesim_renderer_compound_layer_add(r, thiz->proxy);

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	thiz->shape = r;
	enesim_renderer_rectangle_corner_radius_set(r, 8);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_renderer_set(r, thiz->compound);
	enesim_renderer_shape_stroke_color_set(r, 0xffff0000);
	enesim_renderer_shape_stroke_weight_set(r, 1.0);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	t = eon_theme_checkbox_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;
base_err:
	enesim_renderer_unref(thiz->shape);
shape_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->background);
background_err:
	enesim_renderer_unref(thiz->proxy);
content_proxy_err:
	enesim_renderer_unref(thiz->proxy_default);
proxy_default_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_checkbox_background_color_set(Eon_Theme_Widget *t, Enesim_Color background_color)
{
	Eon_Basic_Checkbox *thiz;

	thiz = _checkbox_get(t);
	enesim_renderer_background_color_set(thiz->background, background_color);
}

