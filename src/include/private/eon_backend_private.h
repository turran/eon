#ifndef _EON_BACKEND_PRIVATE_H
#define _EON_BACKEND_PRIVATE_H

typedef struct _Eon_Backend_Descriptor
{
	Eina_Bool (*window_new)(void *backend_data, Ender_Element *layout, unsigned int width, unsigned int height, void **window_data);
	void (*window_delete)(void *backend_data, void *window_data);
} Eon_Backend_Descriptor;

Eon_Backend * eon_backend_new(Eon_Backend_Descriptor *descriptor, void *data);
Eina_Bool eon_backend_window_new(Eon_Backend *backend, Ender_Element *layout,
		unsigned int width, unsigned int height, void *window_data);
void eon_backend_window_delete(Eon_Backend *backend, void *window_data);

#endif
