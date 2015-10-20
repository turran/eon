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

#ifndef EON_RENDERABLE_PRIVATE_H_
#define EON_RENDERABLE_PRIVATE_H_

#include "eon_element_private.h"
#include "eon_renderable.h"

#define EON_RENDERABLE_DESCRIPTOR eon_renderable_descriptor_get()
Enesim_Object_Descriptor * eon_renderable_descriptor_get(void);

#define EON_RENDERABLE_CLASS_GET(o) EON_RENDERABLE_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_RENDERABLE_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Renderable_Class, EON_RENDERABLE_DESCRIPTOR)
#define EON_RENDERABLE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Renderable, EON_RENDERABLE_DESCRIPTOR)

typedef struct _Eon_Renderable
{
	Eon_Element base;
	/* attributes */
	Egueb_Dom_Node *width;
	Egueb_Dom_Node *height;
	Egueb_Dom_Node *valign;
	Egueb_Dom_Node *halign;
	/* private */
	Eon_Renderable_Size size;
	int size_hints;
	Eina_Rectangle geometry;

	Eina_Bool size_hints_cached;
	Eina_Bool needs_geometry;
} Eon_Renderable;

typedef void (*Eon_Renderable_Init)(Eon_Renderable *thiz);
typedef int (*Eon_Renderable_Size_Hints_Get)(Eon_Renderable *thiz, Eon_Renderable_Size *size);
typedef int (*Eon_Renderable_Width_For_Height)(Eon_Renderable *thiz, int height);
typedef int (*Eon_Renderable_Height_For_Width)(Eon_Renderable *thiz, int width);
typedef void (*Eon_Renderable_Geometry_Set)(Eon_Renderable *thiz, Eina_Rectangle *geom);
typedef Enesim_Renderer * (*Eon_Renderable_Renderer_Get)(Eon_Renderable *thiz);
typedef Eina_Bool (*Eon_Renderable_Pre_Process)(Eon_Renderable *thiz);
typedef Eina_Bool (*Eon_Renderable_Process)(Eon_Renderable *thiz);
typedef Egueb_Dom_Node * (*Eon_Renderable_Element_At)(Eon_Renderable *thiz, Eina_Rectangle *cursor);

typedef struct _Eon_Renderable_Class
{
	Eon_Element_Class base;
	Eon_Renderable_Init init;
	Eon_Renderable_Size_Hints_Get size_hints_get;
	Eon_Renderable_Width_For_Height width_for_height;
	Eon_Renderable_Height_For_Width height_for_width;
	Eon_Renderable_Geometry_Set geometry_set;
	Eon_Renderable_Renderer_Get renderer_get;
	Eon_Renderable_Pre_Process pre_process;
	Eon_Renderable_Process process;
	Eon_Renderable_Element_At element_at;
	Eina_Bool child_size_dependant;
} Eon_Renderable_Class;

Eina_Bool eon_is_renderable(Egueb_Dom_Node *n);
void eon_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry);
void eon_renderable_invalidate_geometry(Egueb_Dom_Node *n);
void eon_renderable_geometry_solve(Egueb_Dom_Node *n, Eina_Rectangle *fs, Eina_Rectangle *out);
Enesim_Renderer * eon_renderable_renderer_get(Egueb_Dom_Node *n);
Egueb_Dom_Node * eon_renderable_element_at(Egueb_Dom_Node *n,
		Eina_Rectangle *cursor);

static inline void eon_renderable_size_init(Eon_Renderable_Size *s)
{
	s->min_width = 0;
	s->max_width = -1;
	s->min_height = 0;
	s->max_height = -1;

	s->pref_width = -1;
	s->pref_height = -1;
}

#endif

