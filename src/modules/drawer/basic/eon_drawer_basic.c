/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Eon.h"
#include "Eon_Drawer.h"

#include "eon_drawer_basic.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Namespace * _eon_drawer_basic_namespace = NULL;
/*----------------------------------------------------------------------------*
 *                               Module API                                   *
 *----------------------------------------------------------------------------*/
#if BUILD_STATIC_MODULE_DRAWER_BASIC
Eina_Bool eon_drawer_basic_init(void)
#else
static Eina_Bool eon_drawer_basic_init(void)
#endif
{
	_eon_drawer_basic_namespace = ender_namespace_register("eon_drawer_basic");
	eon_drawer_button_ender_register(_eon_drawer_basic_namespace,
			ENDER_INSTANCE_DESCRIPTOR_CTOR(eon_drawer_basic_button_new));
	eon_drawer_label_ender_register(_eon_drawer_basic_namespace,
			ENDER_INSTANCE_DESCRIPTOR_CTOR(eon_drawer_basic_label_new));
	eon_drawer_eon_ender_register(_eon_drawer_basic_namespace,
			ENDER_INSTANCE_DESCRIPTOR_CTOR(eon_drawer_basic_eon_new));
	if (!_eon_drawer_basic_namespace)
		return EINA_FALSE;
	return EINA_TRUE;
}

#if BUILD_STATIC_MODULE_DRAWER_BASIC
void eon_drawer_basic_shutdown(void)
#else
static void eon_drawer_basic_shutdown(void)
#endif
{
	ender_namespace_unregister(_eon_drawer_basic_namespace);
	_eon_drawer_basic_namespace = NULL;
}

#if !BUILD_STATIC_MODULE_DRAWER_BASIC
EINA_MODULE_INIT(eon_drawer_basic_init);
EINA_MODULE_SHUTDOWN(eon_drawer_basic_shutdown);
#endif
