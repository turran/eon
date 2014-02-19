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
#include "eon_element_button.h"

#include "eon_widget_private.h"
#include "eon_drawer_button_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_BUTTON_DESCRIPTOR eon_element_button_descriptor_get()
#define EON_ELEMENT_BUTTON_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,		\
		Eon_Element_Button_Class, EON_ELEMENT_BUTTON_DESCRIPTOR)
#define EON_ELEMENT_BUTTON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Button, EON_ELEMENT_BUTTON_DESCRIPTOR)

typedef struct _Eon_Element_Button
{
	Eon_Widget base;
} Eon_Element_Button;

typedef struct _Eon_Element_Button_Class
{
	Eon_Widget_Class base;
} Eon_Element_Button_Class;
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static int _eon_element_button_size_hints_get(Eon_Widget *w,
		Eon_Renderable_Size *size)
{
	Eon_Size min;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(w))->n;
	child = egueb_dom_element_child_first_get(n);
	
	if (child)
	{
		int ret;

		/* in case it has a child get the size hints of the content */
		ret = eon_renderable_size_hints_get(child, size);
		egueb_dom_node_unref(child);
		return ret;
	}
	else
	{
		/* otherwise get the min size from the theme */
		eon_drawer_button_min_size_get(w->theme_widget, &min);
		size->min_width = min.width;
		size->min_height = min.height;
		size->max_width = -1;
		size->max_height = -1;

		return EON_SIZE_HINT_MIN_MAX;
	}
}

static Eina_Bool _eon_element_button_process(Eon_Widget *w)
{
	Eon_Element_Button *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;

	n = (EON_ELEMENT(w))->n;
	child = egueb_dom_element_child_first_get(n);
	if (!child) return EINA_TRUE;

	thiz = EON_ELEMENT_BUTTON(w);
	printf("processing>>>>\n");
	/* Se tthe geometry and process */
	eon_renderable_geometry_set(child, &((EON_RENDERABLE(w)))->geometry);
	egueb_dom_element_process(child);
	egueb_dom_node_unref(child);
	
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_button_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_BUTTON);
}

static Eina_Bool _eon_element_button_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
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
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Button,
		Eon_Element_Button_Class, eon_element_button);

static void _eon_element_button_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_button_tag_name_get;
	klass->child_appendable = _eon_element_button_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->size_hints_get = _eon_element_button_size_hints_get;
	w_klass->process = _eon_element_button_process;
}

static void _eon_element_button_instance_init(void *o)
{
}

static void _eon_element_button_instance_deinit(void *o)
{
	Eon_Element_Button *thiz;

	thiz = EON_ELEMENT_BUTTON(o);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_button_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_button);
	return n;
}

