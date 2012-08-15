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

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Input_Proxy
{
	Ender_Element *element;
	Eon_Input_State_Descriptor *descriptor;
	Eina_Hash *states;
	/* FIXME add the ender event callbacks */
};

static Eon_Input_State * _eon_input_proxy_state_get(Eon_Input_Proxy *thiz, Eon_Input *input)
{
	Eon_Input_State *eis;

	eis = eina_hash_find(thiz->states, (const void *)&input);
	if (!eis)
	{
		eis = eon_input_state_new(input, thiz->element, thiz->descriptor);
		eina_hash_add(thiz->states, (const void *)&input, eis);
	}
	return eis;
}

static void _eon_input_proxy_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Input_Proxy *thiz = data;
	Eon_Event_Mouse_Down *ev = event_data;
	Eon_Input_State *eis;

	eis = _eon_input_proxy_state_get(thiz, ev->input);
	eon_input_state_feed_mouse_down(eis);
}

static void _eon_input_proxy_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Input_Proxy *thiz = data;
	Eon_Event_Mouse_Up *ev = event_data;
	Eon_Input_State *eis;

	eis = _eon_input_proxy_state_get(thiz, ev->input);
	eon_input_state_feed_mouse_up(eis);
}

static void _eon_input_proxy_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Input_Proxy *thiz = data;
	Eon_Event_Mouse_In *ev = event_data;
	Eon_Input_State *eis;

	eis = _eon_input_proxy_state_get(thiz, ev->input);
	eon_input_state_feed_mouse_in(eis);
}

static void _eon_input_proxy_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Input_Proxy *thiz = data;
	Eon_Event_Mouse_Out *ev = event_data;
	Eon_Input_State *eis;

	eis = _eon_input_proxy_state_get(thiz, ev->input);
	eon_input_state_feed_mouse_out(eis);
}

static void _eon_input_proxy_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Input_Proxy *thiz = data;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Input_State *eis;
	Eon_Element *ee;
	Eon_Geometry g;

	eis = _eon_input_proxy_state_get(thiz, ev->input);
	ee = ender_element_object_get(thiz->element);
	eon_element_geometry_get(ee, &g);
	eon_input_state_feed_mouse_move(eis, ev->x, ev->y, ev->offset_x + g.x, ev->offset_y + g.y);
}

static void _eon_input_proxy_mouse_wheel(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Input_Proxy *thiz = data;
	Eon_Event_Mouse_Wheel *ev = event_data;
	Eon_Input_State *eis;

	eis = _eon_input_proxy_state_get(thiz, ev->input);
	/* FIXME pass a mouse wheel? the mouse wheel should be set only on the focus element and an input must
	 * have only one focus. i.e the focus is not per input state */
	eon_input_state_feed_mouse_wheel(eis, 0);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Input_Proxy * eon_input_proxy_new(Ender_Element *e,
		Eon_Input_State_Descriptor *descriptor)
{
	Eon_Input_Proxy *thiz;

	thiz = calloc(1, sizeof(Eon_Input_Proxy));
	thiz->element = e;
	thiz->descriptor = descriptor;
	thiz->states = eina_hash_pointer_new(NULL);

	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE],
			_eon_input_proxy_mouse_move, thiz);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN],
			_eon_input_proxy_mouse_in, thiz);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT],
			_eon_input_proxy_mouse_out, thiz);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_DOWN],
			_eon_input_proxy_mouse_down, thiz);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_UP],
			_eon_input_proxy_mouse_up, thiz);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_WHEEL],
			_eon_input_proxy_mouse_wheel, thiz);

	return thiz;
}

void eon_input_proxy_delete(Eon_Input_Proxy *thiz)
{
	/* TODO remove the event listeners */
	/* TODO remove the hash */
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

