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
struct _Eon_Keyboard_Proxy
{
	Eon_Keyboard_Proxy_Descriptor *descriptor;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Keyboard_Proxy * eon_keyboard_proxy_new(
		Eon_Keyboard_Proxy_Descriptor *descriptor,
		void *data)
{
	Eon_Keyboard_Proxy *thiz;

	thiz = calloc(1, sizeof(Eon_Keyboard_Proxy));
	thiz->descriptor = descriptor;
	thiz->data = data;

	return thiz;
}

void eon_keyboard_proxy_delete(Eon_Keyboard_Proxy *thiz)
{
	
}

void eon_keyboard_proxy_feed_key_up(Eon_Keyboard_Proxy *thiz,
		Ender_Element *current,
		Eon_Input *input,
		Ender_Element *from,
		const char *key)
{

}

void eon_keyboard_proxy_feed_key_down(Eon_Keyboard_Proxy *thiz,
		Ender_Element *current,
		Eon_Input *input,
		Ender_Element *from,
		const char *key)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

