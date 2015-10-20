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
#include "eon_theme_private.h"
#include "eon_theme_main_private.h"
#include "eon_theme_document_private.h"
#include "eon_theme_namespace_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_theme_log_dom = -1;
Egueb_Dom_String *EON_THEME_NAME_ELEMENT_EOT;

Egueb_Dom_String *EON_THEME_ELEMENT_EON_THEME;
Egueb_Dom_String *EON_THEME_ELEMENT_INSTANCE;
Egueb_Dom_String *EON_THEME_ELEMENT_OBJECT;
Egueb_Dom_String *EON_THEME_ELEMENT_SCENE;
Egueb_Dom_String *EON_THEME_ELEMENT_STATES;
Egueb_Dom_String *EON_THEME_ELEMENT_STATE;

void eon_theme_init(void)
{
	if (!_init++)
	{
		egueb_dom_init();
 		EON_THEME_NAME_ELEMENT_EOT = egueb_dom_string_new_with_static_string("eot");

 		EON_THEME_ELEMENT_EON_THEME = egueb_dom_string_new_with_static_string("ender");
 		EON_THEME_ELEMENT_INSTANCE = egueb_dom_string_new_with_static_string("instance");
 		EON_THEME_ELEMENT_OBJECT = egueb_dom_string_new_with_static_string("object");
 		EON_THEME_ELEMENT_SCENE = egueb_dom_string_new_with_static_string("scene");
 		EON_THEME_ELEMENT_STATES = egueb_dom_string_new_with_static_string("states");
 		EON_THEME_ELEMENT_STATE = egueb_dom_string_new_with_static_string("state");
		eon_theme_log_dom = eina_log_domain_register("eon-theme", NULL);
		eon_theme_namespace_init();
		eon_theme_document_init();
		eon_drawer_init();
	}
}

void eon_theme_shutdown(void)
{
	if (_init == 1)
	{
		eon_drawer_shutdown();
		eon_theme_document_shutdown();
		eon_theme_namespace_shutdown();
		eina_log_domain_unregister(eon_theme_log_dom);
		egueb_dom_string_unref(EON_THEME_NAME_ELEMENT_EOT);

		egueb_dom_string_unref(EON_THEME_ELEMENT_EON_THEME);
		egueb_dom_string_unref(EON_THEME_ELEMENT_INSTANCE);
		egueb_dom_string_unref(EON_THEME_ELEMENT_OBJECT);
		egueb_dom_string_unref(EON_THEME_ELEMENT_SCENE);
		egueb_dom_string_unref(EON_THEME_ELEMENT_STATES);
		egueb_dom_string_unref(EON_THEME_ELEMENT_STATE);
		egueb_dom_shutdown();
	}
	_init--;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
