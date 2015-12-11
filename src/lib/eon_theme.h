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
#ifndef _EON_THEME_H_
#define _EON_THEME_H_

typedef enum _Eon_Theme_Type
{
	EON_THEME_TYPE_CUSTOM,
	EON_THEME_TYPE_ENVIRONMENT,
	EON_THEME_TYPE_CURRENT_DOCUMENT
} Eon_Theme_Type;

typedef struct _Eon_Theme
{
	Eon_Theme_Type type;
	Egueb_Dom_String *custom;
} Eon_Theme;

EAPI void eon_theme_reset(Eon_Theme *thiz);

#endif
