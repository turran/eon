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
#include "eon_renderable.h"
#include "eon_renderable_private.h"

#include "eon_widget_drawer_private.h"
#include "eon_element_eon_private.h"
#include "eon_event_geometry_private.h"

#include "eon_drawer_widget_private.h"
#include "eon_drawer_eon_private.h"
#include "eon_theme_document_private.h"
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
	Eon_Widget_Drawer base;
	Eina_Bool renderable_changed;
	/* the theme system */
	Eina_List *themes;
	/* input */
	Egueb_Dom_Input *input;
	/* window */
	int width;
	int height;
} Eon_Element_Eon;

typedef struct _Eon_Element_Eon_Class
{
	Eon_Widget_Drawer_Class base;
} Eon_Element_Eon_Class;

static void _eon_element_eon_timeline_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Eon *thiz = data;
	Egueb_Dom_Node *n;

	n = EGUEB_DOM_NODE(egueb_dom_event_target_get(e));
	DBG("Ender document requesting a timeline on %p", n);
	egueb_dom_node_unref(n);
	printf("requesting a timeline!!!\n");
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
	Egueb_Dom_Node *topmost;
	Egueb_Dom_Node *ret;
	Eina_Rectangle ptr;

	/* iterate over the whole tree */
	eina_rectangle_coords_from(&ptr, x, y, 1, 1);
	topmost = egueb_dom_document_document_element_get(n);
	ret = eon_renderable_element_at(topmost, &ptr);
	egueb_dom_node_unref(topmost);

	if (ret)
		DBG_ELEMENT(ret, "Element found at %d %d", x, y);

	return ret;
}

static Egueb_Dom_Input_Descriptor _eon_element_eon_input_descriptor = {
	/* .version		= */ EGUEB_DOM_INPUT_DESCRIPTOR_VERSION,
	/* .element_at 		= */ _eon_element_eon_input_element_at,
	/* .focus_next		= */ NULL,
	/* .focus_prev		= */ NULL,
};

/*----------------------------------------------------------------------------*
 *                          UI feature interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Input * _eon_element_eon_ui_input_get(Egueb_Dom_Node *n)
{
	Eon_Element_Eon *thiz;

	thiz = egueb_dom_element_external_data_get(n);
	return thiz->input;
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
static Eina_Bool _eon_element_eon_window_type_get(
		Egueb_Dom_Node *n, Egueb_Dom_Feature_Window_Type *type)
{
	*type = EGUEB_DOM_FEATURE_WINDOW_TYPE_SLAVE;
	return EINA_TRUE;
}

static Eina_Bool _eon_element_eon_window_content_size_set(
		Egueb_Dom_Node *n, int w, int h)
{
	Eon_Element_Eon *thiz;
	Egueb_Dom_Node *eon;
	Eina_Rectangle geom;
	Eina_Rectangle eon_geom;

	if (w <= 0 || h <= 0)
		return EINA_FALSE;

	eon = egueb_dom_document_document_element_get(n);
	if (!eon)
	{
		return EINA_FALSE;
	}
	eina_rectangle_coords_from(&geom, 0, 0, w, h);
	eon_renderable_geometry_solve(eon, &geom, &eon_geom);
	eon_renderable_geometry_set(eon, &eon_geom);
	egueb_dom_element_enqueue(eon);

	thiz = egueb_dom_element_external_data_get(n);
	thiz->width = w;
	thiz->height = h;

	return EINA_TRUE;
}

static Eina_Bool _eon_element_eon_window_content_size_get(
		Egueb_Dom_Node *n, int *w, int *h)
{
	Eon_Element_Eon *thiz;
	Egueb_Dom_Node *topmost = NULL;

	thiz = egueb_dom_element_external_data_get(n);
	*h = thiz->height;
	*w = thiz->width;

	return EINA_TRUE;
}

static Egueb_Dom_Feature_Window_Descriptor 
_eon_element_eon_window_descriptor = {
	/* .version		= */ EGUEB_DOM_FEATURE_WINDOW_DESCRIPTOR_VERSION,
	/* .type_get 		= */ _eon_element_eon_window_type_get,
	/* .content_size_set 	= */ _eon_element_eon_window_content_size_set,
	/* .content_size_get 	= */ _eon_element_eon_window_content_size_get,
};
/*----------------------------------------------------------------------------*
 *                        Render feature interface                            *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_eon_render_renderer_get(
		Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *topmost;
	Enesim_Renderer *r;

	/* get the topmost element, get the renderer and return it */
	topmost = egueb_dom_document_document_element_get(n);
	if (!topmost) return NULL;

	r = eon_renderable_renderer_get(topmost);
	egueb_dom_node_unref(topmost);
	return r;
}

