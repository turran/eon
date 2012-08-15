#ifndef _EON_BACKEND_PRIVATE_H
#define _EON_BACKEND_PRIVATE_H

typedef struct _Eon_Backend_Token Eon_Backend_Token;

typedef void (*Eon_Backend_Idler_Add)(void *backend_data, Eon_Backend_Idler idler, void *data);
typedef void (*Eon_Backend_Run)(void *backend_data);
typedef void (*Eon_Backend_Quit)(void *backend_data);
typedef Eina_Bool (*Eon_Backend_Window_New)(void *backend_data, Ender_Element *layout, unsigned int width, unsigned int height, void **window_data);
typedef void (*Eon_Backend_Window_Delete)(void *backend_data, void *window_data);

typedef struct _Eon_Backend_Descriptor
{
	/* main loop functions */
	Eon_Backend_Idler_Add idler_add;
	Eon_Backend_Run run;
	Eon_Backend_Quit quit;
	/* thread functions */
	/* window related functions */
	Eon_Backend_Window_New window_new;
	Eon_Backend_Window_Delete window_delete;
} Eon_Backend_Descriptor;

Eon_Backend * eon_backend_new(Eon_Backend_Descriptor *descriptor, void *data);
Eina_Bool eon_backend_window_new(Eon_Backend *backend, Ender_Element *layout,
		unsigned int width, unsigned int height, void *window_data);
void eon_backend_window_delete(Eon_Backend *backend, void *window_data);

#endif
