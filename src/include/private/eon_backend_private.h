#ifndef _EON_BACKEND_PRIVATE_H
#define _EON_BACKEND_PRIVATE_H

typedef struct _Eon_Backend_Data
{
	Enesim_Surface *surface;
	void *prv;
} Eon_Backend_Data;

struct _Eon_Backend
{
	Eina_Bool (*setup)(Ender_Element *layout, unsigned int width, unsigned int height, Eon_Backend_Data *data);
	void (*cleanup)(Eon_Backend_Data *data);
	Eina_Bool (*flush)(Eon_Backend_Data *data, Enesim_Rectangle *area);
};

#endif
