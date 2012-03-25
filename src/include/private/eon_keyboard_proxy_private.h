#ifndef EON_KEYBOARD_PROXY_PRIVATE_H
#define EON_KEYBOARD_PROXY_PRIVATE_H

typedef struct _Eon_Keyboard_Proxy Eon_Keyboard_Proxy;
typedef void (*Eon_Keyboard_Proxy_Key_Up)(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key);
typedef void (*Eon_Keyboard_Proxy_Key_Down)(void *data, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key);
typedef Eina_Bool (*Eon_Keyboard_Proxy_Destroy)(void *data);

typedef struct _Eon_Keyboard_Proxy_Descriptor
{
	Eon_Keyboard_Proxy_Key_Up key_up;
	Eon_Keyboard_Proxy_Key_Down key_down;
	Eon_Keyboard_Proxy_Destroy destroy;
} Eon_Keyboard_Proxy_Descriptor;

Eon_Keyboard_Proxy * eon_keyboard_proxy_new(Eon_Keyboard_Proxy_Descriptor *descriptor, void *data);
void eon_keyboard_proxy_delete(Eon_Keyboard_Proxy *thiz);
void eon_keyboard_proxy_feed_key_up(Eon_Keyboard_Proxy *thiz, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key);
void eon_keyboard_proxy_feed_key_down(Eon_Keyboard_Proxy *thiz, Ender_Element *current, Eon_Input *input, Ender_Element *from, const char *key);


#endif
