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
/**
 * @todo
 * - All this "changed" is not completely correct, same thing happens with the force_redraw
 * as it must be just a signal from the dev to inform that this element should redraw, not
 * a bool property
 * - A way to avoid all this mess of renderers/enders on the global, local and API functions
 * is to store them all on the Eon_Element struct and add the opaque handler of such struct
 * globally. Then add functions to get this type from renderers or enders, and change all the
 * global/local functions to use that. So the API will remain with enders only and the global
 * and local functions with the opaque handler
 * - Add a visibility property to be able to show and hide widgets easily
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_ELEMENT_MAGIC)
#define EON_ELEMENT_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_ELEMENT_MAGIC, ret)

static Ender_Property *EON_ELEMENT_PARENT;
static Ender_Property *EON_ELEMENT_FOCUSABLE;
static Ender_Property *EON_ELEMENT_VISIBILITY;
static Ender_Property *EON_ELEMENT_WIDTH;
static Ender_Property *EON_ELEMENT_HEIGHT;
static Ender_Property *EON_ELEMENT_MIN_WIDTH;
static Ender_Property *EON_ELEMENT_MIN_HEIGHT;
static Ender_Property *EON_ELEMENT_MAX_WIDTH;
static Ender_Property *EON_ELEMENT_MAX_HEIGHT;
static Ender_Property *EON_ELEMENT_PREFERRED_WIDTH;
static Ender_Property *EON_ELEMENT_PREFERRED_HEIGHT;
static Ender_Property *EON_ELEMENT_ACTUAL_WIDTH;
static Ender_Property *EON_ELEMENT_ACTUAL_HEIGHT;

struct _Eon_Element
{
	EINA_MAGIC
	/* properties */
	Eon_Element_State current;
	Eon_Element_State past;
	/* TODO */
	Eon_Size min_size;
	Eon_Size max_size;
	Eon_Size size;
	/* private */
	Ender_Element *parent;
	Eon_Keyboard_Proxy *keyboard_proxy;
	/* function pointers */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Renderer_Get renderer_get;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Free free;
	Eon_Element_Min_Height_Get min_height_get;
	Eon_Element_Min_Width_Get min_width_get;
	Eon_Element_Min_Height_Get max_height_get;
	Eon_Element_Min_Width_Get max_width_get;
	Eon_Element_Actual_X_Set actual_x_set;
	Eon_Element_Actual_Y_Set actual_y_set;
	Eon_Element_Actual_Width_Set actual_width_set;
	Eon_Element_Actual_Height_Set actual_height_set;
	Eon_Element_Preferred_Height_Get preferred_height_get;
	Eon_Element_Preferred_Width_Get preferred_width_get;
	Eina_Bool is_focusable;
	/* misc */
	Ender_Element *current_focus;
	Eina_Bool do_needs_setup : 1;
	const char *name;
	void *data;
	Ender_Element *e;
};

static double _element_min_width_get(Ender_Element *e)
{
	Eon_Element *thiz;
	double v = 0;

	thiz = ender_element_object_get(e);
	if (!thiz) return v;
	if (thiz->min_width_get)
		v = thiz->min_width_get(e);
	return MAX(v, thiz->min_size.width);
}

static double _element_min_height_get(Ender_Element *e)
{
	Eon_Element *thiz;
	double v = 0;

	thiz = ender_element_object_get(e);
	if (!thiz) return v;
	if (thiz->min_height_get)
		v = thiz->min_height_get(e);
	return MAX(v, thiz->min_size.height);
}

static double _element_max_width_get(Ender_Element *e)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	thiz = ender_element_object_get(e);
	if (!thiz) return v;
	if (thiz->max_width_get)
		v = thiz->max_width_get(e);
	return MIN(v, thiz->max_size.width);
}

static double _element_max_height_get(Ender_Element *e)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	thiz = ender_element_object_get(e);
	if (!thiz) return v;
	if (thiz->max_height_get)
		v = thiz->max_height_get(e);
	return MIN(v, thiz->max_size.height);
}

static Eina_Bool _eon_element_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **error)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);

	if (thiz->setup)
		return thiz->setup(e, &thiz->current, s, error);
	return EINA_TRUE;
}

