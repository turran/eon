#ifndef _EON_CONTAINER_PRIVATE_H
#define _EON_CONTAINER_PRIVATE_H

typedef Eina_Bool (*Eon_Container_Child_Foreach_Cb)(Eon_Element *e, Ender_Element *child, void *data);

typedef Eina_Bool (*Eon_Container_Child_Add)(Eon_Element *ee, Ender_Element *child);
typedef Eina_Bool (*Eon_Container_Child_Remove)(Eon_Element *ee, Ender_Element *child);
typedef void (*Eon_Container_Child_Foreach)(Eon_Element *ee, Eon_Container_Child_Foreach_Cb cb, void *data);
typedef Ender_Element * (*Eon_Container_Child_At)(Ender_Element *e, double x, double y);

typedef struct _Eon_Container_Descriptor
{
	/* element interface */
	Eon_Element_Initialize initialize;
	Eon_Element_Backend_Added backend_added;
	Eon_Element_Backend_Removed backend_removed;
	Eon_Element_Geometry_Set geometry_set;
	Eon_Element_Free free;
	const char *name;
	/* widget interface */
	Eon_Widget_Hints_Get hints_get;
	/* container interface */
	Eon_Container_Child_Add child_add;
	Eon_Container_Child_Remove child_remove;
	Eon_Container_Child_Foreach child_foreach;
	Eon_Container_Child_At child_at;
} Eon_Container_Descriptor;

Eon_Element * eon_container_new(Eon_Theme_Instance *theme,
		Eon_Container_Descriptor *descriptor, void *data);
void * eon_container_data_get(Eon_Element *ee);

void eon_container_internal_child_foreach(Eon_Element *e, Eon_Container_Child_Foreach_Cb cb, void *user_data);

void eon_container_actual_size_get(Ender_Element *e, Eon_Size *size);
void eon_container_actual_width_get(Ender_Element *e, double *width);
void eon_container_actual_height_get(Ender_Element *e, double *height);

void eon_container_child_real_width_get(Ender_Element *e, Ender_Element *child, double *width);
void eon_container_child_real_height_get(Ender_Element *e, Ender_Element *child, double *height);

Eon_Input_State * eon_container_input_state_new(Ender_Element *e, Eon_Input *i);

#endif
