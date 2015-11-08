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

#ifndef _EON_THEME_ELEMENT_IMAGE_H_
#define _EON_THEME_ELEMENT_IMAGE_H_

/**
 * @{
 */

#define EON_THEME_ELEMENT_IMAGE_VERSION 0

/* TODO share the eon_theme_renderable definition */
typedef Enesim_Renderer * (*Eon_Theme_Element_Image_Descriptor_Renderer_Get_Cb)(void *data);
typedef void (*Eon_Theme_Element_Image_Descriptor_Surface_Set_Cb)(void *data, Enesim_Surface *s);
typedef struct _Eon_Theme_Element_Image_Descriptor {
	Eon_Theme_Element_Version_Get_Cb version_get;
	Eon_Theme_Element_Ctor_Cb ctor;
	Eon_Theme_Element_Dtor_Cb dtor;
	Eon_Theme_Element_Tag_Name_Get_Cb tag_name_get;
	Eon_Theme_Element_Process_Cb process;
	Eon_Theme_Element_Image_Descriptor_Renderer_Get_Cb renderer_get;
	Eon_Theme_Element_Image_Descriptor_Surface_Set_Cb surface_set;
} Eon_Theme_Element_Image_Descriptor;

EAPI Egueb_Dom_Node * eon_theme_element_image_new(
		const Eon_Theme_Element_Image_Descriptor *d, void *data);

/**
 * @}
 */
#endif

