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
#include "eon_theme_button.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Button
{
	Eon_Basic_Control_Button *cb;
} Eon_Basic_Button;

static inline Eon_Basic_Button * _button_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Button *thiz;

	thiz = eon_theme_button_data_get(t);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Button theme interface                         *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _basic_button_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	return eon_theme_basic_control_button_renderer_get(thiz->cb);
}

static void _basic_button_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_x_set(thiz->cb, x);
}

static void _basic_button_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_y_set(thiz->cb, y);
}

static void _basic_button_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_width_set(thiz->cb, width);
}

static void _basic_button_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_height_set(thiz->cb, height);
}

static void _basic_button_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_free(thiz->cb);
	free(thiz);
}
	
static void _basic_button_child_set(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_renderer_content_set(thiz->cb, r);
}

static void _basic_button_child_get(Eon_Theme_Widget *t, Enesim_Renderer **r)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_renderer_content_get(thiz->cb, r);
}

static Eon_Theme_Bin_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_button_renderer_get,
	/* .x_set 		= */ _basic_button_x_set,
	/* .y_set 		= */ _basic_button_y_set,
	/* .width_set 		= */ _basic_button_width_set,
	/* .height_set 		= */ _basic_button_height_set,
	/* .free 		= */ _basic_button_free,
	/* .child_set 		= */ _basic_button_child_set,
	/* .child_get 		= */ _basic_button_child_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_theme_basic_button_new(void)
{
	Eon_Theme_Widget *t;
	Eon_Basic_Control_Button *cb;
	Eon_Basic_Button *thiz;

	thiz = calloc(1, sizeof(Eon_Basic_Button));
	if (!thiz) return NULL;

	cb = eon_theme_basic_control_button_new();
	thiz->cb = cb;

	t = eon_theme_button_new(&_descriptor, thiz);
	if (!t) goto base_err;
	return t;

base_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_fill_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_fill_color_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_border_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_border_color_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_horizontal_padding_set(Eon_Theme_Widget *t, double padding)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_horizontal_padding_set(thiz->cb, padding);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_vertical_padding_set(Eon_Theme_Widget *t, double padding)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_vertical_padding_set(thiz->cb, padding);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_radius_set(Eon_Theme_Widget *t, double radius)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_radius_set(thiz->cb, radius);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_start_shadow_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_start_shadow_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_end_shadow_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_end_shadow_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_start_bevel_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_start_bevel_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_button_end_bevel_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Button *thiz;

	thiz = _button_get(t);
	eon_theme_basic_control_button_end_bevel_set(thiz->cb, color);
}
