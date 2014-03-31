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

typedef enum _Eon_Vertical_Align
{
	EON_VERTICAL_ALIGN_TOP,
	EON_VERTICAL_ALIGN_MIDDLE,
	EON_VERTICAL_ALIGN_BOTTOM,
} Eon_Vertical_Align;

typedef enum _Eon_Horizontal_Align
{
	EON_HORIZONTAL_ALIGN_LEFT,
	EON_HORIZONTAL_ALIGN_CENTER,
	EON_HORIZONTAL_ALIGN_RIGHT,
} Eon_Horizontal_Align;

typedef enum _Eon_Orientation
{
	EON_ORIENTATION_HORIZONTAL,
	EON_ORIENTATION_VERTICAL,
} Eon_Orientation;

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
EAPI extern Egueb_Dom_String *EON_ATTR_VALIGN;
EAPI extern Egueb_Dom_String *EON_ATTR_HALIGN;
EAPI extern Egueb_Dom_String *EON_ATTR_WEIGHT;
EAPI extern Egueb_Dom_String *EON_ATTR_ORIENTATION;
EAPI extern Egueb_Dom_String *EON_ATTR_ENABLED;
EAPI extern Egueb_Dom_String *EON_STOCK;

EAPI extern Egueb_Dom_String *EON_ELEMENT_EON;
EAPI extern Egueb_Dom_String *EON_ELEMENT_BUTTON;
EAPI extern Egueb_Dom_String *EON_ELEMENT_BUTTON_STOCK;
EAPI extern Egueb_Dom_String *EON_ELEMENT_LABEL;
EAPI extern Egueb_Dom_String *EON_ELEMENT_LABEL_STOCK;
EAPI extern Egueb_Dom_String *EON_ELEMENT_STACK;

EAPI extern Egueb_Dom_String *EON_EVENT_GEOMETRY;

#endif
