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
#ifndef EON_THEME_INSTANCE_H_
#define EON_THEME_INSTANCE_H_

#define EON_THEME_INSTANCE_DESCRIPTOR_CTOR(f) (Eon_Theme_Instance_Descriptor_Ctor)(f)
#define EON_THEME_INSTANCE_DESCRIPTOR_DTOR(f) (Eon_Theme_Instance_Descriptor_Dtor)(f)
#define EON_THEME_INSTANCE_DESCRIPTOR_PROCESS(f) (Eon_Theme_Instance_Descriptor_Process)(f)
#define EON_THEME_INSTANCE_DESCRIPTOR_POPULATE(f) (Eon_Theme_Instance_Descriptor_Populate)(f)
#define EON_THEME_INSTANCE_DESCRIPTOR_CHILD_APPENDABLE(f) (Eon_Theme_Instance_Descriptor_Child_Appendable)(f)
#define EON_THEME_INSTANCE_DESCRIPTOR_CHILD_ADDED(f) (Eon_Theme_Instance_Descriptor_Child_Added)(f)
#define EON_THEME_INSTANCE_DESCRIPTOR_CHILD_REMOVED(f) (Eon_Theme_Instance_Descriptor_Child_Removed)(f)

typedef void * (*Eon_Theme_Instance_Descriptor_Ctor)(void);
typedef void (*Eon_Theme_Instance_Descriptor_Dtor)(void *thiz);
typedef void (*Eon_Theme_Instance_Descriptor_Populate)(Egueb_Dom_Node *n, void *thiz);
typedef Eina_Bool (*Eon_Theme_Instance_Descriptor_Process)(Egueb_Dom_Node *n, void *thiz);
typedef Eina_Bool (*Eon_Theme_Instance_Descriptor_Child_Appendable)(Egueb_Dom_Node *n,
		void *thiz, Egueb_Dom_Node *child);
typedef void (*Eon_Theme_Instance_Descriptor_Child_Added)(Egueb_Dom_Node *n, void *thiz,
		Egueb_Dom_Node *child);
typedef void (*Eon_Theme_Instance_Descriptor_Child_Removed)(Egueb_Dom_Node *n, void *thiz,
		Egueb_Dom_Node *child);

typedef struct _Eon_Theme_Instance_Descriptor {
	Eon_Theme_Instance_Descriptor_Ctor ctor;
	Eon_Theme_Instance_Descriptor_Dtor dtor;
	Eon_Theme_Instance_Descriptor_Populate populate;
	Eon_Theme_Instance_Descriptor_Process process;
	Eon_Theme_Instance_Descriptor_Child_Appendable child_appendable;
	Eon_Theme_Instance_Descriptor_Child_Added child_added;
	Eon_Theme_Instance_Descriptor_Child_Removed child_removed;
} Eon_Theme_Instance_Descriptor;

#endif
