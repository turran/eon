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
#include "eon_element_entry.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_entry_private.h"
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
	/* the unicode index of the current position of the cursor */
	int cursor;
	/* the offset in horizontal pixels */
	int offset;
} Eon_Element_Entry;

typedef struct _Eon_Element_Entry_Class
{
	Eon_Widget_Class base;
} Eon_Element_Entry_Class;


static void _eon_element_entry_focus_in_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Entry *thiz = data;
	Egueb_Dom_Node *theme_element;
	Enesim_Text_Buffer *buffer;

	/* set the cursor position at the end of the string */
	buffer = enesim_renderer_text_span_buffer_get(thiz->r);
	thiz->cursor = enesim_text_buffer_length_get(buffer);
	enesim_text_buffer_unref(buffer);

	theme_element = eon_feature_themable_load(thiz->theme_feature);
	eon_theme_element_entry_cursor_set(theme_element, thiz->cursor);
	egueb_dom_node_unref(theme_element);
}

static void _eon_element_entry_focus_out_cb(Egueb_Dom_Event *e,
		void *data)
{
}

static void _eon_element_entry_click_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Entry *thiz = data;
	Eon_Element *el;
	Eon_Renderable *r;
	Egueb_Dom_Node *theme_element;
	Egueb_Dom_Node *doc;
	Eon_Box padding = { 0, 0, 0, 0};
	Egueb_Dom_Event_Target *et;
	int x, y;
	int value = 0;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* Check the cursor position */
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	/* FIXME we should always have a theme */
	if (!theme_element)
		goto done;

	/* trigger the focus */
	el = EON_ELEMENT(thiz);
	doc = egueb_dom_node_owner_document_get(el->n);
	if (doc)
	{
		Egueb_Dom_Node *topmost;
		Egueb_Dom_Feature *feature;

		topmost = egueb_dom_document_document_element_get(doc);
		feature = egueb_dom_node_feature_get(topmost, EGUEB_DOM_FEATURE_UI_NAME, NULL);
		if (feature)
		{
			Egueb_Dom_Input *input;
			input = egueb_dom_feature_ui_input_get(feature);
			if (input)
			{
				egueb_dom_input_focus_set(input, egueb_dom_node_ref(el->n));
				egueb_dom_input_unref(input);
			}
			egueb_dom_feature_unref(feature);
		}
		egueb_dom_node_unref(doc);
		egueb_dom_node_unref(topmost);
	}
	r = EON_RENDERABLE(thiz);
	/* check that it is inside the text area */
	x = egueb_dom_event_mouse_client_x_get(e);
	y = egueb_dom_event_mouse_client_y_get(e);
	eon_theme_element_entry_padding_get(theme_element, &padding);
	if ((x >= r->geometry.x) && (x < (r->geometry.x + r->geometry.w)) &&	
			(y >= r->geometry.y) && (y < (r->geometry.y + r->geometry.h)))
	{
		int index;
		int start;
		int end;

		if (enesim_renderer_text_span_glyph_coord_at(thiz->r, x, y, &index, &start, &end))
		{
			//printf("index %d x %d y %d, start %d end %d\n", index, x, y, start, end);
		}
		else
		{
			index = -1;
		}
		/* TODO set the cursor position */
		/* TODO change the cursor type when mouse is over the text area */
		thiz->cursor = index;
	}
	egueb_dom_node_unref(theme_element);
done:
	return;
}

