#ifndef _EON_CONTAINER_PRIVATE_H
#define _EON_CONTAINER_PRIVATE_H

typedef double (*Eon_Container_Min_Width_Get)(Ender_Element *e, double cmw);
typedef double (*Eon_Container_Max_Width_Get)(Ender_Element *e, double cmw);
typedef double (*Eon_Container_Min_Height_Get)(Ender_Element *e, double cmh);
typedef double (*Eon_Container_Max_Height_Get)(Ender_Element *e, double cmh);
typedef double (*Eon_Container_Preferred_Height_Get)(Ender_Element *e, double cmh);
typedef double (*Eon_Container_Preferred_Width_Get)(Ender_Element *e, double cmw);
typedef Ender_Element * (*Eon_Container_Element_At)(Ender_Element *e, double x, double y);

typedef struct _Eon_Container_Descriptor
{
	Eon_Container_Min_Width_Get min_width_get;
	Eon_Container_Max_Width_Get max_width_get;
	Eon_Container_Min_Height_Get min_height_get;
	Eon_Container_Max_Height_Get max_height_get;
	Eon_Container_Preferred_Width_Get preferred_width_get;
	Eon_Container_Preferred_Height_Get preferred_height_get;
	Eon_Container_Element_At element_at;
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	const char *name;
	Eina_Bool pass_events;
} Eon_Container_Descriptor;

Enesim_Renderer * eon_container_new(Eon_Container_Descriptor *descriptor, void *data);
void * eon_container_data_get(Enesim_Renderer *r);

#endif
