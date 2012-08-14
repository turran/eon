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

#ifndef _EON_CHECKBOX_H_
#define _EON_CHECKBOX_H_

/**
 * @defgroup Eon_Checkbox Checkbox
 * @ingroup Eon_Container
 * @{
 */

EAPI Ender_Element * eon_checkbox_new(void);
EAPI void eon_checkbox_selected_set(Ender_Element *e, Eina_Bool selected);
EAPI void eon_checkbox_selected_get(Ender_Element *e, Eina_Bool *selected);
EAPI Ender_Element * eon_checkbox_new_with_label(const char *text);

/**
 * @}
 */

#endif
