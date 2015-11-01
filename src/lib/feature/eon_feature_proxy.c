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

#include "eon_element_private.h"
#include "eon_feature_proxy_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_FEATURE_PROXY_DESCRIPTOR eon_feature_proxy_descriptor_get()
#define EON_FEATURE_PROXY(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Feature_Proxy, EON_FEATURE_PROXY_DESCRIPTOR)

typedef struct _Eon_Feature_Proxy
{
	Enesim_Object_Instance base;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *proxy;
	Egueb_Dom_Feature *f;
} Eon_Feature_Proxy;

typedef struct _Eon_Feature_Proxy_Class
{
	Enesim_Object_Class base;
} Eon_Feature_Proxy_Class;

/* propagate the ui events too */
static void _eon_feature_proxy_event_propagate_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Feature_Proxy *thiz = data;
	egueb_dom_node_event_propagate(thiz->proxy, e);
}

static void _eon_feature_proxy_event_monitor_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Feature_Proxy *thiz = data;

	if (egueb_smil_event_is_timeline(e))
	{
		DBG("Proxied element requesting a timeline");
		egueb_dom_node_event_propagate(thiz->n, e);
		return;
	}
	else if (egueb_dom_event_is_mutation(e))
	{
		Egueb_Dom_String *type;
		type = egueb_dom_event_type_get(e);
		if (type == EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED ||
				type == EGUEB_DOM_EVENT_MUTATION_CHARACTER_DATA_MODIFIED)
		{
			DBG("Proxied element triggered a mutation event");
			egueb_dom_node_event_propagate(thiz->n, e);
		}
		egueb_dom_string_unref(type);
		return;
	}
	else if (egueb_dom_event_is_process(e))
	{
		DBG("Proxied element triggered a process event");
		egueb_dom_node_event_propagate(thiz->n, e);
		return;
	}
}

/* Whenever the element is removed/inserted from/to a document, make sure
 * to adopt our own proxied element to it too
 */
static void _eon_feature_proxy_inserted_into_doc_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Feature_Proxy *thiz = data;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *doc;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;

	n = EGUEB_DOM_NODE(egueb_dom_event_target_get(e));
	doc = egueb_dom_node_owner_document_get(n);
	egueb_dom_document_node_adopt(doc, thiz->proxy, NULL);
	egueb_dom_node_unref(n);
	egueb_dom_node_unref(doc);
}
/*----------------------------------------------------------------------------*
 *                              Feature interface                             *
 *----------------------------------------------------------------------------*/
static int _eon_feature_proxy_version_get(void *data)
{
	return EGUEB_DOM_FEATURE_EXTERNAL_DESCRIPTOR_VERSION;
}

static void _eon_feature_proxy_init(void *data)
{

}

static void _eon_feature_proxy_deinit(void *data)
{
	Eon_Feature_Proxy *thiz;

	thiz = EON_FEATURE_PROXY(data);
}

static Egueb_Dom_Feature_External_Descriptor _descriptor = {
	/* .version_get		= */ _eon_feature_proxy_version_get,
	/* .init 		= */ _eon_feature_proxy_init,
	/* .deinit 		= */ _eon_feature_proxy_deinit,
	/* .type_get		= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Feature_Proxy,
		Eon_Feature_Proxy_Class, eon_feature_proxy);

static void _eon_feature_proxy_class_init(void *k)
{
}

static void _eon_feature_proxy_instance_init(void *o)
{
	Eon_Feature_Proxy *thiz;

	thiz = EON_FEATURE_PROXY(o);
}

static void _eon_feature_proxy_instance_deinit(void *o)
{
	Eon_Feature_Proxy *thiz;

	thiz = EON_FEATURE_PROXY(o);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Feature * eon_feature_proxy_add(Egueb_Dom_Node *n, Egueb_Dom_Node *proxy)
{
	Eon_Feature_Proxy *thiz;
	Egueb_Dom_Feature *f;
	Egueb_Dom_Event_Target *et;

	/* create the feature */
	thiz = ENESIM_OBJECT_INSTANCE_NEW(eon_feature_proxy);
	thiz->n = n;
	thiz->proxy = proxy;
	thiz->f = egueb_dom_feature_external_new(&_descriptor, thiz);
	/* assign it */
	egueb_dom_node_feature_add(n, NULL, NULL, egueb_dom_feature_ref(thiz->f));
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	/* in case a node has been added/removed from a document make
	 * sure to set/unset the new/old document too
	 */
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED_INTO_DOCUMENT,
			_eon_feature_proxy_inserted_into_doc_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_DOCUMENT_SET,
			_eon_feature_proxy_inserted_into_doc_cb,
			EINA_FALSE, thiz);
	/* add events for every 'state' that needs to be propagated
	 * like mousein, mouseodown, click, focusin, etc
	 */
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_feature_proxy_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_DOWN,
			_eon_feature_proxy_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_UP,
			_eon_feature_proxy_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_OVER,
			_eon_feature_proxy_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_MOVE,
			_eon_feature_proxy_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_OUT,
			_eon_feature_proxy_event_propagate_cb,
			EINA_FALSE, thiz);
	/* add the monitor */
	egueb_dom_event_target_monitor_add(EGUEB_DOM_EVENT_TARGET(thiz->proxy),
			_eon_feature_proxy_event_monitor_cb, thiz);
	/* return it */
	return thiz->f;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

