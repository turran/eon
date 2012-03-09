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
#include "Eon_Theme.h"
#include "Eon_Basic.h"
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Theme_Basic_Layout
{
	Enesim_Renderer *background;
	Enesim_Renderer *clipper;
	Enesim_Renderer *compound;
} Theme_Basic_Layout;

static inline Theme_Basic_Layout * _layout_get(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = eon_theme_layout_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                      The Eon's layout theme interface                      *
 *----------------------------------------------------------------------------*/
static void _layout_child_add(Enesim_Renderer *r, Enesim_Renderer *rchild)
{
	Theme_Basic_Layout *thiz;
	thiz = _layout_get(r);

	enesim_renderer_compound_layer_add(thiz->compound, rchild);
}

static void _layout_child_remove(Enesim_Renderer *r, Enesim_Renderer *rchild)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	enesim_renderer_compound_layer_remove(thiz->compound, rchild);
}

static void _layout_child_clear(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	enesim_renderer_compound_layer_clear(thiz->compound);
	enesim_renderer_compound_layer_add(thiz->compound, thiz->background);
}

static Enesim_Renderer * _layout_renderer_get(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	return thiz->clipper;
}


static Eina_Bool _layout_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Theme_Basic_Layout *thiz;
	double ox;
	double oy;
	double width;
	double height;

	thiz = _layout_get(r);

	/* setup common properties */
	eon_theme_widget_x_get(r, &ox);
	eon_theme_widget_y_get(r, &oy);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	enesim_renderer_origin_set(thiz->clipper, ox, oy);
	enesim_renderer_clipper_width_set(thiz->clipper, width);
	enesim_renderer_clipper_height_set(thiz->clipper, height);

	return EINA_TRUE;
}

static void _layout_free(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	if (thiz->clipper)
		enesim_renderer_unref(thiz->clipper);
	if (thiz->background)
		enesim_renderer_unref(thiz->background);
	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	free(thiz);
}

static Eon_Theme_Layout_Descriptor _descriptor = {
	.child_add = _layout_child_add,
	.child_remove = _layout_child_remove,
	.child_clear = _layout_child_clear,
	.renderer_get = _layout_renderer_get,
	.setup = _layout_setup,
	.free = _layout_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_layout_new(void)
{
	Enesim_Renderer *r;
	Theme_Basic_Layout *thiz;
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


	r = eon_theme_layout_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->clipper);
clipper_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	enesim_renderer_unref(thiz->background);
background_err:
	free(thiz);
	return NULL;
}

EAPI void eon_basic_layout_background_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	enesim_renderer_background_color_set(thiz->background, color);
}
