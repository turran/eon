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

#ifndef EON_LAYOUT_PRIVATE_H_
#define EON_LAYOUT_PRIVATE_H_

typedef struct _Eon_Layout
{
	Enesim_Object_Instance base;
} Eon_Layout;

typedef int (*Eon_Layout_Size_Hints_Get_Cb)(Eon_Layout *l,
		Eon_Renderable_Size *size); 
typedef void (*Eon_Layout_Geometry_Set_Cb)(Eon_Layout *l,
		Eina_Rectangle *area);

typedef struct _Eon_Layout_Class
{
	Enesim_Object_Class base;
	Eon_Layout_Size_Hints_Get_Cb size_hints_get;
	Eon_Layout_Geometry_Set_Cb geometry_set;
} Eon_Layout_Class;

#define EON_LAYOUT_CLASS_GET(o) EON_LAYOUT_CLASS(				\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_LAYOUT_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Layout_Class, EON_LAYOUT_DESCRIPTOR)
#define EON_LAYOUT_CAST(o) ((Eon_Layout*)o)
#define EON_LAYOUT_DESCRIPTOR eon_layout_descriptor_get()

Enesim_Object_Descriptor * eon_layout_descriptor_get(void);
int eon_layout_size_hints_get(Eon_Layout *thiz, Eon_Renderable_Size *size);
void eon_layout_geometry_set(Eon_Layout *thiz, Eina_Rectangle *area);
void eon_layout_free(Eon_Layout *thiz);

#endif
