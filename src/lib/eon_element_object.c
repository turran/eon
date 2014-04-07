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

/* TODO
 * - forward input events
 * - handle the animation feature
 * - add a way to set/get in code a document
 */
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
	Egueb_Dom_Node *doc;
	Eina_Bool xlink_href_changed;
	Egueb_Dom_String *xlink_href_last;
	Enesim_Renderer *image;
	Enesim_Surface *s;
	Eina_List *damages;
} Eon_Element_Object;

typedef struct _Eon_Element_Object_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Object_Class;

static Eina_Bool _eon_element_object_render_damages(Egueb_Dom_Feature *e EINA_UNUSED,
		Eina_Rectangle *area, void *data)
{
	Eon_Element_Object *thiz = data;
	Eina_Rectangle *r;

	/* FIXME instead of allocating, just get the rectangle from a pool of rectangles */
	r = malloc(sizeof(Eina_Rectangle));
	*r = *area;

	/* append a new damage on the image renderer too */
	enesim_renderer_image_damage_add(thiz->image, r); 
	thiz->damages = eina_list_append(thiz->damages, r);
	return EINA_TRUE;
}

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
		thiz->xlink_href_changed = EINA_TRUE;
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

	egueb_dom_parser_parse(data, &thiz->doc);
	/* The data has been fetched, invalidete the geometry so we can inform
	 * about the new hints, size, etc
	 */
	eon_renderable_invalidate_geometry(EON_RENDERABLE(thiz));
	egueb_dom_element_request_process(n);
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
	return enesim_renderer_ref(thiz->image);
}

static void _eon_element_object_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(r);
	if (thiz->doc)
	{
		Egueb_Dom_Feature *window;
		window = egueb_dom_node_feature_get(thiz->doc,
				EGUEB_DOM_FEATURE_WINDOW_NAME, NULL);
		if (window)
		{
			enesim_renderer_image_position_set(thiz->image, geometry->x, geometry->y);
			enesim_renderer_image_size_set(thiz->image, geometry->w, geometry->h);
			egueb_dom_feature_window_content_size_set(window,
					geometry->w, geometry->h);
			egueb_dom_feature_unref(window);
		}
	}
}

static int _eon_element_object_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Element_Object *thiz;
	int ret = 0;

	thiz = EON_ELEMENT_OBJECT(r);
	/* TODO in case the data is not loaded yet, do not return any hint */
	if (thiz->doc)
	{
		Egueb_Dom_Feature *window;
		window = egueb_dom_node_feature_get(thiz->doc, EGUEB_DOM_FEATURE_WINDOW_NAME, NULL);
		if (window)
		{
			Egueb_Dom_Feature_Window_Type type;
			int cw, ch;

			egueb_dom_feature_window_type_get(window, &type);
			if (type == EGUEB_DOM_FEATURE_WINDOW_TYPE_MASTER)
			{
				egueb_dom_feature_window_content_size_get(window, &cw, &ch);
				if (cw >= 0 && ch >= 0)
				{
					ret |= EON_RENDERABLE_HINT_PREFERRED;
					size->pref_width = cw;
					size->pref_height = ch;
				}
				else
				{
					ret |= EON_RENDERABLE_HINT_MIN_MAX;
					size->min_width = 1;
					size->min_height = 1;
					size->max_width = -1;
					size->max_height = -1;
				}
			}
			egueb_dom_feature_unref(window);
		}
	}
	return ret;
}

