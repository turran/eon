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
#define EON_MAGIC_CHECK(d, magic)			\
	do {						\
		if (!EINA_MAGIC_CHECK(d, magic))	\
			EINA_MAGIC_FAIL(d, magic);	\
	} while(0)

#define EON_MAGIC_CHECK_RETURN(d, magic, ret)	\
	do {						\
		if (!EINA_MAGIC_CHECK(d, magic)) {	\
			EINA_MAGIC_FAIL(d, magic);	\
			return ret;			\
		}					\
	} while(0)

/* element inheritance */
#define EON_ELEMENT_MAGIC 0xe0400001
#define EON_LAYOUT_MAGIC 0xe0400002
#define EON_WIDGET_MAGIC 0xe0400003
/* theme inheritance */
#define EON_THEME_ELEMENT_MAGIC 0xe041001

/* theme */
Escen * eon_theme_get(void);
Eina_Bool eon_theme_init(void);
void eon_theme_shutdown(void);

#include "private/eon_element_private.h"
#include "private/eon_widget_private.h"
#include "private/eon_layout_private.h"
#include "private/eon_theme_element_private.h"
#include "private/eon_theme_widget_private.h"
#include "private/eon_theme_container_private.h"

/* toggle */
typedef struct _Eon_Toggle_Descriptor
{

} Eon_Toggle_Descriptor;

/* widget */

/* container */
void * eon_container_data_get(Enesim_Renderer *r);
Enesim_Renderer * eon_container_new(Eon_Widget_Descriptor *descriptor, void *data);

#endif

