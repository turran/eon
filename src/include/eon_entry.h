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

#ifndef _EON_ENTRY_H_
#define _EON_ENTRY_H_

/**
 * @defgroup Eon_Entry Entry
 * @ingroup Eon_Widget
 * @{
 */

EAPI Ender_Element * eon_entry_new(void);

EAPI void eon_entry_max_length_set(Ender_Element *e, int max_length);
EAPI void eon_entry_max_length_get(Ender_Element *e, int *max_length);
EAPI void eon_entry_alignment_set(Ender_Element *e, Eon_Horizontal_Alignment alignment);
EAPI void eon_entry_alignment_get(Ender_Element *e, Eon_Horizontal_Alignment *alignment);
EAPI void eon_entry_text_set(Ender_Element *e, const char *text);
EAPI void eon_entry_text_get(Ender_Element *e, const char **text);

/**
 * @}
 */

#endif

