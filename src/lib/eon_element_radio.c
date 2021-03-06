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
#include "eon_element_radio.h"

#include "eon_element_radio_private.h"
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
#define EON_ELEMENT_RADIO_DESCRIPTOR eon_element_radio_descriptor_get()
#define EON_ELEMENT_RADIO(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Radio, EON_ELEMENT_RADIO_DESCRIPTOR)

typedef struct _Eon_Element_Radio
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *group;
	Egueb_Dom_Node *activated;
	/* private */
	Egueb_Dom_String *prev_group;
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *proxy;
	Eina_Bool first_run;
} Eon_Element_Radio;

typedef struct _Eon_Element_Radio_Class
{
	Eon_Widget_Class base;
} Eon_Element_Radio_Class;

typedef struct _Eon_Element_Radio_Group
{
	Egueb_Dom_Node *current;
	int count;
} Eon_Element_Radio_Group;

/* TODO handle the deletion of the hash */
/* Used to keep track of every group */
static Eina_Hash *_groups = NULL;

static void _eon_element_radio_group_free(Eon_Element_Radio_Group *g)
{
	free(g);
}

static void _eon_element_radio_group_add(Eon_Element_Radio *thiz)
{
	Eon_Element_Radio_Group *g;
	Egueb_Dom_String *group;
	const char *name;

	egueb_dom_attr_final_get(thiz->group, &group);
	egueb_dom_string_unref(thiz->prev_group);
	thiz->prev_group = egueb_dom_string_dup(group);

	name = egueb_dom_string_chars_get(group);
	if (!name)
	{
		egueb_dom_string_unref(group);
		return;
	}

	DBG("Adding group '%s'", name);
	g = eina_hash_find(_groups, name);
	if (!g)
	{
		g = calloc(1, sizeof(Eon_Element_Radio));
		eina_hash_add(_groups, name, g);
	}
	g->count++;
	egueb_dom_string_unref(group);
}

static void _eon_element_radio_group_remove(Eon_Element_Radio *thiz)
{
	Eon_Element_Radio_Group *g;
	const char *name;

	name = egueb_dom_string_chars_get(thiz->prev_group);
	if (!name)
		return;

	DBG("Removing group '%s'", name);
	g = eina_hash_find(_groups, name);
	if (g)
	{
		Egueb_Dom_Node *n;

		n = (EON_ELEMENT(thiz))->n;
		/* do not activate anything and deactivate ourselves */
		if (g->current == n)
		{
			Egueb_Dom_Event *deactivate;
			Egueb_Dom_Event_Target *et;

			deactivate = eon_event_deactivate_new();
			et = EGUEB_DOM_EVENT_TARGET(n);
			egueb_dom_event_target_event_dispatch(
					EGUEB_DOM_EVENT_TARGET(et),
					deactivate, NULL, NULL);
			g->current = NULL;
		}
		g->count--;
		if (!g->count)
		{
			eina_hash_del(_groups, name, g);
			_eon_element_radio_group_free(g);
		}
	}
}

static void _eon_element_radio_dispatch(
		Eon_Element_Radio *thiz, Eina_Bool activated)
{
	Egueb_Dom_Event_Target *et;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event *ev;

	n = (EON_ELEMENT(thiz))->n;
	et = EGUEB_DOM_EVENT_TARGET(n);
	if (activated)
	{
		Eon_Element_Radio_Group *g;
		Egueb_Dom_String *group;

		ev = eon_event_activate_new();
		/* Get the group this radio belongs to and deactivate the
		 * current activated item
		 */
		egueb_dom_attr_final_get(thiz->group, &group);
		g = eina_hash_find(_groups, egueb_dom_string_chars_get(group));
		if (g)
		{
 			if (g->current)
			{
				Eon_Element_Radio *current;

				current = EON_ELEMENT_RADIO(
						egueb_dom_element_external_data_get(g->current));
				egueb_dom_attr_set(current->activated,
						EGUEB_DOM_ATTR_TYPE_BASE,
						EINA_FALSE);
			}
			g->current = n;
		}
		egueb_dom_string_unref(group);
	}
	else
	{
		ev = eon_event_deactivate_new();
	}
	egueb_dom_event_target_event_dispatch(et, ev, NULL, NULL);
}

