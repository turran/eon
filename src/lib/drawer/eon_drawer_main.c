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
#include "eon_drawer_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init = 0;
static Eina_Array *_modules = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_drawer_log_dom = -1;

void eon_drawer_init(void)
{
	if (!_init++)
	{
		egueb_dom_init();
 		eon_drawer_log_dom = eina_log_domain_register("ender", NULL);
		eon_drawer_namespace_init();
		/* the modules */
		_modules = eina_module_list_get(_modules, PACKAGE_LIB_DIR"/drawer/", 1, NULL, NULL);
		eina_module_list_load(_modules);
		/* the static modules */
	}
}

void eon_drawer_shutdown(void)
{
	if (_init == 1)
	{
		/* unload every module */
		eina_module_list_free(_modules);
		eina_array_free(_modules);
		/* unload every static module */
		eon_drawer_namespace_shutdown();
		eina_log_domain_unregister(eon_drawer_log_dom);
		egueb_dom_shutdown();
	}
	_init--;
}

