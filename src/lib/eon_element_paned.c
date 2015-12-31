/* EON - Toolkit library
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
  Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "eon_private.h"
#include "eon_main.h"
#include "eon_element_paned.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_paned_private.h"
#include "eon_event_activate_private.h"
#include "eon_event_deactivate_private.h"
#include "eon_orientation_private.h"

/*
 * TODO:
 * + In case the position is not set, on the process calculate it
 * + Use the min length on the theme
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_PANED_DESCRIPTOR eon_element_paned_descriptor_get()
#define EON_ELEMENT_PANED(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Paned, EON_ELEMENT_PANED_DESCRIPTOR)

typedef struct _Eon_Element_Paned
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *orientation;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *proxy;
} Eon_Element_Paned;

typedef struct _Eon_Element_Paned_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Paned_Class;

static void _eon_element_paned_add_thickness(Eon_Renderable_Size *size, int hints,
		Eon_Orientation orientation, int thickness)
{
	if (hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (orientation == EON_ORIENTATION_HORIZONTAL)
		{
			if (size->min_width >= 0)
				size->min_width += thickness;
			if (size->max_width >= 0)
				size->max_width += thickness;
		}
		else
		{
			if (size->min_height >= 0)
				size->min_height += thickness;
			if (size->max_height >= 0)
				size->max_height += thickness;
		}
	}

	if (hints & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (orientation == EON_ORIENTATION_HORIZONTAL)
		{
			if (size->pref_width >= 0)
				size->pref_width += thickness;
		}
		else
		{
			if (size->pref_height >= 0)
				size->pref_height += thickness;
		}
	}
}

static void _eon_element_paned_calculate_child_size(Eon_Renderable_Size *size, int hints,
		Eon_Orientation orientation, Eon_Box *padding)
{
	if (hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (size->min_width >= 0)
			size->min_width += padding->left + padding->right;
		if (size->max_width >= 0)
			size->max_width += padding->left + padding->right;
		if (size->min_height >= 0)
			size->min_height += padding->top + padding->bottom;
		if (size->max_height >= 0)
			size->max_height += padding->top + padding->bottom;

	}
	if (hints & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (size->pref_width >= 0)
			size->pref_width += padding->left + padding->right;
		if (size->pref_height >= 0)
			size->pref_height += padding->top + padding->bottom;
	}
}

static int _eon_element_paned_calculate_size(Eon_Renderable_Size *size,
			Eon_Orientation orientation,
			Eon_Renderable_Size *ch1s, int ch1sm,
			Eon_Renderable_Size *ch2s, int ch2sm)
{
	int ret = 0;
	if ((ch1sm | ch2sm) & EON_RENDERABLE_HINT_MIN_MAX)
	{
		ret |= EON_RENDERABLE_HINT_MIN_MAX;
		size->min_width = size->max_width = -1;
		size->min_height = size->max_height = -1;

		if (ch1sm & EON_RENDERABLE_HINT_MIN_MAX)
		{
			if (ch1s->min_width >= 0)
				size->min_width = ch1s->min_width;
			if (ch1s->max_width >= 0)
				size->max_width = ch1s->max_width;
			if (ch1s->min_height >= 0)
				size->min_height = ch1s->min_height;
			if (ch1s->max_height >= 0)
				size->max_height = ch1s->max_height;
		}

		if (orientation == EON_ORIENTATION_HORIZONTAL)
		{
			if (ch2sm & EON_RENDERABLE_HINT_MIN_MAX)
			{
				if (ch2s->min_width >= 0)
				{
					if (size->min_width >= 0)
						size->min_width += ch2s->min_width;
					else
						size->min_width = ch2s->min_width;
				}
				if (ch2s->max_width >= 0)
				{
					if (size->max_width >= 0)
						size->max_width += ch2s->max_width;
					else
						size->max_width = ch2s->max_width;
				}

				if (ch2s->min_height >= 0)
				{
					if (size->min_height >= 0)
						size->min_height = MAX(ch2s->min_height, size->min_height);
					else
						size->min_height = ch2s->min_height;
				}
				if (ch2s->max_height >= 0)
				{
					if (size->max_height >= 0)
						size->max_height = MIN(ch2s->max_height, size->max_height);
					else
						size->max_height = ch2s->max_height;
				}
			}
		}
		else
		{
			if (ch2sm & EON_RENDERABLE_HINT_MIN_MAX)
			{
				if (ch2s->min_height >= 0)
				{
					if (size->min_height >= 0)
						size->min_height += ch2s->min_height;
					else
						size->min_height = ch2s->min_height;
				}
				if (ch2s->max_height >= 0)
				{
					if (size->max_height >= 0)
						size->max_height += ch2s->max_height;
					else
						size->max_height = ch2s->max_height;
				}

				if (ch2s->min_width >= 0)
				{
					if (size->min_width >= 0)
						size->min_width = MAX(ch2s->min_width, size->min_width);
					else
						size->min_width = ch2s->min_width;
				}
				if (ch2s->max_width >= 0)
				{
					if (size->max_width >= 0)
						size->max_width = MIN(ch2s->max_width, size->max_width);
					else
						size->max_width = ch2s->max_width;
				}
			}
		}
	}
	if ((ch1sm | ch2sm) & EON_RENDERABLE_HINT_PREFERRED)
	{
		ret |= EON_RENDERABLE_HINT_PREFERRED;
		size->pref_width = size->pref_height = -1;
		ERR("TODO");
	}

	return ret;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_paned_init(Eon_Renderable *r)
{
	Eon_Element_Paned *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_PANED(r);
	/* attributes */
	thiz->orientation = eon_orientation_attr_new();
	egueb_dom_attr_set(thiz->orientation, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			EON_ORIENTATION_HORIZONTAL);
	n = (EON_ELEMENT(r))->n;
	egueb_dom_element_attribute_node_set(n,
		egueb_dom_node_ref(thiz->orientation), NULL);
	/* events */
	/* private */
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(r);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_PANED));
}

