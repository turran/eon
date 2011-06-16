#ifndef _EON_ELEMENT_PRIVATE_H
#define _EON_ELEMENT_PRIVATE_H

typedef void (*Eon_Element_Initialize)(Ender_Element *ender);
typedef void (*Eon_Element_Free)(Enesim_Renderer *r);

typedef struct _Eon_Element_Descriptor
{
	Eon_Element_Initialize initialize;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Element_Descriptor;

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data);
void * eon_element_data_get(Enesim_Renderer *r);
void eon_element_initialize(Ender_Element *ender);

#endif
