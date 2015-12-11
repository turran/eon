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
#ifndef _EON_THEME_PRIVATE_H_
#define _EON_THEME_PRIVATE_H_

Eina_Bool eon_theme_string_from(Eon_Theme *paint, const char *attr);
char * eon_theme_string_to(Eon_Theme *thiz);
void eon_theme_interpolate(Eon_Theme *v,
		Eon_Theme *a, Eon_Theme *b, double m,
		Eon_Theme *add, Eon_Theme *acc, int mul);
Eina_Bool eon_theme_is_equal(const Eon_Theme *p1,
		const Eon_Theme *p2);
void eon_theme_copy(const Eon_Theme *thiz, Eon_Theme *copy, Eina_Bool full);

#endif
