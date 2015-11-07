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
#include "eon_element_checkbox.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_button_private.h"
#include "eon_layout_frame_private.h"
#include "eon_event_activate_private.h"
#include "eon_event_deactivate_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_CHECKBOX_DESCRIPTOR eon_element_checkbox_descriptor_get()
#define EON_ELEMENT_CHECKBOX(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Checkbox, EON_ELEMENT_CHECKBOX_DESCRIPTOR)

typedef struct _Eon_Element_Checkbox
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *activated;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Eina_Bool first_run;
} Eon_Element_Checkbox;

typedef struct _Eon_Element_Checkbox_Class
{
	Eon_Widget_Class base;
} Eon_Element_Checkbox_Class;

static void _eon_element_checkbox_dispatch(
		Eon_Element_Checkbox *thiz, Eina_Bool activated)
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

static void _eon_element_checkbox_click_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Checkbox *thiz = data;
	int activated = 0;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* first set the new value */
	egueb_dom_attr_final_get(thiz->activated, &activated);
	egueb_dom_attr_set(thiz->activated, EGUEB_DOM_ATTR_TYPE_BASE, !activated);
	/* trigger the events */
	_eon_element_checkbox_dispatch(thiz, !activated);
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_checkbox_init(Eon_Widget *w)
{
	Eon_Element_Checkbox *thiz;
	Eon_Element *e;
	Egueb_Dom_Event_Target *et;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_CHECKBOX(w);

	/* attributes */
	thiz->activated = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_ATTR_ACTIVATED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->activated, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	n = (EON_ELEMENT(w))->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->activated), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_element_checkbox_click_cb,
			EINA_FALSE, thiz);
	/* private */
	thiz->first_run = EINA_TRUE;
	thiz->theme_feature = eon_feature_themable_add(n);
	eon_feature_themable_event_propagate(thiz->theme_feature,
			EON_NAME_EVENT_ACTIVATE);
	eon_feature_themable_event_propagate(thiz->theme_feature,
			EON_NAME_EVENT_DEACTIVATE);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_CHECKBOX));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_checkbox_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Checkbox *thiz;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;

	thiz = EON_ELEMENT_CHECKBOX(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return NULL;
	}
	ren = eon_theme_renderable_renderer_get(theme_element);
	egueb_dom_node_unref(theme_element);
	return ren;
}

static int _eon_element_checkbox_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Checkbox *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	int ret;

	/* get the hints of the content */
	n = (EON_ELEMENT(r))->n;
	ret = eon_layout_frame_size_hints_get(n, size);

	/* get the padding of the theme */
	thiz = EON_ELEMENT_CHECKBOX(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}
	eon_theme_element_button_padding_get(theme_element, &padding);

	/* a checkbox can be of any size */
	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	if (size->min_width > 0)
		size->min_width += padding.left + padding.right;
	if (size->min_height > 0)
		size->min_height += padding.top + padding.bottom;
	size->max_width = -1;
	size->max_height = -1;

	if (ret & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (size->pref_width > 0)
			size->pref_width += padding.left + padding.right;
		if (size->pref_height > 0)
			size->pref_height += padding.top + padding.bottom;
	}

	return ret;
}

static Eina_Bool _eon_element_checkbox_process(Eon_Renderable *r)
{
	Eon_Element_Checkbox *thiz;
	Eon_Widget *w;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_CHECKBOX(r);

	free_space = r->geometry;

	/* get the theme */
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		goto done;
	}

	/* Set the geometry on the child */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);
	/* Set the enabled */
	w = EON_WIDGET(r);
	egueb_dom_attr_final_get(w->enabled, &enabled);
	eon_theme_widget_enabled_set(theme_element, enabled);

	/* finally add our padding */
	eon_theme_element_button_padding_get(theme_element, &padding);
	free_space.x += padding.left;
	free_space.y += padding.top;
	free_space.w -= padding.left + padding.right;
	free_space.h -= padding.bottom + padding.top;

	DBG_ELEMENT(n, "Free space %" EINA_RECTANGLE_FORMAT, EINA_RECTANGLE_ARGS(&free_space));
	/* Set the content renderer */
	child = egueb_dom_element_child_first_get(n);
	if (child)
	{
		Enesim_Renderer *ren;

		ren = eon_renderable_renderer_get(child);
		eon_theme_element_button_content_set(theme_element, ren);
		egueb_dom_node_unref(child);
	}
	else
	{
		eon_theme_element_button_content_set(theme_element, NULL);
	}

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	/* Trigger the initial attribute signal in case we haven't done it yet */
	if (thiz->first_run)
	{
		int activated;

		egueb_dom_attr_final_get(thiz->activated, &activated);
		_eon_element_checkbox_dispatch(thiz, activated);
		thiz->first_run = EINA_FALSE;
	}
	egueb_dom_node_unref(theme_element);

done:
	/* Our basic frame layout algorithm */
	eon_layout_frame_size_geometry_set(n, &free_space);
	
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_checkbox_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_CHECKBOX);
}

static Eina_Bool _eon_element_checkbox_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
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
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Checkbox,
		Eon_Element_Checkbox_Class, eon_element_checkbox);

static void _eon_element_checkbox_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_checkbox_tag_name_get;
	klass->child_appendable = _eon_element_checkbox_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_checkbox_renderer_get;
	r_klass->size_hints_get = _eon_element_checkbox_size_hints_get;
	r_klass->process = _eon_element_checkbox_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_checkbox_init;
}

static void _eon_element_checkbox_instance_init(void *o)
{
}

static void _eon_element_checkbox_instance_deinit(void *o)
{
	Eon_Element_Checkbox *thiz;

	thiz = EON_ELEMENT_CHECKBOX(o);
	/* attributes */
	egueb_dom_node_unref(thiz->activated);
	/* private */
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_checkbox_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_checkbox);
	return n;
}

