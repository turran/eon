#ifndef _EON_PRIVATE_H
#define _EON_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Escen.h"

/* The debugging facility */
#define ERR(...) EINA_LOG_DOM_ERR(eon_log, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eon_log, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eon_log, __VA_ARGS__)
extern int eon_log;

typedef struct _Eon_Layout_Descriptor
{
	void (*child_add)(Enesim_Renderer *r, Ender *child);
	void (*child_remove)(Enesim_Renderer *r, Ender *child);
} Eon_Layout_Descriptor;

#endif

