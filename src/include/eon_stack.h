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


EAPI Ender_Element * eon_stack_new(void);
EAPI void eon_stack_direction_set(Ender_Element *e, Eon_Stack_Direction direction);
EAPI void eon_stack_width_set(Ender_Element *e, unsigned int width);
EAPI void eon_stack_height_set(Ender_Element *e, unsigned int height);

/**
 * @{
 */

#endif
