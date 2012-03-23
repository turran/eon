#ifndef EON_KEYBOARD_PROXY_NAVIGATION_H
#define EON_KEYBOARD_PROXY_NAVIGATION_H

typedef void (*Eon_Keyboard_Proxy_Navigation_Tab)(void *data, Eon_Input *input);
typedef void (*Eon_Keyboard_Proxy_Navigation_Reverse_Tab)(void *data, Eon_Input *input);
typedef void (*Eon_Keyboard_Proxy_Navigation_Top)(void *data, Eon_Input *input);
typedef void (*Eon_Keyboard_Proxy_Navigation_Bottom)(void *data, Eon_Input *input);
typedef void (*Eon_Keyboard_Proxy_Navigation_Left)(void *data, Eon_Input *input);
typedef void (*Eon_Keyboard_Proxy_Navigation_Right)(void *data, Eon_Input *input);

typedef struct _Eon_Keyboard_Proxy_Navigation_Descriptor
{
	Eon_Keyboard_Proxy_Navigation_Tab tab;
	Eon_Keyboard_Proxy_Navigation_Reverse_Tab reverse_tab;
	Eon_Keyboard_Proxy_Navigation_Top top;
	Eon_Keyboard_Proxy_Navigation_Bottom bottom;
	Eon_Keyboard_Proxy_Navigation_Left left;
	Eon_Keyboard_Proxy_Navigation_Right right;
} Eon_Keyboard_Proxy_Navigation_Descriptor;

Eon_Keyboard_Proxy * eon_keyboard_proxy_navigation_new(Eon_Keyboard_Proxy_Navigation_Descriptor *descriptor, void *data);

#endif

