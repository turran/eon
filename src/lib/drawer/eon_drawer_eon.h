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

#ifndef _EON_DRAWER_EON_H_
#define _EON_DRAWER_EON_H_

/**
 * @{
 */

typedef Enesim_Renderer * (*Eon_Drawer_Eon_Descriptor_Background_Renderer_Get)(Eon_Drawer_Widget *w, void *data);

typedef struct _Eon_Drawer_Eon_Descriptor
{
	Eon_Drawer_Eon_Descriptor_Background_Renderer_Get background_renderer_get;
	/* widget descriptor */
	Eon_Drawer_Widget_Descriptor_Eon_Drawer_Populate ender_populate;
	Eon_Drawer_Widget_Descriptor_Eon_Drawer_Process ender_process;
	Eon_Drawer_Widget_Descriptor_Free free;
} Eon_Drawer_Eon_Descriptor;

EAPI Eon_Drawer_Widget * eon_drawer_eon_new(
		const Eon_Drawer_Eon_Descriptor *d, void *data);
EAPI void * eon_drawer_eon_data_get(Eon_Drawer_Widget *w);
EAPI void eon_drawer_eon_ender_register(Eon_Drawer_Namespace *ns,
		Eon_Drawer_Instance_Descriptor_Ctor ctor);

/**
 * @}
 */
#endif