static Egueb_Dom_Node * _eon_element_paned_element_at(Eon_Renderable *r,
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

static Enesim_Renderer * _eon_element_paned_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Paned *thiz;

	thiz = EON_ELEMENT_PANED(r);
	return enesim_renderer_ref(thiz->proxy);
}

/*
 * The size hints of a paned should be:
 * The min/max size are calculated as the sum of the min/max sizes depending
 * on the orientation. For vertical paned:
 * min_width = ch1.min_width + control1.width + ch2.min_width;
 * max_width = ch1.max_width + control1.width + ch2.max_width;
 * min_height = MAX(ch1.min_height, ch2.min_height);
 * max_height = MIN(ch1.min_height, ch2.min_height);
 */
static int _eon_element_paned_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Paned *thiz;
	Eon_Renderable_Size ch1s, ch2s;
	Eon_Orientation orientation;
	Eon_Box padding;
	Egueb_Dom_Node *ch1, *ch2;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int ch1sm, ch2sm;
	int thickness = 0;
	int ret = 0;
	int minw, minh;

	thiz = EON_ELEMENT_PANED(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}

	/* Our own information */
	egueb_dom_attr_final_get(thiz->orientation, &orientation);

	/* Get the theme information */
	thickness = eon_theme_element_paned_thickness_get(theme_element);
	eon_theme_element_paned_padding_get(theme_element, &padding);

	/* Get the children and its hints */
	n = (EON_ELEMENT(r))->n;
	ch1 = egueb_dom_element_child_first_get(n);
	if (ch1)
	{
		Enesim_Renderer *ren;

		ren = eon_renderable_renderer_get(ch1);
		eon_theme_element_paned_first_content_set(theme_element, ren);

		ch1sm = eon_renderable_size_hints_get(ch1, &ch1s);
		ch2 = egueb_dom_element_sibling_next_get(ch1);

		/* Add the padding on each hint */
		_eon_element_paned_calculate_child_size(&ch1s, ch1sm, orientation, &padding);
		if (ch2)
		{
			ren = eon_renderable_renderer_get(ch1);
			eon_theme_element_paned_second_content_set(theme_element, ren);

			ch2sm = eon_renderable_size_hints_get(ch1, &ch2s);
			_eon_element_paned_calculate_child_size(&ch2s, ch2sm,
					orientation, &padding);
			ret = _eon_element_paned_calculate_size(size, orientation,
					&ch1s, ch1sm, &ch2s, ch2sm);
		}
		else
		{
			eon_theme_element_paned_second_content_set(theme_element, NULL);
			ret = ch1sm;
			*size = ch1s;
		}
	}
	else
	{
		eon_theme_element_paned_first_content_set(theme_element, NULL);
		eon_theme_element_paned_second_content_set(theme_element, NULL);
	}

	/* Add the thickness on the hints */
	_eon_element_paned_add_thickness(&ch1s, ch1sm, orientation, thickness);

	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_paned_process(Eon_Renderable *r)
{
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(r))->n;
	ERR_ELEMENT(n, "Geometry %" EINA_RECTANGLE_FORMAT, EINA_RECTANGLE_ARGS(&r->geometry));
#if 0
	Eon_Element_Paned *thiz;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	thiz = EON_ELEMENT_PANED(r);

	free_space = r->geometry;

	/* get the theme */
	theme_element = eon_feature_themable_element_get(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}

	/* Set the geometry on the child */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);

	/* finally add our padding */
	eon_theme_element_paned_padding_get(theme_element, &padding);
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
		eon_theme_element_paned_content_set(theme_element, ren);
		egueb_dom_node_unref(child);
	}
	else
	{
		eon_theme_element_paned_content_set(theme_element, NULL);
	}

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

done:
	/* Our basic paned layout algorithm */
	eon_layout_paned_size_geometry_set(n, &free_space);
#endif
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_paned_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_PANED);
}

static Eina_Bool _eon_element_paned_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *curr_child;
	int count = 0;

	/* only accept one child */
	if (!eon_is_renderable(child))
		return EINA_FALSE;

	/* only two children */
	n = e->n;
	curr_child = egueb_dom_element_child_first_get(n);
	while (curr_child)
	{
		Egueb_Dom_Node *tmp;

		count++;
		tmp = egueb_dom_element_sibling_next_get(curr_child);
		egueb_dom_node_unref(curr_child);
		curr_child = tmp;
	}
	if (count >= 2)
	{
		WARN("Only two child supported");
		return EINA_FALSE;
	}

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Paned,
		Eon_Element_Paned_Class, eon_element_paned);

static void _eon_element_paned_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_paned_tag_name_get;
	klass->child_appendable = _eon_element_paned_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_paned_init;
	r_klass->element_at = _eon_element_paned_element_at;
	r_klass->renderer_get = _eon_element_paned_renderer_get;
	r_klass->size_hints_get = _eon_element_paned_size_hints_get;
	r_klass->process = _eon_element_paned_process;
	r_klass->child_size_dependant = EINA_TRUE;
}

static void _eon_element_paned_instance_init(void *o)
{
}

static void _eon_element_paned_instance_deinit(void *o)
{
	Eon_Element_Paned *thiz;

	thiz = EON_ELEMENT_PANED(o);
	/* attributes */
	egueb_dom_node_unref(thiz->orientation);
	/* private */
	enesim_renderer_unref(thiz->proxy);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_paned_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_paned);
	return n;
}