static void _eon_element_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	thiz->past = thiz->current;
	thiz->do_needs_setup = EINA_FALSE;
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_element_actual_height_get(Eon_Element *thiz, double *height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	*height = thiz->current.actual_size.height;
}

static void _eon_element_actual_width_get(Eon_Element *thiz, double *width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	*width = thiz->current.actual_size.width;
}

static void _eon_element_height_get(Eon_Element *thiz, double *height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	*height = thiz->size.height;
}

static void _eon_element_height_set(Eon_Element *thiz, double height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->size.height = height;
}

static void _eon_element_width_get(Eon_Element *thiz, double *width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	*width = thiz->size.width;
}

static void _eon_element_width_set(Eon_Element *thiz, double width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->size.width = width;
}

static void _eon_element_min_width_set(Eon_Element *thiz, double width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->min_size.width = width;
}

static void _eon_element_min_width_get(Eon_Element *thiz, double *width)
{
	Ender_Element *e;
	double v = 0;
	double max;

	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!width) return;
	e = thiz->e;
	if (!thiz) return;
	v = _element_min_width_get(e);
	/* make sure we dont conflict with the max */
	max = _element_max_width_get(e);
	*width = MIN(v, max);
}

static void _eon_element_min_height_set(Eon_Element *thiz, double height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->min_size.height = height;
}

static void _eon_element_min_height_get(Eon_Element *thiz, double *height)
{
	Ender_Element *e;
	double v = 0;
	double max;

	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!height) return;
	e = thiz->e;
	if (!thiz) return;
	v = _element_min_height_get(e);
	/* make sure we dont conflict with the max */
	max = _element_max_height_get(e);
	*height = MIN(v, max);
}

static void _eon_element_max_width_set(Eon_Element *thiz, double width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->max_size.width = width;
}

static void _eon_element_max_width_get(Eon_Element *thiz, double *width)
{
	Ender_Element *e;
	double v = DBL_MAX;
	double min;

	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!width) return;

	e = thiz->e;
	v = _element_max_width_get(e);
	/* make sure we dont conflict with the min */
	min = _element_min_width_get(e);
	*width = MAX(v, min);
}

static void _eon_element_max_height_set(Eon_Element *thiz, double height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->max_size.height = height;
}

static void _eon_element_max_height_get(Eon_Element *thiz, double *height)
{
	Ender_Element *e;
	double v = DBL_MAX;
	double min;

	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!height) return;

	e = thiz->e;
	v = _element_max_height_get(e);
	/* make sure we dont conflict with the min */
	min = _element_min_height_get(e);
	*height = MAX(v, min);
}

static void _eon_element_preferred_width_get(Eon_Element *thiz, double *width)
{
	Ender_Element *e;

	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!width) return;

	e = thiz->e;
	*width = -1;
	if (thiz->preferred_width_get)
		*width = thiz->preferred_width_get(e);
}

static void _eon_element_preferred_height_get(Eon_Element *thiz, double *height)
{
	Ender_Element *e;

	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!height) return;

	e = thiz->e;
	*height = -1;
	if (thiz->preferred_height_get)
		*height = thiz->preferred_height_get(e);
}


static void _eon_element_visibility_set(Eon_Element *thiz, double visible)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->current.visible = visible;
}

static void _eon_element_visibility_get(Eon_Element *thiz, double *visible)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!visible) return;
	*visible = thiz->current.visible;
}

static void _eon_element_parent_get(Eon_Element *thiz, Ender_Element **parent)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!parent) return;
	*parent = thiz->parent;
}

static void _eon_element_focusable_get(Eon_Element *thiz, Eina_Bool *focusable)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	/* TODO fix this */
	*focusable = EINA_TRUE;
}

