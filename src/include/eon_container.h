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

#ifndef _EON_CONTAINER_H_
#define _EON_CONTAINER_H_

/**
 * @defgroup Eon_Container Container
 * @ingroup Eon_Widget
 * @{
 */

EAPI Eina_Bool eon_container_is_topmost(Ender_Element *e);
EAPI Eina_Bool eon_is_layout(Ender_Element *e);

EAPI void eon_container_child_remove(Ender_Element *e, Ender_Element *child);
EAPI void eon_container_child_add(Ender_Element *e, Ender_Element *child);
EAPI void eon_container_child_clear(Ender_Element *e);

/**
 * @}
 */

#endif
