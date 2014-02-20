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
#include "eon_element_label.h"

#include "eon_widget_private.h"
#include "eon_drawer_label_private.h"
/* Create our own text span renderer to pre-calculate the size of the ellipsized text
 * In acse the text of the character data has changed, be sure to notify upstream too
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_LABEL_DESCRIPTOR eon_element_label_descriptor_get()
#define EON_ELEMENT_LABEL_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,		\
		Eon_Element_Label_Class, EON_ELEMENT_LABEL_DESCRIPTOR)
#define EON_ELEMENT_LABEL(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Label, EON_ELEMENT_LABEL_DESCRIPTOR)

typedef struct _Eon_Element_Label
{
	Eon_Widget base;
	/* properties */
	Egueb_Dom_Node *ellipsize;
	/* private */
	/* the text span renderer from the drawer */
	Enesim_Renderer *r;
	/* the text buffer used in case we can ellipsize */
	Enesim_Text_Buffer *tb;
	Eina_Bool text_changed;
	Eina_Bool font_changed;
} Eon_Element_Label;

typedef struct _Eon_Element_Label_Class
{
	Eon_Widget_Class base;
} Eon_Element_Label_Class;

static void _eon_element_label_tree_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Label *thiz = data;
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
	thiz->text_changed = EINA_TRUE;
}

static void _eon_element_label_drawer_propagate(Eon_Widget *w)
{
	Eon_Element_Label *thiz;

	thiz = EON_ELEMENT_LABEL(w);
	if (thiz->text_changed)
	{
		Egueb_Dom_Node *n;
		Egueb_Dom_Node *child;
		Enesim_Text_Buffer *tb = NULL;

		n = ((EON_ELEMENT(thiz))->n);
		child = egueb_dom_node_child_first_get(n);
		if (child)
		{
			egueb_dom_character_data_buffer_get(child, &tb);
			enesim_renderer_text_span_real_buffer_set(thiz->r, tb);
			/* now make the text node to use our own buffer from now on */
#if 0
			tb = enesim_renderer_text_span_buffer_get(r);
			egueb_dom_character_data_buffer_set(child, tb);
#endif
			egueb_dom_node_unref(child);
		}
		else
		{
			enesim_renderer_text_span_real_buffer_set(thiz->r, NULL);
		}
		thiz->text_changed = EINA_FALSE;
	}

	if (thiz->font_changed)
	{
		/* Pass any font related attribute to the widget drawer */
		thiz->font_changed = EINA_FALSE;
	}
}

#if 0
static double _eon_theme_label_min_width_ellipsized_get(Eon_Element *ee)
{
	Enesim_Rectangle boundings;
	Eon_Theme_Label *thiz;
	const char *font;
	const char *text;
	char *ellipsized_text;
	int size;
	double min_width;

	thiz = _eon_theme_label_get(ee);
	if (!thiz->text)
		thiz->text = enesim_text_span_new();
	eon_theme_label_font_get(ee, &font);
	eon_theme_label_font_size_get(ee, &size);
	eon_theme_label_text_get(ee, &text);
	enesim_text_base_font_name_set(thiz->text, font);
	enesim_text_base_size_set(thiz->text, size);
	enesim_text_span_text_set(thiz->text, text);
	enesim_renderer_boundings(thiz->text, &boundings);
	min_width = boundings.w;

	/* set first letter and ellipsis "E..." as the minimum height
	 * if it is smaller than the text itself, use that
	 */
	ellipsized_text = malloc(sizeof(char) * 5);
	ellipsized_text[0] = text[0];
	ellipsized_text[1] = ellipsized_text[2] = ellipsized_text[3] = '.';
	ellipsized_text[4] = '\0';
	enesim_text_span_text_set(thiz->text, ellipsized_text);
	enesim_renderer_boundings(thiz->text, &boundings);
	free(ellipsized_text);

	return min_width < boundings.w ? min_width : boundings.w;
}
#endif
/*----------------------------------------------------------------------------*
 *                              Widget interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_element_label_theme_created(Eon_Widget *w)
{
	Eon_Element_Label *thiz;

	thiz = EON_ELEMENT_LABEL(w);
	/* Destroy our renderer copy */
	if (thiz->r)
	{
		enesim_renderer_unref(thiz->r);
	}
	thiz->r = eon_drawer_label_text_renderer_get(w->theme_widget);
	/* Get the new one */
	thiz->font_changed = EINA_TRUE;
	thiz->text_changed = EINA_TRUE;
}

