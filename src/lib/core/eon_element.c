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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"

#include "eon_private_input.h"
#include "eon_private_keyboard_proxy.h"
#include "eon_private_element.h"
#include "eon_private_backend.h"

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
#define EON_ELEMENT_MAGIC 0xe0400001
#define EON_ELEMENT_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_ELEMENT_MAGIC)
#define EON_ELEMENT_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_ELEMENT_MAGIC, ret)

typedef struct _Eon_Element_Factory
{
	int id;
} Eon_Element_Factory;

static Eina_Hash *_factories = NULL;

static Ender_Property *EON_ELEMENT_PARENT;
static Ender_Property *EON_ELEMENT_NAME;
static Ender_Property *EON_ELEMENT_FOCUSABLE;
static Ender_Property *EON_ELEMENT_VISIBILITY;
static Ender_Property *EON_ELEMENT_WIDTH;
static Ender_Property *EON_ELEMENT_HEIGHT;
static Ender_Property *EON_ELEMENT_MIN_WIDTH;
static Ender_Property *EON_ELEMENT_MIN_HEIGHT;
static Ender_Property *EON_ELEMENT_MAX_WIDTH;
static Ender_Property *EON_ELEMENT_MAX_HEIGHT;
static Ender_Property *EON_ELEMENT_ACTUAL_WIDTH;
static Ender_Property *EON_ELEMENT_ACTUAL_HEIGHT;
static Ender_Property *EON_ELEMENT_HORIZONTAL_ALIGNMENT;
static Ender_Property *EON_ELEMENT_VERTICAL_ALIGNMENT;

typedef struct _Eon_Element_Parent_Relation
{
	Ender_Element *parent;
	Eon_Free_Func free_func;
	void *data;
} Eon_Element_Parent_Relation;

struct _Eon_Element
{
	EINA_MAGIC
	/* properties */
	Eon_Vertical_Alignment vertical_alignment;
	Eon_Horizontal_Alignment horizontal_alignment;

	Eon_Element_State current;
	Eon_Element_State past;
	/* TODO */
	Eon_Size min_size;
	Eon_Size max_size;
	Eon_Size size;
	/* private */
	Eina_Bool changed;
	Eon_Backend *backend;
	Eon_Geometry geometry;
	Eon_Hints last_hints;

	Eon_Element_Parent_Relation parent_relation;
	Eon_Keyboard_Proxy *keyboard_proxy;
	/* descriptor */
	Eon_Element_Descriptor descriptor;
	/* misc */
	Ender_Element *current_focus;
	Eina_Bool do_needs_setup : 1;
	char *user_name;
	void *data;
	Ender_Element *e;
};
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
	eon_element_inform_change(thiz);
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
	eon_element_inform_change(thiz);
}

static void _eon_element_min_width_set(Eon_Element *thiz, double width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->min_size.width = width;
}

static void _eon_element_min_width_get(Eon_Element *thiz, double *width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!width) return;
	*width = thiz->min_size.width;
}

static void _eon_element_min_height_set(Eon_Element *thiz, double height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->min_size.height = height;
}

static void _eon_element_min_height_get(Eon_Element *thiz, double *height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!height) return;
	*height = thiz->min_size.height;
}

static void _eon_element_max_width_set(Eon_Element *thiz, double width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->max_size.width = width;
}

static void _eon_element_max_width_get(Eon_Element *thiz, double *width)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!width) return;
	*width = thiz->max_size.width;
}

static void _eon_element_max_height_set(Eon_Element *thiz, double height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->max_size.height = height;
}

static void _eon_element_max_height_get(Eon_Element *thiz, double *height)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!height) return;
	*height = thiz->max_size.height;
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
	*parent = thiz->parent_relation.parent;
}

static void _eon_element_focusable_get(Eon_Element *thiz, Eina_Bool *focusable)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);

	if (!focusable) return;

	/* if not visible it can receive focus */
	if (!thiz->current.visible)
	{
		*focusable = EINA_FALSE;
		return;
	}

	if (thiz->descriptor.is_focusable)
		*focusable = thiz->descriptor.is_focusable(thiz);
	else
		*focusable = EINA_FALSE;
}

static void _eon_element_name_set(Eon_Element *thiz, const char *name)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (thiz->user_name)
	{
		free(thiz->user_name);
		thiz->user_name = NULL;
	}
	if (name)
		thiz->user_name= strdup(name);
}

static void _eon_element_name_get(Eon_Element *thiz, const char **name)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	if (!thiz->user_name)
		*name = thiz->descriptor.name;
	else
		*name = thiz->user_name;
}

