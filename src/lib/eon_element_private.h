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

#ifndef EON_ELEMENT_PRIVATE_H_
#define EON_ELEMENT_PRIVATE_H_

#define EON_ELEMENT_DESCRIPTOR eon_element_descriptor_get()
Enesim_Object_Descriptor * eon_element_descriptor_get(void);

#define EON_ELEMENT_CLASS_GET(o) EON_ELEMENT_CLASS(				\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_ELEMENT_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Element_Class, EON_ELEMENT_DESCRIPTOR)
#define EON_ELEMENT(o) ENESIM_OBJECT_INSTANCE_CHECK(o,				\
		Eon_Element, EON_ELEMENT_DESCRIPTOR)

typedef struct _Eon_Element
{
	Enesim_Object_Instance base;
	Egueb_Dom_Node *n;
} Eon_Element;

typedef void * (*Eon_Element_Init)(Egueb_Dom_Node *node);
typedef void (*Eon_Element_Deinit)(
		Egueb_Dom_Node *node, void *data);
typedef Egueb_Dom_String * (*Eon_Element_Tag_Name_Get)(
		Egueb_Dom_Node *node, void *data);
typedef Eina_Bool (*Eon_Element_Process)(Egueb_Dom_Node *node,
		void *data);
typedef Eina_Bool (*Eon_Element_Child_Appendable)(Egueb_Dom_Node *node,
		void *data, Egueb_Dom_Node *child);


typedef struct _Eon_Element_Class
{
	Enesim_Object_Class base;
	Eon_Element_Init init;
	Eon_Element_Deinit deinit;
	Eon_Element_Tag_Name_Get tag_name_get;
	Eon_Element_Child_Appendable child_appendable;
	Eon_Element_Process process;
} Eon_Element_Class;

#endif
