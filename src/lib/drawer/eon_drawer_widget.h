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

#ifndef _EON_DRAWER_WIDGET_H_
#define _EON_DRAWER_WIDGET_H_

/**
 * @{
 */
typedef struct _Eon_Drawer_Widget Eon_Drawer_Widget;

/* widget descriptor functions */
typedef Enesim_Renderer * (*Eon_Drawer_Widget_Descriptor_Renderer_Get)(Eon_Drawer_Widget *w, void *data); 
typedef void (*Eon_Drawer_Widget_Descriptor_Geometry_Set)(Eon_Drawer_Widget *w, void *data, Eina_Rectangle *geom);
typedef void (*Eon_Drawer_Widget_Descriptor_Free)(Eon_Drawer_Widget *w, void *data);

EAPI void eon_drawer_widget_request_process(Eon_Drawer_Widget *thiz);

/**
 * @}
 */

#endif
