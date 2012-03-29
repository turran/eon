#ifndef EON_MOUSE_PROXY_PRIVATE_H
#define EON_MOUSE_PROXY_PRIVATE_H

typedef struct _Eon_Mouse_Proxy Eon_Mouse_Proxy;

typedef void (*Eon_Mouse_Proxy_Move)(void *data,
		Ender_Element *e,
		Eon_Input *input,
		double x, double y, double offset_x, double offset_y);
typedef void (*Eon_Mouse_Proxy_Button_Down)(void *data,
		Ender_Element *e,
		Eon_Input *input,
		int button);
typedef void (*Eon_Mouse_Proxy_Button_Up)(void *data,
		Ender_Element *e,
		Eon_Input *input,
		int button);
typedef void (*Eon_Mouse_Proxy_Wheel)(void *data,
		Ender_Element *e,
		Eon_Input *input,
		int direction);

typedef Eina_Bool (*Eon_Mouse_Proxy_Destroy)(void *data);

typedef struct _Eon_Mouse_Proxy_Descriptor
{
	Eon_Mouse_Proxy_Button_Down button_down;
	Eon_Mouse_Proxy_Button_Up button_up;
	Eon_Mouse_Proxy_Move move;
	Eon_Mouse_Proxy_Wheel wheel;
	Eon_Mouse_Proxy_Destroy destroy;
} Eon_Mouse_Proxy_Descriptor;

void eon_mouse_proxy_feed_move(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		double x, double y, double offset_x, double offset_y);

void eon_mouse_proxy_feed_button_down(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		int button);
void eon_mouse_proxy_feed_button_up(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		int button);
void eon_mouse_proxy_feed_wheel(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		int direction);


#endif
