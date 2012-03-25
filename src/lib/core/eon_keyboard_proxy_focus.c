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
static int _proxy_focus_count = 0;

static Eina_Bool _get_navigation_key(const char *key,
		Eon_Navigation_Key *nkey)
{
	Eina_Bool ret = EINA_TRUE;

	/* FIXME we miss the reverse tab, and so, the modifiers */
	if (!strcmp(key, "Tab"))
		*nkey = EON_NAVIGATION_KEY_TAB;
	else if (!strcmp(key, "Left"))
		*nkey = EON_NAVIGATION_KEY_LEFT;
	else if (!strcmp(key, "Right"))
		*nkey = EON_NAVIGATION_KEY_RIGHT;
	else if (!strcmp(key, "Up"))
		*nkey = EON_NAVIGATION_KEY_UP;
	else if (!strcmp(key, "Down"))
		*nkey = EON_NAVIGATION_KEY_DOWN;
	else
		ret = EINA_FALSE;

	return ret;
}

static void _navigation_send_key_down(Ender_Element *current,
		Eon_Input *input,
		Ender_Element *from,
		const char *key)
{
	Ender_Element *parent;

	/* pass the event to the parent */
	eon_element_parent_get(current, &parent);
	if (parent)
	{
		eon_element_feed_key_down(parent, input, current, key);
	}
}

static void _eon_keyboard_proxy_focus_key_down(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key)
{
	Eon_Navigation_Key nkey;

	printf("focus key down current %p from %p %s\n", current, from, key);
	if (!_get_navigation_key(key, &nkey))
	{
		/* TODO generate the key event */
		printf("key down event\n");
	}
	else
	{
		/* we are the current focus */
		if (!from)
		{
			/* send the event to the parent */
			_navigation_send_key_down(current, input, NULL, key);
		}
		else
		{
			Eina_Bool focusable;

			eon_element_focusable_get(current, &focusable);
			printf("focusable %d\n", focusable);
			if (focusable)
				eon_input_focus_set(input, current);
			else
				_navigation_send_key_down(current, input, from, key);
		}
	}
}

static void _eon_keyboard_proxy_focus_key_up(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key)
{
	Eon_Navigation_Key nkey;

	printf("focus key up current %p\n", current);
	if (!_get_navigation_key(key, &nkey))
	{
	}
	else
	{
		printf("key up event\n");
	}
}

static Eina_Bool _eon_keyboard_proxy_focus_destroy(void *data)
{
	_proxy_focus_count--;
	if (!_proxy_focus_count)
		return EINA_TRUE;
	return EINA_FALSE;
}

static Eon_Keyboard_Proxy_Descriptor _descriptor = {
	/* .key_up 	= */ _eon_keyboard_proxy_focus_key_up,
	/* .key_down 	= */ _eon_keyboard_proxy_focus_key_down,
	/* .destroy 	= */ _eon_keyboard_proxy_focus_destroy,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Keyboard_Proxy * eon_keyboard_proxy_focus_new(void)
{
	static Eon_Keyboard_Proxy *proxy = NULL;

	if (!proxy)
	{
		proxy = eon_keyboard_proxy_new(&_descriptor, NULL);
	}
	_proxy_focus_count++;

	return proxy;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

