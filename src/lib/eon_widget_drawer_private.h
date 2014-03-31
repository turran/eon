/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2014 Jorge Luis Zapata
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

#ifndef EON_WIDGET_DRAWER_PRIVATE_H_
#define EON_WIDGET_DRAWER_PRIVATE_H_

#include "eon_widget_private.h"
#include "eon_drawer_widget.h"

#define EON_WIDGET_DRAWER_DESCRIPTOR eon_widget_drawer_descriptor_get()
Enesim_Object_Descriptor * eon_widget_drawer_descriptor_get(void);

#define EON_WIDGET_DRAWER_CLASS_GET(o) EON_WIDGET_DRAWER_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_WIDGET_DRAWER_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Widget_Drawer_Class, EON_WIDGET_DRAWER_DESCRIPTOR)
#define EON_WIDGET_DRAWER(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Widget_Drawer, EON_WIDGET_DRAWER_DESCRIPTOR)

typedef struct _Eon_Widget_Drawer Eon_Widget_Drawer;
typedef Egueb_Dom_Node * (*Eon_Widget_Drawer_Theme_Instance_Create)(
		Eon_Widget_Drawer *thiz, Egueb_Dom_Node *theme_document,
		void *data);

struct _Eon_Widget_Drawer
{
	Eon_Widget base;
	/* attributes */
	/* private */
	Egueb_Dom_Node *theme_document;
	Egueb_Dom_Node *theme_instance;
	Eon_Drawer_Widget *theme_widget;
	Enesim_Renderer *proxy;
	/* Whenever the parent's theme attr changed or the
	 * widget's theme attr changed this flag will be set
	 */
	Eina_Bool theme_changed;
	Eon_Widget_Drawer_Theme_Instance_Create theme_instance_create;
	void *theme_instance_create_data;
};

typedef int (*Eon_Widget_Drawer_Class_Size_Hints_Get)(Eon_Widget_Drawer *thiz,
		 Eon_Renderable_Size *size);
typedef Eina_Bool (*Eon_Widget_Drawer_Class_Process)(Eon_Widget_Drawer *thiz);
typedef Egueb_Dom_Node * (*Eon_Widget_Drawer_Class_Theme_Instance_Create)(
		Eon_Widget_Drawer *thiz, Egueb_Dom_Node *theme_document);
typedef void (*Eon_Widget_Drawer_Class_Theme_Instance_Created)(
		Eon_Widget_Drawer *thiz);

typedef struct _Eon_Widget_Drawer_Class
{
	Eon_Widget_Class base;
	Eon_Widget_Drawer_Class_Theme_Instance_Create theme_instance_create;
	Eon_Widget_Drawer_Class_Theme_Instance_Created theme_instance_created;
	Eon_Widget_Drawer_Class_Size_Hints_Get size_hints_get;
	Eon_Widget_Drawer_Class_Process process;
} Eon_Widget_Drawer_Class;

void eon_widget_drawer_theme_instance_create_set(Egueb_Dom_Node *n,
		Eon_Widget_Drawer_Theme_Instance_Create cb, void *data);

#endif
