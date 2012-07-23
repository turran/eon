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
static Ender_Property *EON_CONTAINER_CONTENT;

typedef struct _Eon_Container
{
	/* properties */
	Eon_Container_State current;
	Eon_Container_State past;
	/* private */
	Eina_Bool changed : 1;
	Eon_Element_Initialize initialize;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Free free;
	Eon_Container_Setup setup;
	Eon_Container_Min_Width_Get min_width_get;
	Eon_Container_Max_Width_Get max_width_get;
	Eon_Container_Min_Height_Get min_height_get;
	Eon_Container_Max_Height_Get max_height_get;
	Eon_Container_Preferred_Width_Get preferred_width_get;
	Eon_Container_Preferred_Height_Get preferred_height_get;
	Eon_Container_Element_At element_at;
	Eon_Input_Proxy *proxy;
	Eina_Bool pass_events;
	void *data;
} Eon_Container;

static inline Eon_Container * _eon_container_get(Eon_Element *ee)
{
	Eon_Container *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       The Eon Input State interface                        *
 *----------------------------------------------------------------------------*/
static Ender_Element * _eon_container_state_element_get(Ender_Element *e,
		double x, double y)
{
	Eon_Container *thiz;
	Ender_Element *at = NULL;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);

	/* there's a default handler */
	if (thiz->element_at)
		at = thiz->element_at(e, x, y);
	if (!at)
	{
		Eon_Element *content_e;
		double ax, ay;
		Eon_Size size;

		content_e = ender_element_object_get(thiz->current.content);
		eon_element_actual_size_get(content_e, &size);
		eon_element_actual_position_get(content_e, &ax, &ay);

		if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
			at = thiz->current.content;
	}
	return at;
}

static Ender_Element * _eon_container_state_element_next(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Ender_Element * _eon_container_state_element_prev(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Eon_Input_State_Descriptor _container_proxy_descriptor = {
	/* .element_get 	= */ _eon_container_state_element_get,
	/* .element_next 	= */ _eon_container_state_element_next,
	/* .element_prev 	= */ _eon_container_state_element_prev,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_container_content_set(Eon_Element *ee, Ender_Element *content)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	if (!thiz) return;

	if (thiz->current.content == content)
		return;

	if (!eon_is_element(content))
		return;

	/* FIXME the size of the elements of a stack for example is always
	 * calculated before calling the setup, so this first time
	 * an application is run it wont have any valid size
	 */
	{
		Enesim_Renderer *content_t;

		content_t = eon_element_renderer_get(content);
		eon_widget_property_set(ee, "content", content_t, NULL);
	}
	thiz->current.content = content;
	thiz->changed = EINA_TRUE;
}

static void _eon_container_content_get(Eon_Element *ee, const Ender_Element **content)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	if (!thiz) return;

	*content = thiz->current.content;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_container_initialize(Ender_Element *e)
{
	Eon_Container *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	/* we first register our own events */
	if (thiz->pass_events)
	{
		thiz->proxy = eon_input_proxy_new(e, &_container_proxy_descriptor);
	}

	if (thiz->initialize)
		thiz->initialize(e);
}

static Eina_Bool _eon_container_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	Enesim_Renderer *content_t;
	Eina_Bool ret = EINA_TRUE;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	content_t = eon_element_renderer_get(thiz->current.content);
	/* TODO only set whenever the content is different */
	eon_widget_property_set(ee, "content", content_t, NULL);
	if (thiz->setup)
		ret = thiz->setup(e, state, &thiz->current, s, err);
	/* TODO handle the refcounting */
	thiz->changed = EINA_FALSE;
	thiz->past = thiz->current;
	return ret;
}

static double _eon_container_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->current.content)
		eon_element_min_width_get(thiz->current.content, &v);
	if (thiz->max_width_get)
		v = thiz->min_width_get(e, v);

	return v;
}

static double _eon_container_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->current.content)
		eon_element_max_width_get(thiz->current.content, &v);
	else
		return DBL_MAX;
	if (thiz->max_width_get)
		v = thiz->max_width_get(e, v);

	return v;
}

static double _eon_container_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->current.content)
		eon_element_min_height_get(thiz->current.content, &v);
	if (thiz->max_height_get)
		v = thiz->min_height_get(e, v);

	return v;
}

static double _eon_container_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->current.content)
		eon_element_max_height_get(thiz->current.content, &v);
	else
		return DBL_MAX;
	if (thiz->max_height_get)
		v = thiz->max_height_get(e, v);

	return v;
}

static double _eon_container_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->current.content)
		eon_element_preferred_width_get(thiz->current.content, &v);
	if (thiz->preferred_width_get)
		v = thiz->preferred_width_get(e, v);

	return v;
}

static double _eon_container_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	double v = 0;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->current.content)
		eon_element_preferred_height_get(thiz->current.content, &v);
	if (thiz->preferred_height_get)
		v = thiz->preferred_height_get(e, v);

	return v;
}

static Eina_Bool _eon_container_needs_setup(Ender_Element *e)
{
	Eon_Container *thiz;
	Eon_Element *ee;
	Eina_Bool ret;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);

	/* check if we have changed */
	ret = thiz->changed;
	if (ret) return ret;

	if (thiz->needs_setup)
		ret = thiz->needs_setup(e);
	if (ret) return ret;

	/* check if the content has changed */
	if (thiz->current.content)
		ret = eon_element_needs_setup(thiz->current.content);
	return ret;
}

static void _eon_container_free(Eon_Element *ee)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	if (thiz->free)
		thiz->free(ee);
	free(thiz);
}

/* TODO */
/* register the initialize and free too
 * on initialize we should just check whenever the content has changed
 * (i.e mutation event) if so, inform that we have changed too
 */
#define _eon_container_delete NULL
#include "eon_generated_container.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_container_data_get(Eon_Element *ee)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	if (!thiz) return NULL;
	return thiz->data;
}

Eon_Element * eon_container_new(Eon_Container_Descriptor *descriptor, void *data)
{
	Eon_Container *thiz;
	Eon_Widget_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Container));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->initialize = descriptor->initialize;
	thiz->setup = descriptor->setup;
	thiz->needs_setup = descriptor->needs_setup;
	thiz->free = descriptor->free;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->max_width_get = descriptor->max_width_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->max_height_get = descriptor->max_height_get;
	thiz->pass_events = descriptor->pass_events;
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;
	thiz->element_at = descriptor->element_at;

	pdescriptor.initialize = _eon_container_initialize;
	pdescriptor.free = _eon_container_free;
	pdescriptor.setup = _eon_container_setup;
	pdescriptor.needs_setup = _eon_container_needs_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.min_width_get = _eon_container_min_width_get;
	pdescriptor.max_width_get = _eon_container_max_width_get;
	pdescriptor.min_height_get = _eon_container_min_height_get;
	pdescriptor.max_height_get = _eon_container_max_height_get;
	pdescriptor.preferred_width_get = _eon_container_preferred_width_get;
	pdescriptor.preferred_height_get = _eon_container_preferred_height_get;

	ee = eon_widget_new(&pdescriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_set(Ender_Element *e, Ender_Element *content)
{
	ender_element_value_set(e, "content", content, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_get(Ender_Element *e, Ender_Element **content)
{
	ender_element_value_get(e, "content", content, NULL);
}