static void _eon_element_radio_attr_modified_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Element_Radio *thiz = data;
	Egueb_Dom_Node *attr;

	/* check if we are at the target */
	if (egueb_dom_event_phase_get(ev) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* check if the attribute is the group or the activated */
	attr = egueb_dom_event_mutation_related_get(ev);
	if (thiz->group == attr)
	{
		/* in case of the group, make sure to register to the new
		 * group and unregister from the old one
		 */
		_eon_element_radio_group_remove(thiz);
		_eon_element_radio_group_add(thiz);
	}
	else if (thiz->activated == attr)
	{
		const Egueb_Dom_Value *v;

		/* trigger the events */
		egueb_dom_event_mutation_value_new_get(ev, &v);
		_eon_element_radio_dispatch(thiz, v->data.i32);
	}
	egueb_dom_node_unref(attr);
}

static void _eon_element_radio_click_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Radio *thiz = data;
	int activated = 0;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* first set the new value */
	egueb_dom_attr_final_get(thiz->activated, &activated);
	if (activated)
		return;

	egueb_dom_attr_set(thiz->activated, EGUEB_DOM_ATTR_TYPE_BASE,
			EINA_TRUE);
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_radio_init(Eon_Widget *w)
{
	Eon_Element_Radio *thiz;
	Eon_Element *e;
	Egueb_Dom_Event_Target *et;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_RADIO(w);

	/* attributes */
	thiz->group = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EON_NAME_ATTR_GROUP), NULL,
			NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	thiz->activated = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_ATTR_ACTIVATED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->activated, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	n = (EON_ELEMENT(w))->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->group), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->activated), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_element_radio_click_cb, EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_radio_attr_modified_cb, EINA_FALSE, thiz);
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
			egueb_dom_string_ref(EON_NAME_ELEMENT_RADIO));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_radio_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Radio *thiz;

	thiz = EON_ELEMENT_RADIO(r);
	return enesim_renderer_ref(thiz->proxy);
}

static int _eon_element_radio_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Radio *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int ret;

	/* get the hints of the content */
	n = (EON_ELEMENT(r))->n;
	ret = eon_layout_frame_size_hints_get(n, size);

	/* get the padding of the theme */
	thiz = EON_ELEMENT_RADIO(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}
	eon_theme_element_button_padding_get(theme_element, &padding);

	/* a radio can be of any size */
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
	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_radio_process(Eon_Renderable *r)
{
	Eon_Element_Radio *thiz;
	Eon_Widget *w;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_RADIO(r);

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
		_eon_element_radio_dispatch(thiz, activated);
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
static Egueb_Dom_String * _eon_element_radio_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_RADIO);
}

static Eina_Bool _eon_element_radio_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
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
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Radio,
		Eon_Element_Radio_Class, eon_element_radio);

static void _eon_element_radio_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_radio_tag_name_get;
	klass->child_appendable = _eon_element_radio_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_radio_renderer_get;
	r_klass->size_hints_get = _eon_element_radio_size_hints_get;
	r_klass->process = _eon_element_radio_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_radio_init;
}

static void _eon_element_radio_instance_init(void *o)
{
}

static void _eon_element_radio_instance_deinit(void *o)
{
	Eon_Element_Radio *thiz;

	thiz = EON_ELEMENT_RADIO(o);
	/* attributes */
	egueb_dom_node_unref(thiz->group);
	egueb_dom_node_unref(thiz->activated);
	/* private */
	egueb_dom_string_unref(thiz->prev_group);
	enesim_renderer_unref(thiz->proxy);
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_radio_init(void)
{
	_groups = eina_hash_string_superfast_new(
			EINA_FREE_CB(_eon_element_radio_group_free));
}

void eon_element_radio_shutdown(void)
{
	eina_hash_free(_groups);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_radio_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_radio);
	return n;
}

