#include "Eon.h"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int eon_init(void)
{
	eina_init();
	enesim_init();
	ender_init();
}

void eon_shutdown(void)
{
	ender_shutdown();
	enesim_shutdown();
	eina_shutdown();
}
