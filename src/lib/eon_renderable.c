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
#include "eon_renderable.h"

#include "eon_renderable_private.h"
#include "eon_vertical_align_private.h"
#include "eon_horizontal_align_private.h"
#include "eon_event_geometry_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _eon_renderable_attr_modified_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Renderable *thiz = EON_RENDERABLE(data);
	Egueb_Dom_Event_Phase phase;
	Egueb_Dom_Node *attr;

	/* check if we are at the target */
	phase = egueb_dom_event_phase_get(ev);
	if (phase != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;
	/* check if the attribute is the width or the height */
	attr = egueb_dom_event_mutation_related_get(ev);
	if ((thiz->hexpand == attr) || (thiz->vexpand == attr))
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Renderable attr modified");
		eon_renderable_invalidate_geometry((EON_ELEMENT(thiz))->n);
	}
	egueb_dom_node_unref(attr);
}

static void _eon_renderable_geometry_invalidate_cb(Egueb_Dom_Event *ev,
		void *data)
{
	Eon_Renderable *thiz = EON_RENDERABLE(data);
	Eon_Renderable_Class *klass;
	Egueb_Dom_Event_Phase phase;

	phase = egueb_dom_event_phase_get(ev);
	if (phase == EGUEB_DOM_EVENT_PHASE_BUBBLING)
	{
		klass = EON_RENDERABLE_CLASS_GET(thiz);
		/* in case this element is child size dependant, be sure to invalidate
		 * our geometry/hints and let the event flow
		 */
		if (klass->child_size_dependant)
		{
			thiz->size_hints_cached = EINA_FALSE;
			thiz->needs_geometry = EINA_TRUE;
		}
		/* otherwise just process again ourselves which whill set again a size
		 * on the child
		 */
		else
		{
			Egueb_Dom_Node *n;

			n = EGUEB_DOM_NODE(egueb_dom_event_target_current_get(ev));
			egueb_dom_element_enqueue(n);
		}
	}
	/* if we are at the target, be sure clear the geometry */
	else if (phase == EGUEB_DOM_EVENT_PHASE_AT_TARGET)
	{
		thiz->size_hints_cached = EINA_FALSE;
		thiz->needs_geometry = EINA_TRUE;
	}
}

static Egueb_Dom_Node * _eon_renderable_focus_next(Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *tmp;
	Egueb_Dom_Node *next;

	if (!n)
		return NULL;
	next = egueb_dom_element_sibling_next_get(n);
	/* go down */
	while (next)
	{
		tmp = eon_renderable_focus_first(next);
		/* we find one going down */
		if (tmp)
		{
			egueb_dom_node_unref(next);
			return tmp;
		}
		/* continue with the next */
		else
		{
			tmp = egueb_dom_element_sibling_next_get(next);
			egueb_dom_node_unref(next);
			next = tmp;
		}
	}
	/* go up */
	parent = egueb_dom_node_parent_get(n);		
	tmp = _eon_renderable_focus_next(parent);
	egueb_dom_node_unref(parent);

	return tmp;
}


/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_renderable_init(Eon_Element *e)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

 	thiz = EON_RENDERABLE(e);
	/* add the attributes */
	thiz->hexpand = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_HEXPAND), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->hexpand, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	thiz->vexpand = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_VEXPAND), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->vexpand, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);

	thiz->valign = eon_vertical_align_attr_new();
	egueb_dom_attr_set(thiz->valign, EGUEB_DOM_ATTR_TYPE_DEFAULT, EON_VERTICAL_ALIGN_MIDDLE);

	thiz->halign = eon_horizontal_align_attr_new();
	egueb_dom_attr_set(thiz->halign, EGUEB_DOM_ATTR_TYPE_DEFAULT, EON_HORIZONTAL_ALIGN_CENTER);

	n = e->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->vexpand), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->hexpand), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->valign), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->halign), NULL);

	/* in case the attribute width or height has changed be sure to invalidate
	 * the geometry
	 */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_renderable_attr_modified_cb, EINA_FALSE, thiz);

	egueb_dom_event_target_event_listener_add(et,
			EON_EVENT_GEOMETRY_INVALIDATE,
			_eon_renderable_geometry_invalidate_cb, EINA_FALSE, thiz);
	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_renderable_process(Eon_Element *e)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;
	Egueb_Dom_Node *n = e->n;

	thiz = EON_RENDERABLE(e);
	klass = EON_RENDERABLE_CLASS_GET(e);

	DBG_ELEMENT(e->n, "Processing renderable (needs geometry: %d size hints cached: %d)",
			thiz->needs_geometry, thiz->size_hints_cached);
	if (klass->pre_process)
	{
		if (!klass->pre_process(thiz))
		{
			ERR_ELEMENT(e->n, "The pre process failed");
			return EINA_FALSE;
		}
	}

	if (thiz->needs_geometry)
	{
		ERR_ELEMENT(e->n, "Parent does not set a geometry");
		return EINA_FALSE;
	}
	/* process ourselves */
	if (klass->process)
		return klass->process(thiz);

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_ELEMENT_DESCRIPTOR, Eon_Renderable,
		Eon_Renderable_Class, eon_renderable);