static void _eon_element_entry_key_down_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Entry *thiz = data;
	Egueb_Dom_String *key;
	Egueb_Dom_Node *theme_element;
	Enesim_Text_Buffer *buffer;

	key = egueb_dom_event_keyboard_key_get(e);
	buffer = enesim_renderer_text_span_buffer_get(thiz->r);

	theme_element = eon_feature_themable_load(thiz->theme_feature);
	/* check if it is printable */
	if (!egueb_dom_key_is_printable(key))
	{
		const char *skey = egueb_dom_string_string_get(key);
		if (!strcmp(skey, "BackSpace"))
		{
			enesim_text_buffer_string_delete(buffer,
					1, thiz->cursor);
			if (thiz->cursor > 0)
			{
				thiz->cursor--;
				eon_theme_element_entry_cursor_set(
						theme_element, thiz->cursor);
			}
		}
		else if (!strcmp(skey, "ArrowLeft"))
		{
			/* Move the cursor left, scroll if needed */
			if (thiz->cursor > 0)
			{
				thiz->cursor--;
				eon_theme_element_entry_cursor_set(
						theme_element, thiz->cursor);
			}
		}
		else if (!strcmp(skey, "ArrowRight"))
		{
			int len;

			len = enesim_text_buffer_length_get(buffer);
			/* Move the cursor right, scroll if needed */
			if (thiz->cursor < len)
			{
				thiz->cursor++;
				eon_theme_element_entry_cursor_set(
						theme_element, thiz->cursor);
			}
		}
	}
	else
	{
		enesim_text_buffer_string_insert(buffer,
				egueb_dom_string_string_get(key), -1, thiz->cursor);
		/* advance the cursor */
		if (thiz->cursor >= 0)
		{
			thiz->cursor++;
			eon_theme_element_entry_cursor_set(theme_element, thiz->cursor);
		}
	}
	egueb_dom_node_unref(theme_element);
	egueb_dom_string_unref(key);
	enesim_text_buffer_unref(buffer);
}

static Eina_Bool _eon_element_entry_mutation_get_text(
		Eon_Element_Entry *thiz,
		Egueb_Dom_Event *e, Egueb_Dom_Node **text)
{
	Egueb_Dom_Node *target = NULL;
	Egueb_Dom_Node *related = NULL;
	Egueb_Dom_Node_Type type;
	Eina_Bool ret = EINA_FALSE;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_CAPTURING)
		goto not_phase;

	related = egueb_dom_event_mutation_related_get(e);
	if (related != (EON_ELEMENT(thiz))->n)
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
	egueb_dom_node_unref(related);
not_phase:
	return ret;
}

static void _eon_element_entry_node_inserted_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Entry *thiz = data;
	Egueb_Dom_Node *text = NULL;

	if (_eon_element_entry_mutation_get_text(thiz, e, &text))
	{
		Enesim_Text_Buffer *nb = NULL;

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
	Eon_Element_Entry *thiz = data;
	Egueb_Dom_Node *text = NULL;

	if (_eon_element_entry_mutation_get_text(thiz, e, &text))
	{
		Enesim_Text_Buffer *nb;

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
			EGUEB_DOM_EVENT_FOCUS_IN,
			_eon_element_entry_focus_in_cb,
			EINA_TRUE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_FOCUS_OUT,
			_eon_element_entry_focus_out_cb,
			EINA_TRUE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_element_entry_click_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_KEY_DOWN,
			_eon_element_entry_key_down_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_entry_node_inserted_cb,
			EINA_TRUE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_entry_node_removed_cb,
			EINA_TRUE, thiz);
	/* private */
	thiz->offset = -1;
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
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}
	eon_theme_element_entry_text_renderer_set(theme_element,
			enesim_renderer_ref(thiz->r));
	ret = eon_theme_element_entry_size_hints_get(theme_element,
			size, -1);
	return ret;
}

static Eina_Bool _eon_element_entry_process(Eon_Renderable *r)
{
	Eon_Element_Entry *thiz;
	Eon_Widget *w;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_ENTRY(r);

	/* get the theme */
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		goto done;
	}

	/* Set the text renderer */
	eon_theme_element_entry_text_renderer_set(theme_element,
			enesim_renderer_ref(thiz->r));
	/* Set the geometry on the child */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);
	/* Set the enabled */
	w = EON_WIDGET(r);
	egueb_dom_attr_final_get(w->enabled, &enabled);
	eon_theme_widget_enabled_set(theme_element, enabled);

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

	/* only accept one child and of type text */
	if (egueb_dom_node_type_get(child) != EGUEB_DOM_NODE_TYPE_TEXT)
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

