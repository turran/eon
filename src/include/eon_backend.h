#ifndef _EON_BACKEND_H_
#define _EON_BACKEND_H_

typedef struct _Eon_Window Eon_Window;
typedef struct _Eon_Backend Eon_Backend;

typedef Eina_Bool (*Eon_Backend_Idler)(void *data);

Eon_Window * eon_backend_window_new(Eon_Backend *backend, Ender_Element *layout,
		unsigned int width, unsigned int height);

EAPI void eon_backend_idler_add(Eon_Backend *backend, Eon_Backend_Idler cb, void *data);
EAPI void eon_backend_run(Eon_Backend *backend);
EAPI void eon_backend_delete(Eon_Backend *backend);
EAPI void eon_backend_quit(Eon_Backend *backend);

#endif