#ifndef _EON_CONTAINER_PRIVATE_H
#define _EON_CONTAINER_PRIVATE_H

typedef struct _Eon_Container_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Container_Descriptor;

Enesim_Renderer * eon_container_new(Eon_Container_Descriptor *descriptor, void *data);
void * eon_container_data_get(Enesim_Renderer *r);

#endif
