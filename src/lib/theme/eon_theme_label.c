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

#include "eon_theme_widget.h"
#include "eon_theme_label.h"
/**
 * @todo
 * Fix the min_width, max_width and max_height, we should not use the
 * boundings at all
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Label_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Label_Text_Set text_set;
	Eon_Theme_Label_Text_Get text_get;
	Eon_Theme_Label_Font_Size_Get font_size_get;
	Eon_Theme_Label_Font_Size_Set font_size_set;
	Eon_Theme_Label_Font_Get font_get;
	Eon_Theme_Label_Font_Set font_set;
	Eon_Theme_Label_Color_Set color_set;
	Eon_Theme_Label_Color_Get color_get;
} Eon_Theme_Label_Descriptor_Internal;

typedef struct _Eon_Theme_Label
{
	Eon_Theme_Label_Descriptor_Internal descriptor;
	/* private */
	void *data;
} Eon_Theme_Label;

static inline Eon_Theme_Label * _eon_theme_label_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Label *thiz;

	thiz = eon_theme_widget_data_get(t);
	return thiz;
}

static void _eon_theme_label_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_label_size_get(Eon_Theme_Widget *t, Eon_Size *size)
{
	Enesim_Rectangle boundings;
	Enesim_Renderer *r;

	eon_theme_widget_renderer_get(t, &r);
	if (!r) return;
	enesim_renderer_boundings(r, &boundings);
	size->width = boundings.w;
	size->height = boundings.h;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_theme_label_new(Eon_Theme_Label_Descriptor *descriptor, void *data)
{
	Eon_Theme_Label *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Eon_Theme_Widget *t;

	thiz = calloc(1, sizeof(Eon_Theme_Label));
	thiz->data = data;

	/* the label interface */
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.text_set = descriptor->text_set;
	thiz->descriptor.text_get = descriptor->text_get;
	thiz->descriptor.font_size_get = descriptor->font_size_get;
	thiz->descriptor.font_size_set = descriptor->font_size_set;
	thiz->descriptor.font_get = descriptor->font_get;
	thiz->descriptor.font_set = descriptor->font_set;
	thiz->descriptor.color_get = descriptor->color_get;
	thiz->descriptor.color_set = descriptor->color_set;

	/* now the needed enesim functions */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_label_free;

	t = eon_theme_widget_new(&pdescriptor, thiz);
	if (!t) goto renderer_err;

	return t;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_label(Eon_Theme_Widget *t)
{
	Eon_Theme_Label *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_label_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_get(Eon_Theme_Widget *t, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.font_get)
		thiz->descriptor.font_get(t, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_set(Eon_Theme_Widget *t, const char *str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.font_set)
		thiz->descriptor.font_set(t, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_size_get(Eon_Theme_Widget *t, int *size)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.font_size_get)
		thiz->descriptor.font_size_get(t, size);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_label_font_size_set(Eon_Theme_Widget *t, int size)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.font_size_set)
		thiz->descriptor.font_size_set(t, size);
}

EAPI void eon_theme_label_text_get(Eon_Theme_Widget *t, const char **str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.text_get)
		thiz->descriptor.text_get(t, str);
}

EAPI void eon_theme_label_text_set(Eon_Theme_Widget *t, const char *str)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.text_set)
		thiz->descriptor.text_set(t, str);
}

EAPI void eon_theme_label_color_get(Eon_Theme_Widget *t, Enesim_Color *color)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.color_get)
		thiz->descriptor.color_get(t, color);
}

EAPI void eon_theme_label_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Theme_Label *thiz;

	thiz = _eon_theme_label_get(t);
	if (thiz->descriptor.color_set)
		thiz->descriptor.color_set(t, color);
}
