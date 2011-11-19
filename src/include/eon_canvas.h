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

#ifndef _EON_CANVAS_H_
#define _EON_CANVAS_H_

/**
 * @defgroup Eon_Canvas Canvas
 * @ingroup Eon_Layout
 * @{
 */

EAPI Ender_Element * eon_canvas_new(void);
EAPI void eon_canvas_child_x_set(Ender_Element *r, Ender_Element *child, double x);
EAPI void eon_canvas_child_y_set(Ender_Element *r, Ender_Element *child, double y);

EAPI Ender_Element * eon_canvas_child_at_down_relative(Ender_Element *e, Ender_Element *rel, double x, double y);
EAPI Ender_Element * eon_canvas_child_at_up_relative(Ender_Element *e, Ender_Element *rel, double x, double y);

/**
 * @}
 */

#endif
