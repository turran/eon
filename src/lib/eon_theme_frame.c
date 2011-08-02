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
#define EON_THEME_FRAME_MAGIC 0xe0410003
#define EON_THEME_FRAME_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_FRAME_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_FRAME_MAGIC);\
	} while(0)

#define EON_THEME_FRAME_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_FRAME_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_FRAME_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Frame
{
	EINA_MAGIC;
	Enesim_Renderer *description;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Frame;

static inline Eon_Theme_Frame * _eon_theme_frame_get(Enesim_Renderer *r)
{
	Eon_Theme_Frame *thiz;

	thiz = eon_theme_container_data_get(r);
	EON_THEME_FRAME_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_frame_free(Enesim_Renderer *r)
{
	Eon_Theme_Frame *thiz;

	thiz = _eon_theme_frame_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_frame_content_position_get(Enesim_Renderer *r, Enesim_Rectangle
	*description_area, double *left, double *top, double *right,
	double *bottom)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_frame_new(Eon_Theme_Frame_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Frame *thiz;
	Eon_Theme_Container_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Frame));
	EINA_MAGIC_SET(thiz, EON_THEME_FRAME_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

#if THEME_OLD
	pdescriptor.content_position_get = descriptor->content_position_get;
	pdescriptor.decoration_width_get = descriptor->decoration_width_get;
	pdescriptor.decoration_height_get = descriptor->decoration_height_get;
#endif
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_frame_free;

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
EAPI Eina_Bool eon_is_theme_frame(Enesim_Renderer *r)
{
	Eon_Theme_Frame *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_frame_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Frame *thiz;

	thiz = _eon_theme_frame_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_frame_description_set(Enesim_Renderer *r, Enesim_Renderer *description)
{
	Eon_Theme_Frame *thiz;

	thiz = _eon_theme_frame_get(r);
	thiz->description = description;
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_frame_description_get(Enesim_Renderer *r, Enesim_Renderer **description)
{
	Eon_Theme_Frame *thiz;

	thiz = _eon_theme_frame_get(r);
	*description = thiz->description;
}

