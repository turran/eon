#ifndef _EON_STACK_H
#define _EON_STACK_H
/**
 * @}
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
 * @{
 */

#endif
