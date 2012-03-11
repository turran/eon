#ifndef _EON_THEME_BUTTON_BASE_PRIVATE_H_
#define _EON_THEME_BUTTON_BASE_PRIVATE_H_

typedef struct _Eon_Theme_Button_Base_Descriptor
{
	/* button base needed functions */
	Eon_Theme_Button_Base_Margin_Get margin_get;
	Eon_Theme_Button_Base_Position_Get position_get;
	/* renderer needed functions */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Container_Setup setup;
	Enesim_Renderer_Name name;
	Enesim_Renderer_Delete free;
} Eon_Theme_Button_Base_Descriptor;

Enesim_Renderer * eon_theme_button_base_new(Eon_Theme_Button_Base_Descriptor *descriptor, void *data);
Eina_Bool eon_is_theme_button_base(Enesim_Renderer *r);
void * eon_theme_button_base_data_get(Enesim_Renderer *r);
void eon_theme_button_base_margin_get(Enesim_Renderer *r, Eon_Margin *margin);
void eon_theme_button_base_position_get(Enesim_Renderer *r, Eon_Size *size, Eon_Position *position);

#endif