static void _eon_element_horizontal_alignment_set(Eon_Element *thiz, Eon_Horizontal_Alignment alignement)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->horizontal_alignment = alignement;
}

static void _eon_element_vertical_alignment_set(Eon_Element *thiz, Eon_Vertical_Alignment alignement)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);
	thiz->vertical_alignment = alignement;
}

static void _eon_element_delete(Eon_Element *thiz)
{
	if (thiz->descriptor.free)
		thiz->descriptor.free(thiz);
	free(thiz);
}

#define _eon_element_parent_set NULL
#define _eon_element_actual_width_set NULL
#define _eon_element_actual_height_set NULL
#define _eon_element_actual_x_set NULL
#define _eon_element_actual_y_set NULL
#define _eon_element_preferred_width_set NULL
#define _eon_element_preferred_height_set NULL
#define _eon_element_focusable_set NULL
#define _eon_element_vertical_alignment_get NULL
#define _eon_element_horizontal_alignment_get NULL
#include "eon_generated_element.c"
/*----------------------------------------------------------------------------*
 *                             Internal functions                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_factory_setup(Eon_Element *thiz)
{
	Eon_Element_Factory *f;
	char element_name[PATH_MAX];
	const char *descriptor_name = NULL;

	if (!_factories)
		_factories = eina_hash_string_superfast_new(NULL);
	if (thiz->descriptor.name)
		descriptor_name = thiz->descriptor.name;
	if (!descriptor_name)
		descriptor_name = "unknown";
	f = eina_hash_find(_factories, descriptor_name);
	if (!f)
	{
		f = calloc(1, sizeof(Eon_Element_Factory));
		eina_hash_add(_factories, descriptor_name, f);
	}
	/* assign a new name for it automatically */
	snprintf(element_name, PATH_MAX, "%s%d", descriptor_name, f->id++);
	_eon_element_name_set(thiz, element_name);
}
#if 0
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

	//printf("real width %s = %g (%g %g %g)\n", thiz->descriptor.name, rw, min, set, max);
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

	//printf("real height %s = %g (%g %g %g)\n", thiz->descriptor.name, rh, min, set, max);
	*height = rh;
}
#endif
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_init(void)
{
	_eon_element_init();
}

void eon_element_shutdown(void)
{
	_eon_element_shutdown();
}

void eon_element_initialize(Ender_Element *e)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	/* store the renderer and the ender to avoid so many functions calls */
	thiz->e = e;
	/* whenever the theme has changed we should notify
	 * the change on this element too
	 */
	if (thiz->descriptor.initialize)
		thiz->descriptor.initialize(e);
}

/* functions to manage the focus/mouse events */
void eon_element_feed_key_down(Ender_Element *e, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);

	if (!thiz->keyboard_proxy) return;
	eon_keyboard_proxy_feed_key_down(thiz->keyboard_proxy, e, input, from, key);
}

void eon_element_feed_key_up(Ender_Element *e, Eon_Input *input, Ender_Element *from, Eon_Keyboard_Key *key)
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
	thiz->changed = EINA_TRUE;

	thiz->data = data;
	thiz->current.actual_size.width = -1;
	thiz->current.actual_size.height = -1;
	thiz->current.visible = EINA_TRUE;
	thiz->max_size.width = thiz->max_size.height = DBL_MAX;
	thiz->min_size.width = thiz->min_size.height = 0;
	thiz->size.width = -1;
	thiz->size.height = -1;
	thiz->horizontal_alignment = EON_HORIZONTAL_ALIGNMENT_CENTER;
	thiz->vertical_alignment = EON_VERTICAL_ALIGNMENT_CENTER;

	/* Set the function pointers */
	thiz->descriptor.initialize = descriptor->initialize;
	thiz->descriptor.backend_added = descriptor->backend_added;
	thiz->descriptor.backend_removed = descriptor->backend_removed;
	thiz->descriptor.renderer_get = descriptor->renderer_get;
	thiz->descriptor.hints_get = descriptor->hints_get;
	thiz->descriptor.geometry_set = descriptor->geometry_set;
	thiz->descriptor.is_focusable = descriptor->is_focusable;
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.name = descriptor->name;

	_eon_element_factory_setup(thiz);
	printf("element of name %s created %p\n", thiz->user_name, thiz);
	return thiz;
}

void * eon_element_data_get(Eon_Element *thiz)
{
	return thiz->data;
}

