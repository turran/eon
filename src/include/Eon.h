#ifndef _EON_H
#define _EON_H

#include "Enesim.h"
#include "Ender.h"

/**
 * @{
 */
typedef struct _Eon_Input Eon_Input;

EAPI Eon_Input * eon_input_new(void);
EAPI void eon_input_feed_mouse_move(Eon_Input *ei, Ender_Element *l,
		unsigned int x, unsigned int y);
EAPI void eon_input_feed_mouse_in(Eon_Input *ei, Ender_Element *l);
EAPI void eon_input_feed_mouse_out(Eon_Input *ei, Ender_Element *l);
EAPI void eon_input_feed_mouse_down(Eon_Input *ei, Ender_Element *l);
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


/**
 * @}
 * @{
 */

Eina_Bool eon_layout_is_topmost(Enesim_Renderer *r);
EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender_Element *child);
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender_Element *child);
EAPI void eon_layout_redraw_get(Enesim_Renderer *r, Eina_List **redraws);
EAPI Ender_Element * eon_layout_child_get_at_coord(Enesim_Renderer *r, double x, double y);
EAPI Ender_Element * eon_layout_child_get_at_destination_coord(Enesim_Renderer *r, unsigned int x, unsigned int y);

/**
 * @}
 * @{
 */

EAPI Enesim_Renderer * eon_canvas_new(void);
EAPI void eon_canvas_width_set(Enesim_Renderer *r, unsigned int width);
EAPI void eon_canvas_height_set(Enesim_Renderer *r, unsigned int height);
EAPI void eon_canvas_child_x_set(Enesim_Renderer *r, Ender_Element *child, double x);
EAPI void eon_canvas_child_y_set(Enesim_Renderer *r, Ender_Element *child, double y);

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
EAPI void eon_container_content_set(Enesim_Renderer *r, Ender_Element *content);
EAPI void eon_container_content_get(Enesim_Renderer *r, const Ender_Element **content);

/**
 * @}
 * @{
 */

EAPI Enesim_Renderer * eon_radio_new(void);
EAPI void eon_radio_group_name_set(Enesim_Renderer *r, const char *group);
EAPI void eon_radio_group_name_get(Enesim_Renderer *r, const char **group);
EAPI void eon_radio_selected_set(Enesim_Renderer *r, Eina_Bool selected);
EAPI void eon_radio_selected_get(Enesim_Renderer *r, Eina_Bool *selected);

/**
 * @}
 * @{
 */
EAPI Enesim_Renderer * eon_button_new(void);

/**
 * @}
 * @{
 */
EAPI Enesim_Renderer * eon_label_new(void);
EAPI void eon_label_text_set(Enesim_Renderer *r, const char *text);
EAPI void eon_label_text_get(Enesim_Renderer *r, const char **text);

/**
 * @}
 */

#include "eon_element.h"
#include "eon_theme.h"

#endif
