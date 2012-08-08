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

#include "eon_theme_widget.h"
#include "eon_theme_container.h"

#include "theme_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Theme_Basic_Layout
{
	Enesim_Renderer *background;
	Enesim_Renderer *clipper;
	Enesim_Renderer *compound;
} Theme_Basic_Layout;

static inline Theme_Basic_Layout * _layout_get(Eon_Theme_Widget *t)
{
	Theme_Basic_Layout *thiz;

	thiz = eon_theme_container_data_get(t);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                      The Eon's layout theme interface                      *
 *----------------------------------------------------------------------------*/
static void _layout_child_add(Eon_Theme_Widget *t, Enesim_Renderer *rchild)
{
	Theme_Basic_Layout *thiz;
	thiz = _layout_get(t);

	enesim_renderer_compound_layer_add(thiz->compound, rchild);
}

static void _layout_child_remove(Eon_Theme_Widget *t, Enesim_Renderer *rchild)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	enesim_renderer_compound_layer_remove(thiz->compound, rchild);
}

static Enesim_Renderer * _layout_renderer_get(Eon_Theme_Widget *t)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	return thiz->clipper;
}

static void _layout_x_set(Eon_Theme_Widget *t, double x)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	enesim_renderer_x_origin_set(thiz->clipper, x);
}

static void _layout_y_set(Eon_Theme_Widget *t, double y)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	enesim_renderer_y_origin_set(thiz->clipper, y);
}

static void _layout_width_set(Eon_Theme_Widget *t, double width)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	enesim_renderer_clipper_width_set(thiz->clipper, width);
}

static void _layout_height_set(Eon_Theme_Widget *t, double height)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	enesim_renderer_clipper_height_set(thiz->clipper, height);
}

static void _layout_free(Eon_Theme_Widget *t)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	if (thiz->clipper)
		enesim_renderer_unref(thiz->clipper);
	if (thiz->background)
		enesim_renderer_unref(thiz->background);
	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	free(thiz);
}

static Eon_Theme_Container_Descriptor _descriptor = {
	/* .renderer_get 	= */ _layout_renderer_get,
	/* .x_set 		= */ _layout_x_set,
	/* .y_set		= */ _layout_y_set,
	/* .width_set 		= */ _layout_width_set,
	/* .height_set 		= */ _layout_height_set,
	/* .free 		= */ _layout_free,
	/* .child_add 		= */ _layout_child_add,
	/* .child_remove 	= */ _layout_child_remove,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_basic_layout_new(void)
{
	Eon_Theme_Widget *t;
	Theme_Basic_Layout *thiz;
	Enesim_Renderer *r;
#if 0
	const int color[] = { 0xffffffff, 0xff00ff00, 0x88008888 };
	static int i = 0;
#endif

	thiz = calloc(1, sizeof(Theme_Basic_Layout));
	if (!thiz) return NULL;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_rop_set(r, ENESIM_FILL);
#if 0
	enesim_renderer_background_color_set(r, color[i++ % (sizeof(color) / sizeof(int))]);
#else
	enesim_renderer_background_color_set(r, 0xffd7d7d7);
#endif
	thiz->background = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_compound_layer_add(r, thiz->background);
	thiz->compound = r;

	r = enesim_renderer_clipper_new();
	if (!r) goto clipper_err;
	enesim_renderer_clipper_content_set(r, thiz->compound);
	thiz->clipper = r;


	t = eon_theme_container_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	enesim_renderer_unref(thiz->clipper);
clipper_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}

EAPI void eon_basic_layout_background_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(t);
	enesim_renderer_background_color_set(thiz->background, color);
}
