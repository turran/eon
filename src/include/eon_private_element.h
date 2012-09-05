#ifndef _EON_ELEMENT_PRIVATE_H
#define _EON_ELEMENT_PRIVATE_H

typedef struct _Eon_Element Eon_Element;
typedef void (*Eon_Free_Func)(void *data);

typedef struct _Eon_Element_State
{
	Eon_Position actual_position;
	Eon_Size actual_size;
	Eina_Bool visible;
} Eon_Element_State;

typedef void (*Eon_Element_Initialize)(Ender_Element *e);
typedef void (*Eon_Element_Backend_Added)(Eon_Element *e, Eon_Backend *backend);
typedef void (*Eon_Element_Backend_Removed)(Eon_Element *e, Eon_Backend *backend);
typedef void (*Eon_Hints_Get)(Eon_Element *e, Eon_Hints *hints);
typedef void (*Eon_Element_Geometry_Set)(Eon_Element *e, Eon_Geometry *g);
typedef void (*Eon_Element_Free)(Eon_Element *e);
typedef Eina_Bool (*Eon_Element_Is_Focusable)(Eon_Element *e);
/**
 * On eon at the end is not the element itself which is drawn on the screen but the
 * graphics representation of it. This graphics representation is the renderer associated
 * with the element. This function is a way to retrieve such graphical representation.
 */
typedef Enesim_Renderer * (*Eon_Element_Renderer_Get)(Ender_Element *e);


typedef struct _Eon_Element_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Backend_Added backend_added;
	Eon_Element_Backend_Removed backend_removed;
	Eon_Element_Renderer_Get renderer_get;
	Eon_Hints_Get hints_get;
	Eon_Element_Geometry_Set geometry_set;
	Eon_Element_Is_Focusable is_focusable;
	Eon_Element_Free free;
	const char *name;
} Eon_Element_Descriptor;

Eon_Element * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data);
void * eon_element_data_get(Eon_Element *e);

/* new ones */
void eon_element_hints_get(Eon_Element *thiz, Eon_Hints *hints);
void eon_element_geometry_set(Eon_Element *thiz, Eon_Geometry *g);
void eon_element_geometry_get(Eon_Element *thiz, Eon_Geometry *g);
Eon_Backend * eon_element_backend_get(Eon_Element *thiz);
void eon_element_backend_set(Eon_Element *thiz, Eon_Backend *backend);
void eon_element_inform_change(Eon_Element *thiz);
Eina_Bool eon_element_has_changed(Eon_Element *thiz);

void eon_element_initialize(Ender_Element *ender);
void eon_element_real_width_get(Ender_Element *e, double *width);
void eon_element_real_height_get(Ender_Element *e, double *height);
void eon_element_real_relative_size_get(Ender_Element *e, Eon_Size *relative, Eon_Size *size);
void eon_element_real_size_get(Ender_Element *e, Eon_Size *size);

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e);

void eon_element_feed_key_down(Ender_Element *e, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key);
void eon_element_feed_key_up(Ender_Element *e, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key);


/* parent - child relation */
void eon_element_parent_set(Ender_Element *e, Ender_Element *parent, void *data, Eon_Free_Func free_func);

#endif
