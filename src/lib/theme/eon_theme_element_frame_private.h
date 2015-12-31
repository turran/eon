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

#ifndef _EON_THEME_ELEMENT_FRAME_PRIVATE_H
#define _EON_THEME_ELEMENT_FRAME_PRIVATE_H

void eon_theme_element_frame_min_size_get(Egueb_Dom_Node *n, int *w, int *h);
void eon_theme_element_frame_title_set(Egueb_Dom_Node *n, Egueb_Dom_String *s);
void eon_theme_element_frame_padding_get(Egueb_Dom_Node *n, Eon_Box *padding);
void eon_theme_element_frame_content_set(Egueb_Dom_Node *n, Enesim_Renderer *r);

#endif

