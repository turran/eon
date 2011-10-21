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
#define EON_THEME_RADIO_MAGIC 0xe0410004
#define EON_THEME_RADIO_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_RADIO_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_RADIO_MAGIC);\
	} while(0)

#define EON_THEME_RADIO_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_RADIO_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_RADIO_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Radio
{
	EINA_MAGIC;
	/* properties */
	/* private */
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Radio;

static inline Eon_Theme_Radio * _eon_theme_radio_get(Enesim_Renderer *r)
{
	Eon_Theme_Radio *thiz;

	thiz = eon_theme_button_base_data_get(r);
	EON_THEME_RADIO_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_radio_free(Enesim_Renderer *r)
{
	Eon_Theme_Radio *thiz;

	thiz = _eon_theme_radio_get(r);
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
EAPI Enesim_Renderer * eon_theme_radio_new(Eon_Theme_Radio_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Radio *thiz;
	Eon_Theme_Button_Base_Descriptor pdescriptor;
	Enesim_Renderer *r;

	if (!descriptor) return NULL;

	thiz = calloc(1, sizeof(Eon_Theme_Radio));
	EINA_MAGIC_SET(thiz, EON_THEME_RADIO_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

	pdescriptor.position_get = descriptor->position_get;
	pdescriptor.margin_get = descriptor->margin_get;
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.setup = descriptor->setup;
	pdescriptor.cleanup = descriptor->cleanup;
	pdescriptor.free = _eon_theme_radio_free;

	r = eon_theme_button_base_new(&pdescriptor, thiz);
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
EAPI Eina_Bool eon_is_theme_radio(Enesim_Renderer *r)
{
	Eon_Theme_Radio *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_radio_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Radio *thiz;

	thiz = _eon_theme_radio_get(r);
	return thiz->data;
}
