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
	if (!ei->pointer.inside)
		return;

	px = ei->pointer.x;
	py = ei->pointer.y;
	ei->pointer.x = x;
	ei->pointer.y = y;

	if (ei->pointer.grabbed)
	{
		/* TODO dispatch mouse move event */
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
			/* TODO dispatch mouse move envet */
		}
	}
	else
	{
		/* send out event on i->r */
		if (ei->pointer.last)
		{
			/* TODO dispatch mouse out envet */
		}
		/* send in event on r */
		if (child)
		{
			/* TODO dispatch mouse in envet */
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
}
/**
 *
 */
EAPI void eon_input_feed_mouse_out(Eon_Input *ei, Ender *l)
{
	Ender *child;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(l);
	if (!eon_is_layout(r))
		return;
	if (!ei->pointer.inside)
		return;
	ei->pointer.inside = EINA_FALSE;
	child = eon_layout_child_get_at_coord(r, ei->pointer.x, ei->pointer.y);
	if (!child)
		return;
	/* TODO send the event */
}

