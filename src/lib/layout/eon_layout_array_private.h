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

#ifndef EON_LAYOUT_ARRAY_PRIVATE_H_
#define EON_LAYOUT_ARRAY_PRIVATE_H_

typedef Eina_Bool (*Eon_Layout_Array_Child_Geometry_Set_Cb)(void *child,
		Eina_Rectangle *area);

typedef void * (*Eon_Layout_Array_Descriptor_Child_Next_Cb)(void *current,
		Eon_Box *padding, Eon_Layout_Array_Child_Geometry_Set_Cb *process);

typedef struct _Eon_Layout_Array_Descriptor
{
	Eon_Layout_Array_Descriptor_Child_Next_Cb next;
} Eon_Layout_Array_Descriptor;

#endif