static void _eon_renderable_class_init(void *k)
{
	Eon_Element_Class *klass = k;

	klass->init = _eon_renderable_init;
	klass->process = _eon_renderable_process;
}

static void _eon_renderable_instance_init(void *o)
{
	Eon_Renderable *thiz = o;

	thiz->needs_geometry = EINA_TRUE;
	thiz->size_hints_cached = EINA_FALSE;
}

static void _eon_renderable_instance_deinit(void *o)
{
	Eon_Renderable *thiz = o;

	egueb_dom_node_unref(thiz->vexpand);
	egueb_dom_node_unref(thiz->hexpand);
	egueb_dom_node_unref(thiz->valign);
	egueb_dom_node_unref(thiz->halign);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool eon_is_renderable(Egueb_Dom_Node *n)
{
	void *data;

	if (!n) return EINA_FALSE;
	if (!egueb_dom_element_is_external(n))
		return EINA_FALSE;
	data = egueb_dom_element_external_data_get(n);
	if (!enesim_object_instance_inherits(ENESIM_OBJECT_INSTANCE(data),
			EON_RENDERABLE_DESCRIPTOR))
		return EINA_FALSE;
	return EINA_TRUE;
}

void eon_renderable_attrs_propagate(Egueb_Dom_Node *n, Egueb_Dom_Node *other_n)
{
	Eon_Renderable *thiz;
	Eon_Renderable *other;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	other = EON_RENDERABLE(egueb_dom_element_external_data_get(other_n));
	if (egueb_dom_attr_has_changed(thiz->halign))
	{
		Eon_Horizontal_Align halign;

		egueb_dom_attr_final_get(thiz->halign, &halign);
		egueb_dom_attr_set(other->halign, EGUEB_DOM_ATTR_TYPE_BASE, halign);
	}

	if (egueb_dom_attr_has_changed(thiz->valign))
	{
		Eon_Vertical_Align valign;

		egueb_dom_attr_final_get(thiz->valign, &valign);
		egueb_dom_attr_set(other->valign, EGUEB_DOM_ATTR_TYPE_BASE, valign);
	}

	if (egueb_dom_attr_has_changed(thiz->hexpand))
	{
		int hexpand;

		egueb_dom_attr_final_get(thiz->hexpand, &hexpand);
		egueb_dom_attr_set(other->hexpand, EGUEB_DOM_ATTR_TYPE_BASE, hexpand);
	}

	if (egueb_dom_attr_has_changed(thiz->vexpand))
	{
		int vexpand;

		egueb_dom_attr_final_get(thiz->vexpand, &vexpand);
		egueb_dom_attr_set(other->vexpand, EGUEB_DOM_ATTR_TYPE_BASE, vexpand);
	}
}

void eon_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

	DBG_ELEMENT(n, "Setting geometry %" EINA_RECTANGLE_FORMAT,
			EINA_RECTANGLE_ARGS(geometry));
	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	thiz->geometry = *geometry;
	thiz->needs_geometry = EINA_FALSE;

	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->geometry_set)
	{
		klass->geometry_set(thiz, geometry);
	}
}

Enesim_Renderer * eon_renderable_renderer_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->renderer_get)
		return klass->renderer_get(thiz);
	return NULL;
}

