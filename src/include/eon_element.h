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

#ifndef _EON_ELEMENT_H_
#define _EON_ELEMENT_H_

/**
 * @defgroup Eon_Element Element
 * Represents the root of the user interface
 *
 * An Eon Element is the root object on the user interface object inheritance. All other
 * objects that interact with the user interface must inherit from this.
 *
 * An Eon Element has several size properties ...
 *
 * @{
 */

EAPI Eina_Bool eon_is_element(Ender_Element *e);
EAPI void eon_element_actual_height_get(Ender_Element *e, double *height);
EAPI void eon_element_actual_width_get(Ender_Element *e, double *width);
EAPI void eon_element_height_get(Ender_Element *e, double *height);
EAPI void eon_element_height_set(Ender_Element *e, double height);
EAPI void eon_element_width_get(Ender_Element *e, double *width);
EAPI void eon_element_width_set(Ender_Element *e, double width);
EAPI void eon_element_min_width_get(Ender_Element *e, double *width);
EAPI void eon_element_min_width_set(Ender_Element *e, double width);
EAPI void eon_element_min_height_set(Ender_Element *e, double height);
EAPI void eon_element_min_height_get(Ender_Element *e, double *height);
EAPI void eon_element_max_width_get(Ender_Element *e, double *width);
EAPI void eon_element_max_width_set(Ender_Element *e, double width);
EAPI void eon_element_max_height_set(Ender_Element *e, double height);
EAPI void eon_element_max_height_get(Ender_Element *e, double *height);
EAPI void eon_element_visibility_set(Ender_Element *e, double visibility);
EAPI void eon_element_visibility_get(Ender_Element *e, double *visibility);
EAPI void eon_element_parent_get(Ender_Element *e, Ender_Element **parent);
EAPI void eon_element_focusable_get(Ender_Element *e, Eina_Bool *focusable);
EAPI void eon_element_name_get(Ender_Element *e, const char **name);

EAPI void eon_element_horizontal_alignment_set(Ender_Element *e,
		Eon_Horizontal_Alignment alignment);
EAPI void eon_element_vertical_alignment_set(Ender_Element *e,
		Eon_Vertical_Alignment alignment);

/**
 * @}
 */

#endif
