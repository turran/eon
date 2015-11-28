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
#include "eon_element_notebook.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_notebook_private.h"
#include "eon_event_activate_private.h"
#include "eon_event_deactivate_private.h"

/*
 * The notebook should behave in several different ways based on several
 * attributes:
 * - tabs-closable: true/false to add a close button on each tab
 * what if should be visible only in the tab selected? only on the tab over the mouse?
 * - show-arrows: true/false to show arrows to move around the tabs
 * the size hints then should change, not fit to the geometry set on the norebook
 * but be the size of the preferred size?
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_NOTEBOOK_DESCRIPTOR eon_element_notebook_descriptor_get()
#define EON_ELEMENT_NOTEBOOK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Notebook, EON_ELEMENT_NOTEBOOK_DESCRIPTOR)

typedef struct _Eon_Element_Notebook
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *title;
	/* private */
	Egueb_Dom_Feature *theme_feature;
} Eon_Element_Notebook;

typedef struct _Eon_Element_Notebook_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Notebook_Class;

/* Inform of a geometry change */
static void _eon_element_notebook_attr_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Notebook *thiz = data;
	Egueb_Dom_Node *attr;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;

	attr = egueb_dom_event_mutation_related_get(e);
	if (thiz->title == attr)
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Geometry related attr changed, "
				"invalidating geometry");
		eon_renderable_invalidate_geometry((EON_ELEMENT(thiz))->n);
	}
	egueb_dom_node_unref(attr);
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_notebook_init(Eon_Renderable *r)
{
	Eon_Element_Notebook *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_NOTEBOOK(r);

	/* attributes */
	thiz->title = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EON_NAME_ATTR_TITLE), NULL,
			egueb_dom_string_ref(EON_NAME_ON),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	n = (EON_ELEMENT(r))->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->title), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_notebook_attr_modified_cb,
			EINA_FALSE, thiz);
	/* private */
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(r);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_NOTEBOOK));
}

static Egueb_Dom_Node * _eon_element_notebook_element_at(Eon_Renderable *r,
		Eina_Rectangle *cursor)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *found;

	n = (EON_ELEMENT(r))->n;
	/* if no childs, is just inside ourselves */	
	child = egueb_dom_element_child_first_get(n);
	if (!child)
	{
		return egueb_dom_node_ref(n);
	}
	/* is inside some child */
	found = eon_renderable_element_at(child, cursor);
	if (found)
	{
		return found;
	}
	return egueb_dom_node_ref(n);
}

static Enesim_Renderer * _eon_element_notebook_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Notebook *thiz;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;

	thiz = EON_ELEMENT_NOTEBOOK(r);
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

static int _eon_element_notebook_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Notebook *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	int ret = 0;
	int minw, minh;

	/* get the hints of the content */
	n = (EON_ELEMENT(r))->n;
	ret = eon_layout_notebook_size_hints_get(n, size);

	/* get the min size and padding of the theme */
	thiz = EON_ELEMENT_NOTEBOOK(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}

	/* propagate the title values if needed */
	if (egueb_dom_attr_has_changed(thiz->title))
	{
		Egueb_Dom_String *s = NULL;
		egueb_dom_attr_final_get(thiz->title, &s);
		eon_theme_element_notebook_title_set(theme_element, s);
	}

	eon_theme_element_notebook_min_size_get(theme_element, &minw, &minh);
	eon_theme_element_notebook_padding_get(theme_element, &padding);

	/* the min size is the max of the content min size + padding
	 * or the theme min size */
	if (ret & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (size->min_width >= 0)
			size->min_width += padding.left + padding.right;
		if (size->min_height >= 0)
			size->min_height += padding.top + padding.bottom;

		if (minw > size->min_width)
			size->min_width = minw;
		if (minh > size->min_height)
			size->min_height = minh;
	}
	else
	{
		ret |= EON_RENDERABLE_HINT_MIN_MAX;
		size->min_width = minw;
		size->min_height = minh;
	}

	if (ret & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (size->pref_width >= 0)
		{
			size->pref_width += padding.left + padding.right;
			if (minw > size->pref_width)
				size->pref_width = minw;
		}
		if (size->pref_height >= 0)
		{
			size->pref_height += padding.top + padding.bottom;
			if (minh > size->pref_height)
				size->pref_height = minh;
		}
	}

	return ret;
}

static Eina_Bool _eon_element_notebook_process(Eon_Renderable *r)
{
	Eon_Element_Notebook *thiz;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_NOTEBOOK(r);

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

	/* finally add our padding */
	eon_theme_element_notebook_padding_get(theme_element, &padding);
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
		eon_theme_element_notebook_content_set(theme_element, ren);
		egueb_dom_node_unref(child);
	}
	else
	{
		eon_theme_element_notebook_content_set(theme_element, NULL);
	}

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

done:
	/* Our basic notebook layout algorithm */
	eon_layout_notebook_size_geometry_set(n, &free_space);

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_notebook_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_NOTEBOOK);
}

static Eina_Bool _eon_element_notebook_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
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
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Notebook,
		Eon_Element_Notebook_Class, eon_element_notebook);

static void _eon_element_notebook_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_notebook_tag_name_get;
	klass->child_appendable = _eon_element_notebook_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_notebook_init;
	r_klass->element_at = _eon_element_notebook_element_at;
	r_klass->renderer_get = _eon_element_notebook_renderer_get;
	r_klass->size_hints_get = _eon_element_notebook_size_hints_get;
	r_klass->process = _eon_element_notebook_process;
	r_klass->child_size_dependant = EINA_TRUE;
}

static void _eon_element_notebook_instance_init(void *o)
{
}

static void _eon_element_notebook_instance_deinit(void *o)
{
	Eon_Element_Notebook *thiz;

	thiz = EON_ELEMENT_NOTEBOOK(o);
	egueb_dom_node_unref(thiz->title);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_notebook_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_notebook);
	return n;
}

