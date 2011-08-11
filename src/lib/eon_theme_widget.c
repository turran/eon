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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_THEME_WIDGET_MAGIC)
#define EON_THEME_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_THEME_WIDGET_MAGIC, ret)

typedef struct _Eon_Theme_Widget
{
	EINA_MAGIC
	/* properties */
	double width;
	double height;
	double x;
	double y;
	/* private */
	void *data;
	Enesim_Renderer_Descriptor descriptor;
	Enesim_Renderer_Delete free;
} Eon_Theme_Widget;

#define _eon_theme_widget_get(r) \
	enesim_renderer_data_get(r); \
	if (!EINA_MAGIC_CHECK((Eon_Theme_Widget *)enesim_renderer_data_get(r), EON_THEME_WIDGET_MAGIC)) \
		EINA_MAGIC_FAIL((Eon_Theme_Widget *)enesim_renderer_data_get(r), EON_THEME_WIDGET_MAGIC)

#if 0
static inline Eon_Theme_Widget * _eon_theme_widget_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_THEME_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
#endif
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static void _eon_theme_widget_free(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}

static void _eon_theme_widget_boundings(Enesim_Renderer *r, Enesim_Rectangle *rectangle)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	rectangle->x = 0;
	rectangle->y = 0;
	rectangle->w = thiz->width;
	rectangle->h = thiz->height;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Widget *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Widget));
	EINA_MAGIC_SET(thiz, EON_THEME_WIDGET_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

	thiz->descriptor.boundings = _eon_theme_widget_boundings;
	thiz->descriptor.free = _eon_theme_widget_free;
	thiz->descriptor.sw_setup = descriptor->sw_setup;
	thiz->descriptor.sw_cleanup = descriptor->sw_cleanup;

	r = enesim_renderer_new(&thiz->descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

Eina_Bool eon_is_theme_widget(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = enesim_renderer_data_get(r);
	if (!thiz) return EINA_FALSE;
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_WIDGET_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

void * eon_theme_widget_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	return thiz->data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_width_set(Enesim_Renderer *r, double width)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*width = thiz->width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_height_set(Enesim_Renderer *r, double height)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*height = thiz->height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_x_set(Enesim_Renderer *r, double x)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->x = x;
	/* FIXME for now */
	enesim_renderer_x_origin_set(r, x);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_x_get(Enesim_Renderer *r, double *x)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*x = thiz->x;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_y_set(Enesim_Renderer *r, double y)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->y = y;
	/* FIXME for now */
	enesim_renderer_y_origin_set(r, y);
	//printf("setting y @ %g\n", y);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_y_get(Enesim_Renderer *r, double *y)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*y = thiz->y;
}
