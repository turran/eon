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
#include "eon_document.h"
#include "eon_element_eon.h"
#include "eon_element_button.h"
#include "eon_element_button_stock.h"
#include "eon_element_label.h"
#include "eon_element_stack.h"

#include "eon_renderable_private.h"
#include "eon_element_eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Document
{
	/* input */
	Egueb_Dom_Input *input;
	/* window */
	int width;
	int height;
} Eon_Document;

static void _eon_document_request_geometry_cb(Egueb_Dom_Event *ev, void *data)
{
	Eon_Document *thiz;
	Eon_Size size;
	Eon_Renderable_Size renderable_size;
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *eon;
	Eina_Rectangle geometry;
	int size_hints;

	DBG("Rquesting main geometry");
	eon = egueb_dom_document_element_get(n);
	if (!eon)
	{
		ERR("No topmost element found");
		return;
	}

	size_hints = eon_renderable_size_hints_get(eon, &renderable_size);
	DBG("Main hints are %08x", size_hints);
	if (size_hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		/* TODO set the min size for now */
		size.width = renderable_size.min_width;
		size.height = renderable_size.min_height;
	}
	else
	{
		size.width = 0;
		size.height = 0;
	}
	eina_rectangle_coords_from(&geometry, 0, 0, size.width, size.height);
	eon_event_geometry_set(ev, &geometry);
	egueb_dom_node_unref(eon);

	/* Se the width so the user when requesting the geometry will get this value */
	thiz = egueb_dom_document_external_data_get(n);
	thiz->width = size.width;
	thiz->height = size.height;
}

static void _eon_document_node_inserted_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *target;

	parent = egueb_dom_event_mutation_related_get(ev);

	if (parent != n)
	{
		egueb_dom_node_unref(parent);
		return;
	}

	egueb_dom_node_unref(parent);
	target = egueb_dom_event_target_get(ev);
	egueb_dom_node_event_listener_add(target, EON_EVENT_GEOMETRY,
			_eon_document_request_geometry_cb, EINA_FALSE, n);
	egueb_dom_node_unref(target);
}

static void _eon_document_node_removed_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *target;

	parent = egueb_dom_event_mutation_related_get(ev);
	if (parent != n)
	{
		egueb_dom_node_unref(parent);
		return;
	}

	egueb_dom_node_unref(parent);
	target = egueb_dom_event_target_get(ev);
	egueb_dom_node_event_listener_add(target, EON_EVENT_GEOMETRY,
			_eon_document_request_geometry_cb, EINA_FALSE, n);
	egueb_dom_node_unref(target);
}
/*----------------------------------------------------------------------------*
 *                              Input interface                               *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_document_input_element_at(void *data,
		int x, int y)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *topmost;
	Egueb_Dom_Node *ret;
	Eina_Rectangle ptr;

	/* iterate over the whole tree */
	eina_rectangle_coords_from(&ptr, x, y, 1, 1);
	topmost = egueb_dom_document_element_get(n);
	ret = eon_renderable_element_at(topmost, &ptr);
	egueb_dom_node_unref(topmost);

	return ret;
}

static Egueb_Dom_Input_Descriptor _eon_document_input_descriptor = {
	/* .element_at 		= */ _eon_document_input_element_at,
};

/*----------------------------------------------------------------------------*
 *                          UI feature interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Input * _eon_document_ui_input_get(Egueb_Dom_Node *n)
{
	Eon_Document *thiz;

	thiz = egueb_dom_document_external_data_get(n);
	return thiz->input;
}

static Egueb_Dom_Feature_UI_Descriptor 
_eon_document_ui_descriptor = {
	/* .input_get 	= */ _eon_document_ui_input_get,
};
/*----------------------------------------------------------------------------*
 *                      Animation feature interface                           *
 *----------------------------------------------------------------------------*/
static Etch * _eon_document_animation_etch_get(Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *topmost;
	Etch *ret;

	topmost = egueb_dom_document_element_get(n);
	if (!topmost) return NULL;

	ret = eon_element_eon_etch_get(topmost);
	egueb_dom_node_unref(topmost);
	return ret;
}

