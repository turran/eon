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
#include "eon_input.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
/**
 * @todo
 * Handle focus, it should be one per keyboard input
 * Add flags to the input to know for example if we are using a touch based
 * input or a mouse input, etc. The widget may interact differently on that
 * cases
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Input
{
	char *name;
	char *description;
	Ender_Element *grabbed;
	Ender_Element *focus;
	Eina_Bool composed_keys;
	/* in case the input does not handle the mods by itself */
	Eon_Input_Modifiers mods;
	/* FIXME here we should put some device information, like name or type */
};

struct _Eon_Input_State
{
	Eon_Input *input;
	Eon_Input_State_Descriptor *descriptor;
	Ender_Element *element;
	struct
	{
		double offset_x;
		double offset_y;
		/* the coordinates where the mouse down happened */
		double downx;
		double downy;
		/* the coordinates where the pointer is right now */
		double x;
		double y;
		/* same as above but relative to the grabbed object */
		double rel_x;
		double rel_y;
		unsigned int button;
		Eina_Bool inside; /* is the pointer inside the area? */
		Eina_Bool dragging; /* is the pointer dragging */
		Ender_Element *grabbed; /* the object where the mouse down happened */
		Ender_Element *last;
	} pointer;
};

static Eina_Bool _unset_composed(Eon_Input_Modifiers *mods, const char *key)
{
	Eina_Bool ret = EINA_TRUE;

	/* FIXME implement the other modifiers */
	if (!strcmp(key, "Shift_L"))
		*mods &= ~EON_INPUT_MOD_LSHIFT;
	else if (!strcmp(key, "Shift_R"))
		*mods &= ~EON_INPUT_MOD_RSHIFT;
	else
		ret = EINA_FALSE;

	return ret;
}

static Eina_Bool _set_composed(Eon_Input_Modifiers *mods, const char *key)
{
	Eina_Bool ret = EINA_TRUE;

	/* FIXME implement the other modifiers */
	if (!strcmp(key, "Shift_L"))
		*mods |= EON_INPUT_MOD_LSHIFT;
	else if (!strcmp(key, "Shift_R"))
		*mods |= EON_INPUT_MOD_RSHIFT;
	else
		ret = EINA_FALSE;

	return ret;
}

Ender_Element * _eon_input_state_element_get(Eon_Input_State *thiz, double x, double y, double *rel_x,
	double *rel_y)
{
	Eon_Element *e_e;
	Ender_Element *e;
	double ex, ey;

	x -= thiz->pointer.offset_x;
	y -= thiz->pointer.offset_y;

	e = thiz->descriptor->element_get(thiz->element, x, y);
	if (!e) return NULL;
	e_e = ender_element_object_get(e);
	eon_element_actual_position_get(e_e, &ex, &ey);
	if (rel_x) *rel_x = x - ex;
	if (rel_y) *rel_y = y - ey;

	return e;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 * Create a new input
 */
Eon_Input * eon_input_new(void)
{
	Eon_Input *thiz;

	thiz = calloc(1, sizeof(Eon_Input));
	thiz->composed_keys = EINA_FALSE;

	return thiz;
}

Eon_Input_State * eon_input_state_new(Eon_Input *input, Ender_Element *element,
		Eon_Input_State_Descriptor *descriptor)
{
	Eon_Input_State *is;

	if (!descriptor) return NULL;

	is = calloc(1, sizeof(Eon_Input_State));
	is->input = input;
	is->element = element;
	is->descriptor = descriptor;

	return is;
}

/**
 *
 */
void eon_input_state_feed_mouse_move(Eon_Input_State *thiz,
		double x, double y, double offset_x, double offset_y)
{
	Ender_Element *child;
	double px, py;
	double rel_x, rel_y;

	/* SDL eon_ecore does not send an in/out event */
	if (!thiz->pointer.inside)
	{
		eon_input_state_feed_mouse_in(thiz);
	}

	px = thiz->pointer.x;
	py = thiz->pointer.y;
	thiz->pointer.x = x;
	thiz->pointer.y = y;
	thiz->pointer.offset_x = offset_x;
	thiz->pointer.offset_y = offset_y;

	if (thiz->pointer.grabbed)
	{
		Eon_Event_Mouse_Move ev;
		double rel_x, rel_y;


		rel_x = thiz->pointer.rel_x + (px - thiz->pointer.x);
		rel_y = thiz->pointer.rel_y + (py - thiz->pointer.y);
		/* we first send the drag start */
		if (!thiz->pointer.dragging)
		{
			Eon_Event_Mouse_Drag_Start ev_ds;

			ev_ds.input = thiz->input;
			ev_ds.x = x;
			ev_ds.y = y;
			ev_ds.rel_x = rel_x;
			ev_ds.rel_y = rel_y;
			ender_event_dispatch(thiz->pointer.grabbed,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_START], &ev_ds);
			thiz->pointer.dragging = EINA_TRUE;
		}

		ev.input = thiz->input;
		ev.x = x;
		ev.y = y;
		ev.rel_x = rel_x;
		ev.rel_y = rel_y;
		ev.offset_x = offset_x;
		ev.offset_y = offset_y;
		ender_event_dispatch(thiz->pointer.grabbed,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], &ev);

		return;
	}
	child = _eon_input_state_element_get(thiz, x, y, &rel_x, &rel_y);
	if (child == thiz->pointer.last)
	{
		/* send move */
		if (child)
		{
			Eon_Event_Mouse_Move ev;
			ev.input = thiz->input;
			ev.x = x;
			ev.y = y;
			ev.rel_x = rel_x;
			ev.rel_y = rel_y;
			ev.offset_x = offset_x;
			ev.offset_y = offset_y;
			ender_event_dispatch(child,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], &ev);
		}
	}
	else
	{
		/* send out event on i->r */
		if (thiz->pointer.last)
		{
			Eon_Event_Mouse_Out ev;

			ev.input = thiz->input;
			ender_event_dispatch(thiz->pointer.last,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT], &ev);
		}
		/* send in event on r */
		if (child)
		{
			Eon_Event_Mouse_In ev;

			ev.input = thiz->input;
			ender_event_dispatch(child,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN], &ev);
		}
	}
	/* update the current inside */
	thiz->pointer.last = child;
}

