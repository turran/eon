#ifndef _EON_THEME_CONTAINER_PRIVATE_H_
#define _EON_THEME_CONTAINER_PRIVATE_H_

typedef struct _Eon_Theme_Container_Descriptor
{
	/* container needed functions */
	Eon_Theme_Container_Content_Position_Get content_position_get;
	Eon_Theme_Container_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Container_Decoration_Height_Get decoration_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Container_Descriptor;

Enesim_Renderer * eon_theme_container_new(Eon_Theme_Container_Descriptor *descriptor, void *data);
Eina_Bool eon_is_theme_container(Enesim_Renderer *r);
void * eon_theme_container_data_get(Enesim_Renderer *r);
void eon_theme_container_decoration_size_get(Enesim_Renderer *r, double *w, double *h);
void eon_theme_container_content_position_get(Enesim_Renderer *r, double *x, double *y);

#endif
