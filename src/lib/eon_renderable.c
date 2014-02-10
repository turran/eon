/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2014 Jorge Luis Zapata
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
#include "eon_private.h"
#include "eon_main.h"
#include "eon_event_geometry_private.h"
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Bool _eon_renderable_geometry_request(Eon_Renderable *thiz,
		Eina_Rectangle *geometry)
{
	Eon_Element *e;
	Egueb_Dom_Event *ev;
	Eina_Bool ret = EINA_TRUE;

	e = EON_ELEMENT(thiz);
	ev = eon_event_geometry_new();
	egueb_dom_node_event_dispatch(e->n, ev, NULL, NULL);

	ret = eon_event_geometry_get(ev, geometry);

	return ret;
}

static void _eon_renderable_geometry_cb(Egueb_Dom_Event *ev, void *data)
{
	/* check that we are in the capture phase */
} 
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_renderable_init(Eon_Element *e)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;
	Egueb_Dom_Node *n;

 	thiz = EON_RENDERABLE(e);
	/* add the attributes */
	thiz->min_width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MIN_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->min_width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->min_height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MIN_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->min_height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->max_width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MAX_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->max_width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->max_height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MAX_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->max_height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);

	n = e->n;
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->min_width), NULL);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->min_height), NULL);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->max_width), NULL);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->max_height), NULL);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->width), NULL);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->height), NULL);

	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_renderable_process(Eon_Element *e)
{
	Eon_Renderable *thiz;
	Egueb_Dom_Node *n = e->n;

	thiz = EON_RENDERABLE(e);

	/* process whatever needs to be process before setting the geometry */
	/* pre_setup() */
	/* in case the element is enqueued, that is, the element has changed, request
	 * the geometry upstream
	 */
	if (egueb_dom_element_is_enqueued(n))
	{
		/* request the geometry */
		if (!_eon_renderable_geometry_request(thiz, &thiz->geometry))
			return EINA_FALSE;
	}
	/* set the geometry */
	/* setup() */

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Renderable,
		Eon_Renderable_Class, eon_renderable);

static void _eon_renderable_class_init(void *k)
{
	Eon_Element_Class *klass = k;
	klass->init = _eon_renderable_init;
	klass->process = _eon_renderable_process;
}

static void _eon_renderable_instance_init(void *o)
{
}

static void _eon_renderable_instance_deinit(void *o)
{
	Eon_Renderable *thiz = o;

	egueb_dom_node_unref(thiz->min_width);
	egueb_dom_node_unref(thiz->min_height);
	egueb_dom_node_unref(thiz->max_width);
	egueb_dom_node_unref(thiz->max_height);
	egueb_dom_node_unref(thiz->width);
	egueb_dom_node_unref(thiz->height);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool eon_is_renderable(Egueb_Dom_Node *n)
{
	void *data;

	if (!n) return EINA_FALSE;
	if (!egueb_dom_element_is_external(n))
		return EINA_FALSE;
	data = egueb_dom_element_external_data_get(n);
	if (!enesim_object_instance_inherits(ENESIM_OBJECT_INSTANCE(data),
			EON_RENDERABLE_DESCRIPTOR))
		return EINA_FALSE;
	return EINA_TRUE;
}

int eon_renderable_size_hints_get(Egueb_Dom_Node *n, Eon_Renderable_Size *size)
{
	Eon_Renderable *thiz;
	int ret = 0;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	/* in case the element is not enqueued just return the chached hints */
	if (egueb_dom_element_is_enqueued(n))
	{
		Eon_Renderable_Class *klass;

		klass = EON_RENDERABLE_CLASS_GET(thiz);
		if (klass->size_hints_get)
		{
	 		ret = klass->size_hints_get(thiz, size);
		}
	}
	else
	{
		ret = thiz->size_hints;
		*size = thiz->size;
	}
	return ret;
}

void eon_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->geometry_set)
		klass->geometry_set(thiz, geometry);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
