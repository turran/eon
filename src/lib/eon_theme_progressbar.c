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
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Progressbar));
	EINA_MAGIC_SET(thiz, EON_THEME_PROGRESSBAR_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	
	pdescriptor.max_width_get = descriptor->max_width_get;
	pdescriptor.min_width_get = descriptor->min_width_get;
	pdescriptor.max_height_get = descriptor->max_height_get;
	pdescriptor.min_height_get = descriptor->min_height_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
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

