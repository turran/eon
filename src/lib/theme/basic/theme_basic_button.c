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
typedef struct _Basic_Button
{
	Eon_Basic_Control_Button *cb;
} Basic_Button;

static inline Basic_Button * _button_get(Enesim_Renderer *r)
{
	Basic_Button *thiz;

	thiz = eon_theme_button_data_get(r);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                         The Button theme interface                         *
 *----------------------------------------------------------------------------*/
static void _button_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Basic_Button *thiz;
	Enesim_Renderer *content;

	thiz = _button_get(r);
	eon_theme_container_content_get(r, &content);
	eon_basic_control_button_margin_get(thiz->cb, content, margin);
}

static void _button_position_get(Enesim_Renderer *r, Eon_Size *size,
		Eon_Position *position)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_position_get(thiz->cb, size, position);
}

static Enesim_Renderer * _button_renderer_get(Enesim_Renderer *r)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	return eon_basic_control_button_renderer_get(thiz->cb);
}

static Eina_Bool _button_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Basic_Button *thiz;
	Enesim_Renderer *content;
	Enesim_Renderer *real_r;
	double ox, oy;
	double width, height;

	thiz = _button_get(r);

	enesim_renderer_origin_get(r, &ox, &oy);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	eon_theme_container_content_get(r, &content);

	/* setup common properties */
	real_r = eon_basic_control_button_renderer_get(thiz->cb);
	enesim_renderer_origin_set(real_r, ox, oy);

	return eon_basic_control_button_setup(thiz->cb, content, width, height, error);
}

static void _button_free(Enesim_Renderer *r)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_free(thiz->cb);
	free(thiz);
}

static Eina_Bool _button_needs_setup(Enesim_Renderer *r)
{
	Basic_Button *thiz;
	Eina_Bool ret;

	thiz = _button_get(r);

	ret = eon_basic_control_button_has_changed(thiz->cb);
	return ret;
}

static void _button_cleanup(Enesim_Renderer *r)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_cleanup(thiz->cb);
}

static Eon_Theme_Button_Descriptor _descriptor = {
	/* .position_get 	= */ _button_position_get,
	/* .margin_get 		= */ _button_margin_get,
	/* .renderer_get 	= */ _button_renderer_get,
	/* .setup		= */ _button_setup,
	/* .cleanup 		= */ _button_cleanup,
	/* .needs_setup 	= */ _button_needs_setup,
	/* .free 		= */ _button_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_button_new(void)
{
	Enesim_Renderer *r;
	Eon_Basic_Control_Button *cb;
	Basic_Button *thiz;

	thiz = calloc(1, sizeof(Basic_Button));
	if (!thiz) return NULL;

	cb = eon_basic_control_button_new();
	thiz->cb = cb;

	r = eon_theme_button_new(&_descriptor, thiz);
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
EAPI void eon_basic_button_fill_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_fill_color_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_border_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_border_color_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_horizontal_padding_set(Enesim_Renderer *r, double padding)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_horizontal_padding_set(thiz->cb, padding);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_vertical_padding_set(Enesim_Renderer *r, double padding)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_vertical_padding_set(thiz->cb, padding);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_radius_set(Enesim_Renderer *r, double radius)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_radius_set(thiz->cb, radius);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_start_shadow_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_start_shadow_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_end_shadow_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_end_shadow_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_start_bevel_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_start_bevel_set(thiz->cb, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_end_bevel_set(Enesim_Renderer *r, Enesim_Color color)
{
	Basic_Button *thiz;

	thiz = _button_get(r);
	eon_basic_control_button_end_bevel_set(thiz->cb, color);
}
