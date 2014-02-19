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

#ifndef _EON_DRAWER_LABEL_H_
#define _EON_DRAWER_LABEL_H_

/**
 * @{
 */

/*
 * TODO add functions to:
 * 1. Set the font size from the control
 * 2. Set the font color from the control
 * 3. Set the font size from the control
 * At the end we need all of the css font attributes
 * Set the geometry
 * Get the text font?
 */

typedef void (*Eon_Drawer_Label_Descriptor_Text_Buffer_Set)(Eon_Drawer_Widget *t, void *data, Enesim_Text_Buffer *tf);
typedef void (*Eon_Drawer_Label_Descriptor_Text_Font_Set)(Eon_Drawer_Widget *t, void *data, Enesim_Text_Font *tf);
typedef void (*Eon_Drawer_Label_Descriptor_Text_Color_Set)(Eon_Drawer_Widget *t, void *data, Enesim_Color color);

typedef struct _Eon_Drawer_Label_Descriptor
{
	Eon_Drawer_Label_Descriptor_Text_Buffer_Set text_buffer_set;
	Eon_Drawer_Label_Descriptor_Text_Font_Set text_font_set;
	Eon_Drawer_Label_Descriptor_Text_Color_Set text_color_set;
	/* widget descriptor */
	Eon_Drawer_Widget_Descriptor_Renderer_Get renderer_get;
	Eon_Drawer_Widget_Descriptor_Geometry_Set geometry_set;
	Eon_Drawer_Widget_Descriptor_Ender_Populate ender_populate;
	Eon_Drawer_Widget_Descriptor_Ender_Process ender_process;
	Eon_Drawer_Widget_Descriptor_Free free;
} Eon_Drawer_Label_Descriptor;

EAPI Eon_Drawer_Widget * eon_drawer_label_new(
		const Eon_Drawer_Label_Descriptor *d, void *data);
EAPI void * eon_drawer_label_data_get(Eon_Drawer_Widget *w);
EAPI void eon_drawer_label_ender_register(Ender_Namespace *ns,
		Ender_Instance_Descriptor_Ctor ctor);

/**
 * @}
 */
#endif


