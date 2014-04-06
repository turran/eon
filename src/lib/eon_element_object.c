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
#include "eon_element_object.h"
#include "eon_element_eon_private.h"
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_OBJECT_DESCRIPTOR eon_element_object_descriptor_get()
#define EON_ELEMENT_OBJECT(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Object, EON_ELEMENT_OBJECT_DESCRIPTOR)

typedef struct _Eon_Element_Object
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *xlink_href;
	/* private */
	Enesim_Renderer *proxy;
} Eon_Element_Object;

typedef struct _Eon_Element_Object_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Object_Class;

static void _eon_element_object_attr_modified_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Element_Object *thiz = EON_ELEMENT_OBJECT(data);
	Egueb_Dom_Event_Phase phase;
	Egueb_Dom_Node *attr;

	/* check if we are at the target */
	phase = egueb_dom_event_phase_get(ev);
	if (phase != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* check if the attribute is the width or the height */
	attr = egueb_dom_event_mutation_related_get(ev);
	if (thiz->xlink_href == attr)
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Object xlink href modified");
		eon_renderable_invalidate_geometry(EON_RENDERABLE(thiz));
	}
	egueb_dom_node_unref(attr);
}

static void _eon_element_object_data_cb(Egueb_Dom_Node *n,
		Enesim_Stream *data)
{
	Eon_Element_Object *thiz;
	const char *mime;

	thiz = EON_ELEMENT_OBJECT(egueb_dom_element_external_data_get(n));
	mime = enesim_image_mime_data_from(data);
	if (!mime)
	{
		ERR_ELEMENT(n, "Impossible to identify the image data");
		goto done;
	}

	INFO_ELEMENT(n, "Uri fetched with MIME '%s'", mime);
done:
	enesim_stream_unref(data);
}

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_object_init(Eon_Renderable *r)
{
	Eon_Element_Object *thiz;
	Eon_Element_Object_Class *klass;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_OBJECT(r);
	n = (EON_ELEMENT(r))->n;

	thiz->xlink_href = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EGUEB_DOM_XLINK_HREF), NULL,
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->xlink_href), NULL);

	/* in case the attribute xlink href has changed be sure to invalidate
	 * the geometry
	 */
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_object_attr_modified_cb, EINA_FALSE, thiz);

}

static Enesim_Renderer * _eon_element_object_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(r);
	return enesim_renderer_ref(thiz->proxy);
}

static void _eon_element_object_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	/* TODO in case the data is not loaded yet, do not set the geometry */
}

static int _eon_element_object_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	/* TODO in case the data is not loaded yet, do not return any hint */
	return 0;
}

static Eina_Bool _eon_element_object_pre_process(Eon_Renderable *r)
{
	Eon_Element_Object *thiz;
	Egueb_Dom_Event *e;
	Egueb_Dom_String *uri;
	Egueb_Dom_Uri u;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_OBJECT(r);
	n = (EON_ELEMENT(r))->n;

	egueb_dom_attr_final_get(thiz->xlink_href, &uri);
	if (!egueb_dom_string_is_valid(uri))
	{
		WARN_ELEMENT(n, "No uri set, nothing to do");
		return EINA_FALSE;
	}

	if (!egueb_dom_uri_string_from(&u, uri))
	{
		ERR_ELEMENT(n, "Wrong URI");
		return EINA_FALSE;
	}

	e = egueb_dom_event_io_data_new(&u, _eon_element_object_data_cb);
	egueb_dom_node_event_dispatch(n, e, NULL, NULL);
	egueb_dom_uri_cleanup(&u);

	return EINA_TRUE;
}

static Eina_Bool _eon_element_object_process(Eon_Renderable *r)
{
	/* FIXME nothing to process here? */
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_object_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_OBJECT);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Object,
		Eon_Element_Object_Class, eon_element_object);

static void _eon_element_object_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_object_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_object_init;
	r_klass->pre_process = _eon_element_object_pre_process;
}

static void _eon_element_object_instance_init(void *o)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(o);
	thiz->proxy = enesim_renderer_proxy_new();
}

static void _eon_element_object_instance_deinit(void *o)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(o);

	enesim_renderer_unref(thiz->proxy);
	egueb_dom_node_unref(thiz->xlink_href);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_object_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_object);
	return n;
}

EAPI void eon_element_object_xlink_href_set(Egueb_Dom_Node *n,
		Egueb_Dom_String * xlink_href)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->xlink_href, EGUEB_DOM_ATTR_TYPE_BASE,
			xlink_href);
}

EAPI Egueb_Dom_String * eon_element_object_xlink_href_get(Egueb_Dom_Node *n)
{
	Eon_Element_Object *thiz;
	Egueb_Dom_String *ret;

	thiz = EON_ELEMENT_OBJECT(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->xlink_href, &ret);
	return ret;
}

