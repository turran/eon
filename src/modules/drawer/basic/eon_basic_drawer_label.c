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
#include "eon_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Drawer_Label
{
	Enesim_Renderer *text;
} Eon_Basic_Drawer_Label;

/*----------------------------------------------------------------------------*
 *                      The Eon's theme label interface                       *
 *----------------------------------------------------------------------------*/
#if 0
static Enesim_Renderer * _basic_drawer_label_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	return thiz->text;
}

static void _basic_drawer_label_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
}

static void _basic_drawer_label_text_get(Eon_Theme_Widget *t, const char **str)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_text_span_text_get(thiz->text, str);
}

static void _basic_drawer_label_text_set(Eon_Theme_Widget *t, const char *str)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_text_span_text_set(thiz->text, str);
}

static void _basic_drawer_label_font_size_get(Eon_Theme_Widget *t, int *size)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_text_base_size_get(thiz->text, size);
}

static void _basic_drawer_label_font_size_set(Eon_Theme_Widget *t, int size)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_text_base_size_set(thiz->text, size);
}

static void _basic_drawer_label_font_get(Eon_Theme_Widget *t, const char **str)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_text_base_font_name_get(thiz->text, str);
}

static void _basic_drawer_label_font_set(Eon_Theme_Widget *t, const char *str)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_text_base_font_name_set(thiz->text, str);
}

static void _basic_drawer_label_color_get(Eon_Theme_Widget *t, Enesim_Color *color)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_renderer_color_get(thiz->text, color);
}

static void _basic_drawer_label_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Drawer_Label *thiz;

	thiz = _label_get(t);
	enesim_renderer_color_set(thiz->text, color);
}


static Eon_Theme_Label_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_drawer_label_renderer_get,
	/* .x_set 		= */ _basic_drawer_label_x_set,
	/* .y_set 		= */ _basic_drawer_label_y_set,
	/* .width_set 		= */ NULL,
	/* .height_set		= */ NULL,
	/* .free		= */ _basic_drawer_label_free,
	/* .text_set 		= */ _basic_drawer_label_text_set,
	/* .text_get		= */ _basic_drawer_label_text_get,
	/* .font_size_get 	= */ _basic_drawer_label_font_size_get,
	/* .font_size_set	= */ _basic_drawer_label_font_size_set,
	/* .font_get 		= */ _basic_drawer_label_font_get,
	/* .font_set 		= */ _basic_drawer_label_font_set,
	/* .color_get 		= */ _basic_drawer_label_color_get,
	/* .color_set 		= */ _basic_drawer_label_color_set,
};
#endif
/*----------------------------------------------------------------------------*
 *                               Label interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_basic_drawer_label_text_renderer_get(Eon_Drawer_Widget *w,
		void *data)
{
	Eon_Basic_Drawer_Label *thiz = data;

	return enesim_renderer_ref(thiz->text);	
}

static void _eon_basic_drawer_label_text_color_set(Eon_Drawer_Widget *w,
		void *data, Enesim_Color color)
{
	Eon_Basic_Drawer_Label *thiz = data;
	enesim_renderer_color_set(thiz->text, color);
}

static Enesim_Renderer * _eon_basic_drawer_label_renderer_get(
		Eon_Drawer_Widget *w, void *data)
{
	Eon_Basic_Drawer_Label *thiz = data;
	return enesim_renderer_ref(thiz->text);
}

static void _eon_basic_drawer_label_geometry_set(Eon_Drawer_Widget *w,
		void *data, Eina_Rectangle *geom)
{
	Eon_Basic_Drawer_Label *thiz = data;
	enesim_renderer_x_origin_set(thiz->text, geom->x);
	enesim_renderer_y_origin_set(thiz->text, geom->y);
}

static void _eon_basic_drawer_label_free(Eon_Drawer_Widget *w, void *data)
{
	Eon_Basic_Drawer_Label *thiz = data;

	enesim_renderer_unref(thiz->text);
	free(thiz);
}

static Eon_Drawer_Label_Descriptor _descriptor = {
	/* .text_renderer_get	= */ _eon_basic_drawer_label_text_renderer_get,
	/* .text_color_set	= */ _eon_basic_drawer_label_text_color_set,
	/* .renderer_get 	= */ _eon_basic_drawer_label_renderer_get,
	/* .geometry_set 	= */ _eon_basic_drawer_label_geometry_set,
	/* .ender_populate	= */ NULL,
	/* .ender_process	= */ NULL,
	/* .free 		= */ _eon_basic_drawer_label_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Drawer_Widget * eon_basic_drawer_label_new(void)
{
	Eon_Basic_Drawer_Label *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Drawer_Label));
	thiz->text = enesim_renderer_text_span_new();

	return eon_drawer_label_new(&_descriptor, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
