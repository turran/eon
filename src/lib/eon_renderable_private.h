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

#define EON_RENDERABLE_DESCRIPTOR eon_renderable_descriptor_get()
Enesim_Object_Descriptor * eon_renderable_descriptor_get(void);

#define EON_RENDERABLE_CLASS_GET(o) EON_RENDERABLE_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_RENDERABLE_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Renderable_Class, EON_RENDERABLE_DESCRIPTOR)
#define EON_RENDERABLE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Renderable, EON_RENDERABLE_DESCRIPTOR)

typedef enum _Eon_Renderable_Size_Hints
{
	EON_SIZE_HINT_MIN_MAX 		= 1 << 0,
	EON_SIZE_HINT_PREFERRED 	= 1 << 1,
	EON_SIZE_HINT_HEIGHT_FOR_WIDTH 	= 1 << 2,
	EON_SIZE_HINT_WIDTH_FOR_HEIGHT 	= 1 << 3,
} Eon_Renderable_Size_Hints;

typedef struct _Eon_Renderable_Size
{
	/* for the min/max hint */
	int min_width;
	int min_height;
	int max_width;
	int max_height;
	/* fot the preferred hint */
	int pref_width;
	int pref_height;
} Eon_Renderable_Size;

typedef struct _Eon_Renderable
{
	Eon_Element base;
	/* attributes */
	Egueb_Dom_Node *visibility;
	Egueb_Dom_Node *min_width;
	Egueb_Dom_Node *min_height;
	Egueb_Dom_Node *max_width;
	Egueb_Dom_Node *max_height;
	Egueb_Dom_Node *width;
	Egueb_Dom_Node *height;
	/* private */
	Eon_Renderable_Size size;
	int size_hints;
	Eina_Rectangle geometry;
} Eon_Renderable;

typedef void (*Eon_Renderable_Init)(Eon_Renderable *thiz);
typedef int (*Eon_Renderable_Size_Hints_Get)(Eon_Renderable *thiz, Eon_Renderable_Size *size);
typedef int (*Eon_Renderable_Width_For_Height)(Eon_Renderable *thiz, int height);
typedef int (*Eon_Renderable_Height_For_Width)(Eon_Renderable *thiz, int width);
typedef void (*Eon_Renderable_Geometry_Set)(Eon_Renderable *thiz, Eina_Rectangle *geom);
typedef Enesim_Renderer * (*Eon_Renderable_Renderer_Get)(Eon_Renderable *thiz);
typedef Eina_Bool (*Eon_Renderable_Pre_Setup)(Eon_Renderable *thiz);
typedef Eina_Bool (*Eon_Renderable_Setup)(Eon_Renderable *thiz);

typedef struct _Eon_Renderable_Class
{
	Eon_Element_Class base;
	Eon_Renderable_Init init;
	Eon_Renderable_Size_Hints_Get size_hints_get;
	Eon_Renderable_Width_For_Height width_for_height;
	Eon_Renderable_Height_For_Width height_for_width;
	Eon_Renderable_Geometry_Set geometry_set;
	Eon_Renderable_Renderer_Get renderer_get;
	Eon_Renderable_Pre_Setup pre_setup;
	Eon_Renderable_Setup setup;
} Eon_Renderable_Class;

Eina_Bool eon_is_renderable(Egueb_Dom_Node *n);
int eon_renderable_size_hints_get(Egueb_Dom_Node *n, Eon_Renderable_Size *size);
void eon_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry);

#endif

