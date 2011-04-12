#ifndef _EON_PRIVATE_H
#define _EON_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Escen.h"
#include "Etex.h"

#include <math.h>
#include <float.h>

/* The debugging facility */
#define ERR(...) EINA_LOG_DOM_ERR(eon_log, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eon_log, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eon_log, __VA_ARGS__)
extern int eon_log;

typedef struct _Eon_Element_Descriptor
{
	double (*min_width_get)(Enesim_Renderer *r);
	double (*min_height_get)(Enesim_Renderer *r);
	double (*max_width_get)(Enesim_Renderer *r);
	double (*max_height_get)(Enesim_Renderer *r);
} Eon_Element_Descriptor;

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *edescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data);
void * eon_element_data_get(Enesim_Renderer *r);


typedef struct _Eon_Layout_Descriptor
{
	void (*child_add)(Enesim_Renderer *r, Ender *child);
	void (*child_remove)(Enesim_Renderer *r, Ender *child);
} Eon_Layout_Descriptor;

Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *ldescriptor,
		Eon_Element_Descriptor *edescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data);

#endif

