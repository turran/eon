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
/**
 * @}
 * @{
 */

EAPI void eon_element_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_element_min_width_set(Enesim_Renderer *r, double width);
EAPI void eon_element_min_height_set(Enesim_Renderer *r, double height);
EAPI void eon_element_min_height_get(Enesim_Renderer *r, double *height);

/**
 * @}
 * @{
 */

EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender *child);
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender *child);
EAPI void eon_layout_redraw_get(Enesim_Renderer *r, Eina_List **redraws);

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
 */

#endif
