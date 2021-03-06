/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
static int _proxy_focus_count = 0;

static void _navigation_send_key_down_bubble(Ender_Element *current,
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

static void _navigation_send_key_up_bubble(Ender_Element *current,
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

static void _eon_keyboard_proxy_focus_key_down(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key)
{
	Eon_Navigation_Key nkey;
	Ender_Element *focused;
	Eina_Bool is_navigation;

	/* DEBUG */
	{
		const char *cname;
		const char *fname = NULL;

		eon_element_name_get(current, &cname);
		if (from)
			eon_element_name_get(from, &fname);
		printf("focus key down current '%s' from '%s'\n", cname, fname ? fname : "NONE");
	}

	is_navigation = eon_input_navigation_key_get(input, key, &nkey);
	if (!is_navigation || nkey == EON_NAVIGATION_KEY_OK)
	{
		Eon_Event_Key_Down ev;
		ev.key = key;
		ev.input = input;
		ender_event_dispatch(current,
				eon_input_event_names[EON_INPUT_EVENT_KEY_DOWN],
				&ev);
		return;
	}

	eon_input_focus_get(input, &focused);
	/* we are the current focus */
	if (focused == current)
	{
		/* send the event to the parent */
		_navigation_send_key_down_bubble(current, input, key);
	}
	else
	{
		Eina_Bool focusable;

		eon_element_focusable_get(current, &focusable);
		printf("focusable %d\n", focusable);
		if (focusable)
			eon_input_focus_set(input, current);
		else
			_navigation_send_key_down_bubble(current, input, key);
	}
}

static void _eon_keyboard_proxy_focus_key_up(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key)
{
	Eon_Navigation_Key nkey;
	Eina_Bool is_navigation;

	printf("focus key up current %p\n", current);

	is_navigation = eon_input_navigation_key_get(input, key, &nkey);
	if (!is_navigation || nkey == EON_NAVIGATION_KEY_OK)
	{
		Eon_Event_Key_Up ev;
		ev.key = key;
		ev.input = input;
		ender_event_dispatch(current,
				eon_input_event_names[EON_INPUT_EVENT_KEY_UP],
				&ev);
	}
	else
	{
		_navigation_send_key_up_bubble(current, input, key);
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
