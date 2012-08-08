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
#include "eon_theme_bin.h"
#include "eon_theme_image.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_IMAGE_MAGIC 0xe0410004
#define EON_THEME_IMAGE_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_IMAGE_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_IMAGE_MAGIC);\
	} while(0)

#define EON_THEME_IMAGE_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_IMAGE_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_IMAGE_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Image_Descriptor_Internal
{
	Eon_Theme_Image_Max_Size_Get max_size_get;
	Eon_Theme_Image_Min_Size_Get min_size_get;
	Eon_Theme_Image_Preferred_Size_Get preferred_size_get;
	Eon_Theme_Image_Surface_Set surface_set;
	Eon_Theme_Widget_Free free;
} Eon_Theme_Image_Descriptor_Internal;

typedef struct _Eon_Theme_Image
{
	EINA_MAGIC;
	/* properties */
	/* private */
	int original_width;
	int original_height;

	Eon_Theme_Image_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Image;

static inline Eon_Theme_Image * _eon_theme_image_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Image *thiz;

	thiz = eon_theme_widget_data_get(t);
	EON_THEME_IMAGE_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_image_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_theme_image_new(Eon_Theme_Image_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Image *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Eon_Theme_Widget *t;

	thiz = calloc(1, sizeof(Eon_Theme_Image));
	EINA_MAGIC_SET(thiz, EON_THEME_IMAGE_MAGIC);
	thiz->data = data;

	thiz->descriptor.min_size_get = descriptor->min_size_get;
	thiz->descriptor.max_size_get = descriptor->max_size_get;
	thiz->descriptor.preferred_size_get = descriptor->preferred_size_get;
	thiz->descriptor.surface_set = descriptor->surface_set;
	thiz->descriptor.free = descriptor->free;

	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_image_free;

	t = eon_theme_widget_new(&pdescriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_image_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(t);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_surface_set(Eon_Theme_Widget *t, Enesim_Surface *surface)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(t);
	printf("theme setting surface\n");
	if (thiz->descriptor.surface_set)
		thiz->descriptor.surface_set(t, surface);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_max_size_get(Eon_Theme_Widget *t, Eon_Size *image_size, Eon_Size *out)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(t);
	if (thiz->descriptor.max_size_get)
		thiz->descriptor.max_size_get(t, image_size, out);
	else
		*out = *image_size;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_min_size_get(Eon_Theme_Widget *t, Eon_Size *image_size, Eon_Size *out)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(t);
	if (thiz->descriptor.min_size_get)
		thiz->descriptor.min_size_get(t, image_size, out);
	else
		*out = *image_size;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_preferred_size_get(Eon_Theme_Widget *t, Eon_Size *image_size, Eon_Size *out)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(t);
	if (thiz->descriptor.preferred_size_get)
		thiz->descriptor.preferred_size_get(t, image_size, out);
	else
		*out = *image_size;
}