static Egueb_Dom_Feature_Animation_Descriptor 
_eon_document_animation_descriptor = {
	/* .etch_get 	= */ _eon_document_animation_etch_get,
};
/*----------------------------------------------------------------------------*
 *                        Window feature interface                            *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_document_window_type_get(
		Egueb_Dom_Node *n, Egueb_Dom_Feature_Window_Type *type)
{
	*type = EGUEB_DOM_FEATURE_WINDOW_TYPE_SLAVE;
	return EINA_TRUE;
}

static Eina_Bool _eon_document_window_content_size_set(
		Egueb_Dom_Node *n, int w, int h)
{
	Eon_Document *thiz;
	Egueb_Dom_Node *eon;
	Eina_Rectangle geom;
	Eina_Rectangle eon_geom;

	if (w <= 0 || h <= 0)
		return EINA_FALSE;

	eon = egueb_dom_document_element_get(n);
	if (!eon)
	{
		return EINA_FALSE;
	}
	eina_rectangle_coords_from(&geom, 0, 0, w, h);
	eon_renderable_geometry_solve(eon, &geom, &eon_geom);
	eon_renderable_geometry_set(eon, &eon_geom);
	egueb_dom_element_enqueue(eon);

	thiz = egueb_dom_document_external_data_get(n);
	thiz->width = w;
	thiz->height = h;

	return EINA_TRUE;
}

static Eina_Bool _eon_document_window_content_size_get(
		Egueb_Dom_Node *n, int *w, int *h)
{
	Eon_Document *thiz;
	Egueb_Dom_Node *topmost = NULL;

	thiz = egueb_dom_document_external_data_get(n);
	*h = thiz->height;
	*w = thiz->width;

	return EINA_TRUE;
}

static Egueb_Dom_Feature_Window_Descriptor 
_eon_document_window_descriptor = {
	/* .type_get 		= */ _eon_document_window_type_get,
	/* .content_size_set 	= */ _eon_document_window_content_size_set,
	/* .content_size_get 	= */ _eon_document_window_content_size_get,
};
/*----------------------------------------------------------------------------*
 *                        Render feature interface                            *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_document_render_renderer_get(
		Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *topmost;
	Enesim_Renderer *r;

	/* get the topmost element, get the renderer and return it */
	topmost = egueb_dom_document_element_get(n);
	if (!topmost) return NULL;

	r = eon_renderable_renderer_get(topmost);
	egueb_dom_node_unref(topmost);
	return r;
}

static Egueb_Dom_Feature_Render_Descriptor
_eon_document_render_descriptor = {
	/* .renderer_get 	= */ _eon_document_render_renderer_get,
};

/*----------------------------------------------------------------------------*
 *                     The exernal document interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_document_element_create(Egueb_Dom_Node *n,
		void *data, const char *name)
{
	Egueb_Dom_Node *ret = NULL;

	DBG("Creating element '%s'", name);
	if (!strcmp(name, "eon"))
		ret = eon_element_eon_new();
	else if (!strcmp(name, "button"))
		ret = eon_element_button_new();
	else if (!strcmp(name, "buttonStock"))
		ret = eon_element_button_stock_new();
	else if (!strcmp(name, "label"))
		ret = eon_element_label_new();
	else if (!strcmp(name, "stack"))
		ret = eon_element_stack_new();
	return ret;
}

static Eina_Bool _eon_document_child_appendable(Egueb_Dom_Node *n,
		void *data, Egueb_Dom_Node *child)
{
	Egueb_Dom_String *name;
	Eina_Bool ret;

	name = egueb_dom_node_name_get(child);
	ret = egueb_dom_string_is_equal(name, EON_ELEMENT_EON);
	egueb_dom_string_unref(name);
	return ret;
}

static void _eon_document_init(Egueb_Dom_Node *n, void *data)
{
	Eon_Document *thiz = data;

	thiz->input = egueb_dom_input_new(&_eon_document_input_descriptor, n);
	/* register the features */
	egueb_dom_feature_window_add(n,
			&_eon_document_window_descriptor);
	egueb_dom_feature_render_add(n,
			&_eon_document_render_descriptor);
	egueb_dom_feature_animation_add(n,
			&_eon_document_animation_descriptor);
	egueb_dom_feature_ui_add(n,
			&_eon_document_ui_descriptor);
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_document_node_inserted_cb,
			EINA_FALSE, n);
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_document_node_removed_cb,
			EINA_FALSE, n);
}

static void _eon_document_deinit(Egueb_Dom_Node *n, void *data)
{
	Eon_Document *thiz = data;
	egueb_dom_input_free(thiz->input);
	free(thiz);
}

static Egueb_Dom_Document_External_Descriptor _descriptor = {
	/* init 		= */ _eon_document_init,
	/* deinit 		= */ _eon_document_deinit,
	/* element_create	= */ _eon_document_element_create,
	/* child_appendable	= */ _eon_document_child_appendable,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_document_new(void)
{
	Eon_Document *thiz;

	thiz = calloc(1, sizeof(Eon_Document));
	return egueb_dom_document_external_new(&_descriptor, thiz);
}
