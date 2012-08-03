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
#include "eon_input.h"
#include "eon_element.h"
#include "eon_bin.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"
#include "eon_private_bin.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Bin_Descriptor_Internal
{
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Free free;
	Eon_Widget_Hints_Get hints_get;
	Eon_Container_Child_At child_at;
} Eon_Bin_Descriptor_Internal;

typedef struct _Eon_Bin
{
	/* properties */
	Ender_Element *child;
	/* private */
	Eon_Bin_Descriptor_Internal descriptor;
	Eina_Bool changed : 1;
	Eon_Input_Proxy *proxy;
	Eina_Bool pass_events;
	void *data;
} Eon_Bin;

static inline Eon_Bin * _eon_bin_get(Eon_Element *ee)
{
	Eon_Bin *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       The Eon Input State interface                        *
 *----------------------------------------------------------------------------*/
static Ender_Element * _eon_bin_state_element_get(Ender_Element *e,
		double x, double y)
{
	Eon_Bin *thiz;
	Ender_Element *at = NULL;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_bin_get(ee);

	/* there's a default handler */
	if (thiz->descriptor.child_at)
		at = thiz->descriptor.child_at(e, x, y);
	if (!at)
	{
		Eon_Element *content_e;
		double ax, ay;
		Eon_Size size;

		content_e = ender_element_object_get(thiz->child);
		eon_element_actual_size_get(content_e, &size);
		eon_element_actual_position_get(content_e, &ax, &ay);

		if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
			at = thiz->child;
	}
	return at;
}

static Ender_Element * _eon_bin_state_element_next(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Ender_Element * _eon_bin_state_element_prev(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Eon_Input_State_Descriptor _bin_proxy_descriptor = {
	/* .element_get 	= */ _eon_bin_state_element_get,
	/* .element_next 	= */ _eon_bin_state_element_next,
	/* .element_prev 	= */ _eon_bin_state_element_prev,
};
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_bin_initialize(Ender_Element *e)
{
	Eon_Bin *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_bin_get(ee);
	/* we first register our own events */
	if (thiz->pass_events)
	{
		thiz->proxy = eon_input_proxy_new(e, &_bin_proxy_descriptor);
	}

	if (thiz->descriptor.initialize)
		thiz->descriptor.initialize(e);
}

static Eina_Bool _eon_bin_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Bin *thiz;
	Eon_Element *ee;
	Eina_Bool ret = EINA_TRUE;

	ee = ender_element_object_get(e);
	thiz = _eon_bin_get(ee);
	if (thiz->descriptor.setup)
		ret = thiz->descriptor.setup(e, state, s, err);
	thiz->changed = EINA_FALSE;
	return ret;
}

static Eina_Bool _eon_bin_needs_setup(Ender_Element *e)
{
	Eon_Bin *thiz;
	Eon_Element *ee;
	Eina_Bool ret;

	ee = ender_element_object_get(e);
	thiz = _eon_bin_get(ee);

	/* check if we have changed */
	ret = thiz->changed;
	if (ret) return ret;

	if (thiz->descriptor.needs_setup)
		ret = thiz->descriptor.needs_setup(e);
	if (ret) return ret;

	/* check if the content has changed */
	if (thiz->child)
		ret = eon_element_needs_setup(thiz->child);
	return ret;
}

static Eina_Bool _eon_bin_child_add(Eon_Element *e, Ender_Element *child)
{
	Eon_Bin *thiz;
	Eon_Theme_Instance *theme;
	Enesim_Renderer *child_r;

	thiz = _eon_bin_get(e);
	if (thiz->child)
	{
		printf("we already have a child\n");
		return EINA_FALSE;
	}

	/* FIXME this should go as part of the container theme */
	theme = eon_widget_theme_instance_get(e);
	child_r = eon_element_renderer_get(child);
	eon_theme_instance_property_set(theme, "content", child_r, NULL);
	
	/* FIXME the size of the elements of a stack for example is always
	 * calculated before calling the setup, so this first time
	 * an application is run it wont have any valid size
	 */
	thiz->child = child;
	thiz->changed = EINA_TRUE;
	return EINA_TRUE;
}

static Eina_Bool _eon_bin_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Bin *thiz;

	thiz = _eon_bin_get(ee);
	if (thiz->child != child)
		return EINA_FALSE;
	if (thiz->child)
	{
		ender_element_unref(thiz->child);
		thiz->child = NULL;
	}
	return EINA_TRUE;
}

static void _eon_bin_free(Eon_Element *ee)
{
	Eon_Bin *thiz;

	thiz = _eon_bin_get(ee);
	if (thiz->descriptor.free)
		thiz->descriptor.free(ee);
	free(thiz);
}

/* TODO */
/* register the initialize and free too
 * on initialize we should just check whenever the content has changed
 * (i.e mutation event) if so, inform that we have changed too
 */
#define _eon_bin_delete NULL
#include "eon_generated_bin.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_bin_data_get(Eon_Element *ee)
{
	Eon_Bin *thiz;

	thiz = _eon_bin_get(ee);
	if (!thiz) return NULL;
	return thiz->data;
}

Eon_Element * eon_bin_new(Eon_Theme_Instance *theme,
		Eon_Bin_Descriptor *descriptor, void *data)
{
	Eon_Bin *thiz;
	Eon_Container_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Bin));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->descriptor.initialize = descriptor->initialize;
	thiz->descriptor.setup = descriptor->setup;
	thiz->descriptor.needs_setup = descriptor->needs_setup;
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.child_at = descriptor->child_at;
	thiz->pass_events = descriptor->pass_events;

	pdescriptor.initialize = _eon_bin_initialize;
	pdescriptor.free = _eon_bin_free;
	pdescriptor.setup = _eon_bin_setup;
	pdescriptor.needs_setup = _eon_bin_needs_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.hints_get = descriptor->hints_get;
	pdescriptor.child_add = _eon_bin_child_add;
	pdescriptor.child_remove = _eon_bin_child_remove;

	ee = eon_container_new(theme, &pdescriptor, thiz);
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
EAPI void eon_bin_child_set(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_clear(e, "child");
	ender_element_value_add(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_bin_child_get(Ender_Element *e, Ender_Element **child)
{
	Eon_Bin *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_bin_get(ee);
	*child = thiz->child;
}
