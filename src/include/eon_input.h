#ifndef _EON_INPUT_H_
#define _EON_INPUT_H_

/**
 * @{
 */
typedef struct _Eon_Input Eon_Input;

typedef enum _Eon_Input_Event
{
	EON_INPUT_EVENT_MOUSE_MOVE,
	EON_INPUT_EVENT_MOUSE_IN,
	EON_INPUT_EVENT_MOUSE_OUT, 
	EON_INPUT_EVENT_MOUSE_DOWN,
	EON_INPUT_EVENT_MOUSE_UP,
	EON_INPUT_EVENT_MOUSE_CLICK,
	EON_INPUT_EVENT_MOUSE_DRAG_START,
	EON_INPUT_EVENT_MOUSE_DRAG_STOP,
	EON_INPUT_EVENTS,
} Eon_Input_Event;

/* some helpers */
typedef enum _Eon_Input_Event_Mask
{
	EON_INPUT_EVENT_MASK_MOUSE_MOVE 	= (1 << EON_INPUT_EVENT_MOUSE_MOVE),
	EON_INPUT_EVENT_MASK_MOUSE_IN 		= (1 << EON_INPUT_EVENT_MOUSE_IN),
	EON_INPUT_EVENT_MASK_MOUSE_OUT 		= (1 << EON_INPUT_EVENT_MOUSE_OUT),
	EON_INPUT_EVENT_MASK_MOUSE_DOWN 	= (1 << EON_INPUT_EVENT_MOUSE_DOWN),
	EON_INPUT_EVENT_MASK_MOUSE_UP 		= (1 << EON_INPUT_EVENT_MOUSE_UP),
	EON_INPUT_EVENT_MASK_MOUSE_CLICK 	= (1 << EON_INPUT_EVENT_MOUSE_CLICK),
	EON_INPUT_EVENT_MASK_MOUSE_DRAG_START	= (1 << EON_INPUT_EVENT_MOUSE_DRAG_START),
	EON_INPUT_EVENT_MASK_MOUSE_DRAG_STOP	= (1 << EON_INPUT_EVENT_MOUSE_DRAG_STOP),
} Eon_Input_Event_Mask;

extern const char * eon_input_event_names[EON_INPUT_EVENTS];

/**
 * @}
 * @{
 */

typedef struct _Eon_Event_Mouse_Click
{
	Eon_Input *input;
} Eon_Event_Mouse_Click;

typedef struct _Eon_Event_Mouse_Up
{
	Eon_Input *input;
} Eon_Event_Mouse_Up;

typedef struct _Eon_Event_Mouse_Down
{
	Eon_Input *input;
} Eon_Event_Mouse_Down;

typedef struct _Eon_Event_Mouse_Move
{
	Eon_Input *input;
} Eon_Event_Mouse_Move;

typedef struct _Eon_Event_Mouse_In
{
	Eon_Input *input;
} Eon_Event_Mouse_In;

typedef struct _Eon_Event_Mouse_Out
{
	Eon_Input *input;
} Eon_Event_Mouse_Out;


/**
 * @}
 */

#endif
