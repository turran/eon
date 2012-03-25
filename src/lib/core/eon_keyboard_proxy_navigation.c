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

static void _eon_keyboard_proxy_navigation_key_down(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key)
{
	Eon_Keyboard_Proxy_Navigation *thiz = data;
	Eon_Navigation_Key nkey;

	printf("navigation key down current %p\n", current);
	if (!_get_navigation_key(key, &nkey))
	{
		_navigation_send_key_down(current, input, from, key);
	}
	else
	{
		Eon_Keyboard_Proxy_Navigation_Get get = NULL;
		switch (nkey)
		{
			case EON_NAVIGATION_KEY_TAB:
			get = thiz->descriptor->tab;
			break;

			case EON_NAVIGATION_KEY_REVERSE_TAB:
			get = thiz->descriptor->reverse_tab;
			break;

			case EON_NAVIGATION_KEY_LEFT:
			get = thiz->descriptor->left;
			break;

			case EON_NAVIGATION_KEY_RIGHT:
			get = thiz->descriptor->left;
			break;

			case EON_NAVIGATION_KEY_UP:
			get = thiz->descriptor->up;
			break;

			case EON_NAVIGATION_KEY_DOWN:
			get = thiz->descriptor->up;
			break;

			default:
			break;
		}
		/* just pass it */
		if (!get)
		{
			_navigation_send_key_down(current, input, from, key);
		}
		/* get the element and send the event there */
		else
		{
			Ender_Element *got;

			got = get(thiz->data, from);
			printf("got %p\n", got);
			if (!got)
			{
				_navigation_send_key_down(current, input, from, key);
			}
			else
			{
				eon_element_feed_key_down(got, input, current, key);
			}
		}
	}
}

static void _eon_keyboard_proxy_navigation_key_up(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key)
{
	Eon_Navigation_Key nkey;

	printf("navigation key up current %p\n", current);
	if (!_get_navigation_key(key, &nkey))
	{
		Ender_Element *parent;

		/* pass the event to the parent */
		eon_element_parent_get(current, &parent);
		if (parent)
		{
			eon_element_feed_key_up(parent, input, current, key);
		}
	}
}

static Eina_Bool _eon_keyboard_proxy_navigation_destroy(void *data)
{
	Eon_Keyboard_Proxy_Navigation *thiz = data;

	free(thiz);
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
	thiz->data = data;

	return eon_keyboard_proxy_new(&_descriptor, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


