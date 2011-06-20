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
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Label
{
	/* the label interface */
	Eon_Theme_Label_Font_Set font_set;
	Eon_Theme_Label_Font_Get font_get;
	Eon_Theme_Label_Size_Set size_set;
	Eon_Theme_Label_Size_Get size_get;
	Eon_Theme_Label_Text_Set text_set;
	Eon_Theme_Label_Text_Get text_get;
	Eon_Theme_Label_Ellipsize_Set ellipsize_set;
	Eon_Theme_Label_Ellipsize_Get ellipsize_get;
	/* data needed for our own callbacks */
	Enesim_Renderer *text;
	Eina_Bool ellipsized;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Label;

static inline Eon_Theme_Label * _eon_theme_label_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = eon_theme_widget_data_get(r);
	return thiz;
}

static void _eon_theme_label_free(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme label interface                       *
 *----------------------------------------------------------------------------*/
static double _eon_theme_label_min_width_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;
	Eina_Bool ellipsized;

	thiz = _eon_theme_label_get(r);
	eon_theme_label_ellipsize_get(r, &ellipsized);
	if (ellipsized)
	{
		Enesim_Rectangle boundings;
		const char *font;
		const char *text;
		char *ellipsized_text;
		int size;
		double min_width;

		if (!thiz->text)
			thiz->text = etex_span_new();
		eon_theme_label_font_get(r, &font);
		eon_theme_label_size_get(r, &size);
		eon_theme_label_text_get(r, &text);
		etex_span_font_set(thiz->text, font);
		etex_span_size_set(thiz->text, size);
		etex_span_text_set(thiz->text, text);
		enesim_renderer_boundings(thiz->text, &boundings);
		min_width = boundings.w;

		/* set first letter and ellipsis "E..." as the minimum height
		 * if it is smaller than the text itself, use that
		 */
		ellipsized_text = malloc(sizeof(char) * 5);
		ellipsized_text[0] = text[0];
		ellipsized_text[1] = ellipsized_text[2] = ellipsized_text[3] = '.';
		ellipsized_text[4] = '\0';
		etex_span_text_set(thiz->text, ellipsized_text);
		enesim_renderer_boundings(thiz->text, &boundings);
		free(ellipsized_text);

		return min_width < boundings.w ? min_width : boundings.w;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(r, &boundings);
		return boundings.w;
	}
}

static double _eon_theme_label_max_width_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;
	Enesim_Rectangle boundings;

	thiz = _eon_theme_label_get(r);
	enesim_renderer_boundings(r, &boundings);
	return boundings.w;
}

static double _eon_theme_label_min_height_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	return thiz->size_get(r);
}

static double _eon_theme_label_max_height_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;
	Enesim_Rectangle boundings;

	thiz = _eon_theme_label_get(r);
	enesim_renderer_boundings(r, &boundings);
	return boundings.h;
}
/* FIXME do we actually need this? */
static void _eon_theme_label_ellipsize_set(Enesim_Renderer *r,
		Eina_Bool ellipsize)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	thiz->ellipsized = ellipsize;
}

static Eina_Bool _eon_theme_label_ellipsize_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	return thiz->ellipsized;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_label_new(Eon_Theme_Label_Descriptor *descriptor, void *data)
{
	Eon_Theme_Label *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	/* first check the label descriptors */
	if (!descriptor->text_set)
	{
		return NULL;
	}
	if (!descriptor->text_get)
	{
		return NULL;
	}
	if (!descriptor->font_set)
	{
		return NULL;
	}
	if (!descriptor->font_get)
	{
		return NULL;
	}
	if (!descriptor->size_set)
	{
		return NULL;
	}
	if (!descriptor->size_get)
	{
		return NULL;
	}

	thiz = calloc(1, sizeof(Eon_Theme_Label));
	thiz->data = data;
	thiz->free = descriptor->free;
	/* the label interface */
	thiz->text_get = descriptor->text_get;
	thiz->text_set = descriptor->text_set;
	thiz->font_get = descriptor->font_get;
	thiz->font_set = descriptor->font_set;
	thiz->size_get = descriptor->size_get;
	thiz->size_set = descriptor->size_set;
	/* set default values */
	if (!descriptor->ellipsize_set || !descriptor->ellipsize_get)
	{
		thiz->ellipsize_set = _eon_theme_label_ellipsize_set;
		thiz->ellipsize_get = _eon_theme_label_ellipsize_get;
	}
	else
	{
		thiz->ellipsize_set = descriptor->ellipsize_set;
		thiz->ellipsize_get = descriptor->ellipsize_get;
	}

	/* the widget interface */
	if (!descriptor->min_width_get)
		pdescriptor.min_width_get = _eon_theme_label_min_width_get;
	else
		pdescriptor.min_width_get = descriptor->min_width_get;

	if (!descriptor->max_width_get)
		pdescriptor.max_width_get = _eon_theme_label_max_width_get;
	else
		pdescriptor.max_width_get = descriptor->max_width_get;

	if (!descriptor->min_height_get)
		pdescriptor.min_height_get = _eon_theme_label_min_height_get;
	else
		pdescriptor.min_height_get = descriptor->min_height_get;

	if (!descriptor->max_height_get)
		pdescriptor.max_height_get = _eon_theme_label_max_height_get;
	else
		pdescriptor.max_height_get = descriptor->max_height_get;

	/* now the needed enesim functions */
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_label_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
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
EAPI Eina_Bool eon_is_theme_label(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_label_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_get(Enesim_Renderer *r, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	*str = thiz->font_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_set(Enesim_Renderer *r, const char *str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	thiz->font_set(r, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_size_get(Enesim_Renderer *r, int *size)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	*size = thiz->size_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_size_set(Enesim_Renderer *r, int size)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	thiz->size_set(r, size);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_ellipsized_text_get(Enesim_Renderer *r, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	*str = thiz->text_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_text_get(Enesim_Renderer *r, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	*str = thiz->text_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_text_set(Enesim_Renderer *r, const char *str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	thiz->text_set(r, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_ellipsize_get(Enesim_Renderer *r, Eina_Bool *enable)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	*enable = thiz->ellipsize_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_ellipsize_set(Enesim_Renderer *r, Eina_Bool enable)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	thiz->ellipsize_set(r, enable);
}
