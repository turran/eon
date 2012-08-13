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
typedef Eina_Bool (*Eon_Element_Setup)(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **error);
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
/**
 * Every element needs a way to inform whenever something internal has changed
 * in a way that the caller needs to call the Eon_Element_Setup function on this element
 * to make it work properly.
 */
typedef Eina_Bool (*Eon_Element_Needs_Setup)(Ender_Element *e);
/**
 * This function gives you back the damaged area of an element between two setup/draw/cleanup combo
 * Whenever we setup an element we usually set some properties on the graphics representation
 * of an element. The simplest case is the size, but the element can decide to change whatever
 * property it wants. Whenever enesim requires the damages of an element we cannot give back
 * the graphical representation damages, as it might not have all its properties correctly setup
 */

typedef struct _Eon_Element_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Renderer_Get renderer_get;
	Eon_Element_Needs_Setup needs_setup;
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
void eon_element_hints_get(Eon_Element *thiz, Eon_Size *min, Eon_Size *max, Eon_Size *preferred);
void eon_element_geometry_set(Eon_Element *thiz, Eon_Geometry *g);

void eon_element_initialize(Ender_Element *ender);
void eon_element_real_width_get(Ender_Element *e, double *width);
void eon_element_real_height_get(Ender_Element *e, double *height);
void eon_element_real_relative_size_get(Ender_Element *e, Eon_Size *relative, Eon_Size *size);
void eon_element_real_size_get(Ender_Element *e, Eon_Size *size);

Eina_Bool eon_element_needs_setup(Ender_Element *e);

Eina_Bool eon_element_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **err);
void eon_element_cleanup(Ender_Element *e, Enesim_Surface *s);

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e);

void eon_element_feed_key_down(Ender_Element *e, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key);
void eon_element_feed_key_up(Ender_Element *e, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key);


/* parent - child relation */
void eon_element_parent_set(Ender_Element *e, Ender_Element *parent, void *data, Eon_Free_Func free_func);

#endif
