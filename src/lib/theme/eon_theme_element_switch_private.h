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

#ifndef _EON_THEME_ELEMENT_SWITCH_PRIVATE_H
#define _EON_THEME_ELEMENT_SWITCH_PRIVATE_H

int eon_theme_element_switch_size_hints_get(Egueb_Dom_Node *n, Eon_Renderable_Size *size);
void eon_theme_element_switch_active_text_set(Egueb_Dom_Node *n, Egueb_Dom_String *s);
void eon_theme_element_switch_inactive_text_set(Egueb_Dom_Node *n, Egueb_Dom_String *s);

#endif

