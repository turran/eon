#ifndef _EON_THEME_WIDGET_PRIVATE_H_
#define _EON_THEME_WIDGET_PRIVATE_H_

typedef struct _Eon_Theme_Widget_Descriptor
{
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Eon_Theme_Widget_Cleanup cleanup;
	/* renderer needed functions */
	Enesim_Renderer_Name name;
	Enesim_Renderer_Delete free;
} Eon_Theme_Widget_Descriptor;

Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor, void *data);
Eina_Bool eon_is_theme_widget(Enesim_Renderer *r);
void * eon_theme_widget_data_get(Enesim_Renderer *r);

#endif
