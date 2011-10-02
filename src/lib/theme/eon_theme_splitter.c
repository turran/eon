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
#define EON_THEME_SPLITTER_MAGIC 0xe0410003
#define EON_THEME_SPLITTER_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_SPLITTER_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_SPLITTER_MAGIC);\
	} while(0)

#define EON_THEME_SPLITTER_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_SPLITTER_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_SPLITTER_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Splitter
{
	EINA_MAGIC;
	/* properties */
	/* private */
	Enesim_Renderer *second_content;
	Eon_Orientation orientation;
	double position;
	Eon_Theme_Splitter_Thickness_Get thickness_get;
	Eon_Theme_Splitter_Min_Length_Get min_length_get;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Splitter;

static inline Eon_Theme_Splitter * _eon_theme_splitter_get(Enesim_Renderer *r)
{
	Eon_Theme_Splitter *thiz;

	thiz = eon_theme_container_data_get(r);
	EON_THEME_SPLITTER_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_splitter_free(Enesim_Renderer *r)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_splitter_thickness_get(Enesim_Renderer *r, double *thickness)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	if (!thiz) return;
	if (thiz->thickness_get)
		*thickness = thiz->thickness_get(r);
	else
		*thickness = 1;
}

void eon_theme_splitter_min_length_get(Enesim_Renderer *r, double *length)
{
	Eon_Theme_Splitter *thiz;

	if (!length) return;
	thiz = _eon_theme_splitter_get(r);
	if (!thiz) return;
	if (thiz->min_length_get)
		*length = thiz->min_length_get(r);
	else
		*length = 1;
}

void eon_theme_splitter_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	thiz->orientation = orientation;
}

void eon_theme_splitter_second_content_set(Enesim_Renderer *r, Enesim_Renderer *second_content)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	thiz->second_content = second_content;
}

void eon_theme_splitter_position_set(Enesim_Renderer *r, double position)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	thiz->position = position;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_splitter_new(Eon_Theme_Splitter_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Splitter *thiz;
	Eon_Theme_Container_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Splitter));
	EINA_MAGIC_SET(thiz, EON_THEME_SPLITTER_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

	thiz->min_length_get = descriptor->min_length_get;
	thiz->thickness_get = descriptor->thickness_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_splitter_free;

	r = eon_theme_container_new(&pdescriptor, thiz);
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
EAPI Eina_Bool eon_is_theme_splitter(Enesim_Renderer *r)
{
	Eon_Theme_Splitter *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_splitter_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_splitter_second_content_get(Enesim_Renderer *r, Enesim_Renderer **second_content)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	*second_content = thiz->second_content;	
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_splitter_position_get(Enesim_Renderer *r, double *position)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	*position = thiz->position;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_splitter_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation)
{
	Eon_Theme_Splitter *thiz;

	thiz = _eon_theme_splitter_get(r);
	*orientation = thiz->orientation;
}
