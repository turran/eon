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
#include "Eon_Theme.h"
#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_COLOR_MAGIC 0xe0410003
#define EON_THEME_COLOR_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_COLOR_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_COLOR_MAGIC);\
	} while(0)

#define EON_THEME_COLOR_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_COLOR_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_COLOR_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Color
{
	EINA_MAGIC;
	Eon_Theme_Color_Content_Element_Get content_element_get;
	Eon_Theme_Color_Content_Color_Set content_color_set;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Color;

static inline Eon_Theme_Color * _eon_theme_color_get(Enesim_Renderer *r)
{
	Eon_Theme_Color *thiz;

	thiz = eon_theme_button_base_data_get(r);
	EON_THEME_COLOR_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static const char * _eon_theme_color_name(Enesim_Renderer *r)
{
	return "theme_color";
}

static void _eon_theme_color_free(Enesim_Renderer *r)
{
	Eon_Theme_Color *thiz;

	thiz = _eon_theme_color_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Ender_Element * eon_theme_color_content_element_get(Enesim_Renderer *r)
{
	Eon_Theme_Color *thiz;

	thiz = _eon_theme_color_get(r);
	if (thiz->content_element_get)
		return thiz->content_element_get(r);
	else
		return NULL;

}

void eon_theme_color_content_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Eon_Theme_Color *thiz;

	thiz = _eon_theme_color_get(r);
	if (thiz->content_color_set)
		return thiz->content_color_set(r, color);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_color_new(Eon_Theme_Color_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Color *thiz;
	Eon_Theme_Button_Base_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Color));
	EINA_MAGIC_SET(thiz, EON_THEME_COLOR_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->content_element_get = descriptor->content_element_get;
	thiz->content_color_set = descriptor->content_color_set;

	pdescriptor.position_get = descriptor->position_get;
	pdescriptor.margin_get = descriptor->margin_get;
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.setup = descriptor->setup;
	pdescriptor.cleanup = descriptor->cleanup;
	pdescriptor.has_changed = descriptor->has_changed;
	pdescriptor.name = _eon_theme_color_name;
	pdescriptor.free = _eon_theme_color_free;

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
EAPI Eina_Bool eon_is_theme_color(Enesim_Renderer *r)
{
	Eon_Theme_Color *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_color_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Color *thiz;

	thiz = _eon_theme_color_get(r);
	return thiz->data;
}

