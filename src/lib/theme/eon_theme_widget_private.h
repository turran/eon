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

#ifndef EON_THEME_WIDGET_PRIVATE_H_
#define EON_THEME_WIDGET_PRIVATE_H_

#include "eon_theme_renderable_private.h"

#define EON_THEME_WIDGET_DESCRIPTOR eon_theme_widget_descriptor_get()
Enesim_Object_Descriptor * eon_theme_widget_descriptor_get(void);

#define EON_THEME_WIDGET_CLASS_GET(o) EON_THEME_WIDGET_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_THEME_WIDGET_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Theme_Widget_Class, EON_THEME_WIDGET_DESCRIPTOR)
#define EON_THEME_WIDGET(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Widget, EON_THEME_WIDGET_DESCRIPTOR)

typedef struct _Eon_Theme_Widget
{
	Eon_Theme_Renderable base;
	Eina_Bool enabled;
} Eon_Theme_Widget;

typedef struct _Eon_Theme_Widget_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Widget_Class;

void eon_theme_widget_enabled_set(Egueb_Dom_Node *n, Eina_Bool enabled);

#endif

