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

#ifndef EON_THEME_RENDERABLE_PRIVATE_H_
#define EON_THEME_RENDERABLE_PRIVATE_H_

#include "eon_theme_element_private.h"

#define EON_THEME_RENDERABLE_DESCRIPTOR eon_theme_renderable_descriptor_get()
Enesim_Object_Descriptor * eon_theme_renderable_descriptor_get(void);

#define EON_THEME_RENDERABLE_CLASS_GET(o) EON_THEME_RENDERABLE_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_THEME_RENDERABLE_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Theme_Renderable_Class, EON_THEME_RENDERABLE_DESCRIPTOR)
#define EON_THEME_RENDERABLE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Renderable, EON_THEME_RENDERABLE_DESCRIPTOR)

typedef struct _Eon_Theme_Renderable
{
	Eon_Theme_Element base;
	Eina_Rectangle geometry;
} Eon_Theme_Renderable;

typedef void (*Eon_Theme_Renderable_Geometry_Set)(Eon_Theme_Renderable *thiz, Eina_Rectangle *geom);
typedef Enesim_Renderer * (*Eon_Theme_Renderable_Renderer_Get)(Eon_Theme_Renderable *thiz);

typedef struct _Eon_Theme_Renderable_Class
{
	Eon_Theme_Element_Class base;
	Eon_Theme_Renderable_Geometry_Set geometry_set;
	Eon_Theme_Renderable_Renderer_Get renderer_get;
} Eon_Theme_Renderable_Class;

void eon_theme_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry);
Enesim_Renderer * eon_theme_renderable_renderer_get(Egueb_Dom_Node *n);

#endif
