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
	/* FIXME here we should put some device information, like name or type */
};

struct _Eon_Input_State
{
	Eon_Input *input;
	Eon_Input_Element_Get element_get;
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
	struct
	{
		Ender_Element *focus;
		Eon_Input_Modifiers mods;
	} keyboard;
};

Ender_Element * _eon_input_state_element_get(Eon_Input_State *eis, double x, double y, double *rel_x,
	double *rel_y)
{
	Enesim_Renderer *e_r;
	Ender_Element *e;
	Ender_Element *parent;
	double ex, ey;

	x -= eis->pointer.offset_x;
	y -= eis->pointer.offset_y;

	e = eis->element_get(eis->element, x, y);
	if (!e) return NULL;
	e_r = ender_element_renderer_get(e);
	eon_element_actual_position_get(e_r, &ex, &ey);
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
	Eon_Input *ei;

	ei = calloc(1, sizeof(Eon_Input));
	return ei;
}

Eon_Input_State * eon_input_state_new(Eon_Input *input, Ender_Element *element,
		Eon_Input_Element_Get element_get)
{
	Eon_Input_State *is;

	is = calloc(1, sizeof(Eon_Input_State));
	is->input = input;
	is->element = element;
	is->element_get = element_get;

	return is;
}

/**
 *
 */
void eon_input_state_feed_mouse_move(Eon_Input_State *eis,
		double x, double y, double offset_x, double offset_y)
{
	Ender_Element *child;
	double px, py;
	double rel_x, rel_y;

	/* SDL eon_ecore does not send an in/out event */
	if (!eis->pointer.inside)
	{
		eon_input_state_feed_mouse_in(eis);
	}

	px = eis->pointer.x;
	py = eis->pointer.y;
	eis->pointer.x = x;
	eis->pointer.y = y;
	eis->pointer.offset_x = offset_x;
	eis->pointer.offset_y = offset_y;

	if (eis->pointer.grabbed)
	{
		Eon_Event_Mouse_Move ev;
		double rel_x, rel_y;


		rel_x = eis->pointer.rel_x + (px - eis->pointer.x);
		rel_y = eis->pointer.rel_y + (py - eis->pointer.y);
		/* we first send the drag start */
		if (!eis->pointer.dragging)
		{
			Eon_Event_Mouse_Drag_Start ev_ds;

			ev_ds.input = eis->input;
			ev_ds.x = x;
			ev_ds.y = y;
			ev_ds.rel_x = rel_x;
			ev_ds.rel_y = rel_y;
			ender_event_dispatch(eis->pointer.grabbed, "MouseDragStart", &ev_ds);
			eis->pointer.dragging = EINA_TRUE;
		}

		ev.input = eis->input;
		ev.x = x;
		ev.y = y;
		ev.rel_x = rel_x;
		ev.rel_y = rel_y;
		ev.offset_x = offset_x;
		ev.offset_y = offset_y;
		ender_event_dispatch(eis->pointer.grabbed, "MouseMove", &ev);

		return;
	}
	child = _eon_input_state_element_get(eis, x, y, &rel_x, &rel_y);
	if (child == eis->pointer.last)
	{
		/* send move */
		if (child)
		{
			Eon_Event_Mouse_Move ev;
			ev.input = eis->input;
			ev.x = x;
			ev.y = y;
			ev.rel_x = rel_x;
			ev.rel_y = rel_y;
			ev.offset_x = offset_x;
			ev.offset_y = offset_y;
			ender_event_dispatch(child, "MouseMove", &ev);
		}
	}
	else
	{
		/* send out event on i->r */
		if (eis->pointer.last)
		{
			Eon_Event_Mouse_Out ev;

			ev.input = eis->input;
			ender_event_dispatch(eis->pointer.last, "MouseOut", &ev);
		}
		/* send in event on r */
		if (child)
		{
			Eon_Event_Mouse_In ev;

			ev.input = eis->input;
			ender_event_dispatch(child, "MouseIn", &ev);
		}
	}
	/* update the current inside */
	eis->pointer.last = child;
}

