#ifndef _THEME_BASIC_H_
#define _THEME_BASIC_H_

#include <float.h>

/* The button control */
typedef struct _Eon_Basic_Control_Button Eon_Basic_Control_Button;

Eon_Basic_Control_Button * eon_basic_control_button_new(void);
void eon_basic_control_button_renderer_content_set(Eon_Basic_Control_Button *thiz, Enesim_Renderer *content);
void eon_basic_control_button_renderer_content_get(Eon_Basic_Control_Button *thiz, Enesim_Renderer **content);
void eon_basic_control_button_x_set(Eon_Basic_Control_Button *thiz,
		double x);
void eon_basic_control_button_y_set(Eon_Basic_Control_Button *thiz,
		double y);
void eon_basic_control_button_width_set(Eon_Basic_Control_Button *thiz,
		double width);
void eon_basic_control_button_height_set(Eon_Basic_Control_Button *thiz,
		double height);
void eon_basic_control_button_fill_color_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_border_color_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_horizontal_padding_set(Eon_Basic_Control_Button *thiz, double padding);
void eon_basic_control_button_vertical_padding_set(Eon_Basic_Control_Button *thiz, double padding);
void eon_basic_control_button_radius_set(Eon_Basic_Control_Button *thiz, double radius);
void eon_basic_control_button_start_shadow_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_end_shadow_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_start_bevel_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_end_bevel_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_free(Eon_Basic_Control_Button *thiz);
Enesim_Renderer * eon_basic_control_button_renderer_get(Eon_Basic_Control_Button *thiz);

void eon_basic_init(void);
void eon_basic_shutdown(void);

#endif
