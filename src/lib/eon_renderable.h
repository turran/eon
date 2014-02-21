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
#ifndef _EON_RENDERABLE_H_
#define _EON_RENDERABLE_H_

typedef enum _Eon_Renderable_Size_Hints
{
	EON_RENDERABLE_HINT_MIN_MAX 		= 1 << 0,
	EON_RENDERABLE_HINT_PREFERRED 		= 1 << 1,
	EON_RENDERABLE_HINT_HEIGHT_FOR_WIDTH 	= 1 << 2,
	EON_RENDERABLE_HINT_WIDTH_FOR_HEIGHT 	= 1 << 3,
} Eon_Renderable_Size_Hints;

typedef struct _Eon_Renderable_Size
{
	/* for the min/max hint */
	int min_width;
	int min_height;
	int max_width;
	int max_height;
	/* fot the preferred hint */
	int pref_width;
	int pref_height;
} Eon_Renderable_Size;

EAPI void eon_renderable_width_set(Egueb_Dom_Node *n, int w);
EAPI int eon_renderable_width_get(Egueb_Dom_Node *n);
EAPI void eon_renderable_height_set(Egueb_Dom_Node *n, int h);
EAPI int eon_renderable_height_get(Egueb_Dom_Node *n);
EAPI void eon_renderable_valign_set(Egueb_Dom_Node *n, Eon_Vertical_Align valign);
EAPI Eon_Vertical_Align eon_renderable_valign_get(Egueb_Dom_Node *n);
EAPI int eon_renderable_size_hints_get(Egueb_Dom_Node *n, Eon_Renderable_Size *size);



#endif