static Eina_Bool _eon_element_object_pre_process(Eon_Renderable *r)
{
	Eon_Element_Object *thiz;
	Egueb_Dom_Event *e;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_OBJECT(r);
	n = (EON_ELEMENT(r))->n;

	/* in case the xlink href has actually changed, request the application
	 * to load the content
	 */
	if (thiz->xlink_href_changed)
	{
		Egueb_Dom_String *uri;
		Egueb_Dom_Uri u;

		egueb_dom_attr_final_get(thiz->xlink_href, &uri);
		if (!egueb_dom_string_is_equal(uri, thiz->xlink_href_last))
		{
			if (thiz->doc)
			{
				egueb_dom_node_unref(thiz->doc);
				thiz->doc = NULL;
			}

			if (thiz->xlink_href_last)
			{
				egueb_dom_string_unref(thiz->xlink_href_last);
				thiz->xlink_href_last = NULL;
			}
			thiz->xlink_href_last = egueb_dom_string_ref(uri);
			if (!egueb_dom_string_is_valid(uri))
			{
				WARN_ELEMENT(n, "No uri set, nothing to do");
				egueb_dom_string_unref(uri);
				return EINA_FALSE;
			}

			if (!egueb_dom_uri_string_from(&u, uri))
			{
				ERR_ELEMENT(n, "Wrong URI");
				egueb_dom_string_unref(uri);
				return EINA_FALSE;
			}

			DBG_ELEMENT(n, "Requesting data load on '%s'", egueb_dom_string_string_get(uri));
			e = egueb_dom_event_io_data_new(&u, _eon_element_object_data_cb);
			egueb_dom_node_event_dispatch(n, e, NULL, NULL);
			egueb_dom_uri_cleanup(&u);
		}
		egueb_dom_string_unref(uri);
	}
	return EINA_TRUE;
}

static Eina_Bool _eon_element_object_process(Eon_Renderable *r)
{
	Eon_Element_Object *thiz;
	Egueb_Dom_Feature *window, *render;
	Egueb_Dom_Node *n;
	Eina_Rectangle *rect;
	Eina_Bool new_surface = EINA_FALSE;
	int cw, ch;

	thiz = EON_ELEMENT_OBJECT(r);
	n = (EON_ELEMENT(r))->n;

	/* Process the external document */
	if (!thiz->doc)
		goto done;
	egueb_dom_document_process(thiz->doc);

	/* create a surface that fits on the geometry of the object */
	window = egueb_dom_node_feature_get(thiz->doc,
			EGUEB_DOM_FEATURE_WINDOW_NAME, NULL);
	if (!window)
		goto done;

	egueb_dom_feature_window_content_size_get(window, &cw, &ch);
	egueb_dom_feature_unref(window);

	if (thiz->s)
	{
		int sw, sh;

		enesim_surface_size_get(thiz->s, &sw, &sh);
		if (cw != sw || ch != sh)
		{
			enesim_surface_unref(thiz->s);
			enesim_renderer_image_source_surface_set(thiz->image, NULL);
			thiz->s = NULL;
		}
	}

	if (!thiz->s)
	{
		thiz->s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, cw, ch);
		enesim_renderer_image_source_surface_set(thiz->image,
				enesim_surface_ref(thiz->s));
		new_surface = EINA_TRUE;
	}

	/* get the damages of the external document and update the image area */
	render = egueb_dom_node_feature_get(thiz->doc,
			EGUEB_DOM_FEATURE_RENDER_NAME, NULL);
	if (!render)
		goto done;

	egueb_dom_feature_render_damages_get(render, thiz->s,
			_eon_element_object_render_damages, thiz);
	if (!thiz->damages && !new_surface)
		goto no_damages;

	if (thiz->damages)
		egueb_dom_feature_render_draw_list(render, thiz->s,
				ENESIM_ROP_FILL, thiz->damages, 0, 0, NULL);
	else
		egueb_dom_feature_render_draw(render, thiz->s,
				ENESIM_ROP_FILL, NULL, 0, 0, NULL);

	EINA_LIST_FREE(thiz->damages, rect)
	{
		free(rect);
	}
no_damages:
	egueb_dom_feature_unref(render);

	/* Make sure to process again this node, this will make the document
	 * to always process ourselves.
	 */
done:
	egueb_dom_element_enqueue(egueb_dom_node_ref(n));

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
	r_klass->process = _eon_element_object_process;
	r_klass->renderer_get = _eon_element_object_renderer_get;
	r_klass->geometry_set = _eon_element_object_geometry_set;
	r_klass->size_hints_get = _eon_element_object_size_hints_get;
}

static void _eon_element_object_instance_init(void *o)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(o);
	thiz->image = enesim_renderer_image_new();
}

static void _eon_element_object_instance_deinit(void *o)
{
	Eon_Element_Object *thiz;

	thiz = EON_ELEMENT_OBJECT(o);

	if (thiz->doc)
	{
		egueb_dom_node_unref(thiz->doc);
		thiz->doc = NULL;
	}

	if (thiz->s)
	{
		enesim_surface_unref(thiz->s);
		thiz->s = NULL;
	}
	enesim_renderer_unref(thiz->image);
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

