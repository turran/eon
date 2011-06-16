#ifndef _EON_WIDGET_PRIVATE_H
#define _EON_WIDGET_PRIVATE_H

typedef struct _Eon_Widget_Descriptor
{
	Eon_Element_Initialize initialize;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Widget_Descriptor;

Enesim_Renderer * eon_widget_new(Eon_Widget_Descriptor *descriptor,
		void *data);
void * eon_widget_data_get(Enesim_Renderer *r);

#endif
