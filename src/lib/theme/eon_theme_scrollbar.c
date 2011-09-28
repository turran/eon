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
#define EON_THEME_SCROLLBAR_MAGIC 0xe0410003
#define EON_THEME_SCROLLBAR_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_SCROLLBAR_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_SCROLLBAR_MAGIC);\
	} while(0)

#define EON_THEME_SCROLLBAR_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_SCROLLBAR_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_SCROLLBAR_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Scrollbar
{
	EINA_MAGIC;
	/* properties */
	double percent;
	double size;
	/* private */
	Eon_Orientation orientation;
	Eon_Theme_Scrollbar_Max_Width_Get max_width_get;
	Eon_Theme_Scrollbar_Min_Width_Get min_width_get;
	Eon_Theme_Scrollbar_Max_Height_Get max_height_get;
	Eon_Theme_Scrollbar_Min_Height_Get min_height_get;
	Eon_Theme_Scrollbar_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Scrollbar_Preferred_Height_Get preferred_height_get;
	Eon_Theme_Scrollbar_Thumb_Max_Size_Get thumb_max_size_get;
	Eon_Theme_Scrollbar_Thumb_Min_Size_Get thumb_min_size_get;
	Eon_Theme_Scrollbar_Thumb_Percent_Set thumb_percent_set;
	Eon_Theme_Scrollbar_Thumb_Size_Set thumb_size_set;
	Eon_Theme_Scrollbar_Thumb_Geometry_Get thumb_geometry_get;
	Eon_Theme_Scrollbar_Decrement_Arrow_Geometry_Get decrement_arrow_geometry_get;;
	Eon_Theme_Scrollbar_Increment_Arrow_Geometry_Get increment_arrow_geometry_get;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Scrollbar;

static inline Eon_Theme_Scrollbar * _eon_theme_scrollbar_get(Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = eon_theme_widget_data_get(r);
	EON_THEME_SCROLLBAR_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_scrollbar_free(Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_scrollbar_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->min_width_get)
		*width = thiz->min_width_get(r);
	else
		*width = 1;
}

void eon_theme_scrollbar_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Scrollbar *thiz;

	if (!height) return;
	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->min_height_get)
		*height = thiz->min_height_get(r);
	else
		*height = 1;
}

void eon_theme_scrollbar_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->max_width_get)
		*width = thiz->max_width_get(r);
	else
		*width = DBL_MAX;
}

void eon_theme_scrollbar_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Scrollbar *thiz;

	if (!height) return;
	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->max_height_get)
		*height = thiz->max_height_get(r);
	else
		*height = DBL_MAX;
}

void eon_theme_scrollbar_preferred_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->preferred_width_get)
		*width = thiz->preferred_width_get(r);
	else
		*width = -1;
}

void eon_theme_scrollbar_preferred_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Scrollbar *thiz;

	if (!height) return;
	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->preferred_height_get)
		*height = thiz->preferred_height_get(r);
	else
		*height = -1;
}

void eon_theme_scrollbar_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	thiz->orientation = orientation;
}

void eon_theme_scrollbar_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	*orientation = thiz->orientation;
}

void eon_theme_scrollbar_thumb_min_size_get(Enesim_Renderer *r, double *size)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->thumb_min_size_get)
		*size = thiz->thumb_min_size_get(r);
	else
		*size = 1;
}

void eon_theme_scrollbar_thumb_max_size_get(Enesim_Renderer *r, double *size)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->thumb_max_size_get)
		*size = thiz->thumb_max_size_get(r);
	else
		*size = DBL_MAX;

}

void eon_theme_scrollbar_thumb_size_set(Enesim_Renderer *r, double size)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	thiz->size = size;
	if (thiz->thumb_size_set)
		thiz->thumb_size_set(r, size);
}

void eon_theme_scrollbar_thumb_size_get(Enesim_Renderer *r, double *size)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	*size = thiz->size;
}

void eon_theme_scrollbar_thumb_percent_set(Enesim_Renderer *r, double percent)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	thiz->percent = percent;
	if (thiz->thumb_percent_set)
		thiz->thumb_percent_set(r, percent);
}

void eon_theme_scrollbar_thumb_percent_get(Enesim_Renderer *r, double *percent)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	*percent = thiz->percent;
}

void eon_theme_scrollbar_decrement_arrow_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->decrement_arrow_geometry_get)
		thiz->decrement_arrow_geometry_get(r, geometry);
	else
	{
		geometry->x = 0;
		geometry->y = 0;
		geometry->w = 0;
		geometry->h = 0;
	}
}

void eon_theme_scrollbar_increment_arrow_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->increment_arrow_geometry_get)
		thiz->increment_arrow_geometry_get(r, geometry);
	else
	{
		geometry->x = 0;
		geometry->y = 0;
		geometry->w = 0;
		geometry->h = 0;
	}
}

void eon_theme_scrollbar_thumb_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	if (!thiz) return;
	if (thiz->thumb_geometry_get)
		thiz->thumb_geometry_get(r, geometry);
	else
	{
		geometry->x = 0;
		geometry->y = 0;
		geometry->w = 0;
		geometry->h = 0;
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_scrollbar_new(Eon_Theme_Scrollbar_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Scrollbar *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Scrollbar));
	EINA_MAGIC_SET(thiz, EON_THEME_SCROLLBAR_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;

	thiz->max_width_get = descriptor->max_width_get;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->max_height_get = descriptor->max_height_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;
	thiz->thumb_max_size_get = descriptor->thumb_max_size_get;
	thiz->thumb_min_size_get = descriptor->thumb_min_size_get;
	thiz->thumb_percent_set = descriptor->thumb_percent_set;
	thiz->thumb_size_set = descriptor->thumb_size_set;
	thiz->thumb_geometry_get = descriptor->thumb_geometry_get;
	thiz->decrement_arrow_geometry_get = descriptor->decrement_arrow_geometry_get;
	thiz->increment_arrow_geometry_get = descriptor->increment_arrow_geometry_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_scrollbar_free;

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
EAPI Eina_Bool eon_is_theme_scrollbar(Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_scrollbar_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Scrollbar *thiz;

	thiz = _eon_theme_scrollbar_get(r);
	return thiz->data;
}


