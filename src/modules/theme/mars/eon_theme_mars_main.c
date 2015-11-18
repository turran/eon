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
#include "eon_theme_mars_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eon_Theme_Namespace * _namespace = NULL;
/*----------------------------------------------------------------------------*
 *                               Module API                                   *
 *----------------------------------------------------------------------------*/
#if BUILD_STATIC_MODULE_THEME_MARS
Eina_Bool eon_theme_mars_init(void)
#else
static Eina_Bool eon_theme_mars_init(void)
#endif
{
	_namespace = eon_theme_namespace_register("http://www.eon.org/theme/mars");
	if (!_namespace)
		return EINA_FALSE;
	eon_theme_namespace_element_add(_namespace, "button",
			eon_theme_mars_button_new);
	eon_theme_namespace_element_add(_namespace, "entry",
			eon_theme_mars_entry_new);
	eon_theme_namespace_element_add(_namespace, "eon",
			eon_theme_mars_eon_new);
	eon_theme_namespace_element_add(_namespace, "switch",
			eon_theme_mars_switch_new);
	eon_theme_namespace_element_add(_namespace, "radio",
			eon_theme_mars_radio_new);
	eon_theme_namespace_element_add(_namespace, "frame",
			eon_theme_mars_frame_new);
	eon_theme_namespace_element_add(_namespace, "separator",
			eon_theme_mars_separator_new);
	eon_theme_namespace_element_add(_namespace, "image",
			eon_theme_mars_image_new);
	return EINA_TRUE;
}

#if BUILD_STATIC_MODULE_THEME_MARS
void eon_theme_mars_shutdown(void)
#else
static void eon_theme_mars_shutdown(void)
#endif
{
}

#if !BUILD_STATIC_MODULE_THEME_MARS
EINA_MODULE_INIT(eon_theme_mars_init);
EINA_MODULE_SHUTDOWN(eon_theme_mars_shutdown);
#endif

