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
 * + Calculate the min/max position of the slider in the 0-1 range. This will
 * useful when dragging the slider to know if we should enqueue or not the
 * element
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
	Eina_Bool is_dragging;
} Eon_Element_Paned;

typedef struct _Eon_Element_Paned_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Paned_Class;

static void _eon_element_paned_mouse_move_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Element_Paned *thiz = data;

	if (egueb_dom_event_phase_get(ev) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	if (!thiz->is_dragging)
		return;

	ERR("mouse move");
#if 0
	Eon_Splitter *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Element *ee;
	double c;
	double v = 0;
	double length;

	ee = ender_element_object_get(e);
	thiz = _eon_splitter_get(ee);
	if (!thiz->dragging) return;

	/* get the absolute position of the event */
	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		c = ev->x - thiz->offset_dragging;
	}
	else
	{
		c = ev->y - thiz->offset_dragging;
	}
	length = thiz->maxl - thiz->minl;
	if (c < thiz->minl)
		c = thiz->minl;
	if (c > thiz->maxl)
		c = thiz->maxl;
	c -= thiz->minl;
	if (length)
		v = c / length;
	eon_splitter_position_set(e, v);
	/* drag stop */
	Eon_Splitter *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_splitter_get(ee);
	thiz->dragging = EINA_FALSE;
	/* drag start */
	Eon_Splitter *thiz;
	Eon_Event_Mouse_Drag_Start *ev = event_data;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle g;

	ee = ender_element_object_get(e);
	thiz = _eon_splitter_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);

	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		eon_theme_splitter_position_get(theme_r, &g.x);
		g.y = 0;
		eon_theme_splitter_thickness_get(theme_r, &g.w);
		eon_element_actual_height_get(e, &g.h);
		thiz->offset_dragging = ev->x - g.x;
	}
	else
	{
		g.x = 0;
		eon_theme_splitter_position_get(theme_r, &g.y);
		eon_element_actual_width_get(e, &g.w);
		eon_theme_splitter_thickness_get(theme_r, &g.h);
		thiz->offset_dragging = ev->y - g.y;
	}
	if (enesim_rectangle_is_inside(&g, ev->rel_x, ev->rel_y))
	{
		thiz->dragging = EINA_TRUE;
	}
#endif
}


static void _eon_element_paned_mouse_down_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Element_Paned *thiz = data;

	if (egueb_dom_event_phase_get(ev) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	thiz->is_dragging = EINA_TRUE;
	/* We need to capture the mouse move on the document, not on the
	 * element. Basically because the mouse move event is triggered
	 * only in case the cursor in over the element.
	 * When dragging it is possible to drag outside the element
	 * even if redrawing at a cursor move.
	 * One possible workaround is to do something like
	 * element.setCapture/releaseCapture, like:
	 * https://developer.mozilla.org/en-US/docs/Web/API/Element/setCapture
	 */
	ERR("mouse down");
}

static void _eon_element_paned_mouse_up_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Element_Paned *thiz = data;

	if (egueb_dom_event_phase_get(ev) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	thiz->is_dragging = EINA_FALSE;
	ERR("mouse up");
}

