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

#ifndef _EON_TAB_H_
#define _EON_TAB_H_

/**
 * @defgroup Eon_Tab Tab
 * @ingroup Eon_Widget
 * @{
 */

EAPI Ender_Element * eon_tab_new(void);
EAPI void eon_tab_child_add_with_label(Ender_Element *e, const char *text);
EAPI void eon_tab_child_add(Ender_Element *e, Ender_Element *child);
EAPI void eon_tab_child_remove(Ender_Element *e, Ender_Element *child);
EAPI void eon_tab_child_clear(Ender_Element *e);
EAPI void eon_tab_child_content_set(Ender_Element *e, Ender_Element *child, Ender_Element *content);

/**
 * @}
 */

#endif


