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
#include "eon_widget_drawer_private.h"
#include "eon_element_eon_private.h"

#include "eon_drawer_widget_private.h"
/* TODO
 * - Add a callback on the request_process function of the theme
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Bool _eon_widget_drawer_load_theme(Eon_Widget_Drawer *thiz)
{
	Eon_Element *e;
	Eon_Widget *w;
	Egueb_Dom_String *theme = NULL;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *topmost;
	Egueb_Dom_Node *rel_theme = NULL;
	Eina_Bool ret = EINA_TRUE;

	w = EON_WIDGET(thiz);
	e = EON_ELEMENT(w);
	n = e->n;

	if (!thiz->theme_changed)
	{
		if (!thiz->theme_widget)
		{
			ERR_ELEMENT(n, "Theme does not exist");
			return EINA_FALSE;
		}
		return EINA_TRUE;
	}

	if (thiz->theme_instance)
	{
		egueb_dom_node_unref(thiz->theme_instance);
		thiz->theme_instance = NULL;
	}

	if (thiz->theme_document)
	{
		egueb_dom_node_unref(thiz->theme_document);
		thiz->theme_document = NULL;
	}
	thiz->theme_widget = NULL;

	parent = egueb_dom_node_parent_get(n);
	if (parent && (egueb_dom_node_type_get(parent) == EGUEB_DOM_NODE_TYPE_ELEMENT_NODE))
	{
		Eon_Element *other;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(parent));
		rel_theme = egueb_dom_node_ref(other->theme);
		if (w->last_parent_theme)
			egueb_dom_string_unref(w->last_parent_theme);
		egueb_dom_attr_final_get(other->theme, &theme);
		w->last_parent_theme = egueb_dom_string_dup(theme);
		// FIX this egueb_dom_string_unref(theme);
	}
	egueb_dom_attr_inheritable_process(e->theme, rel_theme);

	egueb_dom_node_unref(rel_theme);
	egueb_dom_node_unref(parent);

	egueb_dom_attr_final_get(e->theme, &theme);
	DBG_ELEMENT(n, "Last theme was '%s', new one is '%s'",
			egueb_dom_string_string_get(w->last_theme),
			egueb_dom_string_string_get(theme));
	/* check if the theme has changed, if so, request a new theme */
	if (!egueb_dom_string_is_equal(theme, w->last_theme))
	{
		if (w->last_theme)
			egueb_dom_string_unref(w->last_theme);
		/* ask for the theme */
		/* get the tag name and fetch the instance from it */
		DBG_ELEMENT(n, "Theme is '%s'", egueb_dom_string_string_get(theme));
		w->last_theme = egueb_dom_string_dup(theme);
	}
	// FIX this egueb_dom_string_unref(theme);

	/* load the theme */
	doc = egueb_dom_node_document_get(n);
	if (!doc)
	{
		WARN_ELEMENT(n, "Node does not have a document");
		ret = EINA_FALSE;
		goto no_document;
	}

	topmost = egueb_dom_document_element_get(doc);
	if (!topmost)
	{
		WARN_ELEMENT(n, "Document does not have topmost element");
		ret = EINA_FALSE;
		goto no_topmost;
	}

	INFO_ELEMENT(n, "Loading theme");
	thiz->theme_document = eon_element_eon_theme_load(topmost, theme);
	if (thiz->theme_document)
	{
		Eon_Widget_Drawer_Class *klass;
		Egueb_Dom_Node *theme_instance = NULL;

		klass = EON_WIDGET_DRAWER_CLASS_GET(thiz);
		if (thiz->theme_instance_create)
		{
			theme_instance = thiz->theme_instance_create(thiz,
					thiz->theme_document, thiz->theme_instance_create_data);
		}

		if (!theme_instance && klass->theme_instance_create)
		{
			theme_instance = klass->theme_instance_create(thiz,
					thiz->theme_document);
		}

		if (!theme_instance)
		{
			Egueb_Dom_String *tag_name;

			tag_name = egueb_dom_element_tag_name_get(n);
			theme_instance = ender_document_instance_new(
				thiz->theme_document,
				egueb_dom_string_string_get(tag_name),
				NULL);
		}
		if (theme_instance)
		{
			Enesim_Renderer *r;
			int enabled;

			INFO_ELEMENT(n, "Instance fetched correctly");
			thiz->theme_instance = theme_instance;
			/* get the drawer widget */
			thiz->theme_widget = ender_element_instance_object_get(thiz->theme_instance);
			/* set the renderer */
			r = eon_drawer_widget_renderer_get(thiz->theme_widget);
			enesim_renderer_proxy_proxied_set(thiz->proxy, r);
			/* call the class method */
			if (klass->theme_instance_created)
				klass->theme_instance_created(thiz);
			/* Set the default state first */
			egueb_dom_attr_final_get(w->enabled, &enabled);
			ender_element_instance_state_set(thiz->theme_instance,
					enabled ? "default" : "disabled", NULL);
		}
		else
		{
			ret = EINA_FALSE;
			goto no_theme;
		}
	}
	else
	{
		ret = EINA_FALSE;
		goto no_theme;
	}

