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
#include "eon_private.h"
#include "eon_main.h"
#include "eon_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init_count = 0;

static void _strings_init(void)
{
	/* attributes */
	EON_MIN_WIDTH = egueb_dom_string_new_with_static_string("min-width");
	EON_MIN_HEIGHT = egueb_dom_string_new_with_static_string("min-height");
	EON_MAX_WIDTH = egueb_dom_string_new_with_static_string("max-width");
	EON_MAX_HEIGHT = egueb_dom_string_new_with_static_string("max-height");
	EON_WIDTH = egueb_dom_string_new_with_static_string("width");
	EON_HEIGHT = egueb_dom_string_new_with_static_string("height");
	/* elements */
	EON_ELEMENT_EON = egueb_dom_string_new_with_static_string("eon");
	/* events */
	EON_EVENT_GEOMETRY = egueb_dom_string_new_with_static_string("EONGeometry");
}

static void _strings_shutdown(void)
{
	/* attributes */
	egueb_dom_string_unref(EON_MIN_WIDTH);
	egueb_dom_string_unref(EON_MIN_HEIGHT);
	egueb_dom_string_unref(EON_MAX_WIDTH);
	egueb_dom_string_unref(EON_MAX_HEIGHT);
	egueb_dom_string_unref(EON_WIDTH);
	egueb_dom_string_unref(EON_HEIGHT);
	/* elements */
	egueb_dom_string_unref(EON_ELEMENT_EON);
	/* events */
	egueb_dom_string_unref(EON_EVENT_GEOMETRY);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_String *EON_EVENT_GEOMETRY;
int eon_main_log = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* attributes */
Egueb_Dom_String *EON_MIN_WIDTH;
Egueb_Dom_String *EON_MIN_HEIGHT;
Egueb_Dom_String *EON_MAX_WIDTH;
Egueb_Dom_String *EON_MAX_HEIGHT;
Egueb_Dom_String *EON_WIDTH;
Egueb_Dom_String *EON_HEIGHT;

Egueb_Dom_String *EON_ELEMENT_EON;

/**
 *
 */
EAPI int eon_init(void)
{
	_init_count++;
	if (_init_count == 1)
	{
		eina_init();
		eon_main_log = eina_log_domain_register("eon", NULL);
		ender_init();
		egueb_dom_init();
		_strings_init();
	}
	return _init_count;
}

/**
 *
 */
EAPI int eon_shutdown(void)
{
	if (--_init_count != 0)
		return _init_count;

	_strings_shutdown();
	egueb_dom_shutdown();
	ender_shutdown();
	eina_log_domain_unregister(eon_main_log);
	eon_main_log = -1;
	eina_shutdown();
	return _init_count;
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
