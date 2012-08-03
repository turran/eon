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
#include "eon_private_main.h"

#include "eon_input.h"
#include "eon_main.h"
#include "eon_element.h"

#include "eon_private_input.h"
#include "eon_private_keyboard_proxy.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Keyboard_Proxy_Passthrough
{
	Ender_Element *to;
} Eon_Keyboard_Proxy_Passthrough;

static void _passthrough_send_key_down_bubble(Ender_Element *current,
		Eon_Input *input,
		Eon_Keyboard_Key *key)
{
	Ender_Element *parent;

	/* pass the event to the parent */
	eon_element_parent_get(current, &parent);
	if (parent)
	{
		eon_element_feed_key_down(parent, input, current, key);
	}
}

static void _passthrough_send_key_up_bubble(Ender_Element *current,
		Eon_Input *input,
		Eon_Keyboard_Key *key)
{
	Ender_Element *parent;

	/* pass the event to the parent */
	eon_element_parent_get(current, &parent);
	if (parent)
	{
		eon_element_feed_key_up(parent, input, current, key);
	}
}

static void _eon_keyboard_proxy_passthrough_key_down(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key)
{
	Eon_Keyboard_Proxy_Passthrough *thiz = data;

	/* if the event comes from its own element, pass it up */
	if (thiz->to == from)
		_passthrough_send_key_down_bubble(current, input, key);
	else
		eon_element_feed_key_down(thiz->to, input, from, key);
}

static void _eon_keyboard_proxy_passthrough_key_up(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key)
{
	Eon_Keyboard_Proxy_Passthrough *thiz = data;
	/* if the event comes from its own element, pass it up */
	if (thiz->to == from)
		_passthrough_send_key_up_bubble(current, input, key);
	else
		eon_element_feed_key_up(thiz->to, input, from, key);
}

static Eina_Bool _eon_keyboard_proxy_passthrough_destroy(void *data)
{
	free(data);
	return EINA_TRUE;
}

static Eon_Keyboard_Proxy_Descriptor _descriptor = {
	/* .key_up 	= */ _eon_keyboard_proxy_passthrough_key_up,
	/* .key_down 	= */ _eon_keyboard_proxy_passthrough_key_down,
	/* .destroy 	= */ _eon_keyboard_proxy_passthrough_destroy,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Keyboard_Proxy * eon_keyboard_proxy_passthrough_new(Ender_Element *to)
{
	Eon_Keyboard_Proxy_Passthrough *thiz;

	thiz = calloc(1, sizeof(Eon_Keyboard_Proxy_Passthrough));
	thiz->to = to;

	return eon_keyboard_proxy_new(&_descriptor, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

