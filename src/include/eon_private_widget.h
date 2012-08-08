#ifndef _EON_WIDGET_PRIVATE_H
#define _EON_WIDGET_PRIVATE_H

typedef void (*Eon_Widget_Hints_Get)(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints);

typedef struct _Eon_Widget_Descriptor
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
} Eon_Widget_Descriptor;

Eon_Element * eon_widget_new(Eon_Theme_Instance *theme,
		Eon_Widget_Descriptor *descriptor, void *data);
void * eon_widget_data_get(Eon_Element *e);

Eon_Theme_Instance * eon_widget_theme_instance_get(Eon_Element *e);

#endif
