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

#ifndef _EON_DRAWER_WIDGET_PRIVATE_H_
#define _EON_DRAWER_WIDGET_PRIVATE_H_

#define EON_DRAWER_WIDGET_DESCRIPTOR eon_drawer_widget_descriptor_get()
Enesim_Object_Descriptor * eon_drawer_widget_descriptor_get(void);

#define EON_DRAWER_WIDGET_CLASS_GET(o) EON_DRAWER_WIDGET_CLASS(				\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_DRAWER_WIDGET_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Drawer_Widget_Class, EON_DRAWER_WIDGET_DESCRIPTOR)
#define EON_DRAWER_WIDGET(o) ENESIM_OBJECT_INSTANCE_CHECK(o,				\
		Eon_Drawer_Widget, EON_DRAWER_WIDGET_DESCRIPTOR)

#define EON_DRAWER_WIDGET_NEW(prefix) eon_drawer_widget_new(prefix##_descriptor_get(),	\
				(Enesim_Object_Class *)&prefix##_klass)

struct _Eon_Drawer_Widget
{
	Enesim_Object_Instance base;
};

typedef Enesim_Renderer * (*Eon_Drawer_Widget_Renderer_Get)(Eon_Drawer_Widget *w); 
typedef void (*Eon_Drawer_Widget_Geometry_Set)(Eon_Drawer_Widget *w, Eina_Rectangle *geom);

typedef struct _Eon_Drawer_Widget_Class
{
	Enesim_Object_Class base;
	Eon_Drawer_Widget_Renderer_Get renderer_get;
	Eon_Drawer_Widget_Geometry_Set geometry_set;
} Eon_Drawer_Widget_Class;

/*
 * This functions defines a way for the widget to know whenever there is a
 * change on the drawer that requires the widget to do its own setup again.
 *
 * The widget in order to know whenever it needs to do it's own setup
 * based on some property change of itself also checks whenever the drawer
 * renderer associated with the widget has changed some property that requires
 * the widget to do it's own setup again. A simple usage would be a font size
 * change on the label drawer, if that happens, in case a button has such label
 * as the content, the buttons needs to do it's own setup again given that
 * size (preferred, min, max, whatever) might have changed
 */
Enesim_Renderer * eon_drawer_widget_renderer_get(Eon_Drawer_Widget *thiz);
void eon_drawer_widget_geometry_set(Eon_Drawer_Widget *thiz, Eina_Rectangle *geom);

#endif
