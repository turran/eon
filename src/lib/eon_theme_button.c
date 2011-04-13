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
#define EON_THEME_BUTTON_MAGIC 0xe0410002
#define EON_THEME_BUTTON_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_BUTTON_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_BUTTON_MAGIC);\
	} while(0)

#define EON_THEME_BUTTON_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_BUTTON_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_BUTTON_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Button
{
	EINA_MAGIC;
	/* properties */
	Enesim_Renderer *content;
	/* private */
	Eon_Theme_Widget_Descriptor *twdescriptor;
	void *data;
} Eon_Theme_Button;

static inline Eon_Theme_Button * _eon_theme_button_get(Enesim_Renderer *r)
{
	Eon_Theme_Button *thiz;

	thiz = eon_theme_widget_data_get(r);
	EON_THEME_BUTTON_MAGIC_CHECK_RETURN(thiz, NULL);

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
EAPI Enesim_Renderer * eon_theme_button_new(Eon_Theme_Widget_Descriptor *twdescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Button *thiz;
	Enesim_Renderer *r;

	/* first check the label descriptors */

	thiz = calloc(1, sizeof(Eon_Theme_Button));
	EINA_MAGIC_SET(thiz, EON_THEME_BUTTON_MAGIC);
	thiz->data = data;
	thiz->twdescriptor = twdescriptor;
	r = eon_theme_widget_new(twdescriptor, descriptor, thiz);
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
EAPI Eina_Bool eon_is_theme_button(Enesim_Renderer *r)
{
	Eon_Theme_Button *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_button_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Button *thiz;

	thiz = _eon_theme_button_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_button_content_set(Enesim_Renderer *r, Enesim_Renderer *content)
{
	Eon_Theme_Button *thiz;

	thiz = _eon_theme_button_get(r);
	printf("setting content = %p %p\n", r, content);
	thiz->content = content;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_button_content_get(Enesim_Renderer *r, Enesim_Renderer **content)
{
	Eon_Theme_Button *thiz;

	thiz = _eon_theme_button_get(r);
	printf("getting content %p %p\n", r, thiz->content);
	*content = thiz->content;
}
