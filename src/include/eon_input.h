#ifndef _EON_INPUT_H_
#define _EON_INPUT_H_

/**
 * @{
 */
typedef struct _Eon_Input Eon_Input;

EAPI Eon_Input * eon_input_new(void);
EAPI void eon_input_feed_mouse_move(Eon_Input *ei, Ender_Element *l,
		unsigned int x, unsigned int y);
EAPI void eon_input_feed_mouse_in(Eon_Input *ei, Ender_Element *l);
EAPI void eon_input_feed_mouse_out(Eon_Input *ei, Ender_Element *l);
EAPI void eon_input_feed_mouse_down(Eon_Input *ei, Ender_Element *l);
EAPI void eon_input_feed_mouse_up(Eon_Input *i);

/**
 * @}
 * @{
 */

typedef struct _Eon_Event_Mouse_Click
{
} Eon_Event_Mouse_Click;

typedef struct _Eon_Event_Mouse_Up
{
} Eon_Event_Mouse_Up;

typedef struct _Eon_Event_Mouse_Down
{
} Eon_Event_Mouse_Down;

typedef struct _Eon_Event_Mouse_Move
{
} Eon_Event_Mouse_Move;

typedef struct _Eon_Event_Mouse_In
{
} Eon_Event_Mouse_In;

typedef struct _Eon_Event_Mouse_Out
{
} Eon_Event_Mouse_Out;


/**
 * @}
 */

#endif
