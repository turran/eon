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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Image
{
	/* properties */
	/* private */
	Enesim_Surface *src;
	Enesim_Renderer *image;
} Image;

static inline Image * _image_get(Enesim_Renderer *r)
{
	Image *thiz;

	thiz = eon_theme_image_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Image theme interface                         *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _image_renderer_get(Enesim_Renderer *r)
{
	Image *thiz;

	thiz = _image_get(r);
	return thiz->image;
}

static Enesim_Renderer * _image_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Image *thiz;
	double ox, oy;
	double width, height;
	Enesim_Color color;

	thiz = _image_get(r);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->image, ox, oy);
	enesim_renderer_color_get(r, &color);
	enesim_renderer_color_set(thiz->image, color);
	if (!thiz->src)
	{
		printf("empty image\n");
		return EINA_FALSE;
	}
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	enesim_renderer_image_x_set(thiz->image, 0);
	enesim_renderer_image_y_set(thiz->image, 0);
	enesim_renderer_image_w_set(thiz->image, (int)width);
	enesim_renderer_image_h_set(thiz->image, (int)height);
	enesim_renderer_image_src_set(thiz->image, thiz->src);

	return EINA_TRUE;
}

static void _image_free(Enesim_Renderer *r)
{
	Image *thiz;

	thiz = _image_get(r);
	enesim_renderer_unref(thiz->image);
	free(thiz);
}

static void _image_source_set(Enesim_Renderer *r, Enesim_Surface *src)
{
	Image *thiz;

	thiz = _image_get(r);
	if (thiz->src)
		enesim_surface_unref(thiz->src);
	thiz->src = src;
	if (thiz->src)
		thiz->src = enesim_surface_ref(thiz->src);
}

static Eon_Theme_Image_Descriptor _descriptor = {
	.renderer_get = _image_renderer_get,
	.setup = _image_setup,
	.free = _image_free,
	.source_set = _image_source_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_image_new(void)
{
	Enesim_Renderer *r;
	Image *thiz;

	thiz = calloc(1, sizeof(Image));
	if (!thiz) return NULL;

	r = enesim_renderer_image_new();
	if (!r) goto image_err;
	thiz->image = r;

	r = eon_theme_image_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->image);
image_err:
	free(thiz);
	return NULL;
}
