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

typedef struct _Eon_Element
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
	/* function pointers */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Renderer_Get renderer_get;
	Eon_Element_Needs_Setup needs_setup;
	Enesim_Renderer_Delete free;
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
	/* misc */
	Eina_Bool do_needs_setup : 1;
	const char *name;
	void *data;
	Ender_Element *e;
} Eon_Element;

static inline Eon_Element * _eon_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_ELEMENT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static double _element_min_width_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->min_width_get)
		v = thiz->min_width_get(e);
	return MAX(v, thiz->min_size.width);
}

static double _element_min_height_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->min_height_get)
		v = thiz->min_height_get(e);
	return MAX(v, thiz->min_size.height);
}

static double _element_max_width_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = DBL_MAX;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->max_width_get)
		v = thiz->max_width_get(e);
	return MIN(v, thiz->max_size.width);
}

static double _element_max_height_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = DBL_MAX;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->max_height_get)
		v = thiz->max_height_get(e);
	return MIN(v, thiz->max_size.height);
}

static Eina_Bool _eon_element_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **error)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);

	if (thiz->setup)
		return thiz->setup(e, &thiz->current, s, error);
	return EINA_TRUE;
}

static void _eon_element_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);

	thiz->past = thiz->current;
	thiz->do_needs_setup = EINA_FALSE;
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_element_actual_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->current.actual_size.height;
}

static void _eon_element_actual_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->current.actual_size.width;
}

static void _eon_element_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->size.height;
}

static void _eon_element_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->size.height = height;
}

static void _eon_element_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->size.width;
}

static void _eon_element_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->size.width = width;
}

static void _eon_element_min_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_size.width = width;
}

static void _eon_element_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = 0;
	double max;

	if (!width) return;
	thiz = _eon_element_get(r);
	e = thiz->e;
	if (!thiz) return;
	v = _element_min_width_get(e);
	/* make sure we dont conflict with the max */
	max = _element_max_width_get(e);
	*width = MIN(v, max);
}

static void _eon_element_min_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_size.height = height;
}

static void _eon_element_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = 0;
	double max;

	if (!height) return;
	thiz = _eon_element_get(r);
	e = thiz->e;
	if (!thiz) return;
	v = _element_min_height_get(e);
	/* make sure we dont conflict with the max */
	max = _element_max_height_get(e);
	*height = MIN(v, max);
}

static void _eon_element_max_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_size.width = width;
}


static void _eon_element_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = DBL_MAX;
	double min;

	if (!width) return;

	thiz = _eon_element_get(r);
	e = thiz->e;
	v = _element_max_width_get(e);
	/* make sure we dont conflict with the min */
	min = _element_min_width_get(e);
	*width = MAX(v, min);
}

static void _eon_element_max_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_size.height = height;
}

static void _eon_element_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = DBL_MAX;
	double min;

	if (!height) return;

	thiz = _eon_element_get(r);
	e = thiz->e;
	v = _element_max_height_get(e);
	/* make sure we dont conflict with the min */
	min = _element_min_height_get(e);
	*height = MAX(v, min);
}

static void _eon_element_preferred_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	Ender_Element *e;

	if (!width) return;

	thiz = _eon_element_get(r);
	e = thiz->e;
	*width = -1;
	if (thiz->preferred_width_get)
		*width = thiz->preferred_width_get(e);
}

static void _eon_element_preferred_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	Ender_Element *e;

	if (!height) return;

	thiz = _eon_element_get(r);
	e = thiz->e;
	*height = -1;
	if (thiz->preferred_height_get)
		*height = thiz->preferred_height_get(e);
}


static void _eon_element_visibility_set(Enesim_Renderer *r, double visible)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->current.visible = visible;
}

static void _eon_element_visibility_get(Enesim_Renderer *r, double *visible)
{
	Eon_Element *thiz;

	if (!visible) return;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*visible = thiz->current.visible;
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

	set = thiz->size.width;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		_eon_element_preferred_width_get(r, &set);
	_eon_element_min_width_get(r, &min);
	_eon_element_max_width_get(r, &max);
	rw = MIN(set, max);
	rw = MAX(rw, min);

	//printf("real width %s = %g (%g %g %g)\n", thiz->name, rw, min, set, max);
	*width = rw;
}

static void _eon_element_real_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double rh;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->size.height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		_eon_element_preferred_height_get(r, &set);
	_eon_element_min_height_get(r, &min);
	_eon_element_max_height_get(r, &max);
	rh = MIN(set, max);
	rh = MAX(rh, min);

	//printf("real height %s = %g (%g %g %g)\n", thiz->name, rh, min, set, max);
	*height = rh;
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static const char * _eon_element_name(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->name;
}

