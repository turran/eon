#ifndef _EON_BUTTON_BASE_PRIVATE_H
#define _EON_BUTTON_BASE_PRIVATE_H

typedef struct _Eon_Button_Base_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Button_Base_Descriptor;

Enesim_Renderer * eon_button_base_new(Eon_Button_Base_Descriptor *descriptor, void *data);
void * eon_button_base_data_get(Enesim_Renderer *r);
void eon_button_base_init(void);

#endif

