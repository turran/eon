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
/* TODO
 * in theory we should create a singleton here
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Mouse_Proxy_Basic
{
	Eon_Mouse_Proxy_Basic_Descriptor *descriptor;
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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Mouse_Proxy_Basic * eon_mouse_proxy_basic_new(
		Eon_Mouse_Proxy_Basic_Descriptor *descriptor,
		void *data)
{
	Eon_Mouse_Proxy_Basic *thiz;

	thiz = calloc(1, sizeof(Eon_Mouse_Proxy_Basic));
	thiz->descriptor = descriptor;
	thiz->data = data;

	return thiz;
}

Ender_Element * _eon_mouse_proxy_basic_element_get(Eon_Mouse_Proxy_Basic *thiz, double x, double y, double *rel_x,
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

Eon_Mouse_Proxy_Basic * eon_mouse_proxy_basic_new(Eon_Input *input, Ender_Element *element,
		Eon_Mouse_Proxy_Basic_Descriptor *descriptor)
{
	Eon_Mouse_Proxy_Basic *is;

	if (!descriptor) return NULL;

	is = calloc(1, sizeof(Eon_Mouse_Proxy_Basic));
	is->input = input;
	is->element = element;
	is->descriptor = descriptor;

	return is;
}

/**
 *
 */
void eon_mouse_proxy_basic_feed_mouse_move(Eon_Mouse_Proxy_Basic *thiz,
		double x, double y, double offset_x, double offset_y)
{
	Ender_Element *child;
	double px, py;
	double rel_x, rel_y;

	/* SDL eon_ecore does not send an in/out event */
	if (!thiz->pointer.inside)
	{
		eon_mouse_proxy_basic_feed_mouse_in(thiz);
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
	child = _eon_mouse_proxy_basic_element_get(thiz, x, y, &rel_x, &rel_y);
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
void eon_mouse_proxy_basic_feed_mouse_in(Eon_Mouse_Proxy_Basic *thiz)
{
	Eon_Event_Mouse_In ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (thiz->pointer.inside)
		return;
	thiz->pointer.inside = EINA_TRUE;
	child = _eon_mouse_proxy_basic_element_get(thiz, thiz->pointer.x, thiz->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	ev.input = thiz->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN], &ev);
}
/**
 *
 */
void eon_mouse_proxy_basic_feed_mouse_out(Eon_Mouse_Proxy_Basic *thiz)
{
	Eon_Event_Mouse_Out ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (!thiz->pointer.inside)
		return;
	thiz->pointer.inside = EINA_FALSE;
	child = _eon_mouse_proxy_basic_element_get(thiz, thiz->pointer.x, thiz->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	ev.input = thiz->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT], &ev);
}

void eon_mouse_proxy_basic_feed_mouse_down(Eon_Mouse_Proxy_Basic *thiz)
{
	Eon_Event_Mouse_Down ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (!thiz->pointer.inside)
		return;
	child = _eon_mouse_proxy_basic_element_get(thiz, thiz->pointer.x, thiz->pointer.y, &rel_x, &rel_y);
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

void eon_mouse_proxy_basic_feed_mouse_up(Eon_Mouse_Proxy_Basic *thiz)
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

void eon_mouse_proxy_basic_feed_mouse_wheel(Eon_Mouse_Proxy_Basic *thiz, int direction)
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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