/**
 *
 */
void eon_input_state_feed_mouse_in(Eon_Input_State *eis)
{
	Eon_Event_Mouse_In ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (eis->pointer.inside)
		return;
	eis->pointer.inside = EINA_TRUE;
	child = _eon_input_state_element_get(eis, eis->pointer.x, eis->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	ev.input = eis->input;
	ender_event_dispatch(child, "MouseIn", &ev);
}
/**
 *
 */
void eon_input_state_feed_mouse_out(Eon_Input_State *eis)
{
	Eon_Event_Mouse_Out ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (!eis->pointer.inside)
		return;
	eis->pointer.inside = EINA_FALSE;
	child = _eon_input_state_element_get(eis, eis->pointer.x, eis->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	ev.input = eis->input;
	ender_event_dispatch(child, "MouseOut", &ev);
}

void eon_input_state_feed_mouse_down(Eon_Input_State *eis)
{
	Eon_Event_Mouse_Down ev;
	Ender_Element *child;
	double rel_x, rel_y;

	if (!eis->pointer.inside)
		return;
	child = _eon_input_state_element_get(eis, eis->pointer.x, eis->pointer.y, &rel_x, &rel_y);
	if (!child)
		return;
	/* store the coordinates where the mouse buton down was done to
	 * trigger the click later
	 */
	eis->pointer.grabbed = child;
	eis->pointer.downx = eis->pointer.x;
	eis->pointer.downy = eis->pointer.y;
	eis->pointer.rel_x = rel_x;
	eis->pointer.rel_y = rel_y;
	ev.input = eis->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_DOWN],
			&ev);
	printf("mouse down at %g %g\n", eis->pointer.x, eis->pointer.y);
}

void eon_input_state_feed_mouse_up(Eon_Input_State *eis)
{
	Eon_Event_Mouse_Up ev;
	Ender_Element *child;

	/* send the event to the grabbed object */
	child = eis->pointer.grabbed;
	if (!child)
		return;

	ev.input = eis->input;
	ender_event_dispatch(child,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_UP],
			&ev);
	/* in case the down coordinates are the same as the current coordinates
	 * send a click event
	 */
	printf("mouse up\n");
	if ((fabs(eis->pointer.downx - eis->pointer.x) < DBL_EPSILON) &&
			(fabs(eis->pointer.downy - eis->pointer.y) < DBL_EPSILON))
	{
		Eon_Event_Mouse_Click ev_click;

		ev_click.input = eis->input;
		ev_click.x = eis->pointer.x;
		ev_click.y = eis->pointer.y;
		ev_click.rel_x = eis->pointer.rel_x;
		ev_click.rel_y = eis->pointer.rel_y;
		ender_event_dispatch(child,
				eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
				&ev_click);
		{
			Enesim_Renderer *r;
			char *name;

			r = ender_element_renderer_get(child);
			enesim_renderer_name_get(r, &name);
			printf("mouse click at %s %g %g - %g %g\n", name, ev_click.x, ev_click.y, ev_click.rel_x, ev_click.rel_y);
		}
	}
	/* send the drag stop */
	if (eis->pointer.dragging)
	{
		Eon_Event_Mouse_Drag_Stop ev_ds;

		ev_ds.input = eis->input;
		ender_event_dispatch(eis->pointer.grabbed,
				eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_STOP], &ev_ds);
		eis->pointer.dragging = EINA_FALSE;
	}
	eis->pointer.grabbed = NULL;
}

void eon_input_state_feed_mouse_wheel(Eon_Input_State *eis, int direction)
{
	Eon_Event_Mouse_Wheel ev;
	Ender_Element *child;

	/* send the event to the grabbed object */
	child = eis->pointer.last;
	if (!child)
		return;
	ev.input = eis->input;
	ender_event_dispatch(eis->pointer.last,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_WHEEL], &ev);
}

void eon_input_state_feed_key_down(Eon_Input_State *eis)
{

}

void eon_input_state_feed_key_up(Eon_Input_State *eis)
{

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
