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
#ifndef _EON_THEME_NAME_H_
#define _EON_THEME_NAME_H_

typedef enum _Eon_Theme_Name_Type
{
	EON_THEME_NAME_TYPE_ENVIRONMENT,
	EON_THEME_NAME_TYPE_CURRENT_DOCUMENT,
	EON_THEME_NAME_TYPE_CUSTOM
} Eon_Theme_Name_Type;

typedef struct _Eon_Theme_Name
{
	Eon_Theme_Name_Type type;
	Egueb_Dom_String *custom;
} Eon_Theme_Name;

EAPI void eon_theme_name_reset(Eon_Theme_Name *thiz);

#endif
