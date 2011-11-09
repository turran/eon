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

#ifndef _EON_LAYOUT_H_
#define _EON_LAYOUT_H_

/**
 * @defgroup Eon_Layout Layout
 * @ingroup Eon_Widget
 * @{
 */

Eina_Bool eon_layout_is_topmost(Enesim_Renderer *r);
Eina_Bool eon_is_layout(Enesim_Renderer *r);
EAPI Ender_Element * eon_layout_child_get_at_coord(Enesim_Renderer *r, double x, double y);
EAPI Ender_Element * eon_layout_child_get_at_destination_coord(Enesim_Renderer *r, unsigned int x, unsigned int y);

EAPI void eon_layout_child_remove(Ender_Element *e, Ender_Element *child);
EAPI void eon_layout_child_add(Ender_Element *e, Ender_Element *child);
EAPI void eon_layout_child_clear(Ender_Element *e);

/**
 * @}
 */

#endif
