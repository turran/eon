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
#include "eon_element_eon_private.h"
#include "eon_event_geometry_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_EON_DESCRIPTOR eon_element_eon_descriptor_get()
#define EON_ELEMENT_EON_CLASS_GET(o) EON_ELEMENT_EON_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_ELEMENT_EON_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Element_Eon_Class, EON_ELEMENT_EON_DESCRIPTOR)
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
	Enesim_Renderer *r;
	Eina_Bool renderable_changed;
	/* the theme system */
	Eina_List *themes;
} Eon_Element_Eon;

typedef struct _Eon_Element_Eon_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Eon_Class;

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
	target = egueb_dom_event_target_get(e);
	if (!eon_is_renderable(target))
	{
		egueb_dom_node_unref(target);
		return;
	}

	thiz->renderable_changed = EINA_TRUE;
	egueb_dom_node_unref(target);
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_eon_init(Eon_Renderable *r)
{
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(r))->n;
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_eon_tree_modified_cb,
			EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_eon_tree_modified_cb,
			EINA_FALSE, r);
}

static Enesim_Renderer * _eon_element_eon_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Eon *thiz;

	thiz = EON_ELEMENT_EON(r);
	return enesim_renderer_ref(thiz->r);
}

static void _eon_element_eon_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
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
	child = egueb_dom_element_child_first_get(n);
	if (!child) return 0;

	ret = eon_renderable_size_hints_get(child, size);
	egueb_dom_node_unref(child);
	return ret;
}

static Eina_Bool _eon_element_eon_pre_process(Eon_Renderable *r)
{
	Eon_Element_Eon *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_EON(r);
	e = EON_ELEMENT(r);
	n = e->n;

	/* set the size based on the user provided size */

	if (r->needs_geometry)
	{
		Egueb_Dom_Event *ev;
		Eina_Rectangle geom;
		Eina_Bool set;

		/* request a geometry upstream */
		ev = eon_event_geometry_new();
		egueb_dom_node_event_dispatch(n, egueb_dom_event_ref(ev), NULL, NULL);
		/* check that we have a valid geometry */
		set = eon_event_geometry_get(ev, &geom);
		egueb_dom_event_unref(ev);

		if (!set)
		{
			ERR("Application does not set a geometry");
			return EINA_FALSE;

		}
		eon_renderable_geometry_set(n, &geom);
	}
	return EINA_TRUE;
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_element_eon_process(Eon_Renderable *r)
{
	Eon_Element_Eon *thiz;
	Eon_Renderable_Size size;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	int size_hints;

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
		Enesim_Renderer_Compound_Layer *l;

		enesim_renderer_compound_layer_clear(thiz->r);
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_renderer_set(l, eon_renderable_renderer_get(child));
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
		enesim_renderer_compound_layer_add(thiz->r, l);
		thiz->renderable_changed = EINA_FALSE;
	}

	return EINA_TRUE;
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
	r_klass->child_size_dependant = EINA_TRUE;
	r_klass->renderer_get = _eon_element_eon_renderer_get;
	r_klass->size_hints_get = _eon_element_eon_size_hints_get;
	r_klass->geometry_set = _eon_element_eon_geometry_set;
	r_klass->pre_process = _eon_element_eon_pre_process;
	r_klass->process = _eon_element_eon_process;
}

static void _eon_element_eon_instance_init(void *o)
{
	Eon_Element_Eon *thiz;

	thiz = EON_ELEMENT_EON(o);
	thiz->r = enesim_renderer_compound_new();
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
	enesim_renderer_unref(thiz->r);
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

		doc = ender_document_new();
		egueb_dom_parser_parse(s, &doc);
		enesim_stream_unref(s);

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

EAPI Eina_Bool eon_element_eon_draw(Egueb_Dom_Node *n,
		Enesim_Surface *s, Enesim_Rop rop, Eina_Rectangle *clip,
		int x, int y, Enesim_Log **error)
{
	Eon_Element_Eon *thiz;
	Eina_Bool ret;

	thiz = egueb_dom_element_external_data_get(n);
	return enesim_renderer_draw(thiz->r, s, rop, clip, x, y, error);
}

EAPI void eon_element_eon_theme_tick(Egueb_Dom_Node *n)
{
	Eon_Element_Eon *thiz;
	Eon_Element_Eon_Theme *theme;
	Eina_List *l;

	thiz = egueb_dom_element_external_data_get(n);
	EINA_LIST_FOREACH(thiz->themes, l, theme)
	{
		ender_document_tick(theme->doc);
	}
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
