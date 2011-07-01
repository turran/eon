/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
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
#define EON_ELEMENT_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_ELEMENT_MAGIC)
#define EON_ELEMENT_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_ELEMENT_MAGIC, ret)

typedef struct _Eon_Element
{
	EINA_MAGIC
	/* properties */
	double width;
	double height;
	double max_width;
	double max_height;
	double min_width;
	double min_height;
	double actual_width;
	double actual_height;
	double actual_x;
	double actual_y;
	/* private */
	/* function pointers */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	Eon_Element_Min_Height_Get min_height_get;
	Eon_Element_Min_Width_Get min_width_get;
	/* the theme data */
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
	/* misc */
	Enesim_Renderer_Descriptor descriptor;
	Enesim_Renderer_Sw_Fill fill;
	const char *name;
	void *data;
} Eon_Element;

static inline Eon_Element * _eon_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_ELEMENT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _element_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->fill(thiz->theme_renderer, x, y, len, dst);
}

/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_element_actual_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->actual_height;
}

static void _eon_element_actual_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->actual_width;
}

static void _eon_element_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->height;
}

static void _eon_element_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->height = height;
}

static void _eon_element_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->width;
}

static void _eon_element_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->width = width;
}

static void _eon_element_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double ev = 0;
	double tv = 0;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "min_width", &ev, NULL);
	if (thiz->min_width_get)
		tv = thiz->min_width_get(r);
	ev = ev > tv ? ev : tv;
	*width = ev > thiz->min_width ? ev : thiz->min_width;
}

static void _eon_element_min_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_width = width;
}

static void _eon_element_min_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_height = height;
}

static void _eon_element_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double tv = 0;
	double ev = 0;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "min_height", &tv, NULL);
	if (thiz->min_height_get)
		ev = thiz->min_height_get(r);
	ev = ev > tv ? ev : tv;
	*height = ev > thiz->min_height ? ev : thiz->min_height;
}

static void _eon_element_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "max_width", &v, NULL);
	*width = v < thiz->max_width ? v : thiz->max_width;
}

static void _eon_element_max_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_width = width;
}

static void _eon_element_max_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_height = height;
}

static void _eon_element_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "max_height", &v, NULL);
	*height = v < thiz->max_height ? v : thiz->max_height;
}
/*----------------------------------------------------------------------------*
 *                             Internal functions                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_real_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double rw;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->width;
	_eon_element_min_width_get(r, &min);
	_eon_element_max_width_get(r, &max);
	rw = set > max ? max : set;
	rw = rw < min ? min : rw;

	printf("real width %s = %g (%g %g %g)\n", thiz->name, rw, min, set, max);
	*width = rw;
}

static void _eon_element_real_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double rh;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->height;
	_eon_element_min_height_get(r, &min);
	_eon_element_max_height_get(r, &max);
	rh = set > max ? max : set;
	rh = rh < min ? min : rh;

	printf("real height %s = %g (%g %g %g)\n", thiz->name, rh, min, set, max);
	*height = rh;
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
/* Given that we dont support the x_origin property, we must inform correctly to the enesim system that we can be at a specific position*/
/* TODO Remove the function call */
static void _eon_element_boundings(Enesim_Renderer *r, Enesim_Rectangle *rect)
{
	Eon_Element *thiz;
	double aw, ah;

	thiz = _eon_element_get(r);
	eon_element_actual_size_get(r, &aw, &ah);
	/* There's no layout, or the layout didnt set an active width/height */
	if (aw < 0 || ah < 0)
	{
		_eon_element_real_width_get(r, &aw);
		_eon_element_real_height_get(r, &ah);
	}
	rect->x = thiz->actual_x;
	rect->y = thiz->actual_y;
	rect->w = aw;
	rect->h = ah;
	//printf("boundings for %s are %g %g %g %g\n", thiz->name, rect->x, rect->y, rect->w, rect->h);
}

