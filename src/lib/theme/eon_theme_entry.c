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
#include "eon_private.h"
/*
 * Instead of using the boundings box of the text renderer use
 * add maxdescent/maxascent properties to etex_base, this way we
 * can calculate this without having a string set
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Entry
{
	/* properties */
	Eon_Horizontal_Alignment alignment;
	/* interface */
	Eon_Theme_Entry_Renderer_Get renderer_get;
	Eon_Theme_Entry_Margin_Get margin_get;
	Eon_Theme_Entry_Setup setup;
	Eon_Theme_Entry_Cleanup cleanup;
	Enesim_Renderer_Delete free;
	/* private */
	Enesim_Renderer *text;
	void *data;
} Eon_Theme_Entry;

static inline Eon_Theme_Entry * _eon_theme_entry_get(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = eon_theme_widget_data_get(r);

	return thiz;
}

static void _eon_theme_entry_free(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	enesim_renderer_unref(thiz->text);
	if (thiz->free) thiz->free(r);
	free(thiz);
}

static Enesim_Renderer * _eon_theme_entry_renderer_get(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	if (thiz->renderer_get)
		return thiz->renderer_get(r, thiz->text);
	return thiz->text;
}

static Enesim_Renderer * _eon_theme_entry_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Eon_Theme_Entry *thiz;
	Eina_Bool ret = EINA_TRUE;

	thiz = _eon_theme_entry_get(r);
	if (thiz->setup)
		ret = thiz->setup(r, thiz->text, thiz->alignment, error);
	return ret;
}

static const char * _eon_theme_entry_name(Enesim_Renderer *r)
{
	return "theme_entry";
}

static void _eon_theme_entry_cleanup(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	if (thiz->cleanup)
		thiz->cleanup(r);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_entry_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	if (!thiz) return;

	if (thiz->margin_get)
	{
		Eon_Margin margin;

		thiz->margin_get(r, &margin);
		*width = margin.left + margin.right;
	}
	else
	{
		*width = 1;
	}
}

void eon_theme_entry_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Entry *thiz;
	int max, min, h;

	thiz = _eon_theme_entry_get(r);
	if (!thiz) return;

	etex_base_max_ascent_get(thiz->text, &max);
	etex_base_max_descent_get(thiz->text, &min);
	h = max + min;

	if (thiz->margin_get)
	{
		Eon_Margin margin;

		thiz->margin_get(r, &margin);
		*height = h + margin.top + margin.bottom;
	}
	else
	{
		*height = h;
	}
}

void eon_theme_entry_preferred_width_get(Enesim_Renderer *r, double *width)
{
	/* if the max_length is set, use that as the number of chars we want
	 * to use
	 */
}

Etex_Buffer * eon_theme_entry_buffer_get(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;
	Etex_Buffer *b;

	thiz = _eon_theme_entry_get(r);
	if (!thiz) return NULL;

	etex_span_buffer_get(thiz->text, &b);
	return b;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_entry_new(Eon_Theme_Entry_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Entry *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Entry));
	if (!thiz) return NULL;

	r = etex_span_new();
	if (!r) goto text_err;
	thiz->text = r;

	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->margin_get = descriptor->margin_get;
	thiz->setup = descriptor->setup;
	thiz->cleanup = descriptor->cleanup;
	thiz->renderer_get = descriptor->renderer_get;

	pdescriptor.renderer_get = _eon_theme_entry_renderer_get;
	pdescriptor.setup = _eon_theme_entry_setup;
	pdescriptor.cleanup = _eon_theme_entry_cleanup;
	pdescriptor.name = _eon_theme_entry_name;
	pdescriptor.free = _eon_theme_entry_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->text);
text_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_entry(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_entry_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_entry_font_get(Enesim_Renderer *r, const char **str)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	etex_base_font_name_get(thiz->text, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_entry_font_set(Enesim_Renderer *r, const char *str)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	etex_base_font_name_set(thiz->text, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_entry_size_get(Enesim_Renderer *r, int *size)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	etex_base_size_get(thiz->text, size);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_entry_size_set(Enesim_Renderer *r, int size)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	etex_base_size_set(thiz->text, size);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_entry_alignment_set(Enesim_Renderer *r, Eon_Horizontal_Alignment alignment)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	thiz->alignment = alignment;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_entry_alignment_get(Enesim_Renderer *r, Eon_Horizontal_Alignment *alignment)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	*alignment = thiz->alignment;
}
