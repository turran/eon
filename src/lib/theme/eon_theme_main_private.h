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
#ifndef _EON_THEME_MAIN_PRIVATE_H
#define _EON_THEME_MAIN_PRIVATE_H

extern int eon_theme_log_dom;
extern Egueb_Dom_String *EON_THEME_ELEMENT_EON_THEME;
extern Egueb_Dom_String *EON_THEME_ELEMENT_INSTANCE;
extern Egueb_Dom_String *EON_THEME_ELEMENT_OBJECT;
extern Egueb_Dom_String *EON_THEME_ELEMENT_SCENE;
extern Egueb_Dom_String *EON_THEME_ELEMENT_STATES;
extern Egueb_Dom_String *EON_THEME_ELEMENT_STATE;

void eon_theme_init(void);
void eon_theme_shutdown(void);

#endif
