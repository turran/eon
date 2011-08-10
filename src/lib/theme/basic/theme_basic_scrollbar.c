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
#include <math.h>
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Scrollbar
{
	/* properties */
	Enesim_Renderer *compound;
	Enesim_Renderer *bar;
	Enesim_Renderer *background;
	/* private */
	Enesim_Renderer_Sw_Fill fill;
} Scrollbar;

static inline Scrollbar * _scrollbar_get(Enesim_Renderer *r)
{
	Scrollbar *thiz;

	thiz = eon_theme_scrollbar_data_get(r);
	return thiz;
}

static void _draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Scrollbar *thiz;

	thiz = _scrollbar_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}

/*----------------------------------------------------------------------------*
 *                   The Eon's theme scrollbar interface                    *
 *----------------------------------------------------------------------------*/
static double _scrollbar_min_width_get(Enesim_Renderer *r)
{
	return 16;
}

static double _scrollbar_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _scrollbar_min_height_get(Enesim_Renderer *r)
{
	return 16;
}

static double _scrollbar_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eina_Bool _setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Scrollbar *thiz;
	double ox, oy;

	thiz = _scrollbar_get(r);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	enesim_renderer_rectangle_width_set(thiz->bar, 22);
	enesim_renderer_rectangle_height_set(thiz->bar, 22);
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("failed 1\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _draw;

	return EINA_TRUE;
}

static void _cleanup(Enesim_Renderer *r)
{
	Scrollbar *thiz;

	thiz = _scrollbar_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _free(Enesim_Renderer *r)
{
	Scrollbar *thiz;

	thiz = _scrollbar_get(r);

	thiz = enesim_renderer_data_get(r);

	if (thiz->bar)
		enesim_renderer_delete(thiz->bar);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
}

static Eon_Theme_Scrollbar_Descriptor _descriptor = {
	.max_width_get = _scrollbar_max_width_get,
	.max_height_get = _scrollbar_max_height_get,
	.min_width_get = _scrollbar_min_width_get,
	.min_height_get = _scrollbar_min_height_get,
	.sw_setup = _setup,
	.sw_cleanup = _cleanup,
	.free = _free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
Enesim_Renderer * eon_basic_scrollbar_new(void)
{
	Scrollbar *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Scrollbar));
	if (!thiz) return NULL;

	r = enesim_renderer_rectangle_new();
	if (!r) goto bar_err;
	thiz->bar = r;

	r = enesim_renderer_checker_new();
	if (!r) goto background_err;
	thiz->background = r;
	enesim_renderer_checker_width_set(r, 4);
	enesim_renderer_checker_height_set(r, 4);
	enesim_renderer_checker_even_color_set(r, 0xaaaaaaaa);
	enesim_renderer_checker_odd_color_set(r, 0xaa000000);

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->background);
	enesim_renderer_compound_layer_add(r, thiz->bar);

	r = eon_theme_scrollbar_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	enesim_renderer_delete(thiz->background);
background_err:
	enesim_renderer_delete(thiz->bar);
bar_err:
	free(thiz);
	return NULL;
}