static void _eon_element_free(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

static Eina_Bool _eon_element_sw_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Ender_Element *e;
	Eon_Element *thiz;
	Enesim_Color color;
	double width, height;

	thiz = _eon_element_get(r);
	/* FIXME if the user changes the color of the element, the theme should
	 * reflect that value
	 */
	enesim_renderer_color_get(r, &color);
	enesim_renderer_color_set(thiz->theme_renderer, color);
	eon_element_actual_size_get(r, &width, &height);
	if (width < 0)
		_eon_element_real_width_get(r, &width);
	if (height < 0)
		_eon_element_real_height_get(r, &height);
	eon_element_actual_size_set(r, width, height);
	e = ender_element_renderer_from(r);
	if (!e) return EINA_FALSE;
	if (!eon_element_setup(e))
	{
		printf("cannot setup the eon element\n");
		return EINA_FALSE;
	}
		
	if (!enesim_renderer_sw_setup(thiz->theme_renderer))
	{
		printf("the theme can not setup yet\n");
		return EINA_FALSE;
	}
	/* get the ender from the escen ender and get the fill function */
	thiz->fill = enesim_renderer_sw_fill_get(thiz->theme_renderer);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _element_draw;
	return EINA_TRUE;
}

static void _eon_element_sw_cleanup(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	enesim_renderer_sw_cleanup(thiz->theme_renderer);
}

static void _eon_element_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	enesim_renderer_flags(thiz->theme_renderer, flags);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.boundings = _eon_element_boundings,
	.flags = _eon_element_flags,
	.free = _eon_element_free,
	.sw_setup = _eon_element_sw_setup,
	.sw_cleanup = _eon_element_sw_cleanup,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_initialize(Ender_Element *ender)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(ender);
	thiz = _eon_element_get(r);
	if (thiz->initialize)
		thiz->initialize(ender);
}

Eina_Bool eon_element_setup(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	enesim_renderer_rop_set(thiz->theme_renderer, ENESIM_BLEND);
	if (thiz->setup)
		return thiz->setup(e);
	return EINA_TRUE;
}

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;
	Escen *theme_escen;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
	Enesim_Renderer *r;
	Enesim_Color color;

	theme_escen = eon_theme_get();
	if (!theme_escen) {
		printf("no theme\n");
		return NULL;
	}

	theme_ender = escen_ender_get(theme_escen, descriptor->name);
	if (!theme_ender)
	{
		printf("no ender %s\n", descriptor->name);
		return NULL;
	}

	theme_instance = escen_instance_new(theme_ender);
	if (!theme_instance)
	{
		printf("no instance %s\n", descriptor->name);
		return NULL;
	}

	theme_element = escen_instance_ender_get(theme_instance);
	theme_renderer = ender_element_renderer_get(theme_element);

	thiz = calloc(1, sizeof(Eon_Element));
	EINA_MAGIC_SET(thiz, EON_ELEMENT_MAGIC);
	thiz->data = data;
	thiz->actual_width = -1;
	thiz->actual_height = -1;
	thiz->max_width = thiz->max_height = DBL_MAX;
	thiz->min_width = thiz->min_height = 0;
	thiz->width = -1;
	thiz->height = -1;

	thiz->theme_ender = theme_ender;
	thiz->theme_instance = theme_instance;
	thiz->theme_element = theme_element;
	thiz->theme_renderer = theme_renderer;

	r = enesim_renderer_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	/* Set the function pointers */
	thiz->initialize = descriptor->initialize;
	thiz->setup = descriptor->setup;
	thiz->free = descriptor->free;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->name = descriptor->name;

	/* Set the default properties from the state */
	enesim_renderer_color_get(theme_renderer, &color);
	enesim_renderer_color_set(r, color);

	printf("element of name %s created %p\n", descriptor->name, r);
	return r;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_element_data_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->data;
}

Escen_Ender * eon_element_theme_ender_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->theme_ender;
}

Enesim_Renderer * eon_element_theme_renderer_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->theme_renderer;
}

Ender_Element * eon_element_theme_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->theme_element;
}

