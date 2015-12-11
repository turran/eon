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
#ifndef _EON_THEME_NAME_PRIVATE_H_
#define _EON_THEME_NAME_PRIVATE_H_

Eina_Bool eon_theme_name_string_from(Eon_Theme_Name *paint, const char *attr);
char * eon_theme_name_string_to(Eon_Theme_Name *thiz);
void eon_theme_name_interpolate(Eon_Theme_Name *v,
		Eon_Theme_Name *a, Eon_Theme_Name *b, double m,
		Eon_Theme_Name *add, Eon_Theme_Name *acc, int mul);
Eina_Bool eon_theme_name_is_equal(const Eon_Theme_Name *p1,
		const Eon_Theme_Name *p2);
void eon_theme_name_copy(const Eon_Theme_Name *thiz, Eon_Theme_Name *copy, Eina_Bool full);

#endif
