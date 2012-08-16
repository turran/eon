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

EAPI Ender_Element * eon_stack_new(void);
EAPI Ender_Element * eon_hstack_new(void);
EAPI Ender_Element * eon_vstack_new(void);

EAPI void eon_stack_orientation_set(Ender_Element *e, Eon_Orientation orientation);
EAPI void eon_stack_orientation_get(Ender_Element *e, Eon_Orientation *orientation);
EAPI void eon_stack_homogeneous_set(Ender_Element *e, Eina_Bool homogeneous);
EAPI void eon_stack_homogeneous_get(Ender_Element *e, Eina_Bool *homogeneous);
EAPI void eon_stack_child_gravity_set(Ender_Element *e, Ender_Element *child,
		int gravity);

/**
 * @}
 */

#endif
