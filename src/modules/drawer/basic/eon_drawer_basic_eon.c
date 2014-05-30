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
#include "Eon_Drawer.h"
#include "eon_drawer_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Drawer_Basic_Eon
{
	Enesim_Renderer *background;
} Eon_Drawer_Basic_Eon;
/*----------------------------------------------------------------------------*
 *                         Attribute setters/getters                           *
 *----------------------------------------------------------------------------*/
static void _eon_drawer_basic_eon_color_set(Eon_Drawer_Widget *w, Enesim_Color color)
{
	Eon_Drawer_Basic_Eon *thiz;
	Enesim_Renderer *r;

	thiz = eon_drawer_eon_data_get(w);
	enesim_renderer_background_color_set(thiz->background, color);
}

static Enesim_Color _eon_drawer_basic_eon_color_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Basic_Eon *thiz;

	thiz = eon_drawer_eon_data_get(w);
	return enesim_renderer_background_color_get(thiz->background);
}
/*----------------------------------------------------------------------------*
 *                               Eon interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_drawer_basic_eon_background_renderer_get(
		Eon_Drawer_Widget *w, void *data)
{
	Eon_Drawer_Basic_Eon *thiz = data;

	return enesim_renderer_ref(thiz->background);	
}

static void _eon_drawer_basic_eon_ender_populate(Eon_Drawer_Widget *w,
		void *data, Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *attr;

	/* add our own attributes */
	attr = eon_drawer_attr_enesim_color_new("color",
			EON_DRAWER_ATTR_ENESIM_COLOR_GET(_eon_drawer_basic_eon_color_get),
			EON_DRAWER_ATTR_ENESIM_COLOR_SET(_eon_drawer_basic_eon_color_set));
	egueb_dom_element_attribute_add(n, attr, NULL);
}

static void _eon_drawer_basic_eon_free(Eon_Drawer_Widget *w, void *data)
{
	Eon_Drawer_Basic_Eon *thiz = data;

	enesim_renderer_unref(thiz->background);
	free(thiz);
}

static Eon_Drawer_Eon_Descriptor _descriptor = {
	/* .background_renderer_get	= */ _eon_drawer_basic_eon_background_renderer_get,
	/* .ender_populate		= */ _eon_drawer_basic_eon_ender_populate,
	/* .ender_process		= */ NULL,
	/* .free 			= */ _eon_drawer_basic_eon_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Drawer_Widget * eon_drawer_basic_eon_new(void)
{
	Eon_Drawer_Basic_Eon *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Drawer_Basic_Eon));
	thiz->background = enesim_renderer_background_new();
	enesim_renderer_background_color_set(thiz->background, 0xffff0000);

	return eon_drawer_eon_new(&_descriptor, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