void eon_renderable_invalidate_geometry(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Egueb_Dom_Event *ev;
	Egueb_Dom_Event_Target *et;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	DBG_ELEMENT(n, "Invalidating geometry");

	ev = eon_event_geometry_invalidate_new();
	et = EGUEB_DOM_EVENT_TARGET((EON_ELEMENT(thiz))->n);
	egueb_dom_event_target_event_dispatch(et, ev, NULL, NULL);
}

/* TODO handle the expand, padding, margin or whatever other attr we decide to add */
void eon_renderable_geometry_solve(Egueb_Dom_Node *n, Eina_Rectangle *fs, Eina_Rectangle *out)
{
	Eon_Vertical_Align valign;
	Eon_Horizontal_Align halign;
	Eon_Renderable_Size size;
	Eina_Bool w_set = EINA_FALSE;
	Eina_Bool h_set = EINA_FALSE;
	Eina_Bool hexpand, vexpand;
	int size_hints;
	int w = -1, h = -1;

	size_hints = eon_renderable_size_hints_get(n, &size);
	valign = eon_renderable_valign_get(n);
	halign = eon_renderable_halign_get(n);
	hexpand = eon_renderable_hexpand_get(n);
	vexpand = eon_renderable_vexpand_get(n);

	DBG_ELEMENT(n, "Solving free space %" EINA_RECTANGLE_FORMAT
			" halign: %d, valign: %d, hexpand: %d, vexpand :%d",
			EINA_RECTANGLE_ARGS(fs), halign, valign, hexpand, vexpand);
	w = fs->w;
	h = fs->h;
	/* handle the size */
	if (size_hints & EON_RENDERABLE_HINT_PREFERRED)
	{
		DBG_ELEMENT(n, "Has preferred hints, width: %d, height: %d",
				size.pref_width, size.pref_height);
		if (w > size.pref_width && size.pref_width > 0)
		{
			if (!hexpand)
			{
				w = size.pref_width;
			}
		}

		if (h > size.pref_height && size.pref_height > 0)
		{
			if (!vexpand)
			{
				h = size.pref_height;
			}
		}
	}

	if (size_hints & EON_RENDERABLE_HINT_MIN_MAX)
	{
		DBG_ELEMENT(n, "Has min/max hints, min width: %d, max width: %d, "
				"min height: %d, max height: %d",
				size.min_width, size.max_width,
				size.min_height, size.max_height);
		if (w < size.min_width)
		{
			ERR_ELEMENT(n, "Parent set a width %d less than min %d",
					w, size.min_width);
			w = size.min_width;
		}

		if (w > size.max_width && size.max_width > 0)
			w = size.max_width;

		if (h < size.min_height)
		{
			ERR_ELEMENT(n, "Parent set a height %d less than min %d",
					h, size.min_height);
			h = size.min_height;
		}

		if (h > size.max_height && size.max_height > 0)
			h = size.max_height;
	}

	/* handle the position */
	switch (halign)
	{
		case EON_HORIZONTAL_ALIGN_LEFT:
		out->x = fs->x;
		break;

		case EON_HORIZONTAL_ALIGN_CENTER:
		out->x = fs->x + ((fs->w - w) / 2);
		break;

		case EON_HORIZONTAL_ALIGN_RIGHT:
		out->x = fs->x + (fs->w - w);
		break;
	}
	switch (valign)
	{
		case EON_VERTICAL_ALIGN_TOP:
		out->y = fs->y;
		break;

		case EON_VERTICAL_ALIGN_MIDDLE:
		out->y = fs->y + ((fs->h - h) / 2);
		break;

		case EON_VERTICAL_ALIGN_BOTTOM:
		out->y = fs->y + (fs->h - h);
		break;
	}
	out->w = w;
	out->h = h;
}

Egueb_Dom_Node * eon_renderable_element_at(Egueb_Dom_Node *n,
		Eina_Rectangle *cursor)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	DBG_ELEMENT(n, "Renderable of geometry %" EINA_RECTANGLE_FORMAT " at %d %d?", EINA_RECTANGLE_ARGS(&thiz->geometry), cursor->x, cursor->y); 
	/* check that the mouse is on the current geometry */
	if (!eina_rectangles_intersect(&thiz->geometry, cursor))
		return NULL;

	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->element_at)
		return klass->element_at(thiz, cursor);
	return egueb_dom_node_ref(n);
}

Eina_Bool eon_renderable_is_focusable(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->is_focusable)
		return klass->is_focusable(thiz);
	else
		return EINA_FALSE;
}

