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
 */
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
	Enesim_Renderer_Sw_Fill fill;
	/* function pointers */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Renderer_Get renderer_get;
	Enesim_Renderer_Delete free;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
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
	Eina_Bool changed;
	const char *name;
	void *data;
	/* FIXME for later
	 * Enesim_Renderer *thiz_r;
	 * Ender_Element *thiz_e;
	 */
} Eon_Element;

static inline Eon_Element * _eon_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_ELEMENT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_element_draw(Enesim_Renderer *r, int x, int y, unsigned int len, void *dst)
{
	Eon_Element *thiz;
	Ender_Element *e;
	Enesim_Renderer *real_r;

	thiz = _eon_element_get(r);
	e = ender_element_renderer_from(r);
	real_r = eon_element_renderer_get(e);
	thiz->fill(real_r, x, y, len, dst);
}

static double _element_min_width_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->min_width_get)
		v = thiz->min_width_get(e);
	return MAX(v, thiz->min_width);
}

static double _element_min_height_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->min_height_get)
		v = thiz->min_height_get(e);
	return MAX(v, thiz->min_height);
}

static double _element_max_width_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = DBL_MAX;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->max_width_get)
		v = thiz->max_width_get(e);
	return MIN(v, thiz->max_width);
}

static double _element_max_height_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	double v = DBL_MAX;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return v;
	if (thiz->max_height_get)
		v = thiz->max_height_get(e);
	return MIN(v, thiz->max_height);
}

static void _element_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	eon_element_changed_set(e, EINA_TRUE);
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

static void _eon_element_min_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_width = width;
}

static void _eon_element_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = 0;
	double max;

	if (!width) return;
	e = ender_element_renderer_from(r);
	thiz = _eon_element_get(r);
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
	thiz->min_height = height;
}

static void _eon_element_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = 0;
	double max;

	if (!height) return;
	e = ender_element_renderer_from(r);
	thiz = _eon_element_get(r);
	if (!thiz) return;
	v = _element_min_height_get(e);
	/* make sure we dont conflict with the max */
	max = _element_max_height_get(e);
	*height = MIN(v, max);
}

static void _eon_element_max_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_width = width;
}


static void _eon_element_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	Ender_Element *e;
	double v = DBL_MAX;
	double min;

	if (!width) return;
	e = ender_element_renderer_from(r);
	thiz = _eon_element_get(r);
	if (!thiz) return;
	v = _element_max_width_get(e);
	/* make sure we dont conflict with the min */
	min = _element_min_width_get(e);
	*width = MAX(v, min);
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
	Ender_Element *e;
	double v = DBL_MAX;
	double min;

	if (!height) return;
	e = ender_element_renderer_from(r);
	thiz = _eon_element_get(r);
	if (!thiz) return;
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
	e = ender_element_renderer_from(r);
	*width = -1;
	thiz = _eon_element_get(r);
	if (thiz->preferred_width_get)
		*width = thiz->preferred_width_get(e);
}

static void _eon_element_preferred_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	Ender_Element *e;

	if (!height) return;
	e = ender_element_renderer_from(r);
	thiz = _eon_element_get(r);
	*height = -1;
	if (thiz->preferred_height_get)
		*height = thiz->preferred_height_get(e);
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
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		_eon_element_preferred_width_get(r, &set);
	_eon_element_min_width_get(r, &min);
	_eon_element_max_width_get(r, &max);
	rw = set > max ? max : set;
	rw = rw < min ? min : rw;

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

	set = thiz->height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		_eon_element_preferred_height_get(r, &set);
	_eon_element_min_height_get(r, &min);
	_eon_element_max_height_get(r, &max);
	rh = set > max ? max : set;
	rh = rh < min ? min : rh;

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
static void _eon_element_boundings(Enesim_Renderer *r, Enesim_Rectangle *rect)
{
	Eon_Element *thiz;
	Eon_Size size;

	thiz = _eon_element_get(r);
	eon_element_actual_size_get(r, &size);
	/* There's no layout, or the layout didnt set an active width/height */
	if (size.width < 0 || size.height < 0)
	{
		_eon_element_real_width_get(r, &size.width);
		_eon_element_real_height_get(r, &size.height);
	}
	rect->x = thiz->actual_x;
	rect->y = thiz->actual_y;
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

static Eina_Bool _eon_element_sw_setup(Enesim_Renderer *r,
		const Enesim_Renderer_State *state,
		Enesim_Surface *s,
		Enesim_Renderer_Sw_Fill *fill, Enesim_Error **error)
{
	Eon_Element *thiz;
	Ender_Element *e;
	Enesim_Renderer *real_r;

	thiz = _eon_element_get(r);
	e = ender_element_renderer_from(r);
	if (!e) return EINA_FALSE;
	if (!eon_element_setup(e))
	{
		ENESIM_RENDERER_ERROR(r, error, "The element_setup() failed");
		return EINA_FALSE;
	}

	real_r = eon_element_renderer_get(e);
	if (!enesim_renderer_setup(real_r, s, error))
	{
		ENESIM_RENDERER_ERROR(r, error, "The renderer setup failed");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(real_r);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _eon_element_draw;
	return EINA_TRUE;
}

static void _eon_element_sw_cleanup(Enesim_Renderer *r, Enesim_Surface *s)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	/* FIXME why do we have a sw_cleanup function and not just cleanup
	 * similar to what the setup does
	 */
	if (thiz->sw_cleanup)
		thiz->sw_cleanup(r, s);
}

static void _eon_element_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	*flags = ENESIM_RENDERER_FLAG_ARGB8888;
}