Escen_Instance * eon_element_theme_instance_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->theme_instance;
}

void eon_element_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	ender_element_value_set(thiz->theme_element, "width", width, NULL);
}

void eon_element_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_height = height;
	ender_element_value_set(thiz->theme_element, "height", height, NULL);
}

void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	thiz->actual_height = height;
	ender_element_value_set(thiz->theme_element, "width", width, NULL);
	ender_element_value_set(thiz->theme_element, "height", height, NULL);
}

void eon_element_actual_size_get(Enesim_Renderer *r, double *width, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->actual_width;
	*height = thiz->actual_height;
}

void eon_element_actual_x_set(Enesim_Renderer *r, double x)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->actual_x = x;
	ender_element_value_set(thiz->theme_element, "x", x, NULL);
	//printf("actual x %s is %g\n", thiz->name, x);
}

void eon_element_actual_y_set(Enesim_Renderer *r, double y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->actual_y = y;
	ender_element_value_set(thiz->theme_element, "y", y, NULL);
	//printf("actual y %s is %g\n", thiz->name, y);
}

void eon_element_actual_position_set(Enesim_Renderer *r, double x, double y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->actual_x = x;
	thiz->actual_y = y;
	ender_element_value_set(thiz->theme_element, "x", x, NULL);
	ender_element_value_set(thiz->theme_element, "y", y, NULL);
}

void eon_element_real_width_get(Ender_Element *e, double *width)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	_eon_element_real_width_get(r, width);
}

void eon_element_real_height_get(Ender_Element *e, double *height)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	_eon_element_real_height_get(r, height);
}

void eon_element_property_set(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Element *thiz;
	va_list va_args;

	thiz = _eon_element_get(r);
	va_start(va_args, name);
	ender_element_value_set_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_element_property_get(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Element *thiz;
	va_list va_args;

	thiz = _eon_element_get(r);
	va_start(va_args, name);
	ender_element_value_get_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_element_property_add(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Element *thiz;
	va_list va_args;

	thiz = _eon_element_get(r);
	va_start(va_args, name);
	ender_element_value_add_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_element_property_remove(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Element *thiz;
	va_list va_args;

	thiz = _eon_element_get(r);
	va_start(va_args, name);
	ender_element_value_remove_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

#define _eon_element_actual_width_set NULL
#define _eon_element_actual_height_set NULL
#include "eon_generated_element.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_element(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	if (!thiz) return EINA_FALSE;
	if (!EINA_MAGIC_CHECK(thiz, EON_ELEMENT_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_height_get(Ender_Element *e, double *height)
{
	ender_element_value_get(e, "actual_height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_width_get(Ender_Element *e, double *width)
{
	ender_element_value_get(e, "actual_width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_get(Ender_Element *e, double *height)
{
	ender_element_value_get(e, "height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_set(Ender_Element *e, double height)
{
	ender_element_value_set(e, "height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_get(Ender_Element *e, double *width)
{
	ender_element_value_get(e, "width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_set(Ender_Element *e, double width)
{
	ender_element_value_set(e, "width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_get(Ender_Element *e, double *width)
{
	ender_element_value_get(e, "min_width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_set(Ender_Element *e, double width)
{
	ender_element_value_set(e, "min_width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_set(Ender_Element *e, double height)
{
	ender_element_value_set(e, "min_height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_get(Ender_Element *e, double *height)
{
	ender_element_value_get(e, "min_height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_get(Ender_Element *e, double *width)
{
	ender_element_value_get(e, "max_width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_set(Ender_Element *e, double width)
{
	ender_element_value_set(e, "max_width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_set(Ender_Element *e, double height)
{
	ender_element_value_set(e, "max_height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_get(Ender_Element *e, double *height)
{
	ender_element_value_get(e, "max_height", height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_theme_set(Enesim_Renderer *r, const char *file)
{
	/* remove the theme already associated with the element
	 * and set this, get the correct escen_ender and set
	 * the current state
	 */
}
