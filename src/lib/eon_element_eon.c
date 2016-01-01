/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
#include "eon_renderable.h"

#include "eon_renderable_private.h"
#include "eon_feature_themable_private.h"
#include "eon_theme_renderable_private.h"

#include "eon_element_eon_private.h"
#include "eon_event_geometry_private.h"
#include "eon_theme_element_eot_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_EON_DESCRIPTOR eon_element_eon_descriptor_get()
#define EON_ELEMENT_EON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Eon, EON_ELEMENT_EON_DESCRIPTOR)

typedef struct _Eon_Element_Eon_Theme
{
	Egueb_Dom_Node *doc;
	char *name;
} Eon_Element_Eon_Theme;

typedef struct _Eon_Element_Eon
{
	Eon_Renderable base;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *compound;
	Eina_Bool renderable_changed;
	Eina_Bool renderable_child;
	/* input */
	Egueb_Dom_Input *input;
	/* window */
	int width;
	int height;
} Eon_Element_Eon;

typedef struct _Eon_Element_Eon_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Eon_Class;

static Egueb_Dom_Node * _eon_element_eon_renderable_child_get(Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *child;

	child = egueb_dom_element_child_first_get(n);
	while (child)
	{
		Egueb_Dom_Node *tmp;

		if (eon_is_renderable(child))
			return child;
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
	return NULL;
}

/* Whenever an invalidate geometry event reaches the topmost element,
 * just enqueue the element for later processing
 */
static void _eon_element_eon_geometry_invalidate_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Egueb_Dom_Node *n;

	n = EGUEB_DOM_NODE(egueb_dom_event_target_current_get(ev));
	egueb_dom_element_enqueue(n);
}

static void _eon_element_eon_tree_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Eon *thiz = data;
	Egueb_Dom_Node *related = NULL;
	Egueb_Dom_Node *target = NULL;
	Egueb_Dom_Node *n;

	/* check that the parent is ourself */
	n = (EON_ELEMENT(thiz))->n;
	related = egueb_dom_event_mutation_related_get(e);
	if (related != n)
	{
		egueb_dom_node_unref(related);
		return;
	}
	egueb_dom_node_unref(related);

	/* get the target and check if it is of type renderable */
	target = EGUEB_DOM_NODE(egueb_dom_event_target_get(e));
	if (!eon_is_renderable(target))
	{
		egueb_dom_node_unref(target);
		return;
	}

	thiz->renderable_changed = EINA_TRUE;
	egueb_dom_node_unref(target);
}
/*----------------------------------------------------------------------------*
 *                              Input interface                               *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_element_eon_input_element_at(Egueb_Dom_Node *parent,
		int x, int y, void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *ret;
	Eina_Rectangle ptr;

	/* iterate over the whole tree */
	eina_rectangle_coords_from(&ptr, x, y, 1, 1);
	ret = eon_renderable_element_at(n, &ptr);

	if (ret)
		DBG_ELEMENT(ret, "Element found at %d %d", x, y);

	return ret;
}

static Egueb_Dom_Node * _eon_element_eon_input_focus_next(Egueb_Dom_Node *current, void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *ret;
	if (current)
	{
		ret = eon_renderable_focus_next(current);
	}
	else
	{
		ret = eon_renderable_focus_first(n);
	}
	return ret;
}

static Egueb_Dom_Node * _eon_element_eon_input_focus_prev(Egueb_Dom_Node *current, void *data)
{
	ERR("focus prev");
	return NULL;
}

static Egueb_Dom_Input_Descriptor _eon_element_eon_input_descriptor = {
	/* .version		= */ EGUEB_DOM_INPUT_DESCRIPTOR_VERSION,
	/* .element_at 		= */ _eon_element_eon_input_element_at,
	/* .focus_next		= */ _eon_element_eon_input_focus_next,
	/* .focus_prev		= */ _eon_element_eon_input_focus_prev,
};

/*----------------------------------------------------------------------------*
 *                          UI feature interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Input * _eon_element_eon_ui_input_get(Egueb_Dom_Node *n)
{
	Eon_Element_Eon *thiz;

	thiz = egueb_dom_element_external_data_get(n);
	return egueb_dom_input_ref(thiz->input);
}

static Egueb_Dom_Feature_UI_Descriptor 
_eon_element_eon_ui_descriptor = {
	/* .version	= */ EGUEB_DOM_FEATURE_UI_DESCRIPTOR_VERSION,
	/* .input_get 	= */ _eon_element_eon_ui_input_get,
};
/*----------------------------------------------------------------------------*
 *                      Animation feature interface                           *
 *----------------------------------------------------------------------------*/
