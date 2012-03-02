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
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Theme_Basic_Scrollview
{
	/* properties */
	Eina_Bool over;
	double bar_width;
	double bar_radius;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *background;
	Enesim_Renderer *hbar;
	Enesim_Renderer *vbar;
	Enesim_Renderer *content;
	Enesim_Renderer *clipper;
} Theme_Basic_Scrollview;

static inline Theme_Basic_Scrollview * _scrollview_get(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = eon_theme_scrollview_data_get(r);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                   The Eon's theme scrollview interface                     *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _scrollview_renderer_get(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;
	thiz = _scrollview_get(r);

	return thiz->clipper;
}

static Eina_Bool _scrollview_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Theme_Basic_Scrollview *thiz;
	Eon_Position offset;
	Enesim_Renderer *content;
	double ox, oy;
	double width, height;

	thiz = _scrollview_get(r);

	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	eon_theme_scrollview_offset_get(r, &offset);
	eon_theme_container_content_get(r, &content);
	if (!content)
	{
		printf("scrollview no content\n");
		return EINA_FALSE;
	}
	//printf("offset = %g %g\n", offset.x, offset.y);
	enesim_renderer_origin_set(content, offset.x, offset.y);

	enesim_renderer_compound_layer_clear(thiz->compound);
	enesim_renderer_compound_layer_add(thiz->compound, thiz->background);

	/* the clipper */
	enesim_renderer_origin_set(thiz->clipper, ox, oy);
	enesim_renderer_clipper_width_set(thiz->clipper, width);
	enesim_renderer_clipper_height_set(thiz->clipper, height);
	//printf("boundings %g %g\n", width, height);

	enesim_renderer_compound_layer_add(thiz->compound, content);
	/* the bars */
	eon_theme_scrollview_hbar_get(r, &thiz->hbar);
	if (thiz->hbar)
	{
		enesim_renderer_compound_layer_add(thiz->compound, thiz->hbar);
		enesim_renderer_rop_set(thiz->hbar, ENESIM_BLEND);
	}
	eon_theme_scrollview_vbar_get(r, &thiz->vbar);
	if (thiz->vbar)
	{
		enesim_renderer_compound_layer_add(thiz->compound, thiz->vbar);
		enesim_renderer_rop_set(thiz->vbar, ENESIM_BLEND);
	}

	return EINA_TRUE;
}

static void _scrollview_free(Enesim_Renderer *r)
{
	Theme_Basic_Scrollview *thiz;

	thiz = _scrollview_get(r);
	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	free(thiz);
}

static Eon_Theme_Scrollview_Descriptor _descriptor = {
	.renderer_get = _scrollview_renderer_get,
	.setup = _scrollview_setup,
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
	thiz->bar_radius = 5;
	thiz->bar_width = 10;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_background_color_set(r, 0xffd7d7d7);
	thiz->background = r;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_compound_layer_add(r, thiz->background);
	thiz->compound = r;

	r = enesim_renderer_clipper_new();
	if (!r) goto clipper_err;
	thiz->clipper = r;
	enesim_renderer_clipper_content_set(thiz->clipper, thiz->compound);


	r = eon_theme_scrollview_new(&_descriptor, thiz);
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
