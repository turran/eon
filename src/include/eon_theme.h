#ifndef _EON_THEME_H_
#define _EON_THEME_H_

/**
 * @{
 */
typedef double (*Eon_Theme_Element_Max_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Element_Max_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Element_Min_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Element_Min_Width_Get)(Enesim_Renderer *r);

EAPI void eon_theme_element_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_element_height_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_element_max_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_element_max_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_element_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_element_min_height_get(Enesim_Renderer *r, double *height);

/**
 * @}
 * @{
 */

typedef double (*Eon_Theme_Container_Decoration_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Container_Decoration_Height_Get)(Enesim_Renderer *r);
typedef void (*Eon_Theme_Container_Content_Position_Get)(Enesim_Renderer *r, Enesim_Renderer *content, double *x, double *y);

EAPI void eon_theme_container_content_set(Enesim_Renderer *r, Enesim_Renderer *content);
EAPI void eon_theme_container_content_get(Enesim_Renderer *r, Enesim_Renderer **content);

/**
 * @}
 * @{
 */

typedef void (*Eon_Theme_Radio_Selected_Set)(Enesim_Renderer *r, Eina_Bool selected);

typedef struct _Eon_Theme_Radio_Descriptor
{
	/* container needed functions */
	Eon_Theme_Container_Content_Position_Get content_position_get;
	Eon_Theme_Container_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Container_Decoration_Height_Get decoration_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Radio_Descriptor;

EAPI void * eon_theme_radio_data_get(Enesim_Renderer *r);
EAPI Eina_Bool eon_is_theme_radio(Enesim_Renderer *r);
EAPI Enesim_Renderer * eon_theme_radio_new(Eon_Theme_Radio_Descriptor *descriptor,
		void *data);
/**
 * @}
 * @{
 */

/* font name */
typedef void (*Eon_Theme_Label_Font_Set)(Enesim_Renderer *r, const char *font);
typedef const char * (*Eon_Theme_Label_Font_Get)(Enesim_Renderer *r);
/* font size */
typedef void (*Eon_Theme_Label_Size_Set)(Enesim_Renderer *r, int size);
typedef int (*Eon_Theme_Label_Size_Get)(Enesim_Renderer *r);
/* text */
typedef void (*Eon_Theme_Label_Text_Set)(Enesim_Renderer *r, const char *str);
typedef const char * (*Eon_Theme_Label_Text_Get)(Enesim_Renderer *r);
/* ellipsize */
typedef void (*Eon_Theme_Label_Ellipsize_Set)(Enesim_Renderer *r, Eina_Bool enable);
typedef Eina_Bool (*Eon_Theme_Label_Ellipsize_Get)(Enesim_Renderer *r);
/* height ascender + ascender */
typedef unsigned int (*Eon_Theme_Label_Height_Get)(Enesim_Renderer *r);

typedef struct _Eon_Theme_Label_Descriptor
{
	/* the label needed functions */
	Eon_Theme_Label_Font_Set font_set;
	Eon_Theme_Label_Font_Get font_get;
	Eon_Theme_Label_Size_Set size_set;
	Eon_Theme_Label_Size_Get size_get;
	Eon_Theme_Label_Text_Set text_set;
	Eon_Theme_Label_Text_Get text_get;
	Eon_Theme_Label_Ellipsize_Set ellipsize_set;
	Eon_Theme_Label_Ellipsize_Get ellipsize_get;
	Eon_Theme_Label_Height_Get height_get;
	/* element needed functions */
	Eon_Theme_Element_Max_Width_Get max_width_get;
	Eon_Theme_Element_Min_Width_Get min_width_get;
	Eon_Theme_Element_Max_Height_Get max_height_get;
	Eon_Theme_Element_Min_Height_Get min_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Label_Descriptor;

EAPI Enesim_Renderer * eon_theme_label_new(Eon_Theme_Label_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_label(Enesim_Renderer *r);
EAPI void * eon_theme_label_data_get(Enesim_Renderer *r);
EAPI void eon_theme_label_font_set(Enesim_Renderer *r, const char *str);
EAPI void eon_theme_label_font_get(Enesim_Renderer *r, const char **str);
EAPI void eon_theme_label_size_set(Enesim_Renderer *r, int size);
EAPI void eon_theme_label_size_get(Enesim_Renderer *r, int *size);
EAPI void eon_theme_label_text_set(Enesim_Renderer *r, const char *text);
EAPI void eon_theme_label_text_get(Enesim_Renderer *r, const char **text);
EAPI void eon_theme_label_ellipsize_set(Enesim_Renderer *r, Eina_Bool enable);
EAPI void eon_theme_label_ellipsize_get(Enesim_Renderer *r, Eina_Bool *enable);

/**
 * @}
 * @{
 */

typedef struct _Eon_Theme_Button_Descriptor
{
	/* container needed functions */
	Eon_Theme_Container_Content_Position_Get content_position_get;
	Eon_Theme_Container_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Container_Decoration_Height_Get decoration_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Button_Descriptor;

EAPI Enesim_Renderer * eon_theme_button_new(Eon_Theme_Button_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_button(Enesim_Renderer *r);
EAPI void * eon_theme_button_data_get(Enesim_Renderer *r);

/**
 * @}
 * @{
 */

typedef struct _Eon_Theme_Checkbox_Descriptor
{
	/* container needed functions */
	Eon_Theme_Container_Content_Position_Get content_position_get;
	Eon_Theme_Container_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Container_Decoration_Height_Get decoration_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Checkbox_Descriptor;

EAPI Enesim_Renderer * eon_theme_checkbox_new(Eon_Theme_Checkbox_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_checkbox(Enesim_Renderer *r);
EAPI void * eon_theme_checkbox_data_get(Enesim_Renderer *r);

/**
 * @}
 * @{
 */

typedef struct _Eon_Theme_Progressbar_Descriptor
{
	/* element needed functions */
	Eon_Theme_Element_Max_Width_Get max_width_get;
	Eon_Theme_Element_Min_Width_Get min_width_get;
	Eon_Theme_Element_Max_Height_Get max_height_get;
	Eon_Theme_Element_Min_Height_Get min_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Progressbar_Descriptor;

EAPI Enesim_Renderer * eon_theme_progressbar_new(Eon_Theme_Progressbar_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_progressbar(Enesim_Renderer *r);
EAPI void * eon_theme_progressbar_data_get(Enesim_Renderer *r);

/**
 * @}
 * @{
 */

typedef struct _Eon_Theme_Scrollview_Descriptor
{
	/* container needed functions */
	Eon_Theme_Container_Content_Position_Get content_position_get;
	Eon_Theme_Container_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Container_Decoration_Height_Get decoration_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Scrollview_Descriptor;

EAPI Enesim_Renderer * eon_theme_scrollview_new(Eon_Theme_Scrollview_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_scrollview(Enesim_Renderer *r);
EAPI void * eon_theme_scrollview_data_get(Enesim_Renderer *r);

/**
 * @}
 * @{
 */

typedef void (*Eon_Theme_Layout_Child_Add)(Enesim_Renderer *r, Ender_Element *child);
typedef void (*Eon_Theme_Layout_Child_Remove)(Enesim_Renderer *r, Ender_Element *child);

typedef struct _Eon_Theme_Layout_Descriptor
{
	/* layout needed functions */
	Eon_Theme_Layout_Child_Add child_add;
	Eon_Theme_Layout_Child_Remove child_remove;
	/* element needed functions */
	Eon_Theme_Element_Max_Width_Get max_width_get;
	Eon_Theme_Element_Min_Width_Get min_width_get;
	Eon_Theme_Element_Max_Height_Get max_height_get;
	Eon_Theme_Element_Min_Height_Get min_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Layout_Descriptor;

EAPI Enesim_Renderer * eon_theme_layout_new(Eon_Theme_Layout_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_layout(Enesim_Renderer *r);
EAPI void * eon_theme_layout_data_get(Enesim_Renderer *r);

/**
 * @}
 * @{
 */

typedef struct _Eon_Theme_Image_Descriptor
{
	/* element needed functions */
	Eon_Theme_Element_Max_Width_Get max_width_get;
	Eon_Theme_Element_Min_Width_Get min_width_get;
	Eon_Theme_Element_Max_Height_Get max_height_get;
	Eon_Theme_Element_Min_Height_Get min_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Image_Descriptor;

EAPI Enesim_Renderer * eon_theme_image_new(Eon_Theme_Image_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_image(Enesim_Renderer *r);
EAPI void * eon_theme_image_data_get(Enesim_Renderer *r);

#endif
