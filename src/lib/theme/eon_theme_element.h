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

#ifndef _EON_THEME_ELEMENT_H_
#define _EON_THEME_ELEMENT_H_

typedef int (*Eon_Theme_Element_Version_Get_Cb)(void);
typedef Egueb_Dom_Node * (*Eon_Theme_Element_Ctor_Cb)(void);
typedef void (*Eon_Theme_Element_Dtor_Cb)(void *d);
typedef const char * (*Eon_Theme_Element_Tag_Name_Get_Cb)(void);
typedef Eina_Bool (*Eon_Theme_Element_Process_Cb)(void *d);

#endif
