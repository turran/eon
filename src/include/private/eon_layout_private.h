#ifndef _EON_LAYOUT_PRIVATE_H
#define _EON_LAYOUT_PRIVATE_H

typedef void (*Eon_Layout_Child_Add)(Enesim_Renderer *r, Ender_Element *child);
typedef void (*Eon_Layout_Child_Remove)(Enesim_Renderer *r, Ender_Element *child);
typedef void (*Eon_Layout_Child_Clear)(Enesim_Renderer *r);
typedef Ender_Element * (*Eon_Layout_Child_At)(Ender_Element *e, double x, double y);

typedef struct _Eon_Layout_Descriptor
{
	Eon_Element_Min_Width_Get min_width_get;
	Eon_Element_Min_Height_Get min_height_get;
	Eon_Element_Preferred_Width_Get preferred_width_get;
	Eon_Element_Preferred_Height_Get preferred_height_get;
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Layout_Child_Add child_add;
	Eon_Layout_Child_Remove child_remove;
	Eon_Layout_Child_Clear child_clear;
	Eon_Layout_Child_At child_at;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Layout_Descriptor;

Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *descriptor,
		void *data);
void * eon_layout_data_get(Enesim_Renderer *r);
void eon_layout_actual_size_get(Ender_Element *e, Eon_Size *size);
void eon_layout_actual_width_get(Ender_Element *e, double *width);
void eon_layout_actual_height_get(Ender_Element *e, double *height);

void eon_layout_child_real_width_get(Ender_Element *e, Ender_Element *child, double *width);
void eon_layout_child_real_height_get(Ender_Element *e, Ender_Element *child, double *height);

Eon_Input_State * eon_layout_input_state_new(Ender_Element *e, Eon_Input *i);

#endif
