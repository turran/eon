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
#include "eon_element_radio.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_layout_frame_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_RADIO_DESCRIPTOR eon_element_radio_descriptor_get()
#define EON_ELEMENT_RADIO(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Radio, EON_ELEMENT_RADIO_DESCRIPTOR)

typedef struct _Eon_Element_Radio
{
	Eon_Widget base;
	Egueb_Dom_Feature *theme_feature;
} Eon_Element_Radio;

typedef struct _Eon_Element_Radio_Class
{
	Eon_Widget_Class base;
} Eon_Element_Radio_Class;

#if 0
static int _eon_element_radio_size_hints_get(Eon_Widget *w,
		Eon_Renderable_Size *size)
{
	Eon_Box padding;
	Egueb_Dom_Node *n;
	int ret;

	n = (EON_ELEMENT(w))->n;

	ret = eon_layout_frame_size_hints_get(n, size);

	/* finally add our padding */
	eon_drawer_radio_padding_get(w->theme_widget, &padding);
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

	return ret;
}

static Eina_Bool _eon_element_radio_process(Eon_Widget *w)
{
	Eon_Element_Radio *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Enesim_Renderer *r;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;

	n = (EON_ELEMENT(w))->n;
	child = egueb_dom_element_child_first_get(n);
	if (!child)
	{
		eon_drawer_radio_content_set(w->theme_widget, NULL);
		return EINA_TRUE;
	}

	thiz = EON_ELEMENT_RADIO(w);
	/* Set the geometry on the child */
	eon_drawer_radio_padding_get(w->theme_widget, &padding);
	free_space = (EON_RENDERABLE(w))->geometry;
	free_space.x += padding.left;
	free_space.y += padding.top;
	free_space.w -= padding.left + padding.right;
	free_space.h -= padding.bottom + padding.top;

	DBG_ELEMENT(n, "Free space %" EINA_RECTANGLE_FORMAT, EINA_RECTANGLE_ARGS(&free_space));

	/* Set the content renderer */
	r = eon_renderable_renderer_get(child);
	eon_drawer_radio_content_set(w->theme_widget, r);
	egueb_dom_node_unref(child);

	/* Our basic frame layout algorithm */
	eon_layout_frame_size_geometry_set(n, &free_space);
	
	return EINA_TRUE;
}
#endif
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_radio_init(Eon_Widget *w)
{
	Eon_Element_Radio *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_RADIO(w);

	/* attributes */
	n = (EON_ELEMENT(w))->n;
	/* events */
	/* private */
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_RADIO));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
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

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_radio_init;
#if 0
	w_klass->size_hints_get = _eon_element_radio_size_hints_get;
	w_klass->process = _eon_element_radio_process;
#endif
}

static void _eon_element_radio_instance_init(void *o)
{
}

static void _eon_element_radio_instance_deinit(void *o)
{
	Eon_Element_Radio *thiz;

	thiz = EON_ELEMENT_RADIO(o);
	/* attributes */
	/* private */
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_radio_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_radio);
	return n;
}

