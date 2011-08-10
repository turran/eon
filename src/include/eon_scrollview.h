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

#ifndef _EON_SCROLLVIEW_H_
#define _EON_SCROLLVIEW_H_

/**
 * @defgroup Eon_Scrollview Scrollview
 * @ingroup Eon_Container
 * @{
 */

EAPI Ender_Element * eon_scrollview_new(void);
EAPI void eon_scrollview_x_position_set(Ender_Element *e, double x);
EAPI void eon_scrollview_x_position_get(Ender_Element *e, double *x);
EAPI void eon_scrollview_y_position_set(Ender_Element *e, double y);
EAPI void eon_scrollview_y_position_get(Ender_Element *e, double *y);

/**
 * @}
 */

#endif


