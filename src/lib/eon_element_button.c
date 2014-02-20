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

/* TODO handle the expand, padding, margin or whatever other attr we decide to add */
static void _eon_element_button_child_solver(Eina_Rectangle *fs,
		Eon_Renderable_Size *size, int size_hints, Eina_Rectangle *out)
{
	int w = -1, h = -1;
	Eina_Bool w_set = EINA_FALSE;
	Eina_Bool h_set = EINA_FALSE;

	/* if we have a preferred hint, get the min difference getting that
	 * as a reference, also check that we are in the min/max range
	 */
	if (size_hints & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (size->pref_width > 0)
			w = size->pref_width;
		if (size->pref_height > 0)
			h = size->pref_height;
	}

	if (size_hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (w < 0)
		{
			w = size->min_width;
		}
		if (w > fs->w)
		{
			w = fs->w;
		}

		if (h < 0)
		{
			h = size->min_height;
		}
		if (h > fs->h)
		{
			h = fs->h;
		}
	}

	/* TODO handle halign, valign */
	/* for now center it */
	out->x = fs->x + ((fs->w - w) / 2);
	out->y = fs->y + ((fs->h - h) / 2);
	out->w = w;
	out->h = h;
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static int _eon_element_button_size_hints_get(Eon_Widget *w,
		Eon_Renderable_Size *size)
{
	Eon_Box padding;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(w))->n;
	child = egueb_dom_element_child_first_get(n);

	eon_drawer_button_padding_get(w->theme_widget, &padding);
	if (child)
	{
		int ret;

		/* in case it has a child get the size hints of the content */
		ret = eon_renderable_size_hints_get(child, size);
		egueb_dom_node_unref(child);
		if (ret & EON_RENDERABLE_HINT_MIN_MAX)
		{
			size->min_width += padding.left + padding.right;
			size->min_height += padding.top + padding.bottom;
			if (size->max_width > 0)
			{
				size->max_width += padding.left + padding.right;
			}
			if (size->max_height > 0)
			{
				size->max_height += padding.top + padding.bottom;
			}
		}

		if (ret & EON_RENDERABLE_HINT_PREFERRED)
		{
			if (size->pref_width > 0)
			{
				size->pref_width += padding.left + padding.right;
			}
			if (size->pref_height > 0)
			{
				size->pref_height += padding.top + padding.bottom;
			}

		}

		return ret;
	}
	else
	{
		size->min_width = padding.left + padding.right;
		size->min_height = padding.top + padding.bottom;
		size->max_width = -1;
		size->max_height = -1;

		return EON_RENDERABLE_HINT_MIN_MAX;
	}
}

static Eina_Bool _eon_element_button_process(Eon_Widget *w)
{
	Eon_Element_Button *thiz;
	Eon_Renderable_Size size;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Enesim_Renderer *r;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int size_hints;

	n = (EON_ELEMENT(w))->n;
	child = egueb_dom_element_child_first_get(n);
	if (!child)
	{
		eon_drawer_button_content_set(w->theme_widget, NULL);
		return EINA_TRUE;
	}

	thiz = EON_ELEMENT_BUTTON(w);
	/* Set the geometry on the child */
	eon_drawer_button_padding_get(w->theme_widget, &padding);
	free_space = (EON_RENDERABLE(w))->geometry;
	free_space.x += padding.left;
	free_space.y += padding.top;
	free_space.w -= padding.left + padding.right;
	free_space.h -= padding.bottom;

	ERR_ELEMENT(n, "Free space %" EINA_RECTANGLE_FORMAT, EINA_RECTANGLE_ARGS(&free_space));
	size_hints = eon_renderable_size_hints_get(child, &size);

	/* Our basic frame layout algorithm */
	_eon_element_button_child_solver(&free_space, &size, size_hints, &geometry);
	eon_renderable_geometry_set(child, &geometry);
	
	/* Set the content renderer */
	r = eon_renderable_renderer_get(child);
	eon_drawer_button_content_set(w->theme_widget, r);

	/* Finally process */
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

