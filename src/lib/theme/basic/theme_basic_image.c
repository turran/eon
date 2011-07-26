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
	Enesim_Renderer *image;
	Enesim_Renderer_Sw_Fill fill;
} Image;

static inline Image * _image_get(Enesim_Renderer *r)
{
	Image *thiz;

	thiz = eon_theme_image_data_get(r);
	return thiz;
}

static void _image_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Image *thiz;

	thiz = _image_get(r);
	thiz->fill(thiz->image, x, y, len, dst);
}

static void _empty_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	/* doing nothing */
}
/*----------------------------------------------------------------------------*
 *                         The Image theme interface                         *
 *----------------------------------------------------------------------------*/
static Eina_Bool _image_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Image *thiz;
	Enesim_Surface *s;
	double ox, oy;
	double width, height;

	thiz = _image_get(r);
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->image, ox, oy);

	eon_theme_image_source_get(r, &s);
	if (!s)
	{
		*fill = _empty_draw;
		return EINA_TRUE;
	}
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);
	printf("the image size is %g %g\n", width, height);
	enesim_renderer_image_x_set(thiz->image, 0);
	enesim_renderer_image_y_set(thiz->image, 0);
	enesim_renderer_image_w_set(thiz->image, (int)width);
	enesim_renderer_image_h_set(thiz->image, (int)height);
	enesim_renderer_image_src_set(thiz->image, s);

	if (!enesim_renderer_sw_setup(thiz->image))
	{
		printf("not available to setup yet\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->image);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _image_draw;

	return EINA_TRUE;
}

static void _image_cleanup(Enesim_Renderer *r)
{
	Image *thiz;

	thiz = _image_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->image);
}

static void _image_free(Enesim_Renderer *r)
{
	Image *thiz;

	thiz = _image_get(r);
	if (thiz->image)
		enesim_renderer_delete(thiz->image);
	free(thiz);
}

static Eon_Theme_Image_Descriptor _descriptor = {
	.sw_setup = _image_setup,
	.sw_cleanup = _image_cleanup,
	.free = _image_free,
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
	enesim_renderer_delete(thiz->image);
image_err:
	free(thiz);
	return NULL;
}
