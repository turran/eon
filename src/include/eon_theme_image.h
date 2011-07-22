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
typedef struct _Eon_Theme_Image_Descriptor
{
	/* element needed functions */
	Eon_Theme_Element_Max_Width_Get max_width_get;
	Eon_Theme_Element_Min_Width_Get min_width_get;
	Eon_Theme_Element_Max_Height_Get max_height_get;
	Eon_Theme_Element_Min_Height_Get min_height_get;
	Eon_Theme_Element_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Element_Preferred_Height_Get preferred_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Image_Descriptor;

EAPI Enesim_Renderer * eon_theme_image_new(Eon_Theme_Image_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_image(Enesim_Renderer *r);
EAPI void * eon_theme_image_data_get(Enesim_Renderer *r);

/**
 * @}
 */

#endif