#define _eon_element_parent_set NULL
#define _eon_element_actual_width_set NULL
#define _eon_element_actual_height_set NULL
#define _eon_element_actual_x_set NULL
#define _eon_element_actual_y_set NULL
#define _eon_element_preferred_width_set NULL
#define _eon_element_preferred_height_set NULL
#define _eon_element_focusable_set NULL
#include "eon_generated_element.c"
/*----------------------------------------------------------------------------*
 *                             Internal functions                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_real_width_get(Eon_Element *thiz, double *width)
{
	double rw;
	double min, set, max;

	set = thiz->size.width;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		_eon_element_preferred_width_get(thiz, &set);
	_eon_element_min_width_get(thiz, &min);
	_eon_element_max_width_get(thiz, &max);
	rw = MIN(set, max);
	rw = MAX(rw, min);

	//printf("real width %s = %g (%g %g %g)\n", thiz->name, rw, min, set, max);
	*width = rw;
}

static void _eon_element_real_height_get(Eon_Element *thiz, double *height)
{
	double rh;
	double min, set, max;

	set = thiz->size.height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		_eon_element_preferred_height_get(thiz, &set);
	_eon_element_min_height_get(thiz, &min);
	_eon_element_max_height_get(thiz, &max);
	rh = MIN(set, max);
	rh = MAX(rh, min);

	//printf("real height %s = %g (%g %g %g)\n", thiz->name, rh, min, set, max);
	*height = rh;
}

static void _eon_element_free(Eon_Element *thiz)
{
	if (thiz->free)
		thiz->free(thiz);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_initialize(Ender_Element *e)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	/* store the renderer and the ender to avoid so many functions calls */
	thiz->e = e;
	/* whenever the theme has changed we should notify
	 * the change on this element too
	 */
	if (thiz->initialize)
		thiz->initialize(e);
}

Eina_Bool eon_element_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **error)
{
	Eina_Bool ret;

	ret = _eon_element_setup(e, s, error);
	if (!ret)
	{
		printf("The element_setup() failed");
		return EINA_FALSE;
	}

	return ret;
}

void eon_element_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	_eon_element_cleanup(e, s);
}

/* functions to manage the focus/mouse events */
Ender_Element * eon_element_at(Ender_Element *e, double x, double y)
{
	/* check if the element has implemented the element_at interface
	 * if so, pass it but translated to the new position
	 */
}

void eon_element_feed_key_down(Ender_Element *e, Eon_Input *input, Ender_Element *from, const char *key)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);

	if (!thiz->keyboard_proxy) return;
	eon_keyboard_proxy_feed_key_down(thiz->keyboard_proxy, e, input, from, key);
}

void eon_element_feed_key_up(Ender_Element *e, Eon_Input *input, Ender_Element *from, const char *key)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);

	if (!thiz->keyboard_proxy) return;
	eon_keyboard_proxy_feed_key_up(thiz->keyboard_proxy, e, input, from, key);
}

Eon_Element * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;

	thiz = calloc(1, sizeof(Eon_Element));
	EINA_MAGIC_SET(thiz, EON_ELEMENT_MAGIC);
	thiz->data = data;
	thiz->current.actual_size.width = -1;
	thiz->current.actual_size.height = -1;
	thiz->max_size.width = thiz->max_size.height = DBL_MAX;
	thiz->min_size.width = thiz->min_size.height = 0;
	thiz->size.width = -1;
	thiz->size.height = -1;

	/* Set the function pointers */
	thiz->initialize = descriptor->initialize;
	thiz->setup = descriptor->setup;
	thiz->renderer_get = descriptor->renderer_get;
	thiz->needs_setup = descriptor->needs_setup;
	thiz->free = descriptor->free;
	/* min */
	thiz->min_width_get = descriptor->min_width_get;
	thiz->min_height_get = descriptor->min_height_get;
	/* max */
	thiz->max_width_get = descriptor->max_width_get;
	thiz->max_height_get = descriptor->max_height_get;
	/* actual */
	thiz->actual_x_set = descriptor->actual_x_set;
	thiz->actual_y_set = descriptor->actual_y_set;
	thiz->actual_width_set = descriptor->actual_width_set;
	thiz->actual_height_set = descriptor->actual_height_set;
	/* preferred */
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;
	/* other */
	thiz->is_focusable = descriptor->is_focusable;
	thiz->name = descriptor->name;

	printf("element of name %s created %p\n", descriptor->name, thiz);
	return thiz;
}

void * eon_element_data_get(Eon_Element *thiz)
{
	return thiz->data;
}

/*
 * Check whenever an element needs to do the setup
 * The only property relative to a setup is the size
 */