static Egueb_Smil_Feature_Animation_Descriptor 
_eon_element_eon_animation_descriptor = {
	/* .version		= */ EGUEB_SMIL_FEATURE_ANIMATION_DESCRIPTOR_VERSION,
	/* .timeline_get 	= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                        Window feature interface                            *
 *----------------------------------------------------------------------------*/
static int _eon_element_eon_window_hints_get(Egueb_Dom_Node *n,
			Egueb_Dom_Feature_Window_Hint_Data *data)
{
	Eon_Element_Eon *thiz;
	Eon_Renderable_Size size;
	int size_hints;
	int ret = 0;

	size_hints = eon_renderable_size_hints_get(n, &size);
	if (size_hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		ret |= EGUEB_DOM_FEATURE_WINDOW_HINT_MIN_MAX;
		data->min_width = size.min_width;
		data->max_width = size.max_width;
		data->min_height = size.min_height;
		data->max_height = size.max_height;
	}
	if (size_hints & EON_RENDERABLE_HINT_PREFERRED)
	{
		ret |= EGUEB_DOM_FEATURE_WINDOW_HINT_PREFERRED;
		data->pref_width = size.pref_width;
		data->pref_height = size.pref_height;
	}
	return ret;
}

static Eina_Bool _eon_element_eon_window_size_set(
		Egueb_Dom_Node *n, int w, int h)
{
	Eon_Element_Eon *thiz;

	if (w <= 0 || h <= 0)
		return EINA_FALSE;

	DBG("Setting content window to %d %d", w, h);
	thiz = egueb_dom_element_external_data_get(n);
	if (thiz->width != w || thiz->height != h)
		eon_renderable_invalidate_geometry(n);

	thiz->width = w;
	thiz->height = h;

	return EINA_TRUE;
}

static Egueb_Dom_Feature_Window_Descriptor 
_eon_element_eon_window_descriptor = {
	/* .version		= */ EGUEB_DOM_FEATURE_WINDOW_DESCRIPTOR_VERSION,
	/* .hints_get 		= */ _eon_element_eon_window_hints_get,
	/* .size_set 		= */ _eon_element_eon_window_size_set,
};
/*----------------------------------------------------------------------------*
 *                        Render feature interface                            *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_eon_render_renderer_get(
		Egueb_Dom_Node *n)
{
	Enesim_Renderer *r;

	r = eon_renderable_renderer_get(n);
	return r;
}

static Egueb_Dom_Feature_Render_Descriptor
_eon_element_eon_render_descriptor = {
	/* .version		= */ EGUEB_DOM_FEATURE_RENDER_DESCRIPTOR_VERSION,
	/* .renderer_get 	= */ _eon_element_eon_render_renderer_get,
};
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_eon_init(Eon_Renderable *r)
{
	Eon_Element_Eon *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	n = (EON_ELEMENT(r))->n;

	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_eon_tree_modified_cb,
			EINA_FALSE, r);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_eon_tree_modified_cb,
			EINA_FALSE, r);
	egueb_dom_event_target_event_listener_add(et,
			EON_EVENT_GEOMETRY_INVALIDATE,
			_eon_element_eon_geometry_invalidate_cb, EINA_FALSE,
			r);

	thiz = EON_ELEMENT_EON(r);
	thiz->renderable_changed = EINA_TRUE;
	thiz->input = egueb_dom_input_new(&_eon_element_eon_input_descriptor, n);
	thiz->compound = enesim_renderer_compound_new();
	thiz->theme_feature = eon_feature_themable_add(n);
	/* register the features */
	egueb_dom_feature_window_add(n,
			&_eon_element_eon_window_descriptor);
	egueb_dom_feature_render_add(n,
			&_eon_element_eon_render_descriptor);
	egueb_dom_feature_ui_add(n,
			&_eon_element_eon_ui_descriptor);
	egueb_dom_feature_io_add(n);
	egueb_smil_feature_animation_add(n,
			&_eon_element_eon_animation_descriptor);
	e = EON_ELEMENT(r);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_EON));
}

