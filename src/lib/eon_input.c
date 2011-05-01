#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Input
{
	struct
	{
		unsigned int downx;
		unsigned int downy;
		unsigned int button;
		unsigned int x;
		unsigned int y;
		unsigned int px;
		unsigned int py;
		Eina_Bool inside;
		Ender *grabbed;
	} pointer;
	/* TODO keep the last modifiers */
	struct
	{

	} keyboard;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Create a new input
 */
EAPI Eon_Input * eon_input_new(void)
{
	Eon_Input *ei;

	ei = calloc(1, sizeof(Eon_Input));
	return ei;
}
/**
 *
 */
EAPI void eon_input_feed_mouse_move(Eon_Input *ei, Ender *l,
		unsigned int x, unsigned int y)
{

}
/**
 *
 */
EAPI void eon_input_feed_mouse_in(Eon_Input *ei, Ender *l)
{

}
/**
 *
 */
EAPI void eon_input_feed_mouse_out(Eon_Input *ei, Ender *l)
{

}

