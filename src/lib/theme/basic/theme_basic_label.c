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
#include "Etex.h"

#include "eon_theme_widget.h"
#include "eon_theme_label.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Label
{
	Enesim_Renderer *text;
} Eon_Basic_Label;

static inline Eon_Basic_Label * _label_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Label *thiz;

	thiz = eon_theme_label_data_get(t);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme label interface                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _basic_label_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	return thiz->text;
}

static void _basic_label_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	enesim_renderer_x_origin_set(thiz->text, x);
}

static void _basic_label_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	enesim_renderer_y_origin_set(thiz->text, y);
}

static void _basic_label_text_get(Eon_Theme_Widget *t, const char **str)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	etex_span_text_get(thiz->text, str);
}

static void _basic_label_text_set(Eon_Theme_Widget *t, const char *str)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	etex_span_text_set(thiz->text, str);
}

static void _basic_label_font_get(Eon_Theme_Widget *t, const char **str)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	etex_base_font_name_get(thiz->text, str);
}

static void _basic_label_font_set(Eon_Theme_Widget *t, const char *str)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	etex_base_font_name_set(thiz->text, str);
}

static void _basic_label_font_size_get(Eon_Theme_Widget *t, int *size)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	etex_base_size_get(thiz->text, size);
}

static void _basic_label_font_size_set(Eon_Theme_Widget *t, int size)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	etex_base_size_set(thiz->text, size);
}

static void _basic_label_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Label *thiz;

	thiz = _label_get(t);
	enesim_renderer_unref(thiz->text);
	free(thiz);
}

static Eon_Theme_Label_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_label_renderer_get,
	/* .x_set 		= */ _basic_label_x_set,
	/* .y_set 		= */ _basic_label_y_set,
	/* .width_set 		= */ NULL,
	/* .height_set		= */ NULL,
	/* .free		= */ _basic_label_free,
	/* .text_set 		= */ _basic_label_text_set,
	/* .text_get		= */ _basic_label_text_get,
	/* .font_size_get 	= */ _basic_label_font_size_get,
	/* .font_size_set	= */ _basic_label_font_size_set,
	/* .font_get 		= */ _basic_label_font_get,
	/* .font_set 		= */ _basic_label_font_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_basic_label_new(void)
{
	Eon_Theme_Widget *t;
	Eon_Basic_Label *thiz;

	thiz = calloc(1, sizeof(Eon_Basic_Label));
	if (!thiz) return NULL;
	thiz->text = etex_span_new();

	t = eon_theme_label_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	free(thiz);
	return NULL;
}
