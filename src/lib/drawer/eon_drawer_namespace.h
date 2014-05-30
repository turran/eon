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
#ifndef EON_DRAWER_NAMESPACE_H_
#define EON_DRAWER_NAMESPACE_H_

typedef struct _Eon_Drawer_Namespace Eon_Drawer_Namespace;

EAPI Eon_Drawer_Namespace * eon_drawer_namespace_register(const char *name);
EAPI void eon_drawer_namespace_unregister(Eon_Drawer_Namespace *thiz);
EAPI const Eon_Drawer_Namespace * eon_drawer_namespace_find(const char *name);

#endif
