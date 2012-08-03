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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_label.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_widget.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_LABEL_ELLIPSIZE;
static Ender_Property *EON_LABEL_TEXT;

typedef struct _Eon_Label
{
} Eon_Label;

static inline Eon_Label * _eon_label_get(Eon_Element *ee)
{
	Eon_Label *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
#if 0
static double _eon_theme_label_min_width_ellipsized_get(Eon_Element *ee)
{
	Enesim_Rectangle boundings;
	Eon_Theme_Label *thiz;
	const char *font;
	const char *text;
	char *ellipsized_text;
	int size;
	double min_width;

	thiz = _eon_theme_label_get(ee);
	if (!thiz->text)
		thiz->text = etex_span_new();
	eon_theme_label_font_get(ee, &font);
	eon_theme_label_font_size_get(ee, &size);
	eon_theme_label_text_get(ee, &text);
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
#endif

static double _eon_label_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size size;

	eon_theme_label_size_get(theme_r, &size);

	return size.width;
}

static double _eon_label_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size size;

	eon_theme_label_size_get(theme_r, &size);

	return size.width;
}

static double _eon_label_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size size;

	eon_theme_label_size_get(theme_r, &size);

	return size.height;
}

static double _eon_label_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size size;

	eon_theme_label_size_get(theme_r, &size);

	return size.height;
}

static double _eon_label_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size size;

	eon_theme_label_size_get(theme_r, &size);

	return size.width;
}

static double _eon_label_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size size;

	eon_theme_label_size_get(theme_r, &size);

	return size.height;
}

static Eon_Widget_Descriptor _eon_label_widget_descriptor = {
	.name = "label",
	.min_width_get = _eon_label_min_width_get,
	.max_width_get = _eon_label_max_width_get,
	.min_height_get = _eon_label_min_height_get,
	.max_height_get = _eon_label_max_height_get,
	.preferred_width_get = _eon_label_preferred_width_get,
	.preferred_height_get = _eon_label_preferred_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_label_new(void)
{
	Eon_Label *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Label));
	if (!thiz) return NULL;

	ee = eon_widget_new(&_eon_label_widget_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_label_text_set(Eon_Element *ee, const char *text)
{
	Eon_Label *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_label_get(ee);
	if (!thiz) return;

	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_label_text_set(theme_r, text);
}

static void _eon_label_text_get(Eon_Element *ee, const char **text)
{
	Eon_Label *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_label_get(ee);
	if (!thiz) return;

	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_label_text_get(theme_r, text);
}

static void _eon_label_ellipsize_set(Eon_Element *ee, Eina_Bool enable)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
}

static void _eon_label_ellipsize_get(Eon_Element *ee, Eina_Bool *enabled)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_label_delete NULL
#include "eon_generated_label.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_label_new(void)
{
	return EON_ELEMENT_NEW("label");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_label_new_with_text(const char *text)
{
	Ender_Element *e;

	e = eon_label_new();
	if (!e) return NULL;

	eon_label_text_set(e, text);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_set(Ender_Element *e, const char *text)
{
	ender_element_value_set(e, "text", text, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_get(Ender_Element *e, const char **text)
{
	ender_element_value_get(e, "text", text, NULL);
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_ellipsize_set(Ender_Element *e, Eina_Bool enable)
{
	ender_element_value_set(e, "ellipsize", enable, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_ellipsize_get(Ender_Element *e, Eina_Bool *enable)
{
	ender_element_value_get(e, "ellipsize", enable, NULL);
}

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_direction_set(Ender_Element *e, Etex_Direction direction)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
	eon_widget_property_set(ee, "direction", direction);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_direction_get(Ender_Element *e, Etex_Direction *direction)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
	eon_widget_property_get(ee, "direction", direction);
}
#endif
