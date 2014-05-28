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
#include "eon_theme_scrollbar.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Scrollbar_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Scrollbar_Thumb_Set thumb_set;
	Eon_Theme_Scrollbar_Thumb_Area_Set thumb_area_set;
	Eon_Theme_Scrollbar_Arrow_Increment_Set arrow_increment_set;
	Eon_Theme_Scrollbar_Arrow_Decrement_Set arrow_decrement_set;
} Eon_Theme_Scrollbar_Descriptor_Internal;

typedef struct _Eon_Theme_Scrollbar
{
	Eon_Theme_Scrollbar_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Scrollbar;

static inline Eon_Theme_Scrollbar * _eon_theme_scrollbar_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = eon_theme_widget_data_get(t);
	return thiz;
}

static void _eon_theme_scrollbar_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
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
EAPI Eon_Theme_Widget * eon_theme_scrollbar_new(Eon_Theme_Scrollbar_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Scrollbar *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Eon_Theme_Widget *t;

	thiz = calloc(1, sizeof(Eon_Theme_Scrollbar));
	thiz->descriptor.thumb_set = descriptor->thumb_set;
	thiz->descriptor.thumb_area_set = descriptor->thumb_area_set;
	thiz->descriptor.arrow_increment_set = descriptor->arrow_increment_set;
	thiz->descriptor.arrow_decrement_set = descriptor->arrow_decrement_set;
	thiz->descriptor.free = descriptor->free;
	thiz->data = data;

	/* widget descriptor */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_scrollbar_free;

	t = eon_theme_widget_new(&pdescriptor, thiz);
	return t;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_scrollbar_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(t);
	return thiz->data;
}

EAPI void eon_theme_scrollbar_thumb_set(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(t);
	if (thiz->descriptor.thumb_set)
		thiz->descriptor.thumb_set(t, r);
}

EAPI void eon_theme_scrollbar_thumb_area_set(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(t);
	if (thiz->descriptor.thumb_area_set)
		thiz->descriptor.thumb_area_set(t, r);
}

EAPI void eon_theme_scrollbar_arrow_increment_set(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(t);
	if (thiz->descriptor.arrow_increment_set)
		thiz->descriptor.arrow_increment_set(t, r);
}

EAPI void eon_theme_scrollbar_arrow_decrement_set(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(t);
	if (thiz->descriptor.arrow_decrement_set)
		thiz->descriptor.arrow_decrement_set(t, r);
}