/**
 *
 */
void eon_input_state_feed_mouse_in(Eon_Input_State *thiz)
{
	Eon_Event_Mouse_In ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (thiz->pointer.inside)
		return;
	thiz->pointer.inside = EINA_TRUE;
	child = _eon_input_state_element_get(thiz, thiz->pointer.x, thiz->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	ev.input = thiz->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN], &ev);
}
/**
 *
 */
void eon_input_state_feed_mouse_out(Eon_Input_State *thiz)
{
	Eon_Event_Mouse_Out ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (!thiz->pointer.inside)
		return;
	thiz->pointer.inside = EINA_FALSE;
	child = _eon_input_state_element_get(thiz, thiz->pointer.x, thiz->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	ev.input = thiz->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT], &ev);
}

void eon_input_state_feed_mouse_down(Eon_Input_State *thiz)
{
	Eon_Event_Mouse_Down ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (!thiz->pointer.inside)
		return;
	child = _eon_input_state_element_get(thiz, thiz->pointer.x, thiz->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	/* store the coordinates where the mouse buton down was done to
	 * trigger the click later
	 */
	thiz->pointer.grabbed = child;
	thiz->pointer.downx = thiz->pointer.x;
	thiz->pointer.downy = thiz->pointer.y;
	thiz->pointer.rel_x = rel_x;
	thiz->pointer.rel_y = rel_y;
	ev.input = thiz->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_DOWN],
			&ev);
}

void eon_input_state_feed_mouse_up(Eon_Input_State *thiz)
{
	Eon_Event_Mouse_Up ev;
	Ender_Element *child;

	/* send the event to the grabbed object */
	child = thiz->pointer.grabbed;
	if (!child)
		return;

	ev.input = thiz->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_UP],
			&ev);
	/* in case the down coordinates are the same as the current coordinates
	 * send a click event
	 */
	if ((fabs(thiz->pointer.downx - thiz->pointer.x) < DBL_EPSILON) &&
			(fabs(thiz->pointer.downy - thiz->pointer.y) < DBL_EPSILON))
	{
		Eon_Event_Mouse_Click ev_click;

		ev_click.input = thiz->input;
		ev_click.x = thiz->pointer.x;
		ev_click.y = thiz->pointer.y;
		ev_click.rel_x = thiz->pointer.rel_x;
		ev_click.rel_y = thiz->pointer.rel_y;
		ender_event_dispatch(child,
				eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
				&ev_click);
	}
	/* send the drag stop */
	if (thiz->pointer.dragging)
	{
		Eon_Event_Mouse_Drag_Stop ev_ds;

		ev_ds.input = thiz->input;
		ender_event_dispatch(thiz->pointer.grabbed,
				eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_STOP], &ev_ds);
		thiz->pointer.dragging = EINA_FALSE;
	}
	thiz->pointer.grabbed = NULL;
}

void eon_input_state_feed_mouse_wheel(Eon_Input_State *thiz, int direction)
{
	Eon_Event_Mouse_Wheel ev;
	Ender_Element *child;

	/* send the event to the grabbed object */
	child = thiz->pointer.last;
	if (!child)
		return;
	ev.input = thiz->input;
	ender_event_dispatch(thiz->pointer.last,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_WHEEL], &ev);
}

