#ifndef _EON_BUTTON_BASE_PRIVATE_H
#define _EON_BUTTON_BASE_PRIVATE_H

typedef struct _Eon_Button_Base_Descriptor
{
	/* element interface */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Geometry_Set geometry_set;
	Eon_Element_Free free;
	const char *name;
	/* widget interface */
	Eon_Widget_Hints_Get hints_get;
	/* container interface */
	Eon_Container_Child_At child_at;
	/* bin interface */
	Eina_Bool pass_events;
} Eon_Button_Base_Descriptor;

Eon_Element * eon_button_base_new(Eon_Theme_Instance *theme, Eon_Button_Base_Descriptor *descriptor, void *data);
void * eon_button_base_data_get(Eon_Element *ee);
void eon_button_base_init(void);

#endif

