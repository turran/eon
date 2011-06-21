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
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Bool _init = EINA_FALSE;
static Escen *_theme = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Escen * eon_theme_get(void)
{
	return _theme;
}

Eina_Bool eon_theme_init(void)
{
	if (!_init)
	{
		_theme = escen_parser_load(PACKAGE_DATA_DIR "/themes/basic.escen");
		if (!_theme) return EINA_FALSE;
		/* TODO once we have the theme loaded, append the datadir to the paths */
		_init = EINA_TRUE;
	}
	return _init;
}

void eon_theme_shutdown(void)
{
	if (_init)
	{
		_init = EINA_FALSE;
	}
}
