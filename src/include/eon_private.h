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

/* The magic numbers for easy tracking */
#define EON_ELEMENT_MAGIC 0xe0400001
#define EON_LAYOUT_MAGIC 0xe0400002
#define EON_WIDGET_MAGIC 0xe0400003

/* theme */
Escen * eon_theme_get(void);
Eina_Bool eon_theme_init(void);
void eon_theme_shutdown(void);


/* element */
typedef struct _Eon_Element_Descriptor
{
	void (*initialize)(Ender *ender);
	void (*actual_width_set)(Enesim_Renderer *r, double width);
	void (*actual_height_set)(Enesim_Renderer *r, double height);
	double (*min_width_get)(Enesim_Renderer *r);
	double (*min_height_get)(Enesim_Renderer *r);
	double (*max_width_get)(Enesim_Renderer *r);
	double (*max_height_get)(Enesim_Renderer *r);
} Eon_Element_Descriptor;

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *edescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data);
void * eon_element_data_get(Enesim_Renderer *r);
void eon_element_initialize(Ender *ender);

/* layout */
typedef struct _Eon_Layout_Descriptor
{
	void (*child_add)(Enesim_Renderer *r, Ender *child);
	void (*child_remove)(Enesim_Renderer *r, Ender *child);
	void (*child_clear)(Enesim_Renderer *r);
	Ender * (*child_at)(Enesim_Renderer *r, double x, double y);
} Eon_Layout_Descriptor;

Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *ldescriptor,
		Eon_Element_Descriptor *edescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data);
void * eon_layout_data_get(Enesim_Renderer *r);

/* widget */
Enesim_Renderer * eon_widget_new(const char *name, void *data);
void * eon_widget_data_get(Enesim_Renderer *r);

/* container */
void * eon_container_data_get(Enesim_Renderer *r);
Enesim_Renderer * eon_container_new(const char *name, void *data);

#endif

