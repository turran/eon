#ifndef _EON_ELEMENT_PRIVATE_H
#define _EON_ELEMENT_PRIVATE_H

typedef void (*Eon_Element_Initialize)(Ender_Element *ender);
typedef Eina_Bool (*Eon_Element_Setup)(Ender_Element *ender);
typedef void (*Eon_Element_Free)(Enesim_Renderer *r);
typedef double (*Eon_Element_Min_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Element_Max_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Element_Min_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Element_Max_Height_Get)(Enesim_Renderer *r);

typedef struct _Eon_Element_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Min_Width_Get min_width_get;
	Eon_Element_Max_Width_Get max_width_get;
	Eon_Element_Min_Height_Get min_height_get;
	Eon_Element_Max_Height_Get max_height_get;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Element_Descriptor;

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data);
void * eon_element_data_get(Enesim_Renderer *r);
void eon_element_initialize(Ender_Element *ender);
Ender_Element * eon_element_theme_element_get(Enesim_Renderer *r);
Enesim_Renderer * eon_element_theme_renderer_get(Enesim_Renderer *r);
Escen_Instance * eon_element_theme_instance_get(Enesim_Renderer *r);
Escen_Ender * eon_element_theme_ender_get(Enesim_Renderer *r);
void eon_element_property_set(Enesim_Renderer *r, const char *name, ...);
void eon_element_property_get(Enesim_Renderer *r, const char *name, ...);
void eon_element_property_add(Enesim_Renderer *r, const char *name, ...);
void eon_element_property_remove(Enesim_Renderer *r, const char *name, ...);

/* TODO the issue here is that this functions need to be exported
 * so the theme developer can set the content position, maybe we should
 * provide some interface for getting the desired x,y of the content
 * from the theme?
 */
void eon_element_actual_x_set(Enesim_Renderer *r, double x);
void eon_element_actual_y_set(Enesim_Renderer *r, double y);
void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height);
void eon_element_actual_size_get(Enesim_Renderer *r, double *width, double *height);
void eon_element_actual_width_set(Enesim_Renderer *r, double width);
void eon_element_actual_height_set(Enesim_Renderer *r, double height);
void eon_element_actual_position_set(Enesim_Renderer *r, double x, double y);

Eina_Bool eon_element_setup(Ender_Element *e);
void eon_element_real_width_get(Ender_Element *e, double *width);
void eon_element_real_height_get(Ender_Element *e, double *height);
void eon_element_changed_set(Ender_Element *e, Eina_Bool changed);
Eina_Bool eon_element_has_changed(Ender_Element *e);

#endif
