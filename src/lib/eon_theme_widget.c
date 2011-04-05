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
typedef struct _Eon_Theme_Widget
{
	Eon_Theme_Widget_Descriptor *descriptor;
	void *data;
} Eon_Theme_Widget;

static inline Eon_Theme_Widget * _eon_theme_widget_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = enesim_renderer_data_get(r);

	return thiz;
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
EAPI Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *twdescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Widget *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Widget));
	thiz->data = data;
	thiz->descriptor = twdescriptor;

	r = enesim_renderer_new(descriptor, thiz);
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
EAPI void * eon_theme_widget_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	if (thiz->descriptor->min_width_get)
		*width = thiz->descriptor->min_width_get(r);
	else
		*width = 0;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Widget *thiz;

	if (!height) return;
	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	if (thiz->descriptor->min_height_get)
		*height = thiz->descriptor->min_height_get(r);
	else
		*height = 0;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	if (thiz->descriptor->max_width_get)
		*width = thiz->descriptor->max_width_get(r);
	else
		*width = 0;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Widget *thiz;

	if (!height) return;
	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	if (thiz->descriptor->max_height_get)
		*height = thiz->descriptor->max_height_get(r);
	else
		*height = 0;
}
