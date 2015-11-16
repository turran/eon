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
#include "eon_element_entry.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_button_private.h"
#include "eon_event_activate_private.h"
#include "eon_event_deactivate_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_ENTRY_DESCRIPTOR eon_element_entry_descriptor_get()
#define EON_ELEMENT_ENTRY(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Entry, EON_ELEMENT_ENTRY_DESCRIPTOR)

typedef struct _Eon_Element_Entry
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *default_value;
	Egueb_Dom_Node *text_align;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *r;
	Enesim_Text_Buffer *buffer;
} Eon_Element_Entry;

typedef struct _Eon_Element_Entry_Class
{
	Eon_Widget_Class base;
} Eon_Element_Entry_Class;

static void _eon_element_entry_click_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Entry *thiz = data;
	Egueb_Dom_Event_Target *et;
	int value = 0;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* TODO trigger the focus */
}

static Eina_Bool _eon_element_entry_mutation_get_text(Egueb_Dom_Event *e,
		Egueb_Dom_Node *us, Egueb_Dom_Node **text)
{
	Egueb_Dom_Node *target = NULL;
	Egueb_Dom_Node *related = NULL;
	Egueb_Dom_Node_Type type;
	Eina_Bool ret = EINA_FALSE;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		goto not_us;

	target = EGUEB_DOM_NODE(egueb_dom_event_target_get(e));
	type = egueb_dom_node_type_get(target);
	if (type != EGUEB_DOM_NODE_TYPE_TEXT)
		goto not_text;

	*text = egueb_dom_node_ref(target);
	ret = EINA_TRUE;
not_text:
	egueb_dom_node_unref(target);
not_us:
	return ret;
}

static void _eon_element_entry_node_inserted_cb(Egueb_Dom_Event *e,
		void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *text = NULL;

	if (_eon_element_entry_mutation_get_text(e, n, &text))
	{
		Eon_Element_Entry *thiz;
		Enesim_Text_Buffer *nb = NULL;

		thiz = EON_ELEMENT_ENTRY(egueb_dom_element_external_data_get(n));
		/* set the internal buffer of the entry to be the one
		 * on the text node */
		nb = egueb_dom_character_data_buffer_get(text);
		enesim_renderer_text_span_real_buffer_set(thiz->r, nb);
		/* now make the text node to use our own buffer from now on */
		nb = enesim_renderer_text_span_buffer_get(thiz->r);
		egueb_dom_character_data_buffer_set(text, nb);
		egueb_dom_node_unref(text);
	}
}

static void _eon_element_entry_node_removed_cb(Egueb_Dom_Event *e,
		void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *text = NULL;

	if (_eon_element_entry_mutation_get_text(e, n, &text))
	{
		Eon_Element_Entry *thiz;
		Enesim_Text_Buffer *nb;

		thiz = EON_ELEMENT_ENTRY(egueb_dom_element_external_data_get(n));
		nb = enesim_renderer_text_span_real_buffer_get(thiz->r);
		egueb_dom_character_data_buffer_set(text, nb);
		egueb_dom_node_unref(text);
	}
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_entry_init(Eon_Widget *w)
{
	Eon_Element_Entry *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_ENTRY(w);

	/* attributes */
	thiz->default_value = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EON_NAME_ATTR_DEFAULT_VALUE),
			NULL, NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	n = (EON_ELEMENT(w))->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->default_value), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_element_entry_click_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_entry_node_inserted_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_entry_node_removed_cb,
			EINA_FALSE, thiz);
	/* private */
	thiz->r = enesim_renderer_text_span_new();
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_ENTRY));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_entry_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Entry *thiz;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;

	thiz = EON_ELEMENT_ENTRY(r);
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

static int _eon_element_entry_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Entry *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	int ret = 0;

	n = (EON_ELEMENT(r))->n;

	thiz = EON_ELEMENT_ENTRY(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	eon_theme_element_button_padding_get(theme_element, &padding);
	/* an entry can be of any size? */
	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	if (size->min_width >= 0)
		size->min_width += padding.left + padding.right;
	if (size->min_height >= 0)
		size->min_height += padding.top + padding.bottom;
	size->max_width = -1;
	size->max_height = -1;

	/* get the font/color attributes of the entry text */
	/* get the size of the input to be the preferred size */

	if (ret & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (size->pref_width > 0)
			size->pref_width += padding.left + padding.right;
		if (size->pref_height > 0)
			size->pref_height += padding.top + padding.bottom;
	}

	return ret;
}

static Eina_Bool _eon_element_entry_process(Eon_Renderable *r)
{
	Eon_Element_Entry *thiz;
	Eon_Widget *w;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_ENTRY(r);

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
	/* Set the default text */
	/* Set the text renderer */
	eon_theme_element_button_content_set(theme_element, NULL);

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

done:

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_entry_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_ENTRY);
}

static Eina_Bool _eon_element_entry_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
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
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Entry,
		Eon_Element_Entry_Class, eon_element_entry);

static void _eon_element_entry_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_entry_tag_name_get;
	klass->child_appendable = _eon_element_entry_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_entry_renderer_get;
	r_klass->size_hints_get = _eon_element_entry_size_hints_get;
	r_klass->process = _eon_element_entry_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_entry_init;
}

static void _eon_element_entry_instance_init(void *o)
{
}

static void _eon_element_entry_instance_deinit(void *o)
{
	Eon_Element_Entry *thiz;

	thiz = EON_ELEMENT_ENTRY(o);
	/* attributes */
	egueb_dom_node_unref(thiz->default_value);
	/* private */
	enesim_renderer_unref(thiz->r);
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_entry_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_entry);
	return n;
}

