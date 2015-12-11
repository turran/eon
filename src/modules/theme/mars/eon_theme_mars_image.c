/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
#include "eon_theme_mars_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Mars_Image
{
	Egueb_Dom_Node *n;
	/* attributes */
	/* private */
	Enesim_Renderer *pattern;
	Enesim_Renderer *proxy;
	Enesim_Renderer *rect;
	Enesim_Renderer *image;
} Eon_Theme_Mars_Image;
/*----------------------------------------------------------------------------*
 *                              Image interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_image_version_get(void)
{
	return EON_THEME_ELEMENT_IMAGE_VERSION;
}

static void _eon_theme_mars_image_dtor(void *data)
{
	Eon_Theme_Mars_Image *thiz = data;

	enesim_renderer_unref(thiz->rect);
	enesim_renderer_unref(thiz->image);
	enesim_renderer_unref(thiz->pattern);
	enesim_renderer_unref(thiz->proxy);
	free(thiz);
}

static const char * _eon_theme_mars_image_tag_name_get(void)
{
	return "image";
}

static Eina_Bool _eon_theme_mars_image_process(void *data)
{
	Eon_Theme_Mars_Image *thiz;
	Eina_Rectangle geom;
	Enesim_Rectangle bounds;

	thiz = data;
	/* get the inherited members */
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	/* set the position of the elements */
	enesim_renderer_image_position_set(thiz->image, geom.x, geom.y);
	enesim_renderer_image_size_set(thiz->image, geom.w, geom.h);
	enesim_renderer_rectangle_position_set(thiz->rect, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->rect, geom.w, geom.h);

	return EINA_TRUE;
}

static void _eon_theme_mars_image_surface_set(void *data, Enesim_Surface *s)
{
	Eon_Theme_Mars_Image *thiz = data;
	if (s)
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->image));
	else
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->pattern));
	enesim_renderer_image_source_surface_set(thiz->image, s);
}

static Enesim_Renderer * _eon_theme_mars_image_renderer_get(void *data)
{
	Eon_Theme_Mars_Image *thiz = data;
	return enesim_renderer_ref(thiz->rect);
}

static Eon_Theme_Element_Image_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_image_version_get,
	/* .ctor 		= */ eon_theme_mars_image_new,
	/* .dtor 		= */ _eon_theme_mars_image_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_image_tag_name_get,
	/* .process 		= */ _eon_theme_mars_image_process,
	/* .renderer_get	= */ _eon_theme_mars_image_renderer_get,
	/* .surface_set		= */ _eon_theme_mars_image_surface_set,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_image_new(void)
{
	Eon_Theme_Mars_Image *thiz;
	Egueb_Dom_Node *n;
	Enesim_Matrix m;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Image));
	thiz->pattern = enesim_renderer_stripes_new();
	enesim_renderer_stripes_even_thickness_set(thiz->pattern, EON_THEME_MARS_MARGIN);
	enesim_renderer_stripes_even_color_set(thiz->pattern, 0xffeeeeee);
	enesim_renderer_stripes_odd_thickness_set(thiz->pattern, EON_THEME_MARS_MARGIN);
	enesim_renderer_stripes_odd_color_set(thiz->pattern, 0xffcccccc);
	enesim_matrix_identity(&m);
	enesim_matrix_rotate(&m, 0.785);
	enesim_renderer_transformation_set(thiz->pattern, &m);

	thiz->proxy = enesim_renderer_proxy_new();
	enesim_renderer_proxy_proxied_set(thiz->proxy, enesim_renderer_ref(thiz->pattern));

	thiz->image = enesim_renderer_image_new();
	thiz->rect = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->rect, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_shape_fill_renderer_set(thiz->rect, enesim_renderer_ref(thiz->proxy));
	enesim_renderer_rectangle_corner_radii_set(thiz->rect, EON_THEME_MARS_MARGIN, EON_THEME_MARS_MARGIN);
	enesim_renderer_rectangle_corners_set(thiz->rect, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	
	n = eon_theme_element_image_new(&_descriptor, thiz);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


