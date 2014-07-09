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
#include "eon_widget_proxy_private.h"

/*
 * TODO
 * Whenever the proxied element needs to be processed be sure to request
 * ourselves too
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _eon_widget_proxy_propagate(Eon_Widget_Proxy *thiz)
{
	Eon_Widget_Proxy_Class *klass;

	klass = EON_WIDGET_PROXY_CLASS_GET(thiz);
	if (klass->geometry_propagate)
	{
		klass->geometry_propagate(thiz);
	}
}

static void _eon_widget_proxy_resolve_theme(Eon_Widget_Proxy *thiz)
{
	Eon_Widget *w;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *rel_theme = NULL;
	Egueb_Dom_String *theme = NULL;

	if (!thiz->theme_changed && thiz->adopted)
		return;

	w = EON_WIDGET(thiz);
	e = EON_ELEMENT(thiz);
	n = e->n;
	if (thiz->theme_changed)
	{
		parent = egueb_dom_node_parent_get(n);
		if (parent && (egueb_dom_node_type_get(parent) == EGUEB_DOM_NODE_TYPE_ELEMENT))
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
		thiz->theme_changed = EINA_FALSE;
	}
	/* Adopt the proxied element. We do here instead of on an event
	 * to avoid the case of have a proxy that proxies another proxy.
	 * In such case the adopt function does not trigger an event and is
	 * impossible to adopt the second proxied node
	 */
	if (!thiz->adopted)
	{
		Egueb_Dom_Node *doc;

		doc = egueb_dom_node_owner_document_get((EON_ELEMENT(w))->n);
		thiz->proxy = egueb_dom_document_node_adopt(doc, thiz->proxy, NULL);
		thiz->adopted = EINA_TRUE;
	}
}

/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_widget_proxy_init(Eon_Widget *w)
{
	Eon_Widget_Proxy *thiz;
	Eon_Widget_Proxy_Class *klass;
	Egueb_Dom_Node *n;

	thiz = EON_WIDGET_PROXY(w);
	klass = EON_WIDGET_PROXY_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
	thiz->proxy = klass->proxied_get(thiz);
}

static void _eon_widget_proxy_theme_changed(Eon_Widget *w)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(w);
	thiz->theme_changed = EINA_TRUE;
}

static void _eon_widget_proxy_state_set(Eon_Widget *w, Egueb_Dom_String *s)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(w);
	eon_widget_state_set(thiz->proxy, s);
}

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_widget_proxy_renderer_get(Eon_Renderable *r)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	if (!thiz->proxy)
		return NULL;
	return eon_renderable_renderer_get(thiz->proxy);
}

static void _eon_widget_proxy_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	eon_renderable_geometry_set(thiz->proxy, geometry);
}

static int _eon_widget_proxy_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	/* First propagate any widget attribute that might require a different size hint */
	_eon_widget_proxy_propagate(thiz);
	_eon_widget_proxy_resolve_theme(thiz);
	return eon_renderable_size_hints_get(thiz->proxy, size);
}

static Eina_Bool _eon_widget_proxy_process(Eon_Renderable *r)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	_eon_widget_proxy_resolve_theme(thiz);
	return egueb_dom_element_process(thiz->proxy);
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Widget_Proxy,
		Eon_Widget_Proxy_Class, eon_widget_proxy);

static void _eon_widget_proxy_class_init(void *k)
{
	Eon_Element_Class *klass = EON_ELEMENT_CLASS(k);
	Eon_Widget_Class *w_klass;
	Eon_Renderable_Class *r_klass;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_widget_proxy_renderer_get;
	r_klass->size_hints_get = _eon_widget_proxy_size_hints_get;
	r_klass->geometry_set = _eon_widget_proxy_geometry_set;
	r_klass->process = _eon_widget_proxy_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_widget_proxy_init;
	w_klass->state_set = _eon_widget_proxy_state_set;
	w_klass->theme_changed = _eon_widget_proxy_theme_changed;
}

static void _eon_widget_proxy_instance_init(void *o)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(o);
	thiz->theme_changed = EINA_TRUE;
}

static void _eon_widget_proxy_instance_deinit(void *o)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(o);
	egueb_dom_node_unref(thiz->proxy);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