static void _eon_element_damage(Enesim_Renderer *r, Enesim_Renderer_Damage_Cb cb, void *data)
{
	Ender_Element *e;
	Enesim_Renderer *real_r;

	e = ender_element_renderer_from(r);
	real_r = eon_element_renderer_get(e);
	enesim_renderer_damages_get(real_r, cb, data);
}

static Enesim_Renderer_Descriptor _descriptor = {
	/* .version =     */ ENESIM_RENDERER_API,
	/* .name =        */ _eon_element_name,
	/* .free =        */ _eon_element_free,
	/* .boundings =   */ _eon_element_boundings,
	/* .flags =       */ _eon_element_flags,
	/* .is_inside =   */ NULL,
	/* .damage =      */ _eon_element_damage,
	/* .has_changed = */ NULL,
	/* .sw_setup =    */ _eon_element_sw_setup,
	/* .sw_cleanup =  */ _eon_element_sw_cleanup
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
	/* whenever the theme has changed we should notify
	 * the change on this element too
	 */
	ender_event_listener_add(ender, "Mutation", _element_changed, NULL);
	if (thiz->initialize)
		thiz->initialize(ender);
}

Eina_Bool eon_element_setup(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;
	Eina_Bool ret = EINA_TRUE;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	if (!thiz->changed)
		goto end;

	if (thiz->setup)
		ret = thiz->setup(e);
	thiz->changed = EINA_FALSE;
end:
	return ret;
}

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Element));
	EINA_MAGIC_SET(thiz, EON_ELEMENT_MAGIC);
	thiz->data = data;
	thiz->actual_width = -1;
	thiz->actual_height = -1;
	thiz->max_width = thiz->max_height = DBL_MAX;
	thiz->min_width = thiz->min_height = 0;
	thiz->width = -1;
	thiz->height = -1;

	/* Set the function pointers */
	thiz->initialize = descriptor->initialize;
	thiz->setup = descriptor->setup;
	thiz->renderer_get = descriptor->renderer_get;
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
	thiz->sw_cleanup = descriptor->sw_cleanup;
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

void eon_element_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	if (thiz->actual_width_set)
		thiz->actual_width_set(r, width);
	thiz->changed = EINA_TRUE;
}

void eon_element_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_height = height;
	if (thiz->actual_height_set)
		thiz->actual_height_set(r, height);
	thiz->changed = EINA_TRUE;
}

void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	thiz->actual_height = height;
	if (thiz->actual_width_set)
		thiz->actual_width_set(r, width);
	if (thiz->actual_height_set)
		thiz->actual_height_set(r, height);
	thiz->changed = EINA_TRUE;
}

void eon_element_actual_size_get(Enesim_Renderer *r, Eon_Size *size)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	size->width = thiz->actual_width;
	size->height = thiz->actual_height;
}

void eon_element_actual_x_set(Enesim_Renderer *r, double x)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->actual_x = x;
	if (thiz->actual_x_set)
		thiz->actual_x_set(r, x);
	thiz->changed = EINA_TRUE;
}

void eon_element_actual_y_set(Enesim_Renderer *r, double y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->actual_y = y;
	if (thiz->actual_y_set)
		thiz->actual_y_set(r, y);
	thiz->changed = EINA_TRUE;
}

void eon_element_actual_position_set(Enesim_Renderer *r, double x, double y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	thiz->actual_x = x;
	thiz->actual_y = y;
	if (thiz->actual_x_set)
		thiz->actual_x_set(r, x);
	if (thiz->actual_y_set)
		thiz->actual_y_set(r, y);
	thiz->changed = EINA_TRUE;
}

void eon_element_actual_position_get(Enesim_Renderer *r, double *x, double *y)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (x) *x = thiz->actual_x;
	if (y) *y = thiz->actual_y;
}

void eon_element_real_relative_size_get(Ender_Element *e, Eon_Size *relative, Eon_Size *size)
{
	Enesim_Renderer *r;
	Eon_Element *thiz;
	double rw, rh;
	double min, set, max;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->width;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->width;
	_eon_element_min_width_get(r, &min);
	_eon_element_max_width_get(r, &max);
	rw = set > max ? max : set;
	rw = rw < min ? min : rw;
	size->width = rw;
	//printf("relative width %s = %g (%g %g %g) %g\n", thiz->name, rw, min, set, max, relative->width);

	set = thiz->height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->height;
	_eon_element_min_height_get(r, &min);
	_eon_element_max_height_get(r, &max);
	rh = set > max ? max : set;
	rh = rh < min ? min : rh;
	size->height = rh;
	//printf("relative height %s = %g (%g %g %g) %g\n", thiz->name, rh, min, set, max, relative->height);

	//printf("relative size %s = %gx%g\n", thiz->name, size->width, size->height);
}

void eon_element_real_size_get(Ender_Element *e, Eon_Size *size)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	_eon_element_real_width_get(r, &size->width);
	_eon_element_real_height_get(r, &size->height);
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

Eina_Bool eon_element_has_changed(Ender_Element *e)
{
	Enesim_Renderer *r;
	Eon_Element *thiz;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	return thiz->changed;
}

void eon_element_changed_set(Ender_Element *e, Eina_Bool changed)
{
	Eon_Element *thiz;
	Ender_Element *parent;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_element_get(r);
	thiz->changed = changed;
	parent = ender_element_parent_get(e);
	if (parent)
	{
		eon_element_changed_set(parent, changed);
	}
}

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
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
EAPI void eon_element_preferred_width_get(Ender_Element *e, double *width)
{
	*width = -1;
	ender_element_value_get(e, "preferred_width", width, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_preferred_height_get(Ender_Element *e, double *height)
{
	*height = -1;
	ender_element_value_get(e, "preferred_height", height, NULL);
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
