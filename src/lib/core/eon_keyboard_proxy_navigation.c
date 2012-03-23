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
typedef struct _Eon_Keyboard_Proxy_Navigation
{
	Eon_Keyboard_Proxy_Navigation_Descriptor *descriptor;
	void *data;
} Eon_Keyboard_Proxy_Navigation;

static void _eon_keyboard_proxy_navigation_key_down(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key)
{

}

static void _eon_keyboard_proxy_navigation_key_up(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key)
{

}

static Eina_Bool _eon_keyboard_proxy_navigation_destroy (void *data)
{
	return EINA_TRUE;
}


static Eon_Keyboard_Proxy_Descriptor _descriptor = {
	/* .key_up 	= */ _eon_keyboard_proxy_navigation_key_up,
	/* .key_down 	= */ _eon_keyboard_proxy_navigation_key_down,
	/* .destroy 	= */ _eon_keyboard_proxy_navigation_destroy,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Keyboard_Proxy * eon_keyboard_proxy_navigation_new(
		Eon_Keyboard_Proxy_Navigation_Descriptor *descriptor,
		void *data)
{
	Eon_Keyboard_Proxy_Navigation *thiz;

	thiz = calloc(1, sizeof(Eon_Keyboard_Proxy_Navigation));
	thiz->descriptor = descriptor;

	return eon_keyboard_proxy_new(&_descriptor, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