static int _eon_element_label_size_hints_get(Eon_Widget *w,
		Eon_Renderable_Size *size)
{
	Eon_Element_Label *thiz;
	Eon_Size current;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *n;
	Enesim_Text_Font *tf;
	Enesim_Rectangle geom;
	Eina_Bool ellipsize;

	n = (EON_ELEMENT(w))->n;
	/* For an ellipsized text, the hints are the min size of the three dots
	 * width and always the font line height. Otherwise use the text node
	 * directly. Do not forget the text buffer to be shared
	 */
	_eon_element_label_drawer_propagate(w);
	thiz = EON_ELEMENT_LABEL(w);
	egueb_dom_attr_final_get(thiz->ellipsize, &ellipsize);
	ERR_ELEMENT(n, "Ellipsize %d", ellipsize);

	tf = eon_drawer_label_text_font_get(w->theme_widget);
	if (!tf)
	{
		ERR_ELEMENT(n, "Failed to create a font");
		return 0;
	}

	enesim_renderer_shape_geometry_get(thiz->r, &geom);
	size->min_width = geom.w;
	size->min_height = geom.h;
	size->min_width = geom.w;
	size->max_height = geom.h;
	size->pref_width = geom.w;
	size->pref_height = geom.h;
	return EON_RENDERABLE_HINT_MIN_MAX | EON_RENDERABLE_HINT_PREFERRED;
}


static Eina_Bool _eon_element_label_process(Eon_Widget *w)
{
	Eon_Element_Label *thiz;

	thiz = EON_ELEMENT_LABEL(w);
	/* set the correct text buffer, text font, text color */
	printf("process label\n");

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                            Renderable interface                            *
 *----------------------------------------------------------------------------*/
static void _eon_element_label_init(Eon_Renderable *r)
{
	Eon_Element_Label *thiz;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_LABEL(r);
	thiz->ellipsize = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_ATTR_ELLIPSIZE), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->ellipsize, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	
	n = (EON_ELEMENT(r))->n;
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->ellipsize), NULL);
	/* attributes */
	/* TODO add:
	 * font family
	 * font size
	 * font weight
	 * font style
	 * font variant
	 */
	
	/* events */
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_label_tree_modified_cb,
			EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_label_tree_modified_cb,
			EINA_FALSE, r);
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_label_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_LABEL);
}

static Eina_Bool _eon_element_label_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *our_child;

	/* only accept one child and of type text */
	if (egueb_dom_node_type_get(child) != EGUEB_DOM_NODE_TYPE_TEXT_NODE)
		return EINA_FALSE;

	/* check if we already have one child */
	n = e->n;
	our_child = egueb_dom_element_child_first_get(n);
	if (our_child)
	{
		WARN_ELEMENT(n, "Only one child supported");
		egueb_dom_node_unref(our_child);
		return EINA_FALSE;
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Label,
		Eon_Element_Label_Class, eon_element_label);

static void _eon_element_label_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_label_tag_name_get;
	klass->child_appendable = _eon_element_label_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_label_init;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->theme_created = _eon_element_label_theme_created;
	w_klass->size_hints_get = _eon_element_label_size_hints_get;
	w_klass->process = _eon_element_label_process;
}

static void _eon_element_label_instance_init(void *o)
{
}

static void _eon_element_label_instance_deinit(void *o)
{
	Eon_Element_Label *thiz;

	thiz = EON_ELEMENT_LABEL(o);
	egueb_dom_node_unref(thiz->ellipsize);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_label_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_label);
	return n;
}

/**
 * Sets the ellipsize flag of a label
 * @param[in] n The label element to set the ellipsize
 * @param[in] e EINA_TRUE to ellipsize, EINA_FALSE otherswise
 */
EAPI void eon_element_label_ellipsize_set(Egueb_Dom_Node *n, Eina_Bool e)
{
	Eon_Element_Label *thiz;

	thiz = EON_ELEMENT_LABEL(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->ellipsize, EGUEB_DOM_ATTR_TYPE_BASE, e);
}
