#ifndef _EON_PRIVATE_H
#define _EON_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Escen.h"
#include "Emage.h"
#include "Etex.h"
#include "Ecore.h"

#include <math.h>
#include <float.h>

/* The debugging facility */
#define EON_LOG_COLOR_DEFAULT EINA_COLOR_LIGHTBLUE

#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(EON_LOG_DEFAULT, __VA_ARGS__)

#ifdef WARN
# undef WARN
#endif
#define WARN(...) EINA_LOG_DOM_WARN(EON_LOG_DEFAULT, __VA_ARGS__)

#ifdef INFO
# undef INFO
#endif
#define INFO(...) EINA_LOG_DOM_INFO(EON_LOG_DEFAULT, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(EON_LOG_DEFAULT, __VA_ARGS__)

#define EON_ELEMENT_NEW(name) ender_namespace_element_new(eon_namespace_get(), name);

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


/* helper functions */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b)) 
/* element inheritance */
/* widget inheritance */
#define EON_COMPOUND_MAGIC 0xe0400004

/* theme inheritance */

/* main */
Ender_Namespace * eon_namespace_get(void);

#endif

