#ifndef _EON_INPUT_PRIVATE_H_
#define _EON_INPUT_PRIVATE_H_

typedef struct _Eon_Keyboard_Key
{
	const char *name;
	Eon_Input_Modifiers mods;
} Eon_Keyboard_Key;

typedef enum _Eon_Navigation_Key
{
	EON_NAVIGATION_KEY_TAB,
	EON_NAVIGATION_KEY_REVERSE_TAB,
	EON_NAVIGATION_KEY_LEFT,
	EON_NAVIGATION_KEY_RIGHT,
	EON_NAVIGATION_KEY_UP,
	EON_NAVIGATION_KEY_DOWN,
	EON_NAVIGATION_KEY_OK,
	EON_NAVIGATION_KEY_CANCEL,
} Eon_Navigation_Key;

typedef struct _Eon_Input_State Eon_Input_State;
typedef Ender_Element * (*Eon_Input_State_Element_Get)(Ender_Element *e, double x, double y);
typedef Ender_Element * (*Eon_Input_State_Element_Next)(Ender_Element *e, Ender_Element *current);
typedef Ender_Element * (*Eon_Input_State_Element_Prev)(Ender_Element *e, Ender_Element *current);

typedef struct _Eon_Input_State_Descriptor
{
	Eon_Input_State_Element_Get element_get;
	Eon_Input_State_Element_Next element_next;
	Eon_Input_State_Element_Prev element_prev;
} Eon_Input_State_Descriptor;

Eon_Input * eon_input_new(void);

Eon_Input_State * eon_input_state_new(Eon_Input *i, Ender_Element *e, Eon_Input_State_Descriptor *descriptor);
void eon_input_state_feed_mouse_move(Eon_Input_State *eis,
		double x, double y, double offset_x, double offset_y);
void eon_input_state_feed_mouse_in(Eon_Input_State *eis);
void eon_input_state_feed_mouse_out(Eon_Input_State *eis);
void eon_input_state_feed_mouse_down(Eon_Input_State *eis);
void eon_input_state_feed_mouse_up(Eon_Input_State *eis);
void eon_input_state_feed_mouse_wheel(Eon_Input_State *eis, int direction);
void eon_input_state_feed_key_down(Eon_Input_State *eis, Eon_Keyboard_Key *key);
void eon_input_state_feed_key_up(Eon_Input_State *eis, Eon_Keyboard_Key *key);

typedef struct _Eon_Input_Proxy Eon_Input_Proxy;
Eon_Input_Proxy * eon_input_proxy_new(Ender_Element *e,
		Eon_Input_State_Descriptor *descriptor);
void eon_input_proxy_delete(Eon_Input_Proxy *thiz);

Eina_Bool eon_input_navigation_key_get(Eon_Input *thiz,
		Eon_Keyboard_Key *key,
		Eon_Navigation_Key *nkey);

#endif
