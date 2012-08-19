#ifndef _EON_PRIVATE_H
#define _EON_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <float.h>

#include <Eina.h>
#include <Enesim.h>
#include <Ender.h>
#include <Escen.h>

#include "eon_main.h"

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EON_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EON_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

/* common macros */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b)) 

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


#define EON_ELEMENT_NEW(name) ender_element_new_namespace_from(name, eon_namespace_get());
Ender_Namespace * eon_namespace_get(void);

/* The debugging facility */
#define ERR(...) EINA_LOG_DOM_ERR(eon_log, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eon_log, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eon_log, __VA_ARGS__)
extern int eon_log;

typedef struct _Eon_Hints
{
	Eon_Size min;
	Eon_Size max;
	Eon_Size preferred;
} Eon_Hints;

static inline void eon_hints_initialize(Eon_Hints *h)
{
	eon_size_values_set(&h->min, 0, 0);
	eon_size_values_set(&h->preferred, -1, -1);
	eon_size_values_set(&h->max, DBL_MAX, DBL_MAX);
}

static inline void eon_hints_values_set(Eon_Hints *h, double minw, double maxw,
		double minh, double maxh, double prefw, double prefh)
{
	h->min.width = minw;
	h->min.height = minh;
	h->max.width = maxw;
	h->max.height = maxh;
	h->preferred.width = prefw;
	h->preferred.height = prefh;
}

static inline void eon_hints_sanitize(Eon_Hints *h)
{
	if (h->max.width < h->min.width)
		h->max.width = h->min.width;
	if (h->max.height < h->min.height)
		h->max.height = h->min.height;

	if (h->preferred.width < h->min.width)
		h->preferred.width = h->min.width;
	if (h->preferred.height < h->min.height)
		h->preferred.height = h->min.height;
}

static inline void eon_hints_geometry_align(Eon_Hints *hints, Eon_Geometry *g, Eon_Horizontal_Alignment halign, Eon_Vertical_Alignment valign)
{
	/* check the geometry against the last hints */
	if (g->width > hints->max.width)
	{
		double w = hints->preferred.width;
		double x = 0;

		if (w < 0) w = hints->max.width;
		switch (halign)
		{
			case EON_HORIZONTAL_ALIGNMENT_RIGHT:
			x = g->width - w;
			break;

			case EON_HORIZONTAL_ALIGNMENT_CENTER:
			x = (g->width / 2) - (w / 2);
			break;

			default:
			break;
		}
		g->x += x;
		g->width = w;
	}
	if (g->height > hints->max.height)
	{
		double h = hints->preferred.height;
		double y = 0;

		if (h < 0) h = hints->max.height;
		switch (valign)
		{
			case EON_VERTICAL_ALIGNMENT_BOTTOM:
			y = g->height - h;
			break;

			case EON_VERTICAL_ALIGNMENT_CENTER:
			y = (g->height / 2) - (h / 2);
			break;

			default:
			break;
		}
		g->y += y;
		g->height = h;
	}
}

static inline Eina_Bool eon_geometry_is_inside(Eon_Geometry *g, double x, double y)
{
	if (x >= g->x && x < g->x + g->width && y >= g->y && y < g->y + g->height)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}

#endif
