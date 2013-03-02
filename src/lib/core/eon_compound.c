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
#include "Eon_Theme.h"
#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_COMPOUND_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_COMPOUND_MAGIC)
#define EON_COMPOUND_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_COMPOUND_MAGIC, ret)

typedef struct _Eon_Compound
{
	EINA_MAGIC;
	/* the interface */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Free free;
	/* internal */
	Ender_Element *relative;
	void *data;
} Eon_Compound;

static inline Eon_Compound * _eon_compound_get(Eon_Element *ee)
{
	Eon_Compound *thiz;

	thiz = eon_widget_data_get(ee);
	EON_COMPOUND_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon Widget interface                           *
 *----------------------------------------------------------------------------*/
static void _eon_compound_initialize(Ender_Element *e)
{
	Eon_Compound *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);

	/* set the parent */
	eon_element_parent_set(thiz->relative, e, NULL, NULL);
	if (thiz->initialize)
		thiz->initialize(e);
}

static Eina_Bool _eon_compound_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Log **err)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	Eon_Element *relative_ee;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);

	relative_ee = ender_element_object_get(thiz->relative);
	eon_element_actual_x_set(relative_ee, state->actual_position.x);
	eon_element_actual_y_set(relative_ee, state->actual_position.y);
	eon_element_actual_size_set(relative_ee, state->actual_size.width, state->actual_size.height);

	if (thiz->setup)
		return thiz->setup(e, state, s, err);
	return EINA_TRUE;
}

static void _eon_compound_free(Eon_Element *ee)
{
	Eon_Compound *thiz;

	thiz = _eon_compound_get(ee);
	if (thiz->free)
		thiz->free(ee);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static double _eon_compound_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	double val;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);
	eon_element_min_width_get(thiz->relative, &val);
	return val;
}

static double _eon_compound_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	double val;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);
	eon_element_max_width_get(thiz->relative, &val);
	return val;
}

static double _eon_compound_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	double val;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);
	eon_element_min_height_get(thiz->relative, &val);
	return val;
}

static double _eon_compound_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	double val;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);
	eon_element_max_height_get(thiz->relative, &val);
	return val;
}

static double _eon_compound_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	double val;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);
	eon_element_preferred_width_get(thiz->relative, &val);
	return val;
}

static double _eon_compound_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Compound *thiz;
	Eon_Element *ee;
	double val;

	ee = ender_element_object_get(e);
	thiz = _eon_compound_get(ee);
	eon_element_preferred_height_get(thiz->relative, &val);
	return val;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Element * eon_compound_new(Eon_Compound_Descriptor *descriptor,
		Ender_Element *relative,
		void *data)
{
	Eon_Compound *thiz;
	Eon_Widget_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;
	Enesim_Renderer *r;
	Enesim_Renderer *relative_r;
	Eon_Element *relative_ee;

	thiz = calloc(1, sizeof(Eon_Compound));
	EINA_MAGIC_SET(thiz, EON_COMPOUND_MAGIC);
	thiz->data = data;
	thiz->initialize = descriptor->initialize;
	thiz->free = descriptor->free;
	thiz->relative = relative;

	pdescriptor.initialize = _eon_compound_initialize;
	pdescriptor.free = _eon_compound_free;
	pdescriptor.name = descriptor->name;
	pdescriptor.max_width_get = _eon_compound_max_width_get;
	pdescriptor.max_height_get = _eon_compound_max_height_get;
	pdescriptor.min_width_get = _eon_compound_min_width_get;
	pdescriptor.min_height_get = _eon_compound_min_height_get;
	pdescriptor.preferred_width_get = _eon_compound_preferred_width_get;
	pdescriptor.preferred_height_get = _eon_compound_preferred_height_get;
	pdescriptor.setup = _eon_compound_setup;
	//pdescriptor.needs_setup = _eon_compound_needs_setup;

	ee = eon_widget_new(&pdescriptor, thiz);
	if (!ee) goto renderer_err;

	eon_widget_theme_extend(ee, thiz->relative);
	/* set the theme renderer */
	r = eon_widget_theme_renderer_get(ee);
	relative_ee = ender_element_object_get(thiz->relative);
	relative_r = eon_widget_theme_renderer_get(relative_ee);

	eon_theme_compound_renderer_set(r, relative_r);

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_compound_data_get(Eon_Element *ee)
{
	Eon_Compound *thiz;

	thiz = _eon_compound_get(ee);
	return thiz->data;
}

#define _eon_compound_delete NULL
#include "eon_generated_compound.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
