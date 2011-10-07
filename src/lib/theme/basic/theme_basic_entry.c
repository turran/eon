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
typedef struct _Basic_Entry
{
	/* properties */
	/* private */
	Enesim_Renderer *shape;
	Enesim_Renderer *shape_fill;
	Enesim_Renderer_Sw_Fill fill;
} Basic_Entry;

static inline Basic_Entry * _entry_get(Enesim_Renderer *r)
{
	Basic_Entry *thiz;

	thiz = eon_theme_entry_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                          The Entry theme interface                         *
 *----------------------------------------------------------------------------*/
static void _entry_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	margin->left = margin->right = margin->top = margin->bottom = 2;
}

static Enesim_Renderer * _entry_setup(Enesim_Renderer *r, Enesim_Renderer *text,
		Enesim_Error **error)
{
	Basic_Entry *thiz;
	const char *text_str;
	double ox, oy;
	double width, height;

	thiz = _entry_get(r);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->shape, ox, oy);

	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	printf("width = %g height = %g\n", width, height);
	enesim_renderer_rectangle_size_set(thiz->shape, width, height);
	etex_span_text_get(text, &text_str);
	if (text_str)
	{
		printf("text\n");
		enesim_renderer_shape_fill_renderer_set(thiz->shape, text);
	}
	else
	{
		printf("no text\n");
		enesim_renderer_shape_fill_renderer_set(thiz->shape, NULL);
	}

	/* setup the layers */

	return thiz->shape;
}

static void _entry_free(Enesim_Renderer *r)
{
	Basic_Entry *thiz;

	thiz = _entry_get(r);
	free(thiz);
}

static Eon_Theme_Entry_Descriptor _descriptor = {
	.setup = _entry_setup,
	.free = _entry_free,
	.margin_get = _entry_margin_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_entry_new(void)
{
	Enesim_Renderer *r;
	Basic_Entry *thiz;

	thiz = calloc(1, sizeof(Basic_Entry));
	if (!thiz) return NULL;

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_err;
	thiz->shape = r;

	r = enesim_renderer_compound_new();
	if (!r) goto shape_fill_err;
	thiz->shape_fill = r;

	r = eon_theme_entry_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	enesim_renderer_unref(thiz->shape_fill);
shape_fill_err:
	enesim_renderer_unref(thiz->shape);
shape_err:
	free(thiz);
	return NULL;
}
