#ifndef _EON_THEME_BUTTON_BASE_PRIVATE_H_
#define _EON_THEME_BUTTON_BASE_PRIVATE_H_

typedef struct _Eon_Theme_Button_Base_Descriptor
{
	/* button base needed functions */
	Eon_Theme_Button_Base_Content_Position_Get content_position_get;
	Eon_Theme_Button_Base_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Button_Base_Decoration_Height_Get decoration_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Button_Base_Descriptor;

Enesim_Renderer * eon_theme_button_base_new(Eon_Theme_Button_Base_Descriptor *descriptor, void *data);
Eina_Bool eon_is_theme_button_base(Enesim_Renderer *r);
void * eon_theme_button_base_data_get(Enesim_Renderer *r);
void eon_theme_button_base_decoration_size_get(Enesim_Renderer *r, double *w, double *h);
void eon_theme_button_base_content_position_get(Enesim_Renderer *r, double *x, double *y);

#endif

