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
/**
 * @todo
 * Fix the min_width, max_width and max_height, we should not use the
 * boundings at all
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Label
{
	/* the label interface */
	Eon_Theme_Label_Setup setup;
	Eon_Theme_Label_Cleanup cleanup;
	Eon_Theme_Label_Renderer_Get renderer_get;
	Enesim_Renderer_Delete free;
	Eina_Bool can_ellipsize;
	/* private */
	/* data needed for our own callbacks */
	Enesim_Renderer *text;
	Eina_Bool ellipsized;
	void *data;
} Eon_Theme_Label;

static inline Eon_Theme_Label * _eon_theme_label_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = eon_theme_widget_data_get(r);
	return thiz;
}

static const char * _eon_theme_label_name(Enesim_Renderer *r)
{
	return "theme_label";
}

static Enesim_Renderer * _eon_theme_label_renderer_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);

	if (thiz->renderer_get)
		return thiz->renderer_get(r, thiz->text);
	return thiz->text;
}

static Eina_Bool _eon_theme_label_setup(Enesim_Renderer *r, Enesim_Error **err)
{
	Eon_Theme_Label *thiz;
	Enesim_Color color;
	Eina_Bool ret = EINA_TRUE;
	double ox, oy;

	thiz = _eon_theme_label_get(r);
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->text, ox, oy);
	enesim_renderer_color_get(r, &color);
	enesim_renderer_color_set(thiz->text, color);
	if (thiz->setup)
	{
		ret = thiz->setup(r, thiz->text, err);
	}
	return ret;
}

static void _eon_theme_label_cleanup(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	if (thiz->cleanup)
	{
		thiz->cleanup(r);
	}
}

static void _eon_theme_label_free(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}

static double _eon_theme_label_min_width_ellipsized_get(Enesim_Renderer *r)
{
	Enesim_Rectangle boundings;
	Eon_Theme_Label *thiz;
	const char *font;
	const char *text;
	char *ellipsized_text;
	int size;
	double min_width;

	thiz = _eon_theme_label_get(r);
	if (!thiz->text)
		thiz->text = etex_span_new();
	eon_theme_label_font_get(r, &font);
	eon_theme_label_size_get(r, &size);
	eon_theme_label_text_get(r, &text);
	etex_base_font_name_set(thiz->text, font);
	etex_base_size_set(thiz->text, size);
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

static unsigned int _eon_theme_label_width_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;
	Enesim_Rectangle boundings;

	thiz = _eon_theme_label_get(r);
	enesim_renderer_boundings(thiz->text, &boundings);
	return boundings.w;
}
static unsigned int _eon_theme_label_height_get(Enesim_Renderer *r)
{
	Eon_Theme_Label *thiz;
	Enesim_Rectangle boundings;

	thiz = _eon_theme_label_get(r);
	enesim_renderer_boundings(thiz->text, &boundings);
	return boundings.h;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_label_text_get(Enesim_Renderer *r, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	printf("GETTING TEXXXXXT\n");
	etex_span_text_get(thiz->text, str);
}

void eon_theme_label_text_set(Enesim_Renderer *r, const char *str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	etex_span_text_set(thiz->text, str);
}

void eon_theme_label_ellipsize_get(Enesim_Renderer *r, Eina_Bool *enable)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	*enable = thiz->ellipsized;
}

void eon_theme_label_ellipsize_set(Enesim_Renderer *r, Eina_Bool enable)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	thiz->ellipsized = enable;
}

/* FIX the below functions */
void eon_theme_label_ellipsized_text_get(Enesim_Renderer *r, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	etex_span_text_get(thiz->text, str);
}

void eon_theme_label_min_width_get(Enesim_Renderer *r, double *width)
{
	*width = _eon_theme_label_width_get(r);
}

void eon_theme_label_min_height_get(Enesim_Renderer *r, double *height)
{
	*height = _eon_theme_label_height_get(r);
}

void eon_theme_label_max_width_get(Enesim_Renderer *r, double *width)
{
	*width = _eon_theme_label_width_get(r);
}

void eon_theme_label_max_height_get(Enesim_Renderer *r, double *height)
{
	*height = _eon_theme_label_height_get(r);
}

void eon_theme_label_preferred_width_get(Enesim_Renderer *r, double *width)
{
	*width = _eon_theme_label_width_get(r);
}

void eon_theme_label_preferred_height_get(Enesim_Renderer *r, double *height)
{
	*height = _eon_theme_label_height_get(r);
}
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
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Label));
	thiz->data = data;
	thiz->text = etex_span_new();
	/* the label interface */
	thiz->free = descriptor->free;
	thiz->renderer_get = descriptor->renderer_get;
	thiz->setup = descriptor->setup;
	thiz->cleanup = descriptor->cleanup;
	thiz->can_ellipsize = descriptor->can_ellipsize;
	/* set default values */

	/* now the needed enesim functions */
	pdescriptor.renderer_get = _eon_theme_label_renderer_get;
	pdescriptor.setup = _eon_theme_label_setup;
	pdescriptor.cleanup = _eon_theme_label_cleanup;
	pdescriptor.name = _eon_theme_label_name;
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
	etex_base_font_name_get(thiz->text, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_set(Enesim_Renderer *r, const char *str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	etex_base_font_name_set(thiz->text, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_size_get(Enesim_Renderer *r, int *size)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	etex_base_size_get(thiz->text, size);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_size_set(Enesim_Renderer *r, int size)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(r);
	etex_base_size_set(thiz->text, size);
}


