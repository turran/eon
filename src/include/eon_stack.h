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

#ifndef _EON_STACK_H
#define _EON_STACK_H

/**
 * @defgroup Eon_Stack Stack
 * @ingroup Eon_Layout
 * @{
 */
typedef enum _Eon_Horizontal_Alignment
{
	EON_HORIZONTAL_ALIGNMENT_LEFT,
	EON_HORIZONTAL_ALIGNMENT_RIGHT,
	EON_HORIZONTAL_ALIGNMENT_CENTER,
} Eon_Horizontal_Alignment;

typedef enum _Eon_Vertical_Alignment
{
	EON_VERTICAL_ALIGNMENT_TOP,
	EON_VERTICAL_ALIGNMENT_BOTTOM,
	EON_VERTICAL_ALIGNMENT_CENTER,
} Eon_Vertical_Alignment;

typedef enum _Eon_Stack_Direction
{
	EON_STACK_DIRECTION_HORIZONTAL,
	EON_STACK_DIRECTION_VERTICAL,
	EON_STACK_DIRECTIONS,
} Eon_Stack_Direction;


EAPI Ender_Element * eon_stack_new(void);
EAPI Ender_Element * eon_hstack_new(void);
EAPI Ender_Element * eon_vstack_new(void);

EAPI void eon_stack_direction_set(Ender_Element *e, Eon_Stack_Direction direction);
EAPI void eon_stack_direction_get(Ender_Element *e, Eon_Stack_Direction *direction);
EAPI void eon_stack_child_horizontal_alignment_set(Ender_Element *e, Ender_Element *child,
		Eon_Horizontal_Alignment alignment);
EAPI void eon_stack_child_vertical_alignment_set(Ender_Element *e, Ender_Element *child,
		Eon_Vertical_Alignment alignment);
EAPI void eon_stack_last_expand_set(Ender_Element *e, Eina_Bool expand);
EAPI void eon_stack_last_expand_get(Ender_Element *e, Eina_Bool *expand);
EAPI void eon_stack_homogeneous_set(Ender_Element *e, Eina_Bool homogeneous);
EAPI void eon_stack_homogeneous_get(Ender_Element *e, Eina_Bool *homogeneous);

/**
 * @}
 */

#endif
