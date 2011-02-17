#ifndef _EON_PRIVATE_H
#define _EON_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Escen.h"

typedef struct _Eon_Layout_Descriptor
{
	void * (*child_add)(Enesim_Renderer *r, Ender *child);
	void * (*child_remove)(Enesim_Renderer *r, Ender *child);
	void * (*child_get)(Enesim_Renderer *r, Ender *child);
} Eon_Layout_Descriptor;

#endif

