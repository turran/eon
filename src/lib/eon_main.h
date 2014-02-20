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
#ifndef _EON_MAIN_H_
#define _EON_MAIN_H_

typedef struct _Eon_Size
{
	int width;
	int height;
} Eon_Size;

typedef struct _Eon_Box
{
	int top;
	int left;
	int right;
	int bottom;
} Eon_Box;

EAPI int eon_init(void);
EAPI int eon_shutdown(void);

EAPI extern Egueb_Dom_String *EON_MIN_WIDTH;
EAPI extern Egueb_Dom_String *EON_MIN_HEIGHT;
EAPI extern Egueb_Dom_String *EON_MAX_WIDTH;
EAPI extern Egueb_Dom_String *EON_MAX_HEIGHT;
EAPI extern Egueb_Dom_String *EON_WIDTH;
EAPI extern Egueb_Dom_String *EON_HEIGHT;
EAPI extern Egueb_Dom_String *EON_THEME;
EAPI extern Egueb_Dom_String *EON_ATTR_ELLIPSIZE;

EAPI extern Egueb_Dom_String *EON_ELEMENT_EON;
EAPI extern Egueb_Dom_String *EON_ELEMENT_BUTTON;
EAPI extern Egueb_Dom_String *EON_ELEMENT_LABEL;

EAPI extern Egueb_Dom_String *EON_EVENT_GEOMETRY;

#endif
