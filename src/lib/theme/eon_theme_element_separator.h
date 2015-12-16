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

#ifndef _EON_THEME_ELEMENT_SEPARATOR_H_
#define _EON_THEME_ELEMENT_SEPARATOR_H_

/**
 * @{
 */

#define EON_THEME_ELEMENT_SEPARATOR_VERSION 0

typedef int (*Eon_Theme_Element_Separator_Descriptor_Thickness_Get_Cb)(void *data);

typedef struct _Eon_Theme_Element_Separator_Descriptor {
	Eon_Theme_Element_Version_Get_Cb version_get;
	Eon_Theme_Element_Ctor_Cb ctor;
	Eon_Theme_Element_Dtor_Cb dtor;
	Eon_Theme_Element_Tag_Name_Get_Cb tag_name_get;
	Eon_Theme_Element_Process_Cb process;
	Eon_Theme_Renderable_Descriptor_Renderer_Get_Cb renderer_get;
	Eon_Theme_Element_Separator_Descriptor_Thickness_Get_Cb thickness_get;
} Eon_Theme_Element_Separator_Descriptor;

EAPI Egueb_Dom_Node * eon_theme_element_separator_new(
		const Eon_Theme_Element_Separator_Descriptor *d, void *data);
EAPI Eon_Orientation eon_theme_element_separator_orientation_get(Egueb_Dom_Node *n);

/**
 * @}
 */
#endif