static Eina_Bool _eon_element_eon_pre_process(Eon_Renderable *r)
{
	/* set the size based on the user provided size */
	if (r->needs_geometry)
	{
		Eon_Element_Eon *thiz;
		Egueb_Dom_Node *n;
		Eina_Rectangle geom;

		thiz = EON_ELEMENT_EON(r);

		n = (EON_ELEMENT(r))->n;
		eina_rectangle_coords_from(&geom, 0, 0, thiz->width, thiz->height);
		eon_renderable_geometry_set(n, &geom);
	}
	return EINA_TRUE;
}
static int _eon_element_eon_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	int ret;

	e = EON_ELEMENT(r);
	n = e->n;
	/* get the hints from our child */
	child = _eon_element_eon_renderable_child_get(n);
	if (!child) return 0;

	ret = eon_renderable_size_hints_get(child, size);
	egueb_dom_node_unref(child);
	return ret;
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_element_eon_process(Eon_Renderable *r)
{
	Eon_Element_Eon *thiz;
	Eon_Renderable_Size size;
	Egueb_Dom_Node *theme_element;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child = NULL;

	n = (EON_ELEMENT(r))->n;
	child = _eon_element_eon_renderable_child_get(n);
	/* propagate the geometry */
	if (child)
	{
		eon_renderable_geometry_set(child, &r->geometry);
		egueb_dom_element_process(child);
	}

	thiz = EON_ELEMENT_EON(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	/* update the renderable tree */
	if (thiz->renderable_changed)
	{
		enesim_renderer_compound_layer_clear(thiz->compound);
		if (theme_element)
		{
			Enesim_Renderer_Compound_Layer *l;
			Enesim_Renderer *ren;

			ren = eon_theme_renderable_renderer_get(theme_element);
			l = enesim_renderer_compound_layer_new();
			enesim_renderer_compound_layer_renderer_set(l, ren);
			enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
			enesim_renderer_compound_layer_add(thiz->compound, l);
		}

		/* add our own child */
		if (child)
		{
			Enesim_Renderer *ren_child;

			ren_child = eon_renderable_renderer_get(child);
			if (ren_child)
			{
				Enesim_Renderer_Compound_Layer *l;
				l = enesim_renderer_compound_layer_new();
				enesim_renderer_compound_layer_renderer_set(l, ren_child);
				enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
				enesim_renderer_compound_layer_add(thiz->compound, l);
			}
		}
		thiz->renderable_changed = EINA_FALSE;
	}

	if (theme_element)
	{
		eon_theme_renderable_geometry_set(theme_element, &r->geometry);
		egueb_dom_element_process(theme_element);
		egueb_dom_node_unref(theme_element);
	}
	egueb_dom_node_unref(child);

	return EINA_TRUE;
}

static Egueb_Dom_Node * _eon_element_eon_element_at(Eon_Renderable *r,
		Eina_Rectangle *cursor)
{
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *ret = NULL;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(r))->n;
	child = _eon_element_eon_renderable_child_get(n);
	if (child)
	{
		ret = eon_renderable_element_at(child, cursor);
		egueb_dom_node_unref(child);
	}

	if (!ret)
		ret = egueb_dom_node_ref(n);
	return ret;
}

static Enesim_Renderer * _eon_element_eon_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Eon *thiz;

	thiz = EON_ELEMENT_EON(r);
	return enesim_renderer_ref(thiz->compound);
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_eon_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_EON);
}

static Eina_Bool _eon_element_eon_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Eon_Element_Eon *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *our_child;

	thiz = EON_ELEMENT_EON(e);
	/* only accept or either one renderable or many eot */
	if (eon_theme_element_is_eot(child))
	{
		return EINA_TRUE;
	}
	else if (!thiz->renderable_child)
	{
		if (!eon_is_renderable(child))
			return EINA_FALSE;
		thiz->renderable_child = EINA_TRUE;
		return EINA_TRUE;
	}
	else
	{
		return EINA_FALSE;
	}
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Eon,
		Eon_Element_Eon_Class, eon_element_eon);

static void _eon_element_eon_class_init(void *k)
{
	Eon_Element_Class *klass = k;
	Eon_Renderable_Class *r_klass;

	klass->tag_name_get = _eon_element_eon_tag_name_get;
	klass->child_appendable = _eon_element_eon_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_eon_init;
	r_klass->size_hints_get = _eon_element_eon_size_hints_get;
	r_klass->pre_process = _eon_element_eon_pre_process;
	r_klass->element_at = _eon_element_eon_element_at;
	r_klass->renderer_get = _eon_element_eon_renderer_get;
	r_klass->process = _eon_element_eon_process;
	r_klass->child_size_dependant = EINA_TRUE;
}

static void _eon_element_eon_instance_init(void *o)
{
}

static void _eon_element_eon_instance_deinit(void *o)
{
	Eon_Element_Eon *thiz;
	Eon_Element_Eon_Theme *theme;

	thiz = EON_ELEMENT_EON(o);
	egueb_dom_input_unref(thiz->input);
	enesim_renderer_unref(thiz->compound);
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_eon_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_eon);
	return n;
}
