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
#include "eon_widget_private.h"
#include "eon_element_eon_private.h"

#include "eon_drawer_widget_private.h"
/* TODO
 * - Add a callback on the request_process function of the theme
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _eon_widget_mouse_ui_cb(Egueb_Dom_Event *e, void *data)
{
	Eon_Widget *thiz = data;
	Egueb_Dom_String *type;
	const char *s;

	if (!thiz->theme_instance)
		return;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	type = egueb_dom_event_type_get(e);
	s = egueb_dom_string_string_get(type);
	ender_element_instance_state_set(thiz->theme_instance, s, NULL);
	egueb_dom_string_unref(type);
}

static Eina_Bool _eon_widget_load_theme(Eon_Widget *thiz)
{
	Eon_Element *e;
	Egueb_Dom_String *theme = NULL;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *topmost;
	Eina_Bool ret = EINA_TRUE;

	if (!thiz->theme_changed)
	{
		if (!thiz->theme_widget)
		{
			ERR_ELEMENT((EON_ELEMENT(thiz))->n, "Theme does not exist");
			return EINA_FALSE;
		}
		return EINA_TRUE;
	}

	e = EON_ELEMENT(thiz);
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

	n = e->n;
	parent = egueb_dom_node_parent_get(n);
	if (parent && (egueb_dom_node_type_get(parent) == EGUEB_DOM_NODE_TYPE_ELEMENT_NODE))
	{
		Eon_Element *other;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(parent));
		egueb_dom_attr_inheritable_process(e->theme, other->theme);
		if (thiz->last_parent_theme)
			egueb_dom_string_unref(thiz->last_parent_theme);
		egueb_dom_attr_final_get(other->theme, &theme);
		thiz->last_parent_theme = egueb_dom_string_dup(theme);
		// FIX this egueb_dom_string_unref(theme);
	}
	egueb_dom_node_unref(parent);

	egueb_dom_attr_final_get(e->theme, &theme);
	DBG_ELEMENT(e->n, "Last theme was '%s', new one is '%s'",
			egueb_dom_string_string_get(thiz->last_theme),
			egueb_dom_string_string_get(theme));
	/* check if the theme has changed, if so, request a new theme */
	if (!egueb_dom_string_is_equal(theme, thiz->last_theme))
	{
		if (thiz->last_theme)
			egueb_dom_string_unref(thiz->last_theme);
		/* ask for the theme */
		/* get the tag name and fetch the instance from it */
		DBG_ELEMENT(e->n, "Theme is '%s'", egueb_dom_string_string_get(theme));
		thiz->last_theme = egueb_dom_string_dup(theme);
	}
	// FIX this egueb_dom_string_unref(theme);

	/* load the theme */
	doc = egueb_dom_node_document_get(n);
	if (!doc)
	{
		ret = EINA_FALSE;
		goto no_document;
	}

	topmost = egueb_dom_document_element_get(doc);
	if (!topmost)
	{
		ret = EINA_FALSE;
		goto no_topmost;
	}

	INFO_ELEMENT(e->n, "Loading theme");
	thiz->theme_document = eon_element_eon_theme_load(topmost, theme);
	if (thiz->theme_document)
	{
		Eon_Widget_Class *klass;

		klass = EON_WIDGET_CLASS_GET(thiz);
		if (klass->theme_instance_create)
		{
			thiz->theme_instance = klass->theme_instance_create(thiz);
		}
		else
		{
			Egueb_Dom_String *tag_name;

			tag_name = egueb_dom_element_tag_name_get(n);
			thiz->theme_instance = ender_document_instance_new(
				thiz->theme_document,
				egueb_dom_string_string_get(tag_name),
				NULL);
		}
		if (thiz->theme_instance)
		{
			Enesim_Renderer *r;

			INFO_ELEMENT(e->n, "Instance fetched correctly");
			/* get the drawer widget */
			thiz->theme_widget = ender_element_instance_object_get(thiz->theme_instance);
			/* set the renderer */
			r = eon_drawer_widget_renderer_get(thiz->theme_widget);
			enesim_renderer_proxy_proxied_set(thiz->proxy, r);
			/* call the class method */
			if (klass->theme_instance_created)
				klass->theme_instance_created(thiz);
			/* Set the default state first */
			ender_element_instance_state_set(thiz->theme_instance, "default", NULL);
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
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_widget_init(Eon_Renderable *r)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;
	Egueb_Dom_Node *n;

	thiz = EON_WIDGET(r);
	n = (EON_ELEMENT(r))->n;
	egueb_dom_node_event_listener_add(n, EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_widget_mouse_ui_cb, EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n, EGUEB_DOM_EVENT_MOUSE_UP,
			_eon_widget_mouse_ui_cb, EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n, EGUEB_DOM_EVENT_MOUSE_DOWN,
			_eon_widget_mouse_ui_cb, EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n, EGUEB_DOM_EVENT_MOUSE_OVER,
			_eon_widget_mouse_ui_cb, EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n, EGUEB_DOM_EVENT_MOUSE_OUT,
			_eon_widget_mouse_ui_cb, EINA_FALSE, r);

	klass = EON_WIDGET_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

static Enesim_Renderer * _eon_widget_renderer_get(Eon_Renderable *r)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(r);
	return enesim_renderer_ref(thiz->proxy);
}

static void _eon_widget_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(r);
	if (!thiz->theme_widget)
		return;
	
	/* set the geometry on the theme instance */
	eon_drawer_widget_geometry_set(thiz->theme_widget, geometry);
}

