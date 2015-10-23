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
#include "eon_theme_mars_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Mars_Eon
{
	Enesim_Renderer *background;
	Egueb_Dom_Node *color;
} Eon_Theme_Mars_Eon;

/*----------------------------------------------------------------------------*
 *                              Eon interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_eon_version_get(void)
{
	return EON_THEME_ELEMENT_EON_VERSION;
}

static void _eon_theme_mars_eon_dtor(void *data)
{
	Eon_Theme_Mars_Eon *thiz = data;

	enesim_renderer_unref(thiz->background);
	free(thiz);
}

static const char * _eon_theme_mars_eon_tag_name_get(void)
{
	return "eon";
}

static Eina_Bool _eon_theme_mars_eon_process(void *data)
{
	Eon_Theme_Mars_Eon *thiz = data;

	/* TODO get the value of the attribute, set it here */
	enesim_renderer_background_color_set(thiz->background, 0xff000000);
	return EINA_TRUE;
}

static Enesim_Renderer * _eon_theme_mars_eon_renderer_get(void *data)
{
	Eon_Theme_Mars_Eon *thiz = data;

	return enesim_renderer_ref(thiz->background);
}

static Eon_Theme_Element_Eon_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_eon_version_get,
	/* .ctor 		= */ eon_theme_mars_eon_new,
	/* .dtor 		= */ _eon_theme_mars_eon_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_eon_tag_name_get,
	/* .process 		= */ _eon_theme_mars_eon_process,
	/* .renderer_get	= */ _eon_theme_mars_eon_renderer_get,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_eon_new(void)
{
	Egueb_Dom_Node *n;
	Eon_Theme_Mars_Eon *thiz;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Eon));
	thiz->background = enesim_renderer_background_new();
	enesim_renderer_background_color_set(thiz->background, 0xff000000);
	n = eon_theme_element_eon_new(&_descriptor, thiz);
	/* TODO add the color attribute */

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

