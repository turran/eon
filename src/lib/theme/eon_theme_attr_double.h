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
#ifndef EON_THEME_ATTR_DOUBLE_H_
#define EON_THEME_ATTR_DOUBLE_H_

#define EON_THEME_ATTR_DOUBLE_SET(f) ((Eon_Theme_Attr_Double_Set)(f))
#define EON_THEME_ATTR_DOUBLE_GET(f) ((Eon_Theme_Attr_Double_Get)(f))

typedef void (*Eon_Theme_Attr_Double_Set)(void *o, double i);
typedef double (*Eon_Theme_Attr_Double_Get)(void *o);

EAPI Egueb_Dom_Node * eon_theme_attr_double_new(const char *name,
		Eon_Theme_Attr_Double_Get get, Eon_Theme_Attr_Double_Set set);

#endif

