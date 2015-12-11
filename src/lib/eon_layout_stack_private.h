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

#ifndef _EON_LAYOUT_STACK_PRIVATE_H_
#define _EON_LAYOUT_STACK_PRIVATE_H_

int eon_layout_stack_vertical_size_hints_get(Egueb_Dom_Node *r,
		Eon_Renderable_Size *size, int *min_height, int *weights);
void eon_layout_stack_vertical_size_geometry_set(Egueb_Dom_Node *r,
		Eina_Rectangle *g, int min_height, int weights);
int eon_layout_stack_horizontal_size_hints_get(Egueb_Dom_Node *r,
		Eon_Renderable_Size *size, int *min_width, int *weights);
void eon_layout_stack_horizontal_size_geometry_set(Egueb_Dom_Node *r,
		Eina_Rectangle *g, int min_width, int weights);

#endif
