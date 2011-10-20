#ifndef _EON_BASIC_PRIVATE_H_
#define _EON_BASIC_PRIVATE_H_

#include <float.h>

/* The button control */
typedef struct _Eon_Basic_Control_Button Eon_Basic_Control_Button;
Eon_Basic_Control_Button * eon_basic_control_button_new(void);
void eon_basic_control_button_fill_color_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_border_color_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_horizontal_padding_set(Eon_Basic_Control_Button *thiz, double padding);
void eon_basic_control_button_vertical_padding_set(Eon_Basic_Control_Button *thiz, double padding);
void eon_basic_control_button_radius_set(Eon_Basic_Control_Button *thiz, double radius);
void eon_basic_control_button_start_shadow_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_end_shadow_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_start_bevel_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_end_bevel_set(Eon_Basic_Control_Button *thiz, Enesim_Color color);
void eon_basic_control_button_margin_get(Eon_Basic_Control_Button *thiz,
		Enesim_Renderer *content, Eon_Margin *margin);
void eon_basic_control_button_position_get(Eon_Basic_Control_Button *thiz, Eon_Size *size,
		Eon_Position *position);
Enesim_Renderer * eon_basic_control_button_setup(Eon_Basic_Control_Button *thiz, Enesim_Renderer *content,
		double width, double height, Enesim_Error **error);
void eon_basic_control_button_free(Eon_Basic_Control_Button *thiz);


typedef struct _Eon_Basic_Control_Arrow Eon_Basic_Control_Arrow;
typedef enum _Eon_Basic_Control_Arrow_Direction
{
	EON_BASIC_CONTROL_ARROW_DIRECTION_LEFT,
	EON_BASIC_CONTROL_ARROW_DIRECTION_RIGHT,
	EON_BASIC_CONTROL_ARROW_DIRECTION_TOP,
	EON_BASIC_CONTROL_ARROW_DIRECTION_BOTTOM,
} Eon_Basic_Control_Arrow_Direction;

void eon_basic_control_arrow_setup(Eon_Basic_Control_Arrow *thiz,
		Eon_Position *position,
		Eon_Size *size, Eon_Basic_Control_Arrow_Direction direction);
Enesim_Renderer * eon_basic_control_arrow_renderer_get(Eon_Basic_Control_Arrow *thiz);
Eon_Basic_Control_Arrow * eon_basic_control_arrow_new(void);

void eon_basic_init(void);
void eon_basic_shutdown(void);

#endif
