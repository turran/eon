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

#ifndef EON_THEME_ELEMENT_PRIVATE_H_
#define EON_THEME_ELEMENT_PRIVATE_H_

#define EON_THEME_ELEMENT_DESCRIPTOR eon_theme_element_descriptor_get()
Enesim_Object_Descriptor * eon_theme_element_descriptor_get(void);

#define EON_THEME_ELEMENT_CLASS_GET(o) EON_THEME_ELEMENT_CLASS(				\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_THEME_ELEMENT_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Theme_Element_Class, EON_THEME_ELEMENT_DESCRIPTOR)
#define EON_THEME_ELEMENT(o) ENESIM_OBJECT_INSTANCE_CHECK(o,				\
		Eon_Theme_Element, EON_THEME_ELEMENT_DESCRIPTOR)

#define EON_THEME_ELEMENT_NEW(prefix) eon_theme_element_new(prefix##_descriptor_get(),	\
				(Enesim_Object_Class *)&prefix##_klass)

typedef struct _Eon_Theme_Element
{
	Enesim_Object_Instance base;
	/* private */
	Egueb_Dom_Node *id;
	Egueb_Dom_Node *n;
} Eon_Theme_Element;

/* Create the element */
typedef Egueb_Dom_Node * (*Eon_Theme_Element_Ctor)(Eon_Theme_Element *thiz);

/* Initialize the element. In this moment the element already has the private
 * node
 */
typedef void (*Eon_Theme_Element_Init)(Eon_Theme_Element *thiz);
/* Get the tag name */
typedef Egueb_Dom_String * (*Eon_Theme_Element_Tag_Name_Get)(
		Eon_Theme_Element *thiz);
/* Process the node */
typedef Eina_Bool (*Eon_Theme_Element_Process)(Eon_Theme_Element *thiz);
/* Check if a node is appendable */
typedef Eina_Bool (*Eon_Theme_Element_Child_Appendable)(Eon_Theme_Element *thiz, Egueb_Dom_Node *child);

typedef struct _Eon_Theme_Element_Class
{
	Enesim_Object_Class base;
	Eon_Theme_Element_Ctor ctor;
	Eon_Theme_Element_Init init;
	Eon_Theme_Element_Tag_Name_Get tag_name_get;
	Eon_Theme_Element_Child_Appendable child_appendable;
	Eon_Theme_Element_Process process;
} Eon_Theme_Element_Class;

Eina_Bool eon_theme_is_element(Egueb_Dom_Node *n);
Egueb_Dom_Node * eon_theme_element_new(Enesim_Object_Descriptor *descriptor,
		Enesim_Object_Class *klass);

#endif
