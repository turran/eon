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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _eon_widget_mouse_ui_cb(Egueb_Dom_Event *e, void *data)
{
	Eon_Widget *thiz = data;
	Eon_Widget_Class *klass;
	Egueb_Dom_String *type;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	klass = EON_WIDGET_CLASS_GET(thiz);
	if (!klass->state_set)
		return;

	type = egueb_dom_event_type_get(e);
	klass->state_set(thiz, type);
	egueb_dom_string_unref(type);
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

	thiz->enabled = egueb_dom_attr_boolean_new(egueb_dom_string_ref(EON_ATTR_ENABLED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->enabled, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_TRUE);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->enabled), NULL);

	klass = EON_WIDGET_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

static Eina_Bool _eon_widget_pre_process(Eon_Renderable *r)
{
	Eon_Element *e;
	Eon_Widget_Class *klass;
	Eon_Widget *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;

	thiz = EON_WIDGET(r);
	klass = EON_WIDGET_CLASS_GET(r);

	e = EON_ELEMENT(r);
	/* in case the theme value has been modifed, invalide the geometry */
	if (egueb_dom_attr_has_changed(e->theme))
	{
		DBG_ELEMENT(e->n, "Widget's theme is different");
		if (klass->theme_changed)
			klass->theme_changed(thiz);
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
			DBG_ELEMENT(e->n, "Parent's theme is different than last one ('%s', '%s')",
					egueb_dom_string_string_get(theme),
					egueb_dom_string_string_get(thiz->last_parent_theme));
			if (thiz->last_parent_theme)
				egueb_dom_string_unref(thiz->last_parent_theme);
			thiz->last_parent_theme = egueb_dom_string_dup(theme);
			if (klass->theme_changed)
				klass->theme_changed(thiz);
			eon_renderable_invalidate_geometry(r);
		}
		// FIX this egueb_dom_string_unref(theme);
	}
	egueb_dom_node_unref(parent);

done:
	if (klass->pre_process)
	{
		if (!klass->pre_process(thiz))
			return EINA_FALSE;
	}
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
	r_klass->pre_process = _eon_widget_pre_process;
}

static void _eon_widget_instance_init(void *o)
{
}

static void _eon_widget_instance_deinit(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);

	egueb_dom_node_unref(thiz->enabled);

	if (thiz->last_theme)
		egueb_dom_string_unref(thiz->last_theme);
	if (thiz->last_parent_theme)
		egueb_dom_string_unref(thiz->last_parent_theme);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_widget_state_set(Egueb_Dom_Node *n, Egueb_Dom_String *state)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;

	thiz = EON_WIDGET(egueb_dom_element_external_data_get(n));
	klass = EON_WIDGET_CLASS_GET(thiz);
	if (klass->state_set)
		klass->state_set(thiz, state);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_widget_enabled_set(Egueb_Dom_Node *n, Eina_Bool enabled)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->enabled, EGUEB_DOM_ATTR_TYPE_BASE, enabled);
}

EAPI Eina_Bool eon_widget_enabled_get(Egueb_Dom_Node *n)
{
	Eon_Widget *thiz;
	Eina_Bool ret;

	thiz = EON_WIDGET(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->enabled, &ret);
	return ret;
}