Eina_Bool eon_element_needs_setup(Ender_Element *e)
{
	Eon_Element *thiz;
	Eina_Bool ret;

	thiz = ender_element_object_get(e);
	ret = thiz->do_needs_setup;
	if (ret)
	{
		if (thiz->current.actual_size.width != thiz->past.actual_size.width)
			ret = EINA_FALSE;
		if (thiz->current.actual_size.height != thiz->past.actual_size.height)
			ret = EINA_FALSE;
	}
	if (ret) return ret;

	if (thiz->needs_setup)
		ret = thiz->needs_setup(e);
	return ret;
}

void eon_element_actual_width_set(Eon_Element *thiz, double width)
{
	thiz->current.actual_size.width = width;
	if (thiz->actual_width_set)
		thiz->actual_width_set(thiz, width);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_height_set(Eon_Element *thiz, double height)
{
	thiz->current.actual_size.height = height;
	if (thiz->actual_height_set)
		thiz->actual_height_set(thiz, height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_size_set(Eon_Element *thiz, double width, double height)
{
	thiz->current.actual_size.width = width;
	thiz->current.actual_size.height = height;
	if (thiz->actual_width_set)
		thiz->actual_width_set(thiz, width);
	if (thiz->actual_height_set)
		thiz->actual_height_set(thiz, height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_size_get(Eon_Element *thiz, Eon_Size *size)
{
	size->width = thiz->current.actual_size.width;
	size->height = thiz->current.actual_size.height;
}

void eon_element_actual_x_set(Eon_Element *thiz, double x)
{
	thiz->current.actual_position.x = x;
	if (thiz->actual_x_set)
		thiz->actual_x_set(thiz, x);
}

void eon_element_actual_y_set(Eon_Element *thiz, double y)
{
	thiz->current.actual_position.y = y;
	if (thiz->actual_y_set)
		thiz->actual_y_set(thiz, y);
}

void eon_element_actual_position_set(Eon_Element *thiz, double x, double y)
{
	thiz->current.actual_position.x = x;
	thiz->current.actual_position.y = y;
	if (thiz->actual_x_set)
		thiz->actual_x_set(thiz, x);
	if (thiz->actual_y_set)
		thiz->actual_y_set(thiz, y);
}

void eon_element_actual_position_get(Eon_Element *thiz, double *x, double *y)
{
	if (x) *x = thiz->current.actual_position.x;
	if (y) *y = thiz->current.actual_position.y;
}

void eon_element_actual_geometry_set(Eon_Element *thiz, Eon_Geometry *g)
{
	thiz->current.actual_position.x = g->x;
	thiz->current.actual_position.y = g->y;
	thiz->current.actual_size.width = g->width;
	thiz->current.actual_size.height = g->height;

	if (thiz->actual_x_set)
		thiz->actual_x_set(thiz, g->x);
	if (thiz->actual_y_set)
		thiz->actual_y_set(thiz, g->y);
	if (thiz->actual_width_set)
		thiz->actual_width_set(thiz, g->width);
	if (thiz->actual_height_set)
		thiz->actual_height_set(thiz, g->height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_real_relative_size_get(Ender_Element *e, Eon_Size *relative, Eon_Size *size)
{
	Eon_Element *thiz;
	double rw, rh;
	double min, set, max;

	thiz = ender_element_object_get(e);
	if (!thiz) return;

	set = thiz->size.width;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->width;
	_eon_element_min_width_get(thiz, &min);
	_eon_element_max_width_get(thiz, &max);
	rw = MIN(max, set);
	rw = MAX(rw, min);
	size->width = rw;
	//printf("relative width %s = %g (%g %g %g) %g\n", thiz->name, rw, min, set, max, relative->width);

	set = thiz->size.height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->height;
	_eon_element_min_height_get(thiz, &min);
	_eon_element_max_height_get(thiz, &max);
	rh = MIN(max, set);
	rh = MAX(rh, min);
	size->height = rh;
	//printf("relative height %s = %g (%g %g %g) %g\n", thiz->name, rh, min, set, max, relative->height);

	//printf("relative size %s = %gx%g\n", thiz->name, size->width, size->height);
}

void eon_element_real_size_get(Ender_Element *e, Eon_Size *size)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	_eon_element_real_width_get(thiz, &size->width);
	_eon_element_real_height_get(thiz, &size->height);
}

void eon_element_real_width_get(Ender_Element *e, double *width)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	_eon_element_real_width_get(thiz, width);
}

void eon_element_real_height_get(Ender_Element *e, double *height)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	_eon_element_real_height_get(thiz, height);
}

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	thiz = ender_element_object_get(e);
	r =  thiz->renderer_get(e);
	if (!r)
	{
		printf("FIXME!!!!!!!! the implementation of %s does not returned a renderer\n", ender_element_name_get(e));
	}

	return r;
}

Ender_Element * eon_element_ender_get(Eon_Element *thiz)
{
	return thiz->e;
}

void eon_element_keyboard_proxy_set(Eon_Element *thiz,
		Eon_Keyboard_Proxy *keyboard_proxy)
{
	if (thiz->keyboard_proxy)
		eon_keyboard_proxy_delete(thiz->keyboard_proxy);
	thiz->keyboard_proxy = keyboard_proxy;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_element(Ender_Element *e)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	if (EINA_MAGIC_CHECK(thiz, EON_ELEMENT_MAGIC))
		return EINA_TRUE;
	return EINA_FALSE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_height_get(Ender_Element *e, double *height)
{
	ender_element_property_value_get(e, EON_ELEMENT_ACTUAL_HEIGHT, height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_width_get(Ender_Element *e, double *width)
{
	ender_element_property_value_get(e, EON_ELEMENT_ACTUAL_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_get(Ender_Element *e, double *height)
{
	ender_element_property_value_get(e, EON_ELEMENT_HEIGHT, height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_set(Ender_Element *e, double height)
{
	ender_element_property_value_set(e, EON_ELEMENT_HEIGHT, height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_get(Ender_Element *e, double *width)
{
	ender_element_property_value_get(e, EON_ELEMENT_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_set(Ender_Element *e, double width)
{
	ender_element_property_value_set(e, EON_ELEMENT_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_get(Ender_Element *e, double *width)
{
	ender_element_property_value_get(e, EON_ELEMENT_MIN_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_set(Ender_Element *e, double width)
{
	ender_element_property_value_set(e, EON_ELEMENT_MIN_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_set(Ender_Element *e, double height)
{
	ender_element_property_value_set(e, EON_ELEMENT_MIN_HEIGHT, height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_get(Ender_Element *e, double *height)
{
	ender_element_property_value_get(e, EON_ELEMENT_MIN_HEIGHT, height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_get(Ender_Element *e, double *width)
{
	ender_element_property_value_get(e, EON_ELEMENT_MAX_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_set(Ender_Element *e, double width)
{
	ender_element_property_value_set(e, EON_ELEMENT_MAX_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_preferred_width_get(Ender_Element *e, double *width)
{
	*width = -1;
	ender_element_property_value_get(e, EON_ELEMENT_PREFERRED_WIDTH, width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_preferred_height_get(Ender_Element *e, double *height)
{
	*height = -1;
	ender_element_property_value_get(e, EON_ELEMENT_PREFERRED_HEIGHT, height, NULL);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_set(Ender_Element *e, double height)
{
	ender_element_property_value_set(e, EON_ELEMENT_MAX_HEIGHT, height, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_get(Ender_Element *e, double *height)
{
	ender_element_property_value_get(e, EON_ELEMENT_MAX_HEIGHT, height, NULL);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_visibility_set(Ender_Element *e, double visibility)
{
	ender_element_property_value_set(e, EON_ELEMENT_VISIBILITY, visibility, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_visibility_get(Ender_Element *e, double *visibility)
{
	ender_element_property_value_get(e, EON_ELEMENT_VISIBILITY, visibility, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_parent_get(Ender_Element *e, Ender_Element **parent)
{
	ender_element_property_value_get(e, EON_ELEMENT_PARENT, parent, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_focusable_get(Ender_Element *e, Eina_Bool *focusable)
{
	ender_element_property_value_get(e, EON_ELEMENT_FOCUSABLE, focusable, NULL);
}

