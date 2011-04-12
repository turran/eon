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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Button
{
	Enesim_Renderer *rectangle;
	Enesim_Renderer *compound;
	Enesim_Renderer *text;
	Enesim_Renderer_Sw_Fill fill;
} Button;

static inline Button * _button_get(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = eon_theme_widget_data_get(r);
	return thiz;
}

static void _button_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}

static void _button_update_rectangle(Button *thiz)
{
	Eina_Rectangle boundings;

	/* add 5px of padding to the text */
	/* set the size of the rectangle based on the size of the string */
	enesim_renderer_boundings(thiz->text, &boundings);
	enesim_renderer_rectangle_width_set(thiz->rectangle, boundings.w + 6);
	enesim_renderer_rectangle_height_set(thiz->rectangle, boundings.h + 6);
	enesim_renderer_origin_set(thiz->text, 3, 3);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme widget interface                      *
 *----------------------------------------------------------------------------*/
static Eon_Theme_Widget_Descriptor _twdescriptor;
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _button_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Button *thiz;
	double ox, oy;

	thiz = _button_get(r);
	_button_update_rectangle(thiz);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("not available to setup yet\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _button_draw;

	return EINA_TRUE;
}

static void _button_cleanup(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = _button_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _button_free(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = _button_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static void _button_boundings(Enesim_Renderer *r, Eina_Rectangle *boundings)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_boundings(thiz->compound, boundings);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _button_setup,
	.sw_cleanup = _button_cleanup,
	.boundings = _button_boundings,
	.free = _button_free,
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
	Button *thiz;

	thiz = calloc(1, sizeof(Button));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = etex_span_new();
	if (!r) goto etex_err;
	thiz->text = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto rectangle_err;
	thiz->rectangle = r;

	r = eon_theme_button_new(&_twdescriptor, &_descriptor, thiz);
	if (!r) goto renderer_err;

	/* setup the initial state */
	enesim_renderer_compound_layer_add(thiz->compound, thiz->rectangle);
	enesim_renderer_shape_outline_weight_set(thiz->rectangle, 1);
	enesim_renderer_shape_draw_mode_set(thiz->rectangle, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	enesim_renderer_compound_layer_add(thiz->compound, thiz->text);
	enesim_renderer_rop_set(thiz->text, ENESIM_BLEND);

	return r;

renderer_err:
	enesim_renderer_delete(thiz->rectangle);
rectangle_err:
	enesim_renderer_delete(thiz->text);
etex_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_foreground_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_shape_fill_color_set(thiz->rectangle, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_border_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_shape_outline_color_set(thiz->rectangle, color);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_label_set(Enesim_Renderer *r, char *str)
{
	Button *thiz;

	thiz = _button_get(r);
	etex_span_text_set(thiz->text, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_font_set(Enesim_Renderer *r, const char *file)
{
	Button *thiz;

	thiz = _button_get(r);
	etex_span_font_set(thiz->text, file);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_font_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_color_set(thiz->text, color);
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_basic_button_font_size_set(Enesim_Renderer *r, int size)
{
	Button *thiz;

	thiz = _button_get(r);
	etex_span_size_set(thiz->text, size);
}
