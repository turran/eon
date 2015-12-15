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
#include "eon_element_switch.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_switch_private.h"
#include "eon_event_activate_private.h"
#include "eon_event_deactivate_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_SWITCH_DESCRIPTOR eon_element_switch_descriptor_get()
#define EON_ELEMENT_SWITCH(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Switch, EON_ELEMENT_SWITCH_DESCRIPTOR)

typedef struct _Eon_Element_Switch
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *active;
	Egueb_Dom_Node *inactive;
	Egueb_Dom_Node *activated;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *proxy;
	Eina_Bool first_run;
} Eon_Element_Switch;

typedef struct _Eon_Element_Switch_Class
{
	Eon_Widget_Class base;
} Eon_Element_Switch_Class;

static void _eon_element_switch_dispatch(
		Eon_Element_Switch *thiz, Eina_Bool activated)
{
	Egueb_Dom_Event_Target *et;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event *ev;

	n = (EON_ELEMENT(thiz))->n;
	et = EGUEB_DOM_EVENT_TARGET(n);
	if (activated)
	{
		ev = eon_event_activate_new();
	}
	else
	{
		ev = eon_event_deactivate_new();
	}
	egueb_dom_event_target_event_dispatch(et, ev, NULL, NULL);
}

/* Inform of a geometry change */
static void _eon_element_switch_attr_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Switch *thiz = data;
	Egueb_Dom_Node *attr;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;

	attr = egueb_dom_event_mutation_related_get(e);
	if (thiz->active == attr || thiz->inactive == attr)
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Geometry related attr changed, "
				"invalidating geometry");
		eon_renderable_invalidate_geometry((EON_ELEMENT(thiz))->n);
	}
	egueb_dom_node_unref(attr);
}

static void _eon_element_switch_click_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Switch *thiz = data;
	int activated = 0;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* first set the new value */
	egueb_dom_attr_final_get(thiz->activated, &activated);
	egueb_dom_attr_set(thiz->activated, EGUEB_DOM_ATTR_TYPE_BASE, !activated);
	/* trigger the events */
	_eon_element_switch_dispatch(thiz, !activated);
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_switch_init(Eon_Widget *w)
{
	Eon_Element_Switch *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_SWITCH(w);

	/* attributes */
	thiz->active = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EON_NAME_ATTR_ACTIVE), NULL,
			egueb_dom_string_ref(EON_NAME_ON),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	thiz->inactive = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EON_NAME_ATTR_INACTIVE), NULL,
			egueb_dom_string_ref(EON_NAME_OFF),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	thiz->activated = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_ATTR_ACTIVATED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->activated, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	n = (EON_ELEMENT(w))->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->active), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->inactive), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->activated), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_switch_attr_modified_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_element_switch_click_cb,
			EINA_FALSE, thiz);
	/* private */
	thiz->first_run = EINA_TRUE;
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_feature = eon_feature_themable_add(n);
	eon_feature_themable_event_propagate(thiz->theme_feature,
			EON_NAME_EVENT_ACTIVATE);
	eon_feature_themable_event_propagate(thiz->theme_feature,
			EON_NAME_EVENT_DEACTIVATE);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_SWITCH));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_switch_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Switch *thiz;

	thiz = EON_ELEMENT_SWITCH(r);
	return enesim_renderer_ref(thiz->proxy);
}

static int _eon_element_switch_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Switch *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int ret;

	thiz = EON_ELEMENT_SWITCH(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}

	/* propagate the active/inactive values if needed */
	if (egueb_dom_attr_has_changed(thiz->active))
	{
		Egueb_Dom_String *s = NULL;
		egueb_dom_attr_final_get(thiz->active, &s);
		eon_theme_element_switch_active_text_set(theme_element, s);
	}
	if (egueb_dom_attr_has_changed(thiz->inactive))
	{
		Egueb_Dom_String *s = NULL;
		egueb_dom_attr_final_get(thiz->inactive, &s);
		eon_theme_element_switch_inactive_text_set(theme_element, s);
	}

	n = (EON_ELEMENT(r))->n;
	ret = eon_theme_element_switch_size_hints_get(theme_element, size);
	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_switch_process(Eon_Renderable *r)
{
	Eon_Element_Switch *thiz;
	Eon_Widget *w;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_SWITCH(r);

	free_space = r->geometry;

	/* get the theme */
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}

	/* Set the geometry on the child */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);
	/* Set the enabled */
	w = EON_WIDGET(r);
	egueb_dom_attr_final_get(w->enabled, &enabled);
	eon_theme_widget_enabled_set(theme_element, enabled);

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	/* Trigger the initial attribute signal in case we haven't done it yet */
	if (thiz->first_run)
	{
		int activated;

		egueb_dom_attr_final_get(thiz->activated, &activated);
		_eon_element_switch_dispatch(thiz, activated);
		thiz->first_run = EINA_FALSE;
	}
	egueb_dom_node_unref(theme_element);

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_switch_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_SWITCH);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Switch,
		Eon_Element_Switch_Class, eon_element_switch);

static void _eon_element_switch_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_switch_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_switch_renderer_get;
	r_klass->size_hints_get = _eon_element_switch_size_hints_get;
	r_klass->process = _eon_element_switch_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_switch_init;
}

static void _eon_element_switch_instance_init(void *o)
{
}

static void _eon_element_switch_instance_deinit(void *o)
{
	Eon_Element_Switch *thiz;

	thiz = EON_ELEMENT_SWITCH(o);
	/* attributes */
	egueb_dom_node_unref(thiz->active);
	egueb_dom_node_unref(thiz->inactive);
	egueb_dom_node_unref(thiz->activated);
	/* private */
	enesim_renderer_unref(thiz->proxy);
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_switch_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_switch);
	return n;
}
