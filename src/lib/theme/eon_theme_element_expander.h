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

#ifndef _EON_THEME_ELEMENT_EXPANDER_H_
#define _EON_THEME_ELEMENT_EXPANDER_H_

/**
 * @{
 */

#define EON_THEME_ELEMENT_EXPANDER_VERSION 0

typedef void (*Eon_Theme_Element_Expander_Descriptor_Content_Set_Cb)(void *data, Enesim_Renderer *r, Eina_Rectangle *area);
typedef void (*Eon_Theme_Element_Expander_Descriptor_Padding_Get_Cb)(void *data, Eon_Box *padding);

typedef struct _Eon_Theme_Element_Expander_Descriptor {
	Eon_Theme_Element_Version_Get_Cb version_get;
	Eon_Theme_Element_Ctor_Cb ctor;
	Eon_Theme_Element_Dtor_Cb dtor;
	Eon_Theme_Element_Tag_Name_Get_Cb tag_name_get;
	Eon_Theme_Element_Process_Cb process;
	Eon_Theme_Renderable_Descriptor_Renderer_Get_Cb renderer_get;
	Eon_Theme_Element_Expander_Descriptor_Content_Set_Cb first_content_set;
	Eon_Theme_Element_Expander_Descriptor_Content_Set_Cb second_content_set;
	Eon_Theme_Element_Expander_Descriptor_Padding_Get_Cb first_padding_get;
	Eon_Theme_Element_Expander_Descriptor_Padding_Get_Cb second_padding_get;
} Eon_Theme_Element_Expander_Descriptor;

EAPI Egueb_Dom_Node * eon_theme_element_expander_new(
		const Eon_Theme_Element_Expander_Descriptor *d, void *data);
EAPI Eon_Orientation eon_theme_element_expander_orientation_get(Egueb_Dom_Node *n);

/**
 * @}
 */
#endif

