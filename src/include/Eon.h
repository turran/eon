#ifndef _EON_H
#define _EON_H

#include "Enesim.h"
#include "Ender.h"

/**
 * @{
 */
typedef struct _Eon_Input Eon_Input;

EAPI Eon_Input * eon_input_new(void);
EAPI void eon_input_feed_mouse_move(Eon_Input *ei, Ender *l,
		unsigned int x, unsigned int y);
EAPI void eon_input_feed_mouse_in(Eon_Input *ei, Ender *l);
EAPI void eon_input_feed_mouse_out(Eon_Input *ei, Ender *l);
EAPI void eon_input_feed_mouse_down(Eon_Input *ei, Ender *l);
EAPI void eon_input_feed_mouse_up(Eon_Input *i);

/**
 * @}
 * @{
 */

typedef struct _Eon_Event_Mouse_Click
{
} Eon_Event_Mouse_Click;

typedef struct _Eon_Event_Mouse_Up
{
} Eon_Event_Mouse_Up;

typedef struct _Eon_Event_Mouse_Down
{
} Eon_Event_Mouse_Down;

typedef struct _Eon_Event_Mouse_Move
{
} Eon_Event_Mouse_Move;

typedef struct _Eon_Event_Mouse_In
{
} Eon_Event_Mouse_In;

typedef struct _Eon_Event_Mouse_Out
{
} Eon_Event_Mouse_Out;

/**
 * @}
 * @{
 */

EAPI Eina_Bool eon_is_element(Enesim_Renderer *r);
EAPI void eon_element_actual_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_element_actual_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_element_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_element_height_set(Enesim_Renderer *r, double height);
EAPI void eon_element_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_element_width_set(Enesim_Renderer *r, double width);
EAPI void eon_element_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_element_min_width_set(Enesim_Renderer *r, double width);
EAPI void eon_element_min_height_set(Enesim_Renderer *r, double height);
EAPI void eon_element_min_height_get(Enesim_Renderer *r, double *height);

/**
 * @}
 * @{
 */

Eina_Bool eon_layout_is_topmost(Enesim_Renderer *r);
EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender *child);
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender *child);
EAPI void eon_layout_redraw_get(Enesim_Renderer *r, Eina_List **redraws);
EAPI Ender * eon_layout_child_get_at_coord(Enesim_Renderer *r, unsigned int x, unsigned int y);

/**
 * @}
 * @{
 */

EAPI Enesim_Renderer * eon_canvas_new(void);
EAPI void eon_canvas_width_set(Enesim_Renderer *r, unsigned int width);
EAPI void eon_canvas_height_set(Enesim_Renderer *r, unsigned int height);
EAPI void eon_canvas_child_x_set(Enesim_Renderer *r, Ender *child, double x);
EAPI void eon_canvas_child_y_set(Enesim_Renderer *r, Ender *child, double y);

/**
 * @}
 * @{
 */

typedef enum _Eon_Horizontal_Alignment
{
	EON_HORIZONTAL_ALIGNMENT_LEFT,
	EON_HORIZONTAL_ALIGNMENT_RIGHT,
	EON_HORIZONTAL_ALIGNMENT_CENTER,
} Eon_Horizontal_Alignment;

typedef enum _Eon_Vertical_Alignment
{
	EON_VERTICAL_ALIGNMENT_TOP,
	EON_VERTICAL_ALIGNMENT_BOTTOM,
	EON_VERTICAL_ALIGNMENT_CENTER,
} Eon_Vertical_Alignment;

typedef enum _Eon_Stack_Direction
{
	EON_STACK_DIRECTION_HORIZONTAL,
	EON_STACK_DIRECTION_VERTICAL,
	EON_STACK_DIRECTIONS,
} Eon_Stack_Direction;

EAPI Enesim_Renderer * eon_stack_new(void);
EAPI void eon_stack_direction_set(Enesim_Renderer *r, Eon_Stack_Direction direction);
EAPI void eon_stack_width_set(Enesim_Renderer *r, unsigned int width);
EAPI void eon_stack_height_set(Enesim_Renderer *r, unsigned int height);

/**
 * @}
 * @{
 */
EAPI Enesim_Renderer * eon_label_new(void);
EAPI void eon_label_text_set(Enesim_Renderer *r, const char *text);
EAPI void eon_label_text_get(Enesim_Renderer *r, const char **text);

/**
 * @}
 * @{
 */

typedef struct _Eon_Theme_Widget_Descriptor
{
	double (*max_width_get)(Enesim_Renderer *r);
	double (*max_height_get)(Enesim_Renderer *r);
	double (*min_width_get)(Enesim_Renderer *r);
	double (*min_height_get)(Enesim_Renderer *r);
} Eon_Theme_Widget_Descriptor;

EAPI Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *twdescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data);
EAPI void * eon_theme_widget_data_get(Enesim_Renderer *r);
EAPI void eon_theme_widget_max_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_max_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_widget_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_min_height_get(Enesim_Renderer *r, double *height);

typedef struct _Eon_Theme_Label_Descriptor
{
	/* font name */
	void (*font_set)(Enesim_Renderer *r, const char *font);
	const char * (*font_get)(Enesim_Renderer *r);
	/* font size */
	void (*size_set)(Enesim_Renderer *r, int size);
	int (*size_get)(Enesim_Renderer *r);
	/* text */
	void (*text_set)(Enesim_Renderer *r, const char *str);
	const char * (*text_get)(Enesim_Renderer *r);
	/* ellipsize */
	void (*ellipsize_set)(Enesim_Renderer *r, Eina_Bool enable);
	Eina_Bool (*ellipsize_get)(Enesim_Renderer *r);
} Eon_Theme_Label_Descriptor;

EAPI Enesim_Renderer * eon_theme_label_new(Eon_Theme_Label_Descriptor *tldescriptor,
		Eon_Theme_Widget_Descriptor *twdescriptor,
		Enesim_Renderer_Descriptor *descriptor,
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
 */

#endif