#if 0
void eon_element_actual_width_set(Eon_Element *thiz, double width)
{
	thiz->current.actual_size.width = width;
	if (thiz->descriptor.actual_width_set)
		thiz->descriptor.actual_width_set(thiz, width);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_height_set(Eon_Element *thiz, double height)
{
	thiz->current.actual_size.height = height;
	if (thiz->descriptor.actual_height_set)
		thiz->descriptor.actual_height_set(thiz, height);
	thiz->do_needs_setup = EINA_TRUE;
}

void eon_element_actual_size_set(Eon_Element *thiz, double width, double height)
{
	thiz->current.actual_size.width = width;
	thiz->current.actual_size.height = height;
	if (thiz->descriptor.actual_width_set)
		thiz->descriptor.actual_width_set(thiz, width);
	if (thiz->descriptor.actual_height_set)
		thiz->descriptor.actual_height_set(thiz, height);
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
	if (thiz->descriptor.actual_x_set)
		thiz->descriptor.actual_x_set(thiz, x);
}

void eon_element_actual_y_set(Eon_Element *thiz, double y)
{
	thiz->current.actual_position.y = y;
	if (thiz->descriptor.actual_y_set)
		thiz->descriptor.actual_y_set(thiz, y);
}

void eon_element_actual_position_set(Eon_Element *thiz, double x, double y)
{
	thiz->current.actual_position.x = x;
	thiz->current.actual_position.y = y;
	if (thiz->descriptor.actual_x_set)
		thiz->descriptor.actual_x_set(thiz, x);
	if (thiz->descriptor.actual_y_set)
		thiz->descriptor.actual_y_set(thiz, y);
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

	if (thiz->descriptor.actual_x_set)
		thiz->descriptor.actual_x_set(thiz, g->x);
	if (thiz->descriptor.actual_y_set)
		thiz->descriptor.actual_y_set(thiz, g->y);
	if (thiz->descriptor.actual_width_set)
		thiz->descriptor.actual_width_set(thiz, g->width);
	if (thiz->descriptor.actual_height_set)
		thiz->descriptor.actual_height_set(thiz, g->height);
	thiz->do_needs_setup = EINA_TRUE;
}
#endif

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
	//printf("relative width %s = %g (%g %g %g) %g\n", thiz->descriptor.name, rw, min, set, max, relative->width);

	set = thiz->size.height;
	/* if the user has not set a value we better use the preferred one */
	if (set < 0)
		set = relative->height;
	_eon_element_min_height_get(thiz, &min);
	_eon_element_max_height_get(thiz, &max);
	rh = MIN(max, set);
	rh = MAX(rh, min);
	size->height = rh;
	//printf("relative height %s = %g (%g %g %g) %g\n", thiz->descriptor.name, rh, min, set, max, relative->height);

	//printf("relative size %s = %gx%g\n", thiz->descriptor.name, size->width, size->height);
}

#if 0
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
#endif

Enesim_Renderer * eon_element_renderer_get(Ender_Element *e)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	thiz = ender_element_object_get(e);
	r =  thiz->descriptor.renderer_get(e);
	if (!r)
	{
		printf("FIXME!!!!!!!! the implementation of %s does not returned a renderer\n", ender_element_name_get(e));
	}

	return r;
}

/* this function should be called before setting a geometry. When we set a geometry
 * we need to take into account the last time the min/max/preferred sizes
 * where calculated
 * TODO export this?
 */
void eon_element_hints_get(Eon_Element *thiz, Eon_Hints *hints)
{
	EON_ELEMENT_MAGIC_CHECK(thiz);

	/* just use the cached versions in case we haven't changed */
	if (!thiz->changed)
	{
		printf("HINTS CACHED\n");
		*hints = thiz->last_hints;
		return;
	}

	hints->min = thiz->min_size;
	hints->max = thiz->max_size;
	hints->preferred.width = -1;
	hints->preferred.height = -1;

	if (thiz->descriptor.hints_get)
	{
		Eon_Hints ihints;

		eon_hints_initialize(&ihints);
		thiz->descriptor.hints_get(thiz, &ihints);

		hints->min.width = MAX(hints->min.width, ihints.min.width);
		hints->min.height = MAX(hints->min.height, ihints.min.height);

		hints->max.width = MIN(hints->max.width, ihints.max.width);
		hints->max.height = MIN(hints->max.height, ihints.max.height);

		hints->preferred = ihints.preferred;
	}
	printf("1 '%s' hints are min: %g %g max: %g %g preferred: %g %g\n", thiz->descriptor.name,
			hints->min.width, hints->min.height,
			hints->max.width, hints->max.height,
			hints->preferred.width, hints->preferred.height);
	/* now use the user provided width/height */
	if (thiz->size.width != -1)
	{
		double w = thiz->size.width;

		if (w < hints->min.width)
			w = hints->min.width;
		if (w > hints->max.width)
			w = hints->max.width;

		printf("had width %g\n", thiz->size.width);
		hints->min.width = hints->max.width = hints->preferred.width = w;
	}
	if (thiz->size.height != -1)
	{
		double h = thiz->size.height;

		if (h < hints->min.height)
			h = hints->min.height;
		if (h > hints->max.height)
			h = hints->max.height;

		printf("had height %g\n", thiz->size.height);
		hints->min.height = hints->max.height = hints->preferred.height = h;
	}

	thiz->last_hints = *hints;
	printf("2 '%s' hints are min: %g %g max: %g %g preferred: %g %g\n", thiz->descriptor.name,
			hints->min.width, hints->min.height,
			hints->max.width, hints->max.height,
			hints->preferred.width, hints->preferred.height);
}

