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
#include "Eon_Basic.h"
#include "eon_basic_private.h"
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Theme_Basic_Scrollview
{
	/* properties */
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *background;
	Enesim_Renderer *hbar;
	Enesim_Renderer *vbar;
	Enesim_Renderer *content;
	Enesim_Renderer *clipper;
	Enesim_Renderer_Sw_Fill fill;
} Theme_Basic_Scrollview;

const static int scrollview_bar_width = 10;
const static int bar_radius = 5;

static inline Theme_Basic_Scrollview * _scrollview_get(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = eon_theme_scrollview_data_get(r);
	return thiz;
}

static void _scrollview_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                   The Eon's theme scrollview interface                     *
 *----------------------------------------------------------------------------*/
static double _scrollview_decoration_width_get(Enesim_Renderer *r)
{
	return scrollview_bar_width;
}

static double _scrollview_decoration_height_get(Enesim_Renderer *r)
{
	return scrollview_bar_width;
}

static void _scrollview_content_position_get(Enesim_Renderer *r, Enesim_Renderer *content,
		double *x, double *y)
{
	*x = 0;
	*y = 0;
}

static Eina_Bool _scrollview_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Theme_Basic_Scrollview *thiz;
	Enesim_Renderer *content;
	double ox, oy;
	double width, height;
	double c_width, c_height;

	thiz = _scrollview_get(r);

	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	/* the clipped width/height */
	//eon_theme_widget_width_get(r, &width);
	//eon_theme_widget_height_get(r, &height);
	//enesim_renderer_compound_width_set(thiz->compound, width);
	//enesim_renderer_compound_height_set(thiz->compound, height);

	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		printf("scrollview no content\n");
		return EINA_FALSE;
	}
	if (thiz->content != content)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->background);
		enesim_renderer_compound_layer_add(thiz->compound, content);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->hbar);
		enesim_renderer_compound_layer_add(thiz->compound, thiz->vbar);
		thiz->content = content;
	}
	/* set the needed properties */
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	eon_theme_widget_width_get(content, &c_width);
	eon_theme_widget_height_get(content, &c_height);

	enesim_renderer_rectangle_width_set(thiz->hbar, width - scrollview_bar_width);
	enesim_renderer_rectangle_height_set(thiz->hbar, scrollview_bar_width);
	enesim_renderer_origin_set(thiz->hbar, 0, height - scrollview_bar_width);

	enesim_renderer_rectangle_width_set(thiz->vbar, scrollview_bar_width);
	enesim_renderer_rectangle_height_set(thiz->vbar, height - scrollview_bar_width);
	enesim_renderer_origin_set(thiz->vbar, width - scrollview_bar_width, 0);

	printf("SC size %g %g\n", width, height);
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("the scrollview cannot setup yet\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill)
	{
		return EINA_FALSE;
	}

	*fill = _scrollview_draw;

	return EINA_TRUE;
}

static void _scrollview_cleanup(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _scrollview_free(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static Eon_Theme_Scrollview_Descriptor _descriptor = {
	.content_position_get = _scrollview_content_position_get,
	.decoration_width_get = _scrollview_decoration_width_get,
	.decoration_height_get = _scrollview_decoration_height_get,
	.sw_setup = _scrollview_setup,
	.sw_cleanup = _scrollview_cleanup,
	.free = _scrollview_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_scrollview_new(void)
{
	Enesim_Renderer *r;
	Theme_Basic_Scrollview *thiz;

	thiz = calloc(1, sizeof(Theme_Basic_Scrollview));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_background_color_set(r, 0xff777777);

	r = enesim_renderer_rectangle_new();
	if (!r) goto hbar_err;
	thiz->hbar = r;
	enesim_renderer_rectangle_corner_radius_set(r, bar_radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_color_set(r, 0xff222222);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	
	r = enesim_renderer_rectangle_new();
	if (!r) goto vbar_err;
	thiz->vbar = r;
	enesim_renderer_rectangle_corner_radius_set(r, bar_radius);
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_shape_fill_color_set(r, 0xff222222);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = eon_theme_scrollview_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->vbar);
vbar_err:
	enesim_renderer_delete(thiz->hbar);
hbar_err:
	enesim_renderer_delete(thiz->background);
background_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}
