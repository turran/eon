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

#ifndef _EON_SPLITTER_H_
#define _EON_SPLITTER_H_

/**
 * @defgroup Eon_Splitter Splitter
 * @ingroup Eon_Widget
 * @{
 */

EAPI Ender_Element * eon_splitter_new(void);
EAPI Ender_Element * eon_hsplitter_new(void);
EAPI Ender_Element * eon_vsplitter_new(void);

EAPI void eon_splitter_direction_set(Ender_Element *e, Eon_Direction direction);
EAPI void eon_splitter_direction_get(Ender_Element *e, Eon_Direction *direction);

EAPI void eon_splitter_second_content_set(Ender_Element *e, Ender_Element *content);
EAPI void eon_splitter_second_content_get(Ender_Element *e, Ender_Element **content);
EAPI void eon_splitter_position_set(Ender_Element *e, double position);
EAPI void eon_splitter_position_get(Ender_Element *e, double *position);

/**
 * @}
 */

#endif

