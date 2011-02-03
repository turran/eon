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
static Escen *_theme = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_log = -1;

Escen * eon_theme_get(void)
{

	return _theme;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int eon_init(void)
{
	if (!_init++)
	{
		eina_init();
		enesim_init();
		ender_init();
		escen_init();
		/* initialize the theme */
		/* FIXME, eon init is called from the ender_parser_init, which
		 * causes a recursion, avoid it */
		_theme = escen_parser_load(PACKAGE_DATA_DIR "/themes/basic.escen");
		if (!_theme)
		{
			escen_shutdown();
			ender_shutdown();
			enesim_shutdown();
			eina_shutdown();
		}
		return --_init;
	}
	return _init;
}

void eon_shutdown(void)
{
	if (!_init == 1)
	{
		escen_shutdown();
		ender_shutdown();
		enesim_shutdown();
		eina_shutdown();
	}
	_init--;
}
