/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
	Enesim_Renderer *clipper;
	Enesim_Renderer *content;
	Enesim_Renderer_Sw_Fill fill;
} Theme_Basic_Scrollview;

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
	thiz->fill(thiz->clipper, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme widget interface                      *
 *----------------------------------------------------------------------------*/
static double _scrollview_min_width_get(Enesim_Renderer *r)
{
	return 0;
}

static double _scrollview_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _scrollview_min_height_get(Enesim_Renderer *r)
{
	return 0;
}

static double _scrollview_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eon_Theme_Widget_Descriptor _twdescriptor = {
	.max_width_get = _scrollview_max_width_get,
	.max_height_get = _scrollview_max_height_get,
	.min_width_get = _scrollview_min_width_get,
	.min_height_get = _scrollview_min_height_get,
};
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _scrollview_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	thiz->fill = enesim_renderer_sw_fill_get(thiz->clipper);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _scrollview_draw;

	return EINA_TRUE;
}

static void _scrollview_cleanup(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->clipper);
}

static void _scrollview_free(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	if (thiz->clipper)
		enesim_renderer_delete(thiz->clipper);
	free(thiz);
}

static void _scrollview_boundings(Enesim_Renderer *r, Enesim_Rectangle *boundings)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	enesim_renderer_boundings(thiz->clipper, boundings);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _scrollview_setup,
	.sw_cleanup = _scrollview_cleanup,
	.boundings = _scrollview_boundings,
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

	r = enesim_renderer_clipper_new();
	if (!r) goto clipper_err;
	thiz->clipper = r;

	r = eon_theme_scrollview_new(&_twdescriptor, &_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->clipper);
clipper_err:
	free(thiz);
	return NULL;
}
