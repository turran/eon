#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Input
{
	struct
	{
		unsigned int downx;
		unsigned int downy;
		unsigned int button;
		unsigned int x;
		unsigned int y;
		unsigned int px;
		unsigned int py;
		Eina_Bool inside;
		Ender *grabbed;
		Ender *last;
	} pointer;
	/* TODO keep the last modifiers */
	struct
	{

	} keyboard;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Create a new input
 */
EAPI Eon_Input * eon_input_new(void)
{
	Eon_Input *ei;

	ei = calloc(1, sizeof(Eon_Input));
	return ei;
}
/**
 *
 */
EAPI void eon_input_feed_mouse_move(Eon_Input *ei, Ender *l,
		unsigned int x, unsigned int y)
{
	Ender *child;
	Enesim_Renderer *r;
	unsigned int px, py;

	r = ender_element_renderer_get(l);
	if (!eon_is_layout(r))
		return;
	/* SDL eon_ecore does not send an in/out event */
	if (!ei->pointer.inside)
	{
		eon_input_feed_mouse_in(ei, l);
	}

	px = ei->pointer.x;
	py = ei->pointer.y;
	ei->pointer.x = x;
	ei->pointer.y = y;

	if (ei->pointer.grabbed)
	{
		Eon_Event_Mouse_Move ev;

		ender_event_dispatch(ei->pointer.grabbed, "MouseMove", &ev);

		return;
	}
	/* TODO handle the subcanvas
	 * if mouse in an object and canvas(obj) != canvas => in canvas(obj) ?
	 * if mouse out an object and canvas(obj) != canvas => out canvas(obj) ?
	 */
	child = eon_layout_child_get_at_coord(r, x, y);
	if (child == ei->pointer.last)
	{
		/* send move */
		if (child)
		{
			Eon_Event_Mouse_Move ev;
			ender_event_dispatch(child, "MouseMove", &ev);
		}
	}
	else
	{
		/* send out event on i->r */
		if (ei->pointer.last)
		{
			Eon_Event_Mouse_Out ev;
			ender_event_dispatch(ei->pointer.last, "MouseOut", &ev);
		}
		/* send in event on r */
		if (child)
		{
			Eon_Event_Mouse_In ev;
			ender_event_dispatch(child, "MouseIn", &ev);
		}
	}
	/* update the current inside */
	ei->pointer.last = child;
}
/**
 *
 */
EAPI void eon_input_feed_mouse_in(Eon_Input *ei, Ender *l)
{
	Eon_Event_Mouse_In ev;
	Ender *child;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(l);
	if (!eon_is_layout(r))
		return;
	if (ei->pointer.inside)
		return;
	ei->pointer.inside = EINA_TRUE;
	child = eon_layout_child_get_at_coord(r, ei->pointer.x, ei->pointer.y);
	if (!child)
		return;
	/* TODO send the event */
	ender_event_dispatch(child, "MouseIn", &ev);
}
/**
 *
 */
EAPI void eon_input_feed_mouse_out(Eon_Input *ei, Ender *l)
{
	Eon_Event_Mouse_Out ev;
	Ender *child;
	Enesim_Renderer *r;

	if (!eon_is_layout(r))
		return;
	if (!ei->pointer.inside)
		return;
	ei->pointer.inside = EINA_FALSE;
	r = ender_element_renderer_get(l);
	child = eon_layout_child_get_at_coord(r, ei->pointer.x, ei->pointer.y);
	if (!child)
		return;
	/* TODO send the event */
	ender_event_dispatch(child, "MouseOut", &ev);
}

EAPI void eon_input_feed_mouse_down(Eon_Input *ei, Ender *l)
{
	Eon_Event_Mouse_Down ev;
	Ender *child;
	Enesim_Renderer *r;

	if (!ei->pointer.inside)
		return;
	r = ender_element_renderer_get(l);
	child = eon_layout_child_get_at_coord(r, ei->pointer.x, ei->pointer.y);
	if (!child)
		return;
	/* store the coordinates where the mouse buton down was done to
	 * trigger the click later
	 */
	ei->pointer.grabbed = child;
	ei->pointer.downx = ei->pointer.x;
	ei->pointer.downy = ei->pointer.y;
	ender_event_dispatch(child, "MouseDown", &ev);
}

EAPI void eon_input_feed_mouse_up(Eon_Input *i)
{
	Eon_Event_Mouse_Up ev;
	Ender *child;

	/* send the event to the grabbed object */
	child = i->pointer.grabbed;
	if (!child)
		return;

	ender_event_dispatch(child, "MouseUp", &ev);
	/* in case the down coordinates are the same as the current coordinates
	 * send a click event
	 */
	if ((i->pointer.downx == i->pointer.x) &&
			 (i->pointer.downy == i->pointer.y))
	{
		Eon_Event_Mouse_Click ev_click;

		ender_event_dispatch(child, "MouseClick", &ev_click);
	}
	i->pointer.grabbed = NULL;
}