Egueb_Dom_Node * eon_renderable_focus_first(Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *child;

	if (eon_is_renderable(n) && eon_renderable_is_focusable(n))
		return egueb_dom_node_ref(n);

	child = egueb_dom_element_child_first_get(n);
	while (child)
	{
		Egueb_Dom_Node *tmp;

		/* go depth */
		tmp = eon_renderable_focus_first(child);
		if (tmp)
		{
			egueb_dom_node_unref(child);
			return tmp;
		}
		/* pick next */
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
	return NULL;
}

Egueb_Dom_Node * eon_renderable_focus_next(Egueb_Dom_Node *n)
{
	return _eon_renderable_focus_next(n);
}

Egueb_Dom_Node * eon_renderable_focus_prev(Egueb_Dom_Node *n)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Sets the vexpand of a renderable
 * @param[in] n The renderable node to set the vexpand
 * @param[in] w The vexpand to set
 */
EAPI void eon_renderable_vexpand_set(Egueb_Dom_Node *n, Eina_Bool expand)
{
	Eon_Renderable *thiz;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->vexpand, EGUEB_DOM_ATTR_TYPE_BASE, expand);
}

EAPI Eina_Bool eon_renderable_vexpand_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Eina_Bool ret;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->vexpand, &ret);
	return ret;
}

/**
 * Sets the hexpand of a renderable
 * @param[in] n The renderable node to set the hexpand
 * @param[in] h The hexpand to set
 */
EAPI void eon_renderable_hexpand_set(Egueb_Dom_Node *n, Eina_Bool expand)
{
	Eon_Renderable *thiz;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->hexpand, EGUEB_DOM_ATTR_TYPE_BASE, expand);
}

EAPI Eina_Bool eon_renderable_hexpand_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Eina_Bool ret;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->hexpand, &ret);
	return ret;
}

/**
 * Sets the vertical align of a renderable
 * @param[in] n The renderable node to set the vertical align
 * @param[in] valign The vertical align to set
 */
EAPI void eon_renderable_valign_set(Egueb_Dom_Node *n, Eon_Vertical_Align valign)
{
	Eon_Renderable *thiz;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->valign, EGUEB_DOM_ATTR_TYPE_BASE, valign);
}

EAPI Eon_Vertical_Align eon_renderable_valign_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Eon_Vertical_Align ret;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->valign, &ret);
	return ret;
}

/**
 * Sets the horizontal align of a renderable
 * @param[in] n The renderable node to set the horizontal align
 * @param[in] halign The horizontal align to set
 */
EAPI void eon_renderable_halign_set(Egueb_Dom_Node *n, Eon_Horizontal_Align halign)
{
	Eon_Renderable *thiz;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->halign, EGUEB_DOM_ATTR_TYPE_BASE, halign);
}

EAPI Eon_Horizontal_Align eon_renderable_halign_get(Egueb_Dom_Node *n)
{
	Eon_Renderable *thiz;
	Eon_Horizontal_Align ret;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->halign, &ret);
	return ret;
}

/**
 * @brief Get the size hints of a renderable
 * @param[in] n The renderable node to get the size hints from
 * @param[out] size The pointer to write the size hints 
 * @return The size hints mask
 */
EAPI int eon_renderable_size_hints_get(Egueb_Dom_Node *n, Eon_Renderable_Size *size)
{
	Eon_Renderable *thiz;
	int ret = 0;

	thiz = EON_RENDERABLE(egueb_dom_element_external_data_get(n));
	DBG_ELEMENT(n, "Getting size hints (needs geometry: %d size hints cached: %d)",
			thiz->needs_geometry, thiz->size_hints_cached);
	/* Check if the size hints are already cached */
	if (!thiz->size_hints_cached)
	{
		Eon_Renderable_Class *klass;

		klass = EON_RENDERABLE_CLASS_GET(thiz);
		if (klass->size_hints_get)
		{
			eon_renderable_size_init(size);
	 		ret = klass->size_hints_get(thiz, size);
			/* cache it */
			thiz->size = *size;
			thiz->size_hints = ret;
			thiz->size_hints_cached = EINA_TRUE;
		}
	}
	else
	{
		ret = thiz->size_hints;
		*size = thiz->size;
	}
	return ret;
}
