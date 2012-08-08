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
#include "eon_theme_image.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Basic_Image
{
	/* properties */
	/* private */
	Enesim_Surface *src;
	Enesim_Renderer *image;
} Eon_Basic_Image;

static inline Eon_Basic_Image * _image_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Image *thiz;

	thiz = eon_theme_image_data_get(t);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon_Basic_Image theme interface                 *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _basic_image_renderer_get(Eon_Theme_Widget *t)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	return thiz->image;
}

static void _basic_image_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	enesim_renderer_image_x_set(thiz->image, x);
}

static void _basic_image_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	enesim_renderer_image_y_set(thiz->image, y);
}

static void _basic_image_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	enesim_renderer_image_width_set(thiz->image, width);
}

static void _basic_image_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	enesim_renderer_image_height_set(thiz->image, height);
}

static void _basic_image_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	enesim_renderer_unref(thiz->image);
	free(thiz);
}

static void _basic_image_surface_set(Eon_Theme_Widget *t, Enesim_Surface *src)
{
	Eon_Basic_Image *thiz;

	thiz = _image_get(t);
	printf("setting image surface!! %p\n", src);
	if (thiz->src)
	{
		enesim_surface_unref(thiz->src);
		thiz->src = NULL;
	}
	if (src)
	{
		thiz->src = enesim_surface_ref(src);
		enesim_renderer_image_src_set(thiz->image, src);
	}
}

static Eon_Theme_Image_Descriptor _descriptor = {
	/* .renderer_get 	= */ _basic_image_renderer_get,
	/* .x_set 		= */ _basic_image_x_set,
	/* .y_set 		= */ _basic_image_y_set,
	/* .width_set 		= */ _basic_image_width_set,
	/* .height_set		= */ _basic_image_height_set,
	/* .free		= */ _basic_image_free,
	/* .max_size_get 	= */ NULL,
	/* .min_size_get 	= */ NULL,
	/* .preferred_size_get 	= */ NULL,
	/* .surface_set 	= */ _basic_image_surface_set,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_basic_image_new(void)
{
	Eon_Basic_Image *thiz;
	Eon_Theme_Widget *t;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Basic_Image));

	r = enesim_renderer_image_new();
	if (!r) goto image_err;
	enesim_renderer_rop_set(thiz->image, ENESIM_BLEND);
	thiz->image = r;

	t = eon_theme_image_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	enesim_renderer_unref(thiz->image);
image_err:
	free(thiz);
	return NULL;
}
