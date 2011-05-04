/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init = 0;

static void _destructor_callback(Ender *e, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (!eon_is_element(r))
		return;
}

static void _constructor_callback(Ender *e, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (!eon_is_element(r))
		return;
	eon_element_initialize(e);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_log = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
EAPI int eon_init(void)
{
	if (!_init++)
	{
		eina_init();
		eon_log = eina_log_domain_register("eon", NULL);
		enesim_init();
		ender_init();
		ender_element_new_listener_add(_constructor_callback, NULL);
		escen_init();
		eon_basic_init();
		/* initialize the theme */
		if (!eon_theme_init())
		{
			eon_basic_shutdown();
			escen_shutdown();
			ender_element_new_listener_remove(_constructor_callback, NULL);
			ender_shutdown();
			enesim_shutdown();
			eina_log_domain_unregister(eon_log);
			eina_shutdown();
		}
		return --_init;
	}
	return _init;
}

/**
 *
 */
EAPI void eon_shutdown(void)
{
	if (!_init == 1)
	{
		eon_basic_shutdown();
		escen_shutdown();
		ender_element_new_listener_remove(_constructor_callback, NULL);
		ender_shutdown();
		enesim_shutdown();
		eina_log_domain_unregister(eon_log);
		eina_shutdown();
	}
	_init--;
}

/**
 *
 */
EAPI void eon_version(unsigned int *major, unsigned int *minor, unsigned int *micro)
{
	if (major) *major = VERSION_MAJOR;
	if (minor) *minor = VERSION_MINOR;
	if (micro) *micro = VERSION_MICRO;
}

