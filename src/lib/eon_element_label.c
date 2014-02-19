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
} Eon_Element_Label;

typedef struct _Eon_Element_Label_Class
{
	Eon_Widget_Class base;
} Eon_Element_Label_Class;

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
static int _eon_element_label_size_hints_get(Eon_Widget *w,
		Eon_Renderable_Size *size)
{
	Eon_Size min;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(w))->n;
	/* For an ellipsized text, the hints are the min size of the three dots
	 * width and always the font line height. Otherwise use the text node
	 * directly. Do not forget the text buffer to be shared
	 */
#if 0
	eon_theme_label_size_get(theme->object, &size);
	hints->min = size;
	hints->max = size;
	hints->preferred = size;
#endif
}


static Eina_Bool _eon_element_label_process(Eon_Widget *w)
{
	Eon_Element_Label *thiz;

	thiz = EON_ELEMENT_LABEL(w);
	/* set the correct text buffer */
	printf("process label\n");

	return EINA_TRUE;
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

	w_klass = EON_WIDGET_CLASS(k);
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
