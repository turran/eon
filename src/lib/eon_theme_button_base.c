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
#define EON_THEME_CONTAINER_MAGIC 0xe0410003
#define EON_THEME_CONTAINER_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_CONTAINER_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_CONTAINER_MAGIC);\
	} while(0)

#define EON_THEME_CONTAINER_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_CONTAINER_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_CONTAINER_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Button_Base
{
	EINA_MAGIC;
	/* private */
	Eon_Theme_Button_Base_Margin_Get margin_get;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Button_Base;

static inline Eon_Theme_Button_Base * _eon_theme_button_base_get(Enesim_Renderer *r)
{
	Eon_Theme_Button_Base *thiz;

	thiz = eon_theme_container_data_get(r);
	EON_THEME_CONTAINER_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_button_base_free(Enesim_Renderer *r)
{
	Eon_Theme_Button_Base *thiz;

	thiz = _eon_theme_button_base_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_button_base_new(Eon_Theme_Button_Base_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Button_Base *thiz;
	Eon_Theme_Container_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Button_Base));
	EINA_MAGIC_SET(thiz, EON_THEME_CONTAINER_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->margin_get = descriptor->margin_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_button_base_free;

	r = eon_theme_container_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

Eina_Bool eon_is_theme_button_base(Enesim_Renderer *r)
{
	Eon_Theme_Button_Base *thiz;

	if (!eon_is_theme_widget(r))
		return EINA_FALSE;

	thiz = eon_theme_widget_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_CONTAINER_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

void * eon_theme_button_base_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Button_Base *thiz;

	thiz = _eon_theme_button_base_get(r);
	return thiz->data;
}

void eon_theme_button_base_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Eon_Theme_Button_Base *thiz;

	thiz = _eon_theme_button_base_get(r);
	if (thiz->margin_get)
		thiz->margin_get(r, margin);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