static void _eon_element_paned_add_thickness(Eon_Renderable_Size *size, int hints,
		Eon_Orientation orientation, int thickness)
{
	if (hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (orientation == EON_ORIENTATION_HORIZONTAL)
		{
			if (size->min_width >= 0)
				size->min_width += thickness;
		}
		else
		{
			if (size->min_height >= 0)
				size->min_height += thickness;
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
		if (size->min_height >= 0)
			size->min_height += padding->top + padding->bottom;

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

		if (ch1sm & EON_RENDERABLE_HINT_MIN_MAX)
		{
			if (ch1s->min_width >= 0)
				size->min_width = ch1s->min_width;
			if (ch1s->min_height >= 0)
				size->min_height = ch1s->min_height;
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
				if (ch2s->min_height >= 0)
				{
					if (size->min_height >= 0)
						size->min_height = MAX(ch2s->min_height, size->min_height);
					else
						size->min_height = ch2s->min_height;
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
				if (ch2s->min_width >= 0)
				{
					if (size->min_width >= 0)
						size->min_width = MAX(ch2s->min_width, size->min_width);
					else
						size->min_width = ch2s->min_width;
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
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_PANED(r);
	/* attributes */
	thiz->orientation = eon_orientation_attr_new();
	egueb_dom_attr_set(thiz->orientation, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			EON_ORIENTATION_HORIZONTAL);
	n = (EON_ELEMENT(r))->n;
	egueb_dom_element_attribute_node_set(n,
		egueb_dom_node_ref(thiz->orientation), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_DOWN,
			_eon_element_paned_mouse_down_cb, EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_UP,
			_eon_element_paned_mouse_up_cb, EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_MOVE,
			_eon_element_paned_mouse_move_cb, EINA_FALSE, thiz);
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
	Egueb_Dom_Node *ch1;
	Egueb_Dom_Node *found;

	n = (EON_ELEMENT(r))->n;
	/* if no childs, is just inside ourselves */	
	ch1 = egueb_dom_element_child_first_get(n);
	if (!ch1)
	{
		return egueb_dom_node_ref(n);
	}
	/* is inside some child */
	found = eon_renderable_element_at(ch1, cursor);
	if (found)
	{
		egueb_dom_node_unref(ch1);
		return found;
	}
	else
	{
		Egueb_Dom_Node *ch2;
		ch2 = egueb_dom_element_sibling_next_get(ch1);
		if (ch2)
		{
			found = eon_renderable_element_at(ch2, cursor);
			if (found)
			{
				egueb_dom_node_unref(ch2);
				return found;
			}
			egueb_dom_node_unref(ch2);
		}
		egueb_dom_node_unref(ch1);
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
	Eon_Renderable_Size ch1s;
	Eon_Orientation orientation;
	Eon_Box padding;
	Egueb_Dom_Node *ch1;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int ch1sm;
	int thickness = 0;
	int ret = 0;

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
		Egueb_Dom_Node *ch2;
		Eon_Renderable_Size ch2s;
		int ch2sm;

		/* Add the padding on each hint */
		ch1sm = eon_renderable_size_hints_get(ch1, &ch1s);
		_eon_element_paned_calculate_child_size(&ch1s, ch1sm, orientation, &padding);

		ch2 = egueb_dom_element_sibling_next_get(ch1);
		if (ch2)
		{
			ch2sm = eon_renderable_size_hints_get(ch1, &ch2s);
			_eon_element_paned_calculate_child_size(&ch2s, ch2sm,
					orientation, &padding);
			ret = _eon_element_paned_calculate_size(size, orientation,
					&ch1s, ch1sm, &ch2s, ch2sm);
			egueb_dom_node_unref(ch2);
		}
		else
		{
			ret = ch1sm;
			*size = ch1s;
		}
		egueb_dom_node_unref(ch1);
	}

	/* A paned widget can be of any size */
	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	size->max_width = size->max_height = -1;

	/* Add the thickness on the hints */
	_eon_element_paned_add_thickness(size, ret, orientation, thickness);

	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_paned_process(Eon_Renderable *r)
{
	Eon_Element_Paned *thiz;
	Eon_Renderable_Size ch1s;
	Egueb_Dom_Node *theme_element;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *ch1;
	int ch1sm;
	double position = 0.8;

	thiz = EON_ELEMENT_PANED(r);
	/* get the theme */
	theme_element = eon_feature_themable_element_get(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}

	n = (EON_ELEMENT(r))->n;
	/* Set the geometry on the theme */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);

	/* Set the geometry on every child */
	ch1 = egueb_dom_element_child_first_get(n);
	if (ch1)
	{
		Eon_Renderable_Size ch2s;
		Eon_Box padding = { 0, 0, 0, 0 };
		Egueb_Dom_Node *ch2;
		Enesim_Renderer *ren;
		Eina_Rectangle ch1fs;
		int ch2sm;

		/* Calculate the size */
		ch1sm = eon_renderable_size_hints_get(ch1, &ch1s);
		ch1fs = r->geometry;

		eon_theme_element_paned_padding_get(theme_element, &padding);
		ch1fs.x += padding.left;
		ch1fs.y += padding.top;
		ch1fs.w -= padding.left + padding.right;
		ch1fs.h -= padding.top + padding.bottom;

		ch2 = egueb_dom_element_sibling_next_get(ch1);
		if (ch2)
		{
			Eon_Orientation orientation;
			Eina_Rectangle ch2fs;
			int thickness;

			egueb_dom_attr_final_get(thiz->orientation, &orientation);
			thickness = eon_theme_element_paned_thickness_get(theme_element);

			/* Calculate the size */
			ch2sm = eon_renderable_size_hints_get(ch2, &ch2s);
			if (orientation == EON_ORIENTATION_HORIZONTAL)
			{
				int ch1width = ((r->geometry.w - thickness) * position) - padding.left - padding.right;
				int ch2width = ((r->geometry.w - thickness) - ch1width) - padding.left - padding.right;
				/* calculate the progress limits */
				if ((ch1sm & EON_RENDERABLE_HINT_MIN_MAX) && ch1s.min_width >= 0)
				{
					if (ch1width < ch1s.min_width)
					{
						ch1width = ch1s.min_width;
						ch2width = ((r->geometry.w - thickness) - ch1width) - padding.left - padding.right;
					}
				}
				if ((ch2sm & EON_RENDERABLE_HINT_MIN_MAX) && ch2s.min_width >= 0)
				{
					if (ch2width < ch2s.min_width)
					{
						ch2width = ch2s.min_width;
						ch1width = ((r->geometry.w - thickness) - ch2width) - padding.left - padding.right;
					}
				}
				/* set the real size now */
				ch1fs.w = ch1width;
				ch2fs.x = ch1fs.x + ch1fs.w + thickness;
				ch2fs.y = ch1fs.y;
				ch2fs.w = ch2width;
				ch2fs.h = ch1fs.h;
			}
			else
			{
				int ch1height = ((r->geometry.h - thickness) * position) - padding.top - padding.bottom;
				int ch2height = ((r->geometry.h - thickness) - ch1height) - padding.top - padding.bottom;
				/* calculate the progress limits */
				if ((ch1sm & EON_RENDERABLE_HINT_MIN_MAX) && ch1s.min_height >= 0)
				{
					if (ch1height < ch1s.min_height)
					{
						ch1height = ch1s.min_height;
						ch2height = ((r->geometry.h - thickness) - ch1height) - padding.top - padding.bottom;
					}
				}
				if ((ch2sm & EON_RENDERABLE_HINT_MIN_MAX) && ch2s.min_height >= 0)
				{
					if (ch2height < ch2s.min_height)
					{
						ch2height = ch2s.min_height;
						ch1height = ((r->geometry.h - thickness) - ch2height) - padding.top - padding.bottom;
					}
				}
				/* set the real size now */
				ch1fs.h = ch1height;
				ch2fs.x = ch1fs.x;
				ch2fs.y = ch1fs.y + ch1fs.h + thickness;
				ch2fs.w = ch1fs.w;
				ch2fs.h = ch2height;
			}

			/* Set the renderer */
			ren = eon_renderable_renderer_get(ch2);
			eon_theme_element_paned_second_content_set(theme_element, ren, &ch2fs);
			/* finally process */
			eon_renderable_geometry_set(ch2, &ch2fs);
			egueb_dom_element_process(ch2);
			egueb_dom_node_unref(ch2);
		}
		else
		{
			eon_theme_element_paned_second_content_set(theme_element, NULL, NULL);
		}

		/* Set the renderer */
		ren = eon_renderable_renderer_get(ch1);
		eon_theme_element_paned_first_content_set(theme_element, ren, &ch1fs);

		eon_renderable_geometry_set(ch1, &ch1fs);
		egueb_dom_element_process(ch1);
		egueb_dom_node_unref(ch1);
	}
	else
	{
		eon_theme_element_paned_first_content_set(theme_element, NULL, NULL);
		eon_theme_element_paned_second_content_set(theme_element, NULL, NULL);
	}

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

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
