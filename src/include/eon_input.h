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
	EON_INPUT_EVENT_MOUSE_WHEEL,
	EON_INPUT_EVENT_MOUSE_CLICK,
	EON_INPUT_EVENT_MOUSE_DRAG_START,
	EON_INPUT_EVENT_MOUSE_DRAG_STOP,
	EON_INPUT_EVENT_KEY_DOWN,
	EON_INPUT_EVENT_KEY_UP,
	EON_INPUT_EVENTS,
} Eon_Input_Event;

typedef enum _Eon_Input_Modifiers
{
	EON_INPUT_MOD_NONE 	= 0,
	EON_INPUT_MOD_LSHIFT	= (1 << 0),
	EON_INPUT_MOD_RSHIFT	= (1 << 1),
	EON_INPUT_MOD_LCTRL	= (1 << 2),
	EON_INPUT_MOD_RCTRL	= (1 << 3),
	EON_INPUT_MOD_LALT	= (1 << 4),
	EON_INPUT_MOD_RALT	= (1 << 5),
	EON_INPUT_MOD_LMETA	= (1 << 6),
	EON_INPUT_MOD_RMETA	= (1 << 7),
} Eon_Input_Modifiers;

/* some helpers */
typedef enum _Eon_Input_Event_Mask
{
	EON_INPUT_EVENT_MASK_MOUSE_MOVE 	= (1 << EON_INPUT_EVENT_MOUSE_MOVE),
	EON_INPUT_EVENT_MASK_MOUSE_IN 		= (1 << EON_INPUT_EVENT_MOUSE_IN),
	EON_INPUT_EVENT_MASK_MOUSE_OUT 		= (1 << EON_INPUT_EVENT_MOUSE_OUT),
	EON_INPUT_EVENT_MASK_MOUSE_DOWN 	= (1 << EON_INPUT_EVENT_MOUSE_DOWN),
	EON_INPUT_EVENT_MASK_MOUSE_UP 		= (1 << EON_INPUT_EVENT_MOUSE_UP),
	EON_INPUT_EVENT_MASK_MOUSE_WHEEL	= (1 << EON_INPUT_EVENT_MOUSE_WHEEL),
	EON_INPUT_EVENT_MASK_MOUSE_CLICK 	= (1 << EON_INPUT_EVENT_MOUSE_CLICK),
	EON_INPUT_EVENT_MASK_MOUSE_DRAG_START	= (1 << EON_INPUT_EVENT_MOUSE_DRAG_START),
	EON_INPUT_EVENT_MASK_MOUSE_DRAG_STOP	= (1 << EON_INPUT_EVENT_MOUSE_DRAG_STOP),
	EON_INPUT_EVENT_MASK_KEY_DOWN 		= (1 << EON_INPUT_EVENT_KEY_DOWN),
	EON_INPUT_EVENT_MASK_KEY_UP 		= (1 << EON_INPUT_EVENT_KEY_UP),
} Eon_Input_Event_Mask;

extern const char * eon_input_event_names[EON_INPUT_EVENTS];

/**
 * @}
 * @{
 */

typedef struct _Eon_Event_Mouse_Click
{
	Eon_Input *input;
	double x;
	double y;
	double rel_x;
	double rel_y;
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
	double x;
	double y;
	double rel_x;
	double rel_y;
	double offset_x;
	double offset_y;
} Eon_Event_Mouse_Move;

typedef struct _Eon_Event_Mouse_In
{
	Eon_Input *input;
} Eon_Event_Mouse_In;

typedef struct _Eon_Event_Mouse_Out
{
	Eon_Input *input;
} Eon_Event_Mouse_Out;

typedef struct _Eon_Event_Mouse_Wheel
{
	Eon_Input *input;
} Eon_Event_Mouse_Wheel;

typedef struct _Eon_Event_Mouse_Drag_Start
{
	Eon_Input *input;
	double x;
	double y;
	double rel_x;
	double rel_y;
} Eon_Event_Mouse_Drag_Start;

typedef struct _Eon_Event_Mouse_Drag_Stop
{
	Eon_Input *input;
} Eon_Event_Mouse_Drag_Stop;

typedef struct _Eon_Event_Key_Down
{
	Eon_Input *input;
} Eon_Event_Key_Down;

typedef struct _Eon_Event_Key_Up
{
	Eon_Input *input;
} Eon_Event_Key_Up;

typedef struct _Eon_Event_Selected
{
	Eina_Bool selected;
} Eon_Event_Selected;


/**
 * @}
 */

#endif
