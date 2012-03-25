#ifndef EON_KEYBOARD_PROXY_NAVIGATION_PRIVATE_H
#define EON_KEYBOARD_PROXY_NAVIGATION_PRIVATE_H

typedef Ender_Element * (*Eon_Keyboard_Proxy_Navigation_Get)(void *data, Ender_Element *current);

typedef enum _Eon_Navigation_Key
{
	EON_NAVIGATION_KEY_TAB,
	EON_NAVIGATION_KEY_REVERSE_TAB,
	EON_NAVIGATION_KEY_LEFT,
	EON_NAVIGATION_KEY_RIGHT,
	EON_NAVIGATION_KEY_UP,
	EON_NAVIGATION_KEY_DOWN,
} Eon_Navigation_Key;

typedef struct _Eon_Keyboard_Proxy_Navigation_Descriptor
{
	Eon_Keyboard_Proxy_Navigation_Get tab;
	Eon_Keyboard_Proxy_Navigation_Get reverse_tab;
	Eon_Keyboard_Proxy_Navigation_Get up;
	Eon_Keyboard_Proxy_Navigation_Get down;
	Eon_Keyboard_Proxy_Navigation_Get left;
	Eon_Keyboard_Proxy_Navigation_Get right;
} Eon_Keyboard_Proxy_Navigation_Descriptor;

Eon_Keyboard_Proxy * eon_keyboard_proxy_navigation_new(Eon_Keyboard_Proxy_Navigation_Descriptor *descriptor, void *data);

#endif

