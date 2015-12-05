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

#ifndef _EON_THEME_ELEMENT_ENTRY_PRIVATE_H
#define _EON_THEME_ELEMENT_ENTRY_PRIVATE_H

void eon_theme_element_entry_text_renderer_set(Egueb_Dom_Node *n,
		Enesim_Renderer *r);
int eon_theme_element_entry_size_hints_get(Egueb_Dom_Node *n,
		Eon_Renderable_Size *size, int max_length);
void eon_theme_element_entry_halign_set(Egueb_Dom_Node *n,
		Eon_Horizontal_Align halign);
void eon_theme_element_entry_cursor_set(Egueb_Dom_Node *n, int cursor);

#endif

