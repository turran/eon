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

#ifndef _EON_SCROLLBAR_H_
#define _EON_SCROLLBAR_H_

/**
 * @defgroup Eon_Scrollbar Scrollbar
 * @ingroup Eon_Widget
 * @{
 */

EAPI Ender_Element * eon_scrollbar_new(void);
EAPI Ender_Element * eon_hscrollbar_new(void);
EAPI Ender_Element * eon_vscrollbar_new(void);

EAPI void eon_scrollbar_orientation_set(Ender_Element *e, Eon_Orientation direction);
EAPI void eon_scrollbar_orientation_get(Ender_Element *e, Eon_Orientation *direction);
EAPI void eon_scrollbar_max_set(Ender_Element *e, double max);
EAPI void eon_scrollbar_max_get(Ender_Element *e, double *max);
EAPI void eon_scrollbar_min_set(Ender_Element *e, double min);
EAPI void eon_scrollbar_min_get(Ender_Element *e, double *min);
EAPI void eon_scrollbar_page_increment_set(Ender_Element *e, double page_increment);
EAPI void eon_scrollbar_page_increment_get(Ender_Element *e, double *page_increment);
EAPI void eon_scrollbar_step_increment_set(Ender_Element *e, double step_increment);
EAPI void eon_scrollbar_step_increment_get(Ender_Element *e, double *step_increment);
EAPI void eon_scrollbar_value_set(Ender_Element *e, double value);
EAPI void eon_scrollbar_value_get(Ender_Element *e, double *value);
EAPI void eon_scrollbar_page_size_set(Ender_Element *e, double page_size);
EAPI void eon_scrollbar_page_size_get(Ender_Element *e, double *page_size);

/**
 * @}
 */

#endif