/* Given that we dont support the x_origin property, we must inform correctly to the enesim system that we can be at a specific position*/
/* TODO Remove the function call */
static void _eon_element_boundings(Enesim_Renderer *r,
		const Enesim_Renderer_State *states[ENESIM_RENDERER_STATES],
		Enesim_Rectangle *rect)
{
	Eon_Element *thiz;
	Eon_Size size;

	thiz = _eon_element_get(r);
	size = thiz->current.actual_size;
	/* There's no layout, or the layout didnt set an active width/height */
	if (size.width < 0 || size.height < 0)
	{
		_eon_element_real_width_get(r, &size.width);
		_eon_element_real_height_get(r, &size.height);
	}
	rect->x = thiz->current.actual_position.x;
	rect->y = thiz->current.actual_position.y;
	rect->w = size.width;
	rect->h = size.height;
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

static Enesim_Renderer_Descriptor _descriptor = {
	/* .version = 			*/ ENESIM_RENDERER_API,
	/* .name = 			*/ _eon_element_name,
	/* .free = 			*/ _eon_element_free,
	/* .boundings = 		*/ _eon_element_boundings,
	/* .destination_boundings = 	*/ NULL,
	/* .flags = 			*/ NULL,
	/* .hints_get =			*/ NULL,
	/* .is_inside = 		*/ NULL,
	/* .damage = 			*/ NULL,
	/* .has_changed = 		*/ NULL,
	/* .sw_setup = 			*/ NULL,
	/* .sw_cleanup = 		*/ NULL
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_initialize(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
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

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

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
	thiz->name = descriptor->name;

	r = enesim_renderer_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

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

/*
 * Check whenever an element has changed, both a setup case or not
 * It is just a wrapper on top of the enesim interface
 */
Eina_Bool eon_element_has_changed(Ender_Element *e)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);

	return enesim_renderer_has_changed(r);
}

/*
 * Check whenever an element needs to do the setup
 * The only property relative to a setup is the size
 */
Eina_Bool eon_element_needs_setup(Ender_Element *e)
{
	Enesim_Renderer *r;
	Eon_Element *thiz;
	Eina_Bool ret;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);

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

void eon_element_damages_get(Ender_Element *e, Enesim_Renderer_Damage_Cb cb, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	enesim_renderer_damages_get(r, cb, data);
}

void eon_element_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->current.actual_size.width = width;
	if (thiz->actual_width_set)
		thiz->actual_width_set(r, width);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->current.actual_size.height = height;
	if (thiz->actual_height_set)
		thiz->actual_height_set(r, height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->current.actual_size.width = width;
	thiz->current.actual_size.height = height;
	if (thiz->actual_width_set)
		thiz->actual_width_set(r, width);
	if (thiz->actual_height_set)
		thiz->actual_height_set(r, height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_size_get(Enesim_Renderer *r, Eon_Size *size)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	size->width = thiz->current.actual_size.width;
	size->height = thiz->current.actual_size.height;
}

void eon_element_actual_x_set(Enesim_Renderer *r, double x)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->current.actual_position.x = x;
	if (thiz->actual_x_set)
		thiz->actual_x_set(r, x);
}

void eon_element_actual_y_set(Enesim_Renderer *r, double y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->current.actual_position.y = y;
	if (thiz->actual_y_set)
		thiz->actual_y_set(r, y);
}

void eon_element_actual_position_set(Enesim_Renderer *r, double x, double y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->current.actual_position.x = x;
	thiz->current.actual_position.y = y;
	if (thiz->actual_x_set)
		thiz->actual_x_set(r, x);
	if (thiz->actual_y_set)
		thiz->actual_y_set(r, y);
}

void eon_element_actual_position_get(Enesim_Renderer *r, double *x, double *y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (x) *x = thiz->current.actual_position.x;
	if (y) *y = thiz->current.actual_position.y;
}

void eon_element_actual_geometry_set(Enesim_Renderer *r, Eon_Geometry *g)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->current.actual_position.x = g->x;
	thiz->current.actual_position.y = g->y;
	thiz->current.actual_size.width = g->width;
	thiz->current.actual_size.height = g->height;

	if (thiz->actual_x_set)
		thiz->actual_x_set(r, g->x);
	if (thiz->actual_y_set)
		thiz->actual_y_set(r, g->y);
	if (thiz->actual_width_set)
		thiz->actual_width_set(r, g->width);
	if (thiz->actual_height_set)
		thiz->actual_height_set(r, g->height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_real_relative_size_get(Ender_Element *e, Eon_Size *relative, Eon_Size *size)
{
	Enesim_Renderer *r;
	Eon_Element *thiz;
	double rw, rh;
	double min, set, max;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->size.width;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->width;
	_eon_element_min_width_get(r, &min);
	_eon_element_max_width_get(r, &max);
	rw = MIN(max, set);
	rw = MAX(rw, min);
	size->width = rw;
	//printf("relative width %s = %g (%g %g %g) %g\n", thiz->name, rw, min, set, max, relative->width);

	set = thiz->size.height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->height;
	_eon_element_min_height_get(r, &min);
	_eon_element_max_height_get(r, &max);
	rh = MIN(max, set);
	rh = MAX(rh, min);
	size->height = rh;
	//printf("relative height %s = %g (%g %g %g) %g\n", thiz->name, rh, min, set, max, relative->height);

	//printf("relative size %s = %gx%g\n", thiz->name, size->width, size->height);
}

void eon_element_real_size_get(Ender_Element *e, Eon_Size *size)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	_eon_element_real_width_get(r, &size->width);
	_eon_element_real_height_get(r, &size->height);
}

void eon_element_real_width_get(Ender_Element *e, double *width)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	_eon_element_real_width_get(r, width);
}

void eon_element_real_height_get(Ender_Element *e, double *height)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	_eon_element_real_height_get(r, height);
}

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_element_get(r);
	r =  thiz->renderer_get(e);
	if (!r)
	{
		printf("FIXME!!!!!!!! the implementation of %s does not returned a renderer\n", ender_element_name_get(e));
	}

	return r;
}

#define _eon_element_actual_width_set NULL
#define _eon_element_actual_height_set NULL
#define _eon_element_actual_x_set NULL
#define _eon_element_actual_y_set NULL
#define _eon_element_preferred_width_set NULL
#define _eon_element_preferred_height_set NULL
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