no_theme:
	egueb_dom_node_unref(topmost);
no_topmost:
	egueb_dom_node_unref(doc);
no_document:
	egueb_dom_string_unref(theme);
	thiz->theme_changed = EINA_FALSE;

	return ret;
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_widget_drawer_theme_changed(Eon_Widget *w)
{
	Eon_Widget_Drawer *thiz;

	thiz = EON_WIDGET_DRAWER(w);
	thiz->theme_changed = EINA_TRUE;
}

static void _eon_widget_drawer_state_set(Eon_Widget *w, Egueb_Dom_String *state)
{
	Eon_Widget_Drawer *thiz;
	const char *s;

	thiz = EON_WIDGET_DRAWER(w);
	if (!thiz->theme_instance)
		return;

	s = egueb_dom_string_string_get(state);	
	ender_element_instance_state_set(thiz->theme_instance, s, NULL);
}

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_widget_drawer_renderer_get(Eon_Renderable *r)
{
	Eon_Widget_Drawer *thiz;

	thiz = EON_WIDGET_DRAWER(r);
	return enesim_renderer_ref(thiz->proxy);
}

static void _eon_widget_drawer_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	Eon_Widget_Drawer *thiz;

	thiz = EON_WIDGET_DRAWER(r);
	if (!thiz->theme_widget)
		return;
	
	/* set the geometry on the theme instance */
	eon_drawer_widget_geometry_set(thiz->theme_widget, geometry);
}

static int _eon_widget_drawer_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Widget_Drawer *thiz;
	Eon_Widget_Drawer_Class *klass;
	int ret = 0;

	thiz = EON_WIDGET_DRAWER(r);
	klass = EON_WIDGET_DRAWER_CLASS_GET(thiz);
	if (!_eon_widget_drawer_load_theme(thiz))
	{
		ERR_ELEMENT((EON_ELEMENT(r))->n, "Failed to load the theme");
		return 0;
	}

	if (klass->size_hints_get)
		ret = klass->size_hints_get(thiz, size);
	return ret;
}

static Eina_Bool _eon_widget_drawer_process(Eon_Renderable *r)
{
	Eon_Widget_Drawer *thiz;
	Eon_Widget_Drawer_Class *klass;

	thiz = EON_WIDGET_DRAWER(r);
	if (!_eon_widget_drawer_load_theme(thiz))
	{
		ERR_ELEMENT((EON_ELEMENT(r))->n, "Failed to load the theme");
		return EINA_FALSE;
	}

	klass = EON_WIDGET_DRAWER_CLASS_GET(r);
	if (klass->process)
	{
		if (!klass->process(thiz))
			return EINA_FALSE;
	}
 	/* process the theme instance too? */
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Widget_Drawer,
		Eon_Widget_Drawer_Class, eon_widget_drawer);

static void _eon_widget_drawer_class_init(void *k)
{
	Eon_Element_Class *klass = EON_ELEMENT_CLASS(k);
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_widget_drawer_renderer_get;
	r_klass->size_hints_get = _eon_widget_drawer_size_hints_get;
	r_klass->geometry_set = _eon_widget_drawer_geometry_set;
	r_klass->process = _eon_widget_drawer_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->state_set = _eon_widget_drawer_state_set;
}

static void _eon_widget_drawer_instance_init(void *o)
{
	Eon_Widget_Drawer *thiz;

	thiz = EON_WIDGET_DRAWER(o);
	/* instantiate the theme element */
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_changed = EINA_TRUE;
}

static void _eon_widget_drawer_instance_deinit(void *o)
{
	Eon_Widget_Drawer *thiz;

	thiz = EON_WIDGET_DRAWER(o);
	if (thiz->proxy)
		enesim_renderer_unref(thiz->proxy);
	if (thiz->theme_instance)
		egueb_dom_node_unref(thiz->theme_instance);
	if (thiz->theme_document)
		egueb_dom_node_unref(thiz->theme_document);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_widget_drawer_theme_instance_create_set(Egueb_Dom_Node *n,
		Eon_Widget_Drawer_Theme_Instance_Create cb, void *data)
{
	Eon_Widget_Drawer *thiz;

	thiz = EON_WIDGET_DRAWER(egueb_dom_element_external_data_get(n));
	thiz->theme_instance_create = cb;
	thiz->theme_instance_create_data = data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
