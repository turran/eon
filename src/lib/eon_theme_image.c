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
#include "eon_private.h"
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

typedef struct _Eon_Theme_Image
{
	EINA_MAGIC;
	/* properties */
	Enesim_Surface *source;
	/* private */
	Eon_Theme_Image_Max_Width_Get max_width_get;
	Eon_Theme_Image_Min_Width_Get min_width_get;
	Eon_Theme_Image_Max_Height_Get max_height_get;
	Eon_Theme_Image_Min_Height_Get min_height_get;
	Eon_Theme_Image_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Image_Preferred_Height_Get preferred_height_get;
	Eina_Bool source_changed;
	int original_width;
	int original_height;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Image;

static inline Eon_Theme_Image * _eon_theme_image_get(Enesim_Renderer *r)
{
	Eon_Theme_Image *thiz;

	thiz = eon_theme_widget_data_get(r);
	EON_THEME_IMAGE_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_image_free(Enesim_Renderer *r)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	free(thiz);
}

static inline void _eon_theme_image_original_size_get(Eon_Theme_Image *thiz)
{
	if (!thiz->source_changed)
		return;
	if (thiz->source)
		enesim_surface_size_get(thiz->source, &thiz->original_width, &thiz->original_height);
	else
	{
		thiz->original_width = -1;
		thiz->original_height = -1;
	}
	thiz->source_changed = EINA_FALSE;
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
EAPI Enesim_Renderer * eon_theme_image_new(Eon_Theme_Image_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Image *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Image));
	EINA_MAGIC_SET(thiz, EON_THEME_IMAGE_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

	thiz->max_width_get = descriptor->max_width_get;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->max_height_get = descriptor->max_height_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;

	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_image_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_image(Enesim_Renderer *r)
{
	Eon_Theme_Image *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_image_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_source_set(Enesim_Renderer *r, Enesim_Surface *source)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	thiz->source_changed = EINA_TRUE;
	thiz->source = source;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_source_get(Enesim_Renderer *r, Enesim_Surface **source)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	*source = thiz->source;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	if (thiz->max_width_get)
		*width = thiz->max_width_get(r);
	else
		*width = 2048;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	if (thiz->max_height_get)
		*height = thiz->max_height_get(r);
	else
		*height = 2048;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	if (thiz->min_width_get)
		*width = thiz->min_width_get(r);
	else
		*width = 1;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	if (thiz->min_height_get)
		*height = thiz->min_height_get(r);
	else
		*height = 1;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_preferred_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	if (thiz->preferred_height_get)
		*height = thiz->preferred_height_get(r);
	else
	{
		_eon_theme_image_original_size_get(thiz);
		*height = thiz->original_height;
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_image_preferred_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Image *thiz;

	thiz = _eon_theme_image_get(r);
	if (thiz->preferred_width_get)
		*width = thiz->preferred_width_get(r);
	else
	{
		_eon_theme_image_original_size_get(thiz);
		*width = thiz->original_width;
	}
}
