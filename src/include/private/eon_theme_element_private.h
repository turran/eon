#ifndef _EON_THEME_ELEMENT_PRIVATE_H_
#define _EON_THEME_ELEMENT_PRIVATE_H_

typedef struct _Eon_Theme_Element_Descriptor
{
	Eon_Theme_Element_Max_Width_Get max_width_get;
	Eon_Theme_Element_Min_Width_Get min_width_get;
	Eon_Theme_Element_Max_Height_Get max_height_get;
	Eon_Theme_Element_Min_Height_Get min_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Element_Descriptor;

Enesim_Renderer * eon_theme_element_new(Eon_Theme_Element_Descriptor *descriptor,
		void *data);
Eina_Bool eon_is_theme_element(Enesim_Renderer *r);
void * eon_theme_element_data_get(Enesim_Renderer *r);

#endif
