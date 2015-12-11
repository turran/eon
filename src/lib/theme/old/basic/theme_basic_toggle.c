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
#include "Eon_Theme.h"
#include "Eon_Basic.h"
#include "eon_theme_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Basic_Toggle
{
	Eon_Basic_Control_Button *cb;
} Basic_Toggle;

static inline Basic_Toggle * _toggle_get(Enesim_Renderer *r)
{
	Basic_Toggle *thiz;

	thiz = eon_theme_toggle_data_get(r);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                         The Button theme interface                         *
 *----------------------------------------------------------------------------*/
static void _toggle_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Basic_Toggle *thiz;
	Enesim_Renderer *content;

	thiz = _toggle_get(r);
	eon_theme_container_content_get(r, &content);
	eon_theme_basic_control_button_margin_get(thiz->cb, content, margin);
}

static void _toggle_position_get(Enesim_Renderer *r, Eon_Size *size,
		Eon_Position *position)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_position_get(thiz->cb, size, position);
}

static Enesim_Renderer * _toggle_renderer_get(Enesim_Renderer *r)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	return eon_theme_basic_control_button_renderer_get(thiz->cb);
}

static Eina_Bool _toggle_setup(Enesim_Renderer *r, Enesim_Log **error)
{
	Basic_Toggle *thiz;
	Enesim_Renderer *content;
	Enesim_Renderer *real_r;
	double x, y;
	double width, height;

	thiz = _toggle_get(r);

	eon_theme_widget_x_get(r, &x);
	eon_theme_widget_y_get(r, &y);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	eon_theme_container_content_get(r, &content);

	return EINA_TRUE;
	//return eon_theme_basic_control_button_setup(thiz->cb, content, x, y, width, height, error);
}

static void _toggle_free(Enesim_Renderer *r)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_free(thiz->cb);
	free(thiz);
}

static Eon_Theme_Toggle_Descriptor _descriptor = {
	/* .position_get 	= */ _toggle_position_get,
	/* .margin_get 		= */ _toggle_margin_get,
	/* .renderer_get 	= */ _toggle_renderer_get,
	/* .setup		= */ _toggle_setup,
	/* .free 		= */ _toggle_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_basic_toggle_new(void)
{
	Enesim_Renderer *r;
	Eon_Basic_Control_Button *cb;
	Basic_Toggle *thiz;

	thiz = calloc(1, sizeof(Basic_Toggle));
	if (!thiz) return NULL;

	cb = eon_theme_basic_control_button_new();
	thiz->cb = cb;

	r = eon_theme_toggle_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_fill_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_fill_color_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_border_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_border_color_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_horizontal_padding_set(Enesim_Renderer *r, double padding)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_horizontal_padding_set(thiz->cb, padding);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_vertical_padding_set(Enesim_Renderer *r, double padding)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_vertical_padding_set(thiz->cb, padding);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_radius_set(Enesim_Renderer *r, double radius)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_radius_set(thiz->cb, radius);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_start_shadow_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_start_shadow_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_end_shadow_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_end_shadow_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_start_bevel_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_start_bevel_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_basic_toggle_end_bevel_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Toggle *thiz;

	thiz = _toggle_get(r);
	eon_theme_basic_control_button_end_bevel_set(thiz->cb, color);
}
