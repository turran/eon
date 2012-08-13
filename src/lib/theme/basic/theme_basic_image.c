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
typedef enum _Eon_Basic_Image_State
{
	EON_BASIC_IMAGE_STATE_LOADING,
	EON_BASIC_IMAGE_STATE_LOADED,
	EON_BASIC_IMAGE_STATE_FAILED,
} Eon_Basic_Image_State;

typedef struct _Eon_Basic_Image
{
	/* properties */
	/* private */
	Enesim_Surface *old_src;
	Enesim_Surface *src;
	/* TODO we should have two surfaces, to handle the states correctly
	 * for example when in 'loading' we might want to blink the previous
	 * image. when 'loaded', just display, and on 'failed' put
	 * something else ...
	 */
	Enesim_Renderer *image;
	Enesim_Renderer *transition;
	Enesim_Renderer *proxy;
	/* in case there is no image set, use this one */
	Enesim_Renderer *rectangle;
	Enesim_Renderer *background;
} Eon_Basic_Image;

static inline Eon_Basic_Image * _basic_image_get(Eon_Theme_Widget *t)
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

	thiz = _basic_image_get(t);
	return thiz->proxy;
}

static void _basic_image_x_set(Eon_Theme_Widget *t, double x)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
	enesim_renderer_image_x_set(thiz->image, x);
	enesim_renderer_rectangle_x_set(thiz->rectangle, x);
	enesim_renderer_x_origin_set(thiz->background, x);
}

static void _basic_image_y_set(Eon_Theme_Widget *t, double y)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
	enesim_renderer_image_y_set(thiz->image, y);
	enesim_renderer_rectangle_y_set(thiz->rectangle, y);
	enesim_renderer_y_origin_set(thiz->background, y);
}

static void _basic_image_width_set(Eon_Theme_Widget *t, double width)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
	enesim_renderer_image_width_set(thiz->image, width);
	enesim_renderer_rectangle_width_set(thiz->rectangle, width);
}

static void _basic_image_height_set(Eon_Theme_Widget *t, double height)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
	enesim_renderer_image_height_set(thiz->image, height);
	enesim_renderer_rectangle_height_set(thiz->rectangle, height);
}

static void _basic_image_free(Eon_Theme_Widget *t)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
	enesim_renderer_unref(thiz->image);
	free(thiz);
}

static void _basic_image_surface_set(Eon_Theme_Widget *t, Enesim_Surface *src)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
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
	r = enesim_renderer_checker_new();
	if (!r) goto checker_err;
	enesim_renderer_rop_set(r, ENESIM_FILL);
	enesim_renderer_checker_odd_color_set(r, 0x55000000);
	enesim_renderer_checker_even_color_set(r, 0x55555555);
	enesim_renderer_checker_width_set(r, 10);
	enesim_renderer_checker_height_set(r, 10);
	thiz->background = r;

	r = enesim_renderer_rectangle_new();
	if (!r) goto rect_err;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_shape_fill_renderer_set(r, thiz->background);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	thiz->rectangle = r;

	r = enesim_renderer_proxy_new();
	if (!r) goto proxy_err;
	enesim_renderer_proxy_proxied_set(r, thiz->rectangle);
	thiz->proxy = r;

	r = enesim_renderer_image_new();
	if (!r) goto image_err;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->image = r;

	t = eon_theme_image_new(&_descriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	enesim_renderer_unref(thiz->image);
image_err:
	enesim_renderer_unref(thiz->proxy);
proxy_err:
	enesim_renderer_unref(thiz->rectangle);
rect_err:
	enesim_renderer_unref(thiz->background);
checker_err:
	free(thiz);
	return NULL;
}

EAPI void eon_basic_image_color_set(Eon_Theme_Widget *t, Enesim_Color color)
{
	Eon_Basic_Image *thiz;

	thiz = _basic_image_get(t);
	enesim_renderer_color_set(thiz->image, color);
}

EAPI void eon_basic_image_status_set(Eon_Theme_Widget *t, Eon_Basic_Image_State state)
{
	Eon_Basic_Image *thiz;

	/* when the state is loaded, we should inform enesim that the surface has changed the contents */
	/* when the state is failed, we should display something, like a pattern or something? */
	thiz = _basic_image_get(t);
	switch (state)
	{
		case EON_BASIC_IMAGE_STATE_LOADING:
		case EON_BASIC_IMAGE_STATE_FAILED:
		break;

		case EON_BASIC_IMAGE_STATE_LOADED:
		{
			Eina_Rectangle area;

			area.x = area.y = 0;
			enesim_surface_size_get(thiz->src, &area.w, &area.h);
			enesim_renderer_image_damage_add(thiz->image, &area);
			enesim_renderer_proxy_proxied_set(thiz->proxy, thiz->image);
		}
		break;
	}
}


