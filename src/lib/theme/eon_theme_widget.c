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
/**
 * @todo
 * We must need to simplify the theme creation, all of them need a renderer
 * inside (being a compound or whatever), abstract the whole thing to simplify
 * the callbacks needed for each theme_widget child
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_WIDGET_MAGIC 0xe041001
#define EON_THEME_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_THEME_WIDGET_MAGIC)
#define EON_THEME_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_THEME_WIDGET_MAGIC, ret)

struct _Eon_Theme_Widget
{
	EINA_MAGIC
	/* properties */
	double x;
	double y;
	double width;
	double height;
	Eon_Theme_Widget_Size_Changed_Cb size_changed;
	void *size_changed_data;
	/* private */
	Eon_Theme_Widget_Descriptor descriptor;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Theme_Widget * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Widget *thiz;

	/* sanity checks */
	if (!descriptor->renderer_get)
	{
		printf("no renderer get\n");
		return NULL;
	}

	thiz = calloc(1, sizeof(Eon_Theme_Widget));
	EINA_MAGIC_SET(thiz, EON_THEME_WIDGET_MAGIC);
	thiz->descriptor.renderer_get = descriptor->renderer_get;
	thiz->descriptor.x_set = descriptor->x_set;
	thiz->descriptor.y_set = descriptor->y_set;
	thiz->descriptor.width_set = descriptor->width_set;
	thiz->descriptor.height_set = descriptor->height_set;
	thiz->descriptor.free = descriptor->free;
	thiz->data = data;

	return thiz;
}

void * eon_theme_widget_data_get(Eon_Theme_Widget *thiz)
{
	return thiz->data;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_x_set(Eon_Theme_Widget *thiz, double x)
{
	thiz->x = x;
	if (thiz->descriptor.x_set)
		thiz->descriptor.x_set(thiz, x);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_x_get(Eon_Theme_Widget *thiz, double *x)
{
	if (!x) return;
	*x = thiz->x;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_y_set(Eon_Theme_Widget *thiz, double y)
{
	thiz->y = y;
	if (thiz->descriptor.y_set)
		thiz->descriptor.y_set(thiz, y);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_y_get(Eon_Theme_Widget *thiz, double *y)
{
	if (!y) return;
	*y = thiz->y;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_width_set(Eon_Theme_Widget *thiz, double width)
{
	thiz->width = width;
	if (thiz->descriptor.width_set)
		thiz->descriptor.width_set(thiz, width);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_width_get(Eon_Theme_Widget *thiz, double *width)
{
	if (!width) return;
	*width = thiz->width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_height_set(Eon_Theme_Widget *thiz, double height)
{
	thiz->height = height;
	if (thiz->descriptor.height_set)
		thiz->descriptor.height_set(thiz, height);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_height_get(Eon_Theme_Widget *thiz, double *height)
{
	if (!height) return;
	*height = thiz->height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_renderer_get(Eon_Theme_Widget *thiz, Enesim_Renderer **r)
{
	if (!r) return;
	if (thiz->descriptor.renderer_get)
		*r = thiz->descriptor.renderer_get(thiz);
	else
		*r = NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_size_changed(Eon_Theme_Widget *thiz)
{
	if (thiz->size_changed)
		return thiz->size_changed(thiz->size_changed_data);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_size_changed_cb_set(Eon_Theme_Widget *thiz, Eon_Theme_Widget_Size_Changed_Cb cb, void *data)
{
	thiz->size_changed = cb;
	thiz->size_changed_data = data;
}