static Egueb_Dom_Feature_Render_Descriptor
_eon_element_eon_render_descriptor = {
	/* .version		= */ EGUEB_DOM_FEATURE_RENDER_DESCRIPTOR_VERSION,
	/* .renderer_get 	= */ _eon_element_eon_render_renderer_get,
};
/*----------------------------------------------------------------------------*
 *                        Widget Drawer interface                             *
 *----------------------------------------------------------------------------*/
static int _eon_element_eon_size_hints_get(Eon_Widget_Drawer *w, Eon_Renderable_Size *size)
{
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	int ret;

	e = EON_ELEMENT(w);
	n = e->n;
	/* get the hints from our child */
	child = egueb_dom_element_child_first_get(n);
	if (!child) return 0;

	ret = eon_renderable_size_hints_get(child, size);
	egueb_dom_node_unref(child);
	return ret;
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_element_eon_process(Eon_Widget_Drawer *w)
{
	Eon_Element_Eon *thiz;
	Eon_Renderable *r;
	Eon_Renderable_Size size;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	int size_hints;

	r = EON_RENDERABLE(w);

	n = (EON_ELEMENT(r))->n;
	child = egueb_dom_element_child_first_get(n);
	if (!child) return EINA_TRUE;

	size_hints = eon_renderable_size_hints_get(child, &size);
	if (size_hints)
	{
		/* TODO calculate the geometry */ 
		eon_renderable_geometry_set(child, &r->geometry);
	}
	else
	{
		eon_renderable_geometry_set(child, &r->geometry);
	}
	egueb_dom_element_process(child);
	egueb_dom_node_unref(child);

	thiz = EON_ELEMENT_EON(r);
	/* update the renderable tree */
	if (thiz->renderable_changed)
	{
		Enesim_Renderer *r;
		Enesim_Renderer *r_child;

		r = eon_drawer_widget_renderer_get(w->theme_widget);
		enesim_renderer_compound_layer_clear(r);

		/* update the backgrund */
		eon_drawer_eon_update_background(w->theme_widget);
		/* add our own child */
		r_child = eon_renderable_renderer_get(child);
		if (r_child)
		{
			Enesim_Renderer_Compound_Layer *l;

			l = enesim_renderer_compound_layer_new();
			enesim_renderer_compound_layer_renderer_set(l, r_child);
			enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
			enesim_renderer_compound_layer_add(r, l);
		}
		thiz->renderable_changed = EINA_FALSE;
		enesim_renderer_unref(r);
	}

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_eon_init(Eon_Widget *w)
{
	Egueb_Dom_Node *n;
	Eon_Element_Eon *thiz;

	n = (EON_ELEMENT(w))->n;
	egueb_dom_event_target_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_eon_tree_modified_cb,
			EINA_FALSE, w);
	egueb_dom_event_target_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_eon_tree_modified_cb,
			EINA_FALSE, w);
	egueb_dom_event_target_event_listener_add(n,
			EON_EVENT_GEOMETRY_INVALIDATE,
			_eon_element_eon_geometry_invalidate_cb, EINA_FALSE,
			w);

	thiz = EON_ELEMENT_EON(w);
	thiz->input = egueb_dom_input_new(&_eon_element_eon_input_descriptor, n);
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
}

static Eina_Bool _eon_element_eon_pre_process(Eon_Widget *w)
{
	Eon_Renderable *r;

	r = EON_RENDERABLE(w);
	/* set the size based on the user provided size */
	if (r->needs_geometry)
	{
		Eon_Element_Eon *thiz;
		Egueb_Dom_Node *n;
		Eina_Rectangle geom;

		thiz = EON_ELEMENT_EON(w);

		n = (EON_ELEMENT(r))->n;
		eina_rectangle_coords_from(&geom, 0, 0, thiz->width, thiz->height);
		eon_renderable_geometry_set(n, &geom);
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_element_eon_element_at(Eon_Renderable *r,
		Eina_Rectangle *cursor)
{
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *ret = NULL;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(r))->n;
	child = egueb_dom_element_child_first_get(n);
	if (child)
		ret = eon_renderable_element_at(child, cursor);
	egueb_dom_node_unref(child);
	if (!ret)
		ret = egueb_dom_node_ref(n);
	return ret;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_eon_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_EON);
}

