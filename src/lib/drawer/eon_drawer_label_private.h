/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2014 Jorge Luis Zapata
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

#ifndef _EON_DRAWER_LABEL_PRIVATE_H_
#define _EON_DRAWER_LABEL_PRIVATE_H_

Enesim_Renderer * eon_drawer_label_text_renderer_get(Eon_Drawer_Widget *w);
Enesim_Text_Font * eon_drawer_label_text_font_get(Eon_Drawer_Widget *w);
void eon_drawer_label_text_color_set(Eon_Drawer_Widget *w, Enesim_Color color);
void eon_drawer_label_text_color_unset(Eon_Drawer_Widget *w);

#endif

