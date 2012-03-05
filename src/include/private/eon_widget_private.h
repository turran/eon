#ifndef _EON_WIDGET_PRIVATE_H
#define _EON_WIDGET_PRIVATE_H

typedef double (*Eon_Widget_Max_Width_Get)(Ender_Element *e, Enesim_Renderer *theme);
typedef double (*Eon_Widget_Min_Width_Get)(Ender_Element *e, Enesim_Renderer *theme);
typedef double (*Eon_Widget_Max_Height_Get)(Ender_Element *e, Enesim_Renderer *theme);
typedef double (*Eon_Widget_Min_Height_Get)(Ender_Element *e, Enesim_Renderer *theme);
typedef double (*Eon_Widget_Preferred_Width_Get)(Ender_Element *e, Enesim_Renderer *theme);
typedef double (*Eon_Widget_Preferred_Height_Get)(Ender_Element *e, Enesim_Renderer *theme);

typedef struct _Eon_Widget_Descriptor
{
	Eon_Widget_Min_Width_Get min_width_get;
	Eon_Widget_Max_Width_Get max_width_get;
	Eon_Widget_Min_Height_Get min_height_get;
	Eon_Widget_Max_Height_Get max_height_get;
	Eon_Widget_Preferred_Width_Get preferred_width_get;
	Eon_Widget_Preferred_Height_Get preferred_height_get;
	Eon_Element_Initialize initialize;
	Eon_Element_Cleanup cleanup;
	Eon_Element_Setup setup;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Damage damage;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Widget_Descriptor;

Enesim_Renderer * eon_widget_new(Eon_Widget_Descriptor *descriptor,
		void *data);
void * eon_widget_data_get(Enesim_Renderer *r);
Ender_Element * eon_widget_theme_element_get(Enesim_Renderer *r);
Enesim_Renderer * eon_widget_theme_renderer_get(Enesim_Renderer *r);
Escen_Instance * eon_widget_theme_instance_get(Enesim_Renderer *r);
Escen_Ender * eon_widget_theme_ender_get(Enesim_Renderer *r);
void eon_widget_property_clear(Enesim_Renderer *r, const char *name);
void eon_widget_property_set(Enesim_Renderer *r, const char *name, ...);
void eon_widget_property_get(Enesim_Renderer *r, const char *name, ...);
void eon_widget_property_add(Enesim_Renderer *r, const char *name, ...);
void eon_widget_property_remove(Enesim_Renderer *r, const char *name, ...);
void eon_widget_state_set(Enesim_Renderer *r, const char *name, Eina_Bool be_finalized);

#endif