static Eina_Bool _eon_element_eon_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *our_child;

	/* only accept one child */
	if (!eon_is_renderable(child))
		return EINA_FALSE;

	/* check if we already have one child */
	n = e->n;
	our_child = egueb_dom_element_child_first_get(n);
	if (our_child)
	{
		WARN("Only one child supported");
		egueb_dom_node_unref(our_child);
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DRAWER_DESCRIPTOR, Eon_Element_Eon,
		Eon_Element_Eon_Class, eon_element_eon);

static void _eon_element_eon_class_init(void *k)
{
	Eon_Element_Class *klass = k;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;
	Eon_Widget_Drawer_Class *wd_klass;

	klass->tag_name_get = _eon_element_eon_tag_name_get;
	klass->child_appendable = _eon_element_eon_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->child_size_dependant = EINA_TRUE;
	r_klass->element_at = _eon_element_eon_element_at;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_eon_init;
	w_klass->pre_process = _eon_element_eon_pre_process;

	wd_klass = EON_WIDGET_DRAWER_CLASS(k);
	wd_klass->size_hints_get = _eon_element_eon_size_hints_get;
	wd_klass->process = _eon_element_eon_process;
}

static void _eon_element_eon_instance_init(void *o)
{
}

static void _eon_element_eon_instance_deinit(void *o)
{
	Eon_Element_Eon *thiz;
	Eon_Element_Eon_Theme *theme;

	thiz = EON_ELEMENT_EON(o);
	EINA_LIST_FREE(thiz->themes, theme)
	{
		egueb_dom_node_unref(theme->doc);
		free(theme->name);
		free(theme);
	}
	egueb_dom_input_unref(thiz->input);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_element_eon_theme_load(Egueb_Dom_Node *n, Egueb_Dom_String *theme_name)
{
	Eon_Element_Eon *thiz;
	Eon_Element_Eon_Theme *theme;
	Egueb_Dom_Node *ret = NULL;
	Eina_List *l;

	if (!theme_name) return NULL;

	thiz = egueb_dom_element_external_data_get(n);
	EINA_LIST_FOREACH(thiz->themes, l, theme)
	{
		if (!strcmp(egueb_dom_string_string_get(theme_name), theme->name))
		{
			ret = egueb_dom_node_ref(theme->doc);
			break;
		}
	}

	/* load the file based from the fs */
	if (!ret)
	{
		Enesim_Stream *s;
		Egueb_Dom_Node *doc;
		const char *theme_path;
		char path[PATH_MAX];
		

		theme_path = getenv("EON_THEME_PATH");
		if (!theme_path)
		{
			theme_path = "/usr/local/share/eon/themes";
		}
		snprintf(path, PATH_MAX, "%s/%s.ender", theme_path, egueb_dom_string_string_get(theme_name));
		DBG("Loading theme at '%s'", path);
		s = enesim_stream_file_new(path, "r");
		if (!s) return NULL;

		doc = eon_theme_document_new();
#if 0
		egueb_dom_event_target_event_listener_add(doc, EGUEB_SMIL_EVENT_TIMELINE,
				_eon_element_eon_timeline_cb, EINA_TRUE, thiz);
#endif
		egueb_dom_parser_parse(s, &doc);

		theme = calloc(1, sizeof(Eon_Element_Eon_Theme));
		theme->doc = doc;
		theme->name = strdup(egueb_dom_string_string_get(theme_name));
		thiz->themes = eina_list_append(thiz->themes, theme);

		ret = egueb_dom_node_ref(doc);
	}

	return ret;

}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_eon_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_eon);
	return n;
}

#if 0
EAPI void eon_document_theme_set_from_document(Egueb_Dom_Node *n, Egueb_Dom_Node *theme)
{
	/* iterate over every widget, check if the theme set is NULL, if so
	 * update the instance
	 */
}

EAPI void eon_document_theme_set_from_stream(Egueb_Dom_Node *n, Enesim_Stream *s)
{
	Egueb_Dom_Node *theme;

	eon_document_theme_set_from_document(n, theme);
}
#endif