static int _eon_widget_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;
	int ret = 0;

	thiz = EON_WIDGET(r);
	klass = EON_WIDGET_CLASS_GET(thiz);
	if (!_eon_widget_load_theme(thiz))
	{
		ERR_ELEMENT((EON_ELEMENT(r))->n, "Failed to load the theme");
		return 0;
	}

	if (klass->size_hints_get)
		ret = klass->size_hints_get(thiz, size);
	return ret;
}

static Eina_Bool _eon_widget_pre_process(Eon_Renderable *r)
{
	Eon_Element *e;
	Eon_Widget_Class *klass;
	Eon_Widget *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;

	thiz = EON_WIDGET(r);
	e = EON_ELEMENT(r);
	/* in case the theme value has been modifed, invalide the geometry */
	if (egueb_dom_attr_has_changed(e->theme))
	{
		DBG_ELEMENT(e->n, "Widget's theme is different");
		thiz->theme_changed = EINA_TRUE;
		eon_renderable_invalidate_geometry(r);
		goto done;
	}
	/* in case the parent's theme is different than the previous
	 * theme, invalidate it too
	 */
	n = e->n;
	parent = egueb_dom_node_parent_get(n);
	if (parent && (egueb_dom_node_type_get(parent) == EGUEB_DOM_NODE_TYPE_ELEMENT_NODE))
	{
		Eon_Element *other;
		Egueb_Dom_String *theme;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(parent));
		egueb_dom_attr_final_get(other->theme, &theme);
		if (!egueb_dom_string_is_equal(theme, thiz->last_parent_theme))
		{
			if (thiz->last_parent_theme)
				egueb_dom_string_unref(thiz->last_parent_theme);
			thiz->last_parent_theme = egueb_dom_string_dup(theme);
			thiz->theme_changed = EINA_TRUE;
			eon_renderable_invalidate_geometry(r);
			DBG_ELEMENT(e->n, "Parent's theme is different than last one");
		}
		// FIX this egueb_dom_string_unref(theme);
	}
	egueb_dom_node_unref(parent);

done:
	klass = EON_WIDGET_CLASS_GET(r);
	if (klass->pre_process)
	{
		if (!klass->pre_process(thiz))
			return EINA_FALSE;
	}
	return EINA_TRUE;
}

static Eina_Bool _eon_widget_process(Eon_Renderable *r)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;

	thiz = EON_WIDGET(r);
	if (!_eon_widget_load_theme(thiz))
	{
		ERR_ELEMENT((EON_ELEMENT(r))->n, "Failed to load the theme");
		return EINA_FALSE;
	}

	klass = EON_WIDGET_CLASS_GET(r);
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
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Widget,
		Eon_Widget_Class, eon_widget);

static void _eon_widget_class_init(void *k)
{
	Eon_Element_Class *klass = EON_ELEMENT_CLASS(k);
	Eon_Renderable_Class *r_klass;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_widget_init;
	r_klass->renderer_get = _eon_widget_renderer_get;
	r_klass->size_hints_get = _eon_widget_size_hints_get;
	r_klass->geometry_set = _eon_widget_geometry_set;
	r_klass->pre_process = _eon_widget_pre_process;
	r_klass->process = _eon_widget_process;
}

static void _eon_widget_instance_init(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);
	/* instantiate the theme element */
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_changed = EINA_TRUE;
}

static void _eon_widget_instance_deinit(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);
	if (thiz->last_theme)
		egueb_dom_string_unref(thiz->last_theme);
	if (thiz->last_parent_theme)
		egueb_dom_string_unref(thiz->last_parent_theme);
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
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
