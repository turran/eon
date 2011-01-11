#include "Eon.h"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int eon_init(void)
{
	enesim_init();
	ender_init();
}

void eon_shutdown(void)
{
	ender_shutdown();
	enesim_shutdown();
}
