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
#define EON_THEME_PROGRESSBAR_MAGIC 0xe0410003
#define EON_THEME_PROGRESSBAR_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_PROGRESSBAR_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_PROGRESSBAR_MAGIC);\
	} while(0)

#define EON_THEME_PROGRESSBAR_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_PROGRESSBAR_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_PROGRESSBAR_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Progressbar
{
	EINA_MAGIC;
	Eon_Theme_Progressbar_Max_Width_Get max_width_get;
	Eon_Theme_Progressbar_Min_Width_Get min_width_get;
	Eon_Theme_Progressbar_Max_Height_Get max_height_get;
	Eon_Theme_Progressbar_Min_Height_Get min_height_get;
	Eon_Theme_Progressbar_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Progressbar_Preferred_Height_Get preferred_height_get;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Progressbar;

static inline Eon_Theme_Progressbar * _eon_theme_progressbar_get(Enesim_Renderer *r)
{
	Eon_Theme_Progressbar *thiz;

	thiz = eon_theme_widget_data_get(r);
	EON_THEME_PROGRESSBAR_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static const char * _eon_theme_progressbar_name(Enesim_Renderer *r)
{
	return "theme_progressbar";
}

static void _eon_theme_progressbar_free(Enesim_Renderer *r)
{
	Eon_Theme_Progressbar *thiz;

	thiz = _eon_theme_progressbar_get(r);
	if (thiz->free) thiz->free(r);
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
EAPI Enesim_Renderer * eon_theme_progressbar_new(Eon_Theme_Progressbar_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Progressbar *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Progressbar));
	EINA_MAGIC_SET(thiz, EON_THEME_PROGRESSBAR_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

	thiz->max_width_get = descriptor->max_width_get;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->max_height_get = descriptor->max_height_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.setup = descriptor->setup;
	pdescriptor.cleanup = descriptor->cleanup;
	pdescriptor.has_changed = descriptor->has_changed;
	pdescriptor.name = _eon_theme_progressbar_name;
	pdescriptor.free = _eon_theme_progressbar_free;

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
EAPI Eina_Bool eon_is_theme_progressbar(Enesim_Renderer *r)
{
	Eon_Theme_Progressbar *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_progressbar_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Progressbar *thiz;

	thiz = _eon_theme_progressbar_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_progressbar_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Progressbar *thiz;

	thiz = _eon_theme_progressbar_get(r);
	if (!thiz) return;
	if (thiz->min_width_get)
		*width = thiz->min_width_get(r);
	else
		*width = 1;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_progressbar_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Progressbar *thiz;

	if (!height) return;
	thiz = _eon_theme_progressbar_get(r);
	if (!thiz) return;
	if (thiz->min_height_get)
		*height = thiz->min_height_get(r);
	else
		*height = 1;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_progressbar_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Progressbar *thiz;

	thiz = _eon_theme_progressbar_get(r);
	if (!thiz) return;
	if (thiz->max_width_get)
		*width = thiz->max_width_get(r);
	else
		*width = DBL_MAX;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_progressbar_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Progressbar *thiz;

	if (!height) return;
	thiz = _eon_theme_progressbar_get(r);
	if (!thiz) return;
	if (thiz->max_height_get)
		*height = thiz->max_height_get(r);
	else
		*height = DBL_MAX;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_progressbar_preferred_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Progressbar *thiz;

	thiz = _eon_theme_progressbar_get(r);
	if (!thiz) return;
	if (thiz->preferred_width_get)
		*width = thiz->preferred_width_get(r);
	else
		*width = -1;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_progressbar_preferred_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Progressbar *thiz;

	if (!height) return;
	thiz = _eon_theme_progressbar_get(r);
	if (!thiz) return;
	if (thiz->preferred_height_get)
		*height = thiz->preferred_height_get(r);
	else
		*height = -1;
}
