#ifndef _EON_WINDOW_H_
#define _EON_WINDOW_H_

typedef struct _Eon_Window Eon_Window;
typedef struct _Eon_Backend Eon_Backend;

Eon_Window * eon_window_new(Eon_Backend *backend, Ender_Element *layout,
		unsigned int width, unsigned int height);

#endif
