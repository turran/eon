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

#ifndef _EON_THEME_IMAGE_H_
#define _EON_THEME_IMAGE_H_

/**
 * @{
 */
typedef double (*Eon_Theme_Image_Source_Set)(Enesim_Renderer *r, Enesim_Surface *src);
typedef void (*Eon_Theme_Image_Min_Size_Get)(Enesim_Renderer *r, Eon_Size *image_size, Eon_Size *out);
typedef void (*Eon_Theme_Image_Max_Size_Get)(Enesim_Renderer *r, Eon_Size *image_size, Eon_Size *out);
typedef void (*Eon_Theme_Image_Preferred_Size_Get)(Enesim_Renderer *r, Eon_Size *image_size, Eon_Size *out);

typedef struct _Eon_Theme_Image_Descriptor
{
	/* element needed functions */
	Eon_Theme_Image_Max_Size_Get max_size_get;
	Eon_Theme_Image_Min_Size_Get min_size_get;
	Eon_Theme_Image_Preferred_Size_Get preferred_size_get;
	Eon_Theme_Image_Source_Set source_set;
	/* renderer needed functions */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Eon_Theme_Widget_Cleanup cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Image_Descriptor;

EAPI Enesim_Renderer * eon_theme_image_new(Eon_Theme_Image_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_image(Enesim_Renderer *r);
EAPI void * eon_theme_image_data_get(Enesim_Renderer *r);

EAPI void eon_theme_image_max_size_get(Enesim_Renderer *r, Eon_Size *image_size, Eon_Size *out);
EAPI void eon_theme_image_min_size_get(Enesim_Renderer *r, Eon_Size *image_size, Eon_Size *out);
EAPI void eon_theme_image_preferred_size_get(Enesim_Renderer *r, Eon_Size *image_size, Eon_Size *out);

/**
 * @}
 */

#endif
