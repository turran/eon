#ifndef _EON_LAYOUT_PRIVATE_H
#define _EON_LAYOUT_PRIVATE_H

typedef void (*Eon_Layout_Child_Add)(Enesim_Renderer *r, Ender_Element *child);
typedef void (*Eon_Layout_Child_Remove)(Enesim_Renderer *r, Ender_Element *child);
typedef void (*Eon_Layout_Child_Clear)(Enesim_Renderer *r);
typedef Ender_Element * (*Eon_Layout_Child_At)(Enesim_Renderer *r, double x, double y);

typedef struct _Eon_Layout_Descriptor
{
	Eon_Layout_Child_Add child_add;
	Eon_Layout_Child_Remove child_remove;
	Eon_Layout_Child_Clear child_clear;
	Eon_Layout_Child_At child_at;
	Eon_Element_Initialize initialize;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Layout_Descriptor;

Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *descriptor,
		void *data);
void * eon_layout_data_get(Enesim_Renderer *r);

#endif
