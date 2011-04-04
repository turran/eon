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
	Escen_Ender_Instance *eei;
	/* FIXME add a way to setup and cleanup an ender whenever
	 * a widget ender is associated with a layout
	 */
	Enesim_Renderer_Sw_Fill fill;
	double max_width;
	double max_height;
	double min_width;
	double min_height;
} Eon_Widget;

/* placeholder for virtual functions every widget
 * should implement
 */
typedef struct _Eon_Widget_Descriptor
{

} Eon_Widget_Descriptor;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	if (!r) return NULL;
	thiz = eon_element_data_get(r);
	EON_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _widget_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_ender_instance_ender_get(ew->eei));
	ew->fill(er, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static double _eon_widget_max_width_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_ender_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "max_width", &v, NULL);
	return v < thiz->max_width ? v : thiz->max_width;
}

static void _eon_widget_max_width_set(Enesim_Renderer *r, double width)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	thiz->max_width = width;
}

static double _eon_widget_min_width_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_ender_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "min_width", &v, NULL);
	return v > thiz->min_width ? v : thiz->min_width;
}

static void _eon_widget_min_width_set(Enesim_Renderer *r, double width)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	thiz->min_width = width;
}

static double _eon_widget_max_height_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_ender_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "max_height", &v, NULL);
	return v < thiz->max_height ? v : thiz->max_height;
}

static void _eon_widget_max_height_set(Enesim_Renderer *r, double height)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	thiz->max_height = height;
}

static double _eon_widget_min_height_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_ender_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "min_height", &v, NULL);
	return v > thiz->min_height ? v : thiz->min_height;
}

static void _eon_widget_min_height_set(Enesim_Renderer *r, double height)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	thiz->min_height = height;
}

static Eon_Element_Descriptor _eon_widget_element_descriptor = {
	.max_width_get = _eon_widget_max_width_get,
	.max_width_set = _eon_widget_max_width_set,
	.min_width_get = _eon_widget_min_width_get,
	.min_width_set = _eon_widget_min_width_set,
	.max_height_get = _eon_widget_max_height_get,
	.max_height_set = _eon_widget_max_height_set,
	.min_height_get = _eon_widget_min_height_get,
	.min_height_set = _eon_widget_min_height_set,
};
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_widget_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;
	double ox, oy;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_ender_instance_ender_get(ew->eei));
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(er, ox, oy);
	if (!enesim_renderer_sw_setup(er))
	{
		printf("not available to setup yet\n");
		return EINA_FALSE;
	}
	/* get the ender from the escen ender and get the fill function */
	ew->fill = enesim_renderer_sw_fill_get(er);
	if (!ew->fill) return EINA_FALSE;

	*fill = _widget_draw;
	return EINA_TRUE;
}

static void _eon_widget_cleanup(Enesim_Renderer *r)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_ender_instance_ender_get(ew->eei));
	enesim_renderer_sw_cleanup(er);
}

static void _eon_widget_free(Enesim_Renderer *r)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);
}

static void _eon_widget_boundings(Enesim_Renderer *r, Eina_Rectangle *rect)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_ender_instance_ender_get(ew->eei));
	enesim_renderer_boundings(er, rect);
}

static void _eon_widget_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Eon_Widget *thiz;
	Enesim_Renderer *er;

	thiz = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_ender_instance_ender_get(thiz->eei));
	enesim_renderer_flags(er, flags);
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
	Escen *escen;
	Escen_Ender *escen_ender;
	Escen_State *escen_state;
	Enesim_Renderer *thiz;
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
	e->max_width = DBL_MIN;
	e->min_width = 0;

	escen_ender = escen_ender_get(escen, name);
	if (!escen_ender) goto renderer_err;
	e->escen_ender = escen_ender;
	e->eei = escen_ender_instance_get(e->escen_ender);
	escen_renderer = ender_element_renderer_get(escen_ender_instance_ender_get(e->eei));
	if (!escen_renderer) goto escen_renderer_err;
	/* Set the default state in case it has one */
	escen_state = escen_ender_state_get(escen_ender, "default");
	escen_ender_instance_state_set(e->eei, escen_state);

	thiz = eon_element_new(&_eon_widget_element_descriptor,
			&_eon_widget_descriptor, e);
	if (!thiz) goto renderer_err;

	return thiz;

renderer_err:
	/* free the escen_ender */
escen_renderer_err:

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
	Eon_Widget *thiz;

	thiz = eon_element_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_WIDGET_MAGIC))
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
	Eon_Widget *thiz;
	Ender *ender;
	va_list va_args;

	thiz = _eon_widget_get(r);
	ender = escen_ender_instance_ender_get(thiz->eei);
	va_start(va_args, name);
	ender_element_value_set_valist(ender, name, va_args);
	va_end(va_args);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_property_get(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	Ender *ender;
	va_list va_args;

	thiz = _eon_widget_get(r);
	ender = escen_ender_instance_ender_get(thiz->eei);
	va_start(va_args, name);
	ender_element_value_get_valist(ender, name, va_args);
	va_end(va_args);
}