/* FIXME the above code must removed/refactored */

void eon_input_feed_key_down(Eon_Input *thiz, Ender_Element *topmost, Eon_Keyboard_Key *key)
{
	Ender_Element *dst = topmost;
	Eon_Keyboard_Key *send_key;
	Eon_Keyboard_Key new_key;

	if (thiz->focus)
		dst = thiz->focus;
	/* first check if the key is a modifier */
	if (!thiz->composed_keys)
	{
 		if (_set_composed(&thiz->mods, key->name))
			return;

		new_key.name = key->name;
		new_key.mods = thiz->mods;
		send_key = &new_key;
	}
	else
	{
		send_key = key;
	}

	printf("key down %s %08x\n", send_key->name, send_key->mods);
	eon_element_feed_key_down(dst, thiz, NULL, send_key);
}

void eon_input_feed_key_up(Eon_Input *thiz, Ender_Element *topmost, Eon_Keyboard_Key *key)
{
	Ender_Element *dst = topmost;
	Eon_Keyboard_Key *send_key;
	Eon_Keyboard_Key new_key;

	if (thiz->focus)
		dst = thiz->focus;
	if (!thiz->composed_keys)
	{
 		if (_unset_composed(&thiz->mods, key->name))
			return;

		new_key.name = key->name;
		new_key.mods = thiz->mods;
		send_key = &new_key;
	}
	else
	{
		send_key = key;
	}

	printf("key up %s %08x\n", send_key->name, send_key->mods);
	eon_element_feed_key_up(dst, thiz, NULL, send_key);
}

Eina_Bool eon_input_navigation_key_get(Eon_Input *thiz,
		Eon_Keyboard_Key *key,
		Eon_Navigation_Key *nkey)
{
	Eina_Bool ret = EINA_TRUE;

	/* FIXME we miss the reverse tab, and so, the modifiers */
	if (!key->mods)
	{
		if (!strcmp(key->name, "Tab"))
			*nkey = EON_NAVIGATION_KEY_TAB;
		else if (!strcmp(key->name, "Left"))
			*nkey = EON_NAVIGATION_KEY_LEFT;
		else if (!strcmp(key->name, "Right"))
			*nkey = EON_NAVIGATION_KEY_RIGHT;
		else if (!strcmp(key->name, "Up"))
			*nkey = EON_NAVIGATION_KEY_UP;
		else if (!strcmp(key->name, "Down"))
			*nkey = EON_NAVIGATION_KEY_DOWN;
		else if (!strcmp(key->name, "Return") || !strcmp(key->name, "space"))
			*nkey = EON_NAVIGATION_KEY_OK;
		else if (!strcmp(key->name, "Escape"))
			*nkey = EON_NAVIGATION_KEY_CANCEL;
		else
			ret = EINA_FALSE;
	}
	else
	{
		if (key->mods == EON_INPUT_MOD_LSHIFT && !strcmp(key->name, "Tab"))
			*nkey = EON_NAVIGATION_KEY_REVERSE_TAB;
		else
			ret = EINA_FALSE;
	}

	return ret;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
const char * eon_input_event_names[EON_INPUT_EVENTS] = {
	"MouseMove",
	"MouseIn",
	"MouseOut",
	"MouseDown",
	"MouseUp",
	"MouseWheel",
	"MouseClick",
	"MouseDragStart",
	"MouseDragStop",
	"FocusIn",
	"FocusOut",
	"KeyDown",
	"KeyUp",
};

/* FIXME
 * add something like this:
EAPI void eon_input_flags_get(Eon_Input *i)
{

}
*/

EAPI const char * eon_input_name_get(Eon_Input *i)
{
	if (!i) return NULL;

	return i->name;
}

EAPI const char * eon_input_description_get(Eon_Input *i)
{
	if (!i) return NULL;

	return i->description;
}

EAPI void eon_input_focus_set(Eon_Input *thiz, Ender_Element *focus)
{
	if (!thiz) return;
	if (focus == thiz->focus)
		return;
	if (thiz->focus)
		ender_event_dispatch(thiz->focus,
				eon_input_event_names[EON_INPUT_EVENT_FOCUS_OUT], NULL);
	if (focus)
	{
		ender_event_dispatch(focus,
				eon_input_event_names[EON_INPUT_EVENT_FOCUS_IN], NULL);
	}
	thiz->focus = focus;
}

EAPI void eon_input_focus_get(Eon_Input *thiz, Ender_Element **focus)
{
	if (!thiz) return;
	if (focus) *focus = thiz->focus;

}
