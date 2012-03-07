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

#ifndef _EON_THEME_SCROLLBAR_H_
#define _EON_THEME_SCROLLBAR_H_

/**
 * @{
 */
typedef double (*Eon_Theme_Scrollbar_Max_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Max_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Min_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Min_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Preferred_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Preferred_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Thumb_Max_Size_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Scrollbar_Thumb_Min_Size_Get)(Enesim_Renderer *r);
typedef void (*Eon_Theme_Scrollbar_Thumb_Percent_Set)(Enesim_Renderer *r, double percent);
typedef void (*Eon_Theme_Scrollbar_Thumb_Size_Set)(Enesim_Renderer *r, double size);
typedef void (*Eon_Theme_Scrollbar_Decrement_Arrow_Geometry_Get)(Enesim_Renderer *r, Enesim_Rectangle *geometry);
typedef void (*Eon_Theme_Scrollbar_Increment_Arrow_Geometry_Get)(Enesim_Renderer *r, Enesim_Rectangle *geometry);
typedef void (*Eon_Theme_Scrollbar_Thumb_Geometry_Get)(Enesim_Renderer *r, Enesim_Rectangle *geometry);

typedef struct _Eon_Theme_Scrollbar_Descriptor
{
	Eon_Theme_Scrollbar_Max_Width_Get max_width_get;
	Eon_Theme_Scrollbar_Min_Width_Get min_width_get;
	Eon_Theme_Scrollbar_Max_Height_Get max_height_get;
	Eon_Theme_Scrollbar_Min_Height_Get min_height_get;
	Eon_Theme_Scrollbar_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Scrollbar_Preferred_Height_Get preferred_height_get;
	Eon_Theme_Scrollbar_Thumb_Max_Size_Get thumb_max_size_get;
	Eon_Theme_Scrollbar_Thumb_Min_Size_Get thumb_min_size_get;
	Eon_Theme_Scrollbar_Thumb_Size_Set thumb_size_set;
	Eon_Theme_Scrollbar_Thumb_Percent_Set thumb_percent_set;
	Eon_Theme_Scrollbar_Thumb_Geometry_Get thumb_geometry_get;
	Eon_Theme_Scrollbar_Decrement_Arrow_Geometry_Get decrement_arrow_geometry_get;;
	Eon_Theme_Scrollbar_Increment_Arrow_Geometry_Get increment_arrow_geometry_get;
	/* renderer needed functions */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Eon_Theme_Widget_Cleanup cleanup;
	Eon_Theme_Widget_Needs_Setup needs_setup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Scrollbar_Descriptor;

EAPI Enesim_Renderer * eon_theme_scrollbar_new(Eon_Theme_Scrollbar_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_scrollbar(Enesim_Renderer *r);
EAPI void * eon_theme_scrollbar_data_get(Enesim_Renderer *r);

EAPI void eon_theme_scrollbar_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_scrollbar_min_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_scrollbar_max_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_scrollbar_max_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_scrollbar_preferred_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_scrollbar_preferred_height_get(Enesim_Renderer *r, double *height);

EAPI void eon_theme_scrollbar_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation);
EAPI void eon_theme_scrollbar_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation);

EAPI void eon_theme_scrollbar_thumb_min_size_get(Enesim_Renderer *r, double *size);
EAPI void eon_theme_scrollbar_thumb_max_size_get(Enesim_Renderer *r, double *size);

EAPI void eon_theme_scrollbar_thumb_size_set(Enesim_Renderer *r, double size);
EAPI void eon_theme_scrollbar_thumb_size_get(Enesim_Renderer *r, double *size);
EAPI void eon_theme_scrollbar_thumb_percent_set(Enesim_Renderer *r, double percent);
EAPI void eon_theme_scrollbar_thumb_percent_get(Enesim_Renderer *r, double *percent);

/**
 * @}
 */


#endif
