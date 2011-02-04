/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_WIDGET_MAGIC 0xe0400001
#define EON_WIDGET_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_WIDGET_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_WIDGET_MAGIC);\
	} while(0)

#define EON_WIDGET_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_WIDGET_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_WIDGET_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Widget
{
	EINA_MAGIC;
	void *data; /* the data provided by the widget types */
	Escen_Ender *escen_ender; /* the theme ender */
	/* FIXME add a way to setup and cleanup an ender whenever
	 * a widget ender is associated with a layout
	 */
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = enesim_renderer_data_get(r);
	EON_WIDGET_MAGIC_CHECK_RETURN(e, NULL);

	return e;
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_widget_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	/* get the ender from the escen ender and get the fill function */
	return EINA_TRUE;
}

static void _eon_widget_cleanup(Enesim_Renderer *r)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);

}

static void _eon_widget_free(Enesim_Renderer *r)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);

}

static void _eon_widget_boundings(Enesim_Renderer *r, Eina_Rectangle *rect)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);
}

static Enesim_Renderer_Descriptor _eon_widget_descriptor = {
	.sw_setup = _eon_widget_setup,
	.sw_cleanup = _eon_widget_cleanup,
	.boundings = _eon_widget_boundings,
	.free = _eon_widget_free,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_widget_new(const char *name, void *data)
{
	Eon_Widget *e;
	Enesim_Renderer *thiz;
	Enesim_Renderer_Flag flags;
	Escen *escen;
	Escen_Ender *escen_ender;
	Enesim_Renderer *escen_renderer;
	char theme[PATH_MAX];

	escen = eon_theme_get();
	if (!escen) {
		printf("no theme\n");
		return NULL;
	}

	e = calloc(1, sizeof(Eon_Widget));
	EINA_MAGIC_SET(e, EON_WIDGET_MAGIC);
	e->data = data;

	/* get the flags from the theme */
	escen_ender = escen_ender_get(escen, name);
	if (!escen_ender) goto renderer_err;

	escen_renderer = ender_renderer_get(escen_ender_ender_get(escen_ender));
	enesim_renderer_flags(escen_renderer, &flags);

	thiz = enesim_renderer_new(&_eon_widget_descriptor, flags, e);
	if (!thiz) goto renderer_err;

	return thiz;

renderer_err:
	/* free the escen_ender */
ender_err:
	free(e);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_widget(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = enesim_renderer_data_get(r);
	if (!EINA_MAGIC_CHECK(e, EON_WIDGET_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_theme_set(Enesim_Renderer *r, const char *file)
{
	/* remove the theme already associated with the widget
	 * and set this, get the correct escen_ender and set
	 * the current state
	 */

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_widget_data_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = _eon_widget_get(r);
	if (!e) return NULL;

	return e->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_property_set(Enesim_Renderer *r, const char *name, ...)
{

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_property_get(Enesim_Renderer *r, const char *name, ...)
{

}
