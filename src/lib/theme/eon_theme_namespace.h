/* ENDER - Enesim's descriptor library
 * Copyright (C) 2010 - 2012 Jorge Luis Zapata
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
#ifndef ENDER_NAMESPACE_H_
#define ENDER_NAMESPACE_H_

typedef struct _Eon_Theme_Namespace Eon_Theme_Namespace;

EAPI Eon_Theme_Namespace * eon_theme_namespace_register(const char *name);
EAPI void eon_theme_namespace_unregister(Eon_Theme_Namespace *thiz);
EAPI Eina_Bool eon_theme_namespace_instance_register(Eon_Theme_Namespace *thiz, const Eon_Theme_Instance_Descriptor *descriptor, const char *name);
EAPI const Eon_Theme_Namespace * eon_theme_namespace_find(const char *name);

#endif
