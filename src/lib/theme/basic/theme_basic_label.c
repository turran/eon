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
#include "Eon_Basic.h"
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Label
{
	Enesim_Renderer *text;
	Enesim_Renderer_Sw_Fill fill;
} Label;

static inline Label * _label_get(Enesim_Renderer *r)
{
	Label *thiz;

	thiz = eon_theme_label_data_get(r);
	return thiz;
}

static void _label_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Label *thiz;

	thiz = _label_get(r);
	thiz->fill(thiz->text, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme label interface                       *
 *----------------------------------------------------------------------------*/
static void _label_font_set(Enesim_Renderer *r, const char *name)
{
	Label *thiz;

	thiz = _label_get(r);
	etex_base_font_name_set(thiz->text, name);
}

static const char * _label_font_get(Enesim_Renderer *r)
{
	Label *thiz;
	const char *name;

	thiz = _label_get(r);
	etex_base_font_get(thiz->text, &name);
	return name;
}

static void _label_size_set(Enesim_Renderer *r, int size)
{
	Label *thiz;

	thiz = _label_get(r);
	etex_base_size_set(thiz->text, size);
}

static int _label_size_get(Enesim_Renderer *r)
{
	Label *thiz;
	unsigned int size;

	thiz = _label_get(r);
	etex_base_size_get(thiz->text, &size);
	return size;
}

static void _label_text_set(Enesim_Renderer *r, const char *str)
{
	Label *thiz;

	thiz = _label_get(r);
	etex_span_text_set(thiz->text, str);
}

static const char * _label_text_get(Enesim_Renderer *r)
{
	Label *thiz;
	const char *str;

	thiz = _label_get(r);
	etex_span_text_get(thiz->text, &str);
	return str;
}

static unsigned int _label_height_get(Enesim_Renderer *r)
{
	Label *thiz;
	Enesim_Rectangle boundings;

	thiz = _label_get(r);
	enesim_renderer_boundings(thiz->text, &boundings);
	return boundings.h;
}

static unsigned int _label_width_get(Enesim_Renderer *r)
{
	Label *thiz;
	Enesim_Rectangle boundings;

	thiz = _label_get(r);
	enesim_renderer_boundings(thiz->text, &boundings);
	return boundings.w;
}

static Eina_Bool _label_setup(Enesim_Renderer *r, Enesim_Surface *s,
		Enesim_Renderer_Sw_Fill *fill, Enesim_Error **error)
{
	Label *thiz;
	Enesim_Color color;
	double ox, oy;

	thiz = _label_get(r);
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->text, ox, oy);
	enesim_renderer_color_get(r, &color);
	enesim_renderer_color_set(thiz->text, color);
	if (!enesim_renderer_sw_setup(thiz->text, s, error))
	{
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->text);
	if (!thiz->fill)
	{
		return EINA_FALSE;
	}

	*fill = _label_draw;

	return EINA_TRUE;
}

static void _label_cleanup(Enesim_Renderer *r)
{
	Label *thiz;

	thiz = _label_get(r);
	enesim_renderer_sw_cleanup(thiz->text);
}

static void _label_free(Enesim_Renderer *r)
{
	Label *thiz;

	thiz = _label_get(r);
	if (thiz->text)
		enesim_renderer_unref(thiz->text);
	free(thiz);
}

static Eon_Theme_Label_Descriptor _descriptor = {
	.text_set = _label_text_set,
	.text_get = _label_text_get,
	.size_set = _label_size_set,
	.size_get = _label_size_get,
	.font_set = _label_font_set,
	.font_get = _label_font_get,
	.height_get = _label_height_get,
	.width_get = _label_width_get,
	.sw_setup = _label_setup,
	.sw_cleanup = _label_cleanup,
	.free = _label_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_label_new(void)
{
	Enesim_Renderer *r;
	Label *thiz;

	thiz = calloc(1, sizeof(Label));
	if (!thiz) return NULL;

	r = etex_span_new();
	if (!r) goto etex_err;
	thiz->text = r;

	r = eon_theme_label_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_unref(thiz->text);
etex_err:
	free(thiz);
	return NULL;
}
