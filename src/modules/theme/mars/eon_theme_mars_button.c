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
typedef struct _Eon_Theme_Mars_Button
{
	Enesim_Renderer *button;
	Enesim_Renderer *blur;
	Enesim_Renderer *content;
} Eon_Theme_Mars_Button;

/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_button_version_get(void)
{

}

static void _eon_theme_mars_button_dtor(void *data)
{
	Eon_Theme_Mars_Button *thiz = data;

	enesim_renderer_unref(thiz->button);
	enesim_renderer_unref(thiz->blur);
	enesim_renderer_unref(thiz->content);
	free(thiz);
}

static const char * _eon_theme_mars_button_tag_name_get(void)
{
	return "button";
}

static Eina_Bool _eon_theme_mars_button_process(void *data)
{
	/* get the geometry */
	/* set the rounded rectangle area */
	/* set the border color */
	/* apply the blur value */
	return EINA_TRUE;
}

static void _eon_theme_mars_button_padding_get(void *data, Eon_Box *padding)
{

}

static void _eon_theme_mars_button_content_set(void *data, Enesim_Renderer *r)
{
	Eon_Theme_Mars_Button *thiz = data;
	if (thiz->content)
	{
		enesim_renderer_unref(thiz->content);
		thiz->content = NULL;
	}
	thiz->content = r;
}

static Enesim_Renderer * _eon_theme_mars_button_renderer_get(void *data)
{
	Eon_Theme_Mars_Button *thiz = data;
	return enesim_renderer_ref(thiz->button);
}

static Eon_Theme_Element_Button_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_button_version_get,
	/* .ctor 		= */ eon_theme_mars_button_new,
	/* .dtor 		= */ _eon_theme_mars_button_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_button_tag_name_get,
	/* .process 		= */ _eon_theme_mars_button_process,
	/* .content_set		= */ _eon_theme_mars_button_content_set,
	/* .padding_get		= */ _eon_theme_mars_button_padding_get,
	/* .renderer_get	= */ _eon_theme_mars_button_renderer_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_button_new(void)
{
	Egueb_Dom_Node *n;
	Eon_Theme_Mars_Button *thiz;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Button));
	thiz->button = enesim_renderer_rectangle_new();
	thiz->blur = enesim_renderer_blur_new();
	n = eon_theme_element_button_new(&_descriptor, thiz);

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
