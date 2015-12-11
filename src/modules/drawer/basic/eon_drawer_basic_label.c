/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
typedef struct _Eon_Drawer_Basic_Label
{
	Enesim_Renderer *text;
} Eon_Drawer_Basic_Label;

/*----------------------------------------------------------------------------*
 *                               Label interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_drawer_basic_label_text_renderer_get(Eon_Drawer_Widget *w,
		void *data)
{
	Eon_Drawer_Basic_Label *thiz = data;

	return enesim_renderer_ref(thiz->text);	
}

static void _eon_drawer_basic_label_text_color_set(Eon_Drawer_Widget *w,
		void *data, Enesim_Color color)
{
	Eon_Drawer_Basic_Label *thiz = data;
	enesim_renderer_color_set(thiz->text, color);
}

static Enesim_Renderer * _eon_drawer_basic_label_renderer_get(
		Eon_Drawer_Widget *w, void *data)
{
	Eon_Drawer_Basic_Label *thiz = data;
	return enesim_renderer_ref(thiz->text);
}

static void _eon_drawer_basic_label_geometry_set(Eon_Drawer_Widget *w,
		void *data, Eina_Rectangle *geom)
{
	Eon_Drawer_Basic_Label *thiz = data;
	enesim_renderer_x_origin_set(thiz->text, geom->x);
	enesim_renderer_y_origin_set(thiz->text, geom->y);
}

static void _eon_drawer_basic_label_free(Eon_Drawer_Widget *w, void *data)
{
	Eon_Drawer_Basic_Label *thiz = data;

	enesim_renderer_unref(thiz->text);
	free(thiz);
}

static Eon_Drawer_Label_Descriptor _descriptor = {
	/* .text_renderer_get	= */ _eon_drawer_basic_label_text_renderer_get,
	/* .text_color_set	= */ _eon_drawer_basic_label_text_color_set,
	/* .renderer_get 	= */ _eon_drawer_basic_label_renderer_get,
	/* .geometry_set 	= */ _eon_drawer_basic_label_geometry_set,
	/* .ender_populate	= */ NULL,
	/* .ender_process	= */ NULL,
	/* .free 		= */ _eon_drawer_basic_label_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Drawer_Widget * eon_drawer_basic_label_new(void)
{
	Eon_Drawer_Basic_Label *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Drawer_Basic_Label));
	thiz->text = enesim_renderer_text_span_new();

	return eon_drawer_label_new(&_descriptor, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