/* this should be called always after the hints_get and also pass the last
 * size hints
 */
void eon_element_geometry_set(Eon_Element *thiz, Eon_Geometry *g)
{
	Eon_Geometry rg = *g;

	printf("1 '%s' element setting geometry %g %g %g %g\n", thiz->descriptor.name, rg.x, rg.y, rg.width, rg.height);

	/* check the geometry against the last hints */
	eon_hints_geometry_align(&thiz->last_hints, &rg, thiz->horizontal_alignment, thiz->vertical_alignment);
	printf("2 '%s' element setting geometry %g %g %g %g\n", thiz->descriptor.name, rg.x, rg.y, rg.width, rg.height);

	if (thiz->descriptor.geometry_set)
		thiz->descriptor.geometry_set(thiz, &rg);
	thiz->geometry = rg;
	thiz->changed = EINA_FALSE;
}

void eon_element_geometry_get(Eon_Element *thiz, Eon_Geometry *g)
{
	*g = thiz->geometry;
}

Eon_Backend * eon_element_backend_get(Eon_Element *thiz)
{
	return thiz->backend;
}

void eon_element_backend_set(Eon_Element *thiz, Eon_Backend *backend)
{
	Ender_Element *e;

	e = thiz->e;
	if (thiz->backend)
	{
		if (thiz->descriptor.backend_removed)
			thiz->descriptor.backend_removed(thiz, thiz->backend);
		eon_backend_element_remove(thiz->backend, e);
	}
	thiz->backend = backend;
	if (thiz->backend)
	{
		if (thiz->descriptor.backend_added)
			thiz->descriptor.backend_added(thiz, thiz->backend);
		eon_backend_element_add(thiz->backend, e);
	}
}

/* Use this function to informa that the geometry (min/max/preferred) has to be
 * recalculated. 
 */
void eon_element_inform_change(Eon_Element *thiz)
{
	Eon_Element *thiz_parent;

	if (thiz->changed) return;
	thiz->changed = EINA_TRUE;
	/* inform the parent */
	if (!thiz->parent_relation.parent) return;
	thiz_parent = ender_element_object_get(thiz->parent_relation.parent);
	eon_element_inform_change(thiz_parent);
}

Eina_Bool eon_element_has_changed(Eon_Element *thiz)
{
	return thiz->changed;
}

Ender_Element * eon_element_ender_get(Eon_Element *thiz)
{
	return thiz->e;
}

void eon_element_parent_set(Ender_Element *e, Ender_Element *parent, void *data, Eon_Free_Func free_func)
{
	Eon_Element *thiz;

	thiz = ender_element_object_get(e);
	if (thiz->parent_relation.parent)
	{
		/* FIXME here we should do more stuff, like trigger
		 * an event, remove the old data, etc, etc
		 */
		printf("element already has a parent\n");
	}
	thiz->parent_relation.parent = parent;
	thiz->parent_relation.data = data;
	thiz->parent_relation.free_func = free_func;
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
EAPI void eon_element_name_set(Ender_Element *e, Eina_Bool *name)
{
	ender_element_property_value_set(e, EON_ELEMENT_NAME, name, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_name_get(Ender_Element *e, const char **name)
{
	ender_element_property_value_get(e, EON_ELEMENT_NAME, name, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_focusable_get(Ender_Element *e, Eina_Bool *focusable)
{
	ender_element_property_value_get(e, EON_ELEMENT_FOCUSABLE, focusable, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_horizontal_alignment_set(Ender_Element *e,
		Eon_Horizontal_Alignment alignment)
{
	ender_element_property_value_set(e, EON_ELEMENT_HORIZONTAL_ALIGNMENT, alignment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_vertical_alignment_set(Ender_Element *e,
		Eon_Vertical_Alignment alignment)
{
	ender_element_property_value_set(e, EON_ELEMENT_VERTICAL_ALIGNMENT, alignment, NULL);
}
