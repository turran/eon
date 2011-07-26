#ifndef _EON_THEME_WIDGET_PRIVATE_H_
#define _EON_THEME_WIDGET_PRIVATE_H_

typedef struct _Eon_Theme_Widget_Descriptor
{
	Eon_Theme_Widget_Max_Width_Get max_width_get;
	Eon_Theme_Widget_Min_Width_Get min_width_get;
	Eon_Theme_Widget_Max_Height_Get max_height_get;
	Eon_Theme_Widget_Min_Height_Get min_height_get;
	Eon_Theme_Widget_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Widget_Preferred_Height_Get preferred_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Widget_Descriptor;

Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor, void *data);
Eina_Bool eon_is_theme_widget(Enesim_Renderer *r);
void * eon_theme_widget_data_get(Enesim_Renderer *r);

#endif
