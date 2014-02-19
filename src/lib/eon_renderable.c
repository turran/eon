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
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _eon_renderable_attr_modified_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Renderable *thiz = EON_RENDERABLE(data);
	Egueb_Dom_Event_Phase phase;
	Egueb_Dom_Node *attr;

	/* check if we are at the target */
	phase = egueb_dom_event_phase_get(ev);
	if (phase != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* check if the attribute is the width or the height */
	attr = egueb_dom_event_mutation_related_get(ev);
	if ((thiz->width == attr) || (thiz->height == attr))
	{
		ERR_ELEMENT((EON_ELEMENT(thiz))->n, "Renderable attr modified");
		eon_renderable_invalidate_geometry(thiz);
	}
}

static void _eon_renderable_mutation_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Renderable *thiz = EON_RENDERABLE(data);
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;

	if (egueb_dom_event_phase_get(ev) != EGUEB_DOM_EVENT_PHASE_BUBBLING)
		return;
	child = egueb_dom_event_relative_get(ev);
	if (egueb_dom_element_is_enqueued(child))
		egueb_dom_element_dequeue(egueb_dom_node_ref(child));

	egueb_dom_event_mutation_process_prevent(ev);
	n = egueb_dom_event_target_current_get(ev);
	egueb_dom_element_enqueue(n);
	egueb_dom_node_unref(child);
}

/* The idea is that if a direct child is being enqueued we should be
 * enqueued first and not our child.
 */
static void _eon_renderable_process_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Renderable *thiz = EON_RENDERABLE(data);
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *n;

	if (egueb_dom_event_phase_get(ev) != EGUEB_DOM_EVENT_PHASE_BUBBLING)
		return;
	child = egueb_dom_event_relative_get(ev);
	if (egueb_dom_element_is_enqueued(child))
		egueb_dom_element_dequeue(egueb_dom_node_ref(child));

	n = egueb_dom_event_target_current_get(ev);
	egueb_dom_element_enqueue(n);
	egueb_dom_node_unref(n);
	egueb_dom_node_unref(child);
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
	thiz->width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);

	n = e->n;
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->width), NULL);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->height), NULL);

	/* in case the attribute width or height has changed be sure to invalidate
	 * the geometry
	 */
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_renderable_attr_modified_cb, EINA_FALSE, thiz);

	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->child_size_dependant)
	{
		/* in case this is dependent on the child size, be sure to
		 * register some events in order to process this node
		 * whenever our child requires a process
		 */
		egueb_dom_node_event_listener_add(n,
				EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
				_eon_renderable_mutation_cb, EINA_FALSE, thiz);
		egueb_dom_node_event_listener_add(n,
				EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
				_eon_renderable_mutation_cb, EINA_FALSE, thiz);
		egueb_dom_node_event_listener_add(n,
				EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
				_eon_renderable_mutation_cb, EINA_FALSE, thiz);
		egueb_dom_node_event_listener_add(n,
				EGUEB_DOM_EVENT_PROCESS,
				_eon_renderable_process_cb, EINA_FALSE, thiz);
	}

	if (klass->init)
		klass->init(thiz);
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_renderable_process(Eon_Element *e)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;
	Egueb_Dom_Node *n = e->n;

	thiz = EON_RENDERABLE(e);
	klass = EON_RENDERABLE_CLASS_GET(e);

	DBG_ELEMENT(e->n, "Processing renderable (needs geometry: %d size hints cached: %d)",
			thiz->needs_geometry, thiz->size_hints_cached);
	if (klass->pre_process)
	{
		if (!klass->pre_process(thiz))
		{
			ERR_ELEMENT(e->n, "The pre process failed");
			return EINA_FALSE;
		}
	}

	if (thiz->needs_geometry)
	{
		ERR_ELEMENT(e->n, "Parent does not set a geometry");
		return EINA_FALSE;
	}
	/* process ourselves */
	if (klass->process)
		return klass->process(thiz);

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_ELEMENT_DESCRIPTOR, Eon_Renderable,
		Eon_Renderable_Class, eon_renderable);

static void _eon_renderable_class_init(void *k)
{
	Eon_Element_Class *klass = k;

	klass->init = _eon_renderable_init;
	klass->process = _eon_renderable_process;
}

static void _eon_renderable_instance_init(void *o)
{
	Eon_Renderable *thiz = o;

	thiz->needs_geometry = EINA_TRUE;
	thiz->size_hints_cached = EINA_FALSE;
}

static void _eon_renderable_instance_deinit(void *o)
{
	Eon_Renderable *thiz = o;

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

void eon_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->geometry_set)
	{
		DBG_ELEMENT(n, "Setting geometry %" EINA_RECTANGLE_FORMAT,
				EINA_RECTANGLE_ARGS(geometry));
		klass->geometry_set(thiz, geometry);
		thiz->geometry = *geometry;
		thiz->needs_geometry = EINA_FALSE;
	}
}

Enesim_Renderer * eon_renderable_renderer_get(Eon_Renderable *thiz)
{
	Eon_Renderable_Class *klass;

	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->renderer_get)
		return klass->renderer_get(thiz);
	return NULL;
}

void eon_renderable_invalidate_geometry(Eon_Renderable *thiz)
{
	DBG_ELEMENT(((EON_ELEMENT(thiz))->n), "Invalidating geometry");
	thiz->size_hints_cached = EINA_FALSE;
	thiz->needs_geometry = EINA_TRUE;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Sets the width of a renderable
 * @param[in] n The renderable node to set the width
 * @param[in] w The width to set
 */
EAPI void eon_renderable_width_set(Egueb_Dom_Node *n, int w)
{
	Eon_Renderable *thiz;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->width, EGUEB_DOM_ATTR_TYPE_BASE, w);
}

EAPI int eon_renderable_width_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	int ret;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->width, &ret);
	return ret;
}

/**
 * Sets the height of a renderable
 * @param[in] n The renderable node to set the height
 * @param[in] h The height to set
 */
EAPI void eon_renderable_height_set(Egueb_Dom_Node *n, int h)
{
	Eon_Renderable *thiz;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->height, EGUEB_DOM_ATTR_TYPE_BASE, h);
}

EAPI int eon_renderable_height_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	int ret;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->height, &ret);
	return ret;
}


/**
 * @brief Get the size hints of a renderable
 * @param[in] n The renderable node to get the size hints from
 * @param[out] size The pointer to write the size hints 
 * @return The size hints mask
 */
int eon_renderable_size_hints_get(Egueb_Dom_Node *n, Eon_Renderable_Size *size)
{
	Eon_Renderable *thiz;
	int ret = 0;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	/* Check if the size hints are already cached */
	if (!thiz->size_hints_cached)
	{
		Eon_Renderable_Class *klass;

		klass = EON_RENDERABLE_CLASS_GET(thiz);
		if (klass->size_hints_get)
		{
	 		ret = klass->size_hints_get(thiz, size);
			/* cache it */
			thiz->size = *size;
			thiz->size_hints = ret;
			thiz->size_hints_cached = EINA_TRUE;
		}
	}
	else
	{
		ret = thiz->size_hints;
		*size = thiz->size;
	}
	return ret;
}