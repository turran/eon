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
#ifndef EON_DRAWER_NAMESPACE_PRIVATE_H_
#define EON_DRAWER_NAMESPACE_PRIVATE_H_

void eon_drawer_namespace_init(void);
void eon_drawer_namespace_shutdown(void);
const Eon_Drawer_Instance_Descriptor * eon_drawer_namespace_instance_find(
		const Eon_Drawer_Namespace *thiz, const char *name);
Eina_Bool eon_drawer_namespace_instance_register(Eon_Drawer_Namespace *thiz,
		const Eon_Drawer_Instance_Descriptor *descriptor, const char *name);

#endif
