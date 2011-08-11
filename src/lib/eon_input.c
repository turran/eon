#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Input
{
	/* FIXME here we should put some device information, like name or type */
};

struct _Eon_Input_State
{
	Eon_Input *input;
	Eon_Input_Element_Get element_get;
	Ender_Element *element;
	struct
	{
		/* the coordinates where the mouse down happened */
		double downx;
		double downy;
		/* the coordinates where the pointer is right now */
		double x;
		double y;
		/* ?? */
		double px;
		double py;
		unsigned int button;
		Eina_Bool inside;
		Eina_Bool dragging;
		Ender_Element *grabbed;
		Ender_Element *last;
	} pointer;
	/* TODO keep the last modifiers */
	struct
	{

	} keyboard;
};

Ender_Element * _eon_input_element_get(Eon_Input_State *eis, double x, double y)
{
	Enesim_Renderer *r;
	double px, py;

	r = ender_element_renderer_get(eis->element);
#if 0
	/* should we check for transformation here? */
	eon_element_actual_position_get(r, &px, &py);
	printf("changing %p %d %d - %g %g\n", eis->element, x, y, px, py);
#endif
	return eis->element_get(eis->element, x, y);
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
		double x, double y)
{
	Ender_Element *child;
	double px, py;

	/* SDL eon_ecore does not send an in/out event */
	if (!eis->pointer.inside)
	{
		eon_input_state_feed_mouse_in(eis);
	}

	px = eis->pointer.x;
	py = eis->pointer.y;
	eis->pointer.x = x;
	eis->pointer.y = y;

	if (eis->pointer.grabbed)
	{
		Eon_Event_Mouse_Move ev;

		/* we first send the drag start */
		if (!eis->pointer.dragging)
		{
			Eon_Event_Mouse_Drag_Start ev_ds;

			ev_ds.input = eis->input;
			ender_event_dispatch(eis->pointer.grabbed, "MouseDragStart", &ev_ds);
			eis->pointer.dragging = EINA_TRUE;
		}

		ev.input = eis->input;
		ev.x = x;
		ev.y = y;
		ender_event_dispatch(eis->pointer.grabbed, "MouseMove", &ev);

		return;
	}
	child = _eon_input_element_get(eis, x, y);
	if (child == eis->pointer.last)
	{
		/* send move */
		if (child)
		{
			Eon_Event_Mouse_Move ev;
			ev.input = eis->input;
			ev.x = x;
			ev.y = y;
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

	if (eis->pointer.inside)
		return;
	eis->pointer.inside = EINA_TRUE;
	child = _eon_input_element_get(eis, eis->pointer.x, eis->pointer.y);
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

	if (!eis->pointer.inside)
		return;
	eis->pointer.inside = EINA_FALSE;
	child = _eon_input_element_get(eis, eis->pointer.x, eis->pointer.y);
	if (!child)
		return;
	ev.input = eis->input;
	ender_event_dispatch(child, "MouseOut", &ev);
}

void eon_input_state_feed_mouse_down(Eon_Input_State *eis)
{
	Eon_Event_Mouse_Down ev;
	Ender_Element *child;

	if (!eis->pointer.inside)
		return;
	child = _eon_input_element_get(eis, eis->pointer.x, eis->pointer.y);
	if (!child)
		return;
	/* store the coordinates where the mouse buton down was done to
	 * trigger the click later
	 */
	eis->pointer.grabbed = child;
	eis->pointer.downx = eis->pointer.x;
	eis->pointer.downy = eis->pointer.y;
	ev.input = eis->input;
	ender_event_dispatch(child, "MouseDown", &ev);
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
	ender_event_dispatch(child, "MouseUp", &ev);
	/* in case the down coordinates are the same as the current coordinates
	 * send a click event
	 */
	if ((fabs(eis->pointer.downx - eis->pointer.x) < DBL_EPSILON) &&
			(fabs(eis->pointer.downy - eis->pointer.y) < DBL_EPSILON))
	{
		Eon_Event_Mouse_Click ev_click;

		ev_click.input = eis->input;
		ender_event_dispatch(child, "MouseClick", &ev_click);
	}
	/* we first send the drag start */
	if (eis->pointer.dragging)
	{
		Eon_Event_Mouse_Drag_Stop ev_ds;

		ev_ds.input = eis->input;
		ender_event_dispatch(eis->pointer.grabbed, "MouseDragStop", &ev_ds);
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
	ender_event_dispatch(eis->pointer.last, "MouseWheel", &ev);
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
	"MouseDragEnd",
};

