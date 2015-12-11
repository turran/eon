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

#ifndef _EON_THEME_ELEMENT_ENTRY_H_
#define _EON_THEME_ELEMENT_ENTRY_H_

/**
 * @{
 * - A entry theme must define the size hints by itself.
 * - The text renderer from the element is passed in
 * - The position of such text renderer is set when
 *   setting the geometry of the theme element
 * - The theme element base class must have the font/color
 *   attributes. It must do the setup on the size hints
 *   and process calls
 * - The element must set the cursor style by default
 *   only when its inside the input area
 * - How to define the preferred size? It should be max
 *   number of chars available * the max width of a glyph
 *   (xMax_Extent) for that we need the font, so we can
 *   query it.
 *  - Make the base class handle the size hints and the theme
 *    add the padding
 *  - Make the theme element request the area of the cursor change
 *    by asking the renderer geometry
 */

#define EON_THEME_ELEMENT_ENTRY_VERSION 0

typedef void (*Eon_Theme_Element_Entry_Descriptor_Padding_Get_Cb)(void *data, Eon_Box *padding);

typedef struct _Eon_Theme_Element_Entry_Descriptor {
	Eon_Theme_Element_Version_Get_Cb version_get;
	Eon_Theme_Element_Ctor_Cb ctor;
	Eon_Theme_Element_Dtor_Cb dtor;
	Eon_Theme_Element_Tag_Name_Get_Cb tag_name_get;
	Eon_Theme_Element_Process_Cb process;
	Eon_Theme_Renderable_Descriptor_Renderer_Get_Cb renderer_get;
	Eon_Theme_Element_Entry_Descriptor_Padding_Get_Cb padding_get;
} Eon_Theme_Element_Entry_Descriptor;

EAPI Egueb_Dom_Node * eon_theme_element_entry_new(
		const Eon_Theme_Element_Entry_Descriptor *d, void *data);
EAPI void * eon_theme_element_entry_data_get(Egueb_Dom_Node *n);
EAPI Enesim_Renderer * eon_theme_element_entry_text_renderer_get(Egueb_Dom_Node *n);
EAPI int eon_theme_element_entry_cursor_get(Egueb_Dom_Node *n);

/**
 * @}
 */
#endif

