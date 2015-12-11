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
#ifndef _EON_THEME_NAMESPACE_PRIVATE_H_
#define _EON_THEME_NAMESPACE_PRIVATE_H_

void eon_theme_namespace_init(void);
void eon_theme_namespace_shutdown(void);

Eon_Theme_Namespace * eon_theme_namespace_find(const char *name);
Egueb_Dom_Node * eon_theme_namespace_element_ctor(Eon_Theme_Namespace *thiz,
		const char *name);

#endif
