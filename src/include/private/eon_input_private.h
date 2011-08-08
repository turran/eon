#ifndef _EON_INPUT_PRIVATE_H_
#define _EON_INPUT_PRIVATE_H_

/* TODO
 * we need a good way to handle recursive
 * event triggering. For example the case
 * of a layout which has a mouse_in event
 * callback set and inside there's a
 * scrollview (container) which should
 * have a drag event callback set on it
 * and the content itself might have a
 * click event set, like a button
 * how to handle such cases with the
 * current approach?
 * Maybe a good way would be that every
 * element that repeats events should have
 * its own input state, this way we share
 * the code on the input system (something
 * like and interface)
 */
typedef struct _Eon_Input_State
{

} Eon_Input_State;

Eon_Input * eon_input_new(void);
void eon_input_feed_mouse_move(Eon_Input *ei, Ender_Element *l,
		unsigned int x, unsigned int y);
void eon_input_feed_mouse_in(Eon_Input *ei, Ender_Element *l);
void eon_input_feed_mouse_out(Eon_Input *ei, Ender_Element *l);
void eon_input_feed_mouse_down(Eon_Input *ei, Ender_Element *l);
void eon_input_feed_mouse_up(Eon_Input *i);

#endif