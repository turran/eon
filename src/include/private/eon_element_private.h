#ifndef _EON_ELEMENT_PRIVATE_H
#define _EON_ELEMENT_PRIVATE_H

typedef struct _Eon_Element_State
{
	Eon_Size preferred_size;
	Eon_Size max_size;
	Eon_Size min_size;
	Eon_Size size;
	Eon_Position actual_position;
	Eon_Size actual_size;
} Eon_Element_State;

/* TODO rename all this max/min/preferred width/height into min/max/preferred size only */
typedef void (*Eon_Element_Initialize)(Ender_Element *ender);
typedef Eina_Bool (*Eon_Element_Setup)(Ender_Element *ender);
typedef void (*Eon_Element_Free)(Enesim_Renderer *r);
typedef double (*Eon_Element_Min_Width_Get)(Ender_Element *e);
typedef double (*Eon_Element_Max_Width_Get)(Ender_Element *e);
typedef double (*Eon_Element_Min_Height_Get)(Ender_Element *e);
typedef double (*Eon_Element_Max_Height_Get)(Ender_Element *e);
typedef double (*Eon_Element_Preferred_Height_Get)(Ender_Element *e);
typedef double (*Eon_Element_Preferred_Width_Get)(Ender_Element *e);
typedef void (*Eon_Element_Actual_X_Set)(Enesim_Renderer *r, double x);
typedef void (*Eon_Element_Actual_Y_Set)(Enesim_Renderer *r, double y);
typedef void (*Eon_Element_Actual_Width_Set)(Enesim_Renderer *r, double width);
typedef void (*Eon_Element_Actual_Height_Set)(Enesim_Renderer *r, double height);
typedef Enesim_Renderer* (*Eon_Element_Renderer_Get)(Ender_Element *e);

typedef struct _Eon_Element_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Renderer_Get renderer_get;
	Eon_Element_Min_Width_Get min_width_get;
	Eon_Element_Max_Width_Get max_width_get;
	Eon_Element_Min_Height_Get min_height_get;
	Eon_Element_Max_Height_Get max_height_get;
	Eon_Element_Preferred_Width_Get preferred_width_get;
	Eon_Element_Preferred_Height_Get preferred_height_get;
	Eon_Element_Actual_X_Set actual_x_set;
	Eon_Element_Actual_Y_Set actual_y_set;
	Eon_Element_Actual_Width_Set actual_width_set;
	Eon_Element_Actual_Height_Set actual_height_set;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
	const char *name;
} Eon_Element_Descriptor;

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data);
void * eon_element_data_get(Enesim_Renderer *r);
void eon_element_initialize(Ender_Element *ender);

/* TODO the issue here is that this functions need to be exported
 * so the theme developer can set the content position, maybe we should
 * provide some interface for getting the desired x,y of the content
 * from the theme?
 */
void eon_element_actual_x_set(Enesim_Renderer *r, double x);
void eon_element_actual_y_set(Enesim_Renderer *r, double y);
void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height);
void eon_element_actual_size_get(Enesim_Renderer *r, Eon_Size *size);
void eon_element_actual_width_set(Enesim_Renderer *r, double width);
void eon_element_actual_height_set(Enesim_Renderer *r, double height);
void eon_element_actual_position_get(Enesim_Renderer *r, double *x, double *y);
void eon_element_actual_position_set(Enesim_Renderer *r, double x, double y);

void eon_element_real_width_get(Ender_Element *e, double *width);
void eon_element_real_height_get(Ender_Element *e, double *height);
void eon_element_real_relative_size_get(Ender_Element *e, Eon_Size *relative, Eon_Size *size);
void eon_element_real_size_get(Ender_Element *e, Eon_Size *size);

void eon_element_changed_set(Ender_Element *e, Eina_Bool changed);
Eina_Bool eon_element_has_changed(Ender_Element *e);

Eina_Bool eon_element_setup(Ender_Element *e);

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e);

#endif
