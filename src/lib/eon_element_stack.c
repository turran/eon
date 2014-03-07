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

#include "eon_renderable_private.h"
#include "eon_layout_stack_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_STACK_DESCRIPTOR eon_element_stack_descriptor_get()
#define EON_ELEMENT_STACK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Stack, EON_ELEMENT_STACK_DESCRIPTOR)

typedef struct _Eon_Element_Stack
{
	Eon_Renderable base;
	/* attributes */
	/* private */
	Enesim_Renderer *r;
	int weights;
	int min_length;
	Eina_Bool renderable_changed;
} Eon_Element_Stack;

typedef struct _Eon_Element_Stack_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Stack_Class;

static void _eon_element_stack_tree_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Stack *thiz = data;
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

	/* get the target and check if it is of type renderable */
	target = egueb_dom_event_target_get(e);
	if (!eon_is_renderable(target))
	{
		egueb_dom_node_unref(target);
		return;
	}

	thiz->renderable_changed = EINA_TRUE;
	egueb_dom_node_unref(target);
}

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_stack_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Stack *thiz;

	thiz = EON_ELEMENT_STACK(r);
	return enesim_renderer_ref(thiz->r);
}

static int _eon_element_stack_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Stack *thiz;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_STACK(r);

	n = (EON_ELEMENT(r))->n;
	return eon_layout_stack_vertical_size_hints_get(n, size,
			&thiz->min_length, &thiz->weights);
}

static Eina_Bool _eon_element_stack_process(Eon_Renderable *r)
{
	Eon_Element_Stack *thiz;
	Eon_Renderable_Size size;
	Eon_Box padding;
	Eon_Vertical_Align valign;
	Eon_Horizontal_Align halign;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int size_hints;

	thiz = EON_ELEMENT_STACK(r);
	n = (EON_ELEMENT(r))->n;

	free_space = r->geometry;
	DBG_ELEMENT(n, "Free space %" EINA_RECTANGLE_FORMAT, EINA_RECTANGLE_ARGS(&free_space));

	/* Regenerate the renderable tree */
	if (thiz->renderable_changed)
	{
		enesim_renderer_compound_layer_clear(thiz->r);
		child = egueb_dom_element_child_first_get(n);
		while (child)
		{
			Egueb_Dom_Node *tmp;

			if (!eon_is_renderable(child))
				goto next1;

			/* Add child renderer */
			if (thiz->renderable_changed)
			{
				Enesim_Renderer_Compound_Layer *l;
				Enesim_Renderer *ren;

				l = enesim_renderer_compound_layer_new();
				ren = eon_renderable_renderer_get(child);
				enesim_renderer_compound_layer_renderer_set(l, ren);
				enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
				enesim_renderer_compound_layer_add(thiz->r, l);
			}
next1:
			tmp = egueb_dom_element_sibling_next_get(child);
			egueb_dom_node_unref(child);
			child = tmp;
		}
		thiz->renderable_changed = EINA_FALSE;
	}
	/* Set the geometry on each child */
	eon_layout_stack_vertical_size_geometry_set(n, &free_space,
			thiz->min_length, thiz->weights);
	return EINA_TRUE;
}

static void _eon_element_stack_init(Eon_Renderable *r)
{
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(r))->n;
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_element_stack_tree_modified_cb,
			EINA_FALSE, r);
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_element_stack_tree_modified_cb,
			EINA_FALSE, r);
}

static Egueb_Dom_Node * _eon_element_stack_element_at(Eon_Renderable *r,
		Eina_Rectangle *cursor)
{
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *ret = NULL;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(r))->n;
	child = egueb_dom_element_child_first_get(n);
	while (child)
	{
		Egueb_Dom_Node *tmp;

		if (!eon_is_renderable(child))
			goto next;

		ret = eon_renderable_element_at(child, cursor);
		if (ret)
		{
			egueb_dom_node_unref(child);
			break;
		}
next:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
	if (!ret)
		ret = egueb_dom_node_ref(n);

	return ret;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_stack_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_STACK);
}

static Eina_Bool _eon_element_stack_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *our_child;

	/* only accept one child */
	if (!eon_is_renderable(child))
		return EINA_FALSE;

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Stack,
		Eon_Element_Stack_Class, eon_element_stack);

static void _eon_element_stack_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_stack_tag_name_get;
	klass->child_appendable = _eon_element_stack_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_stack_init;
	r_klass->renderer_get = _eon_element_stack_renderer_get;
	r_klass->size_hints_get = _eon_element_stack_size_hints_get;
	r_klass->process = _eon_element_stack_process;
	r_klass->element_at = _eon_element_stack_element_at;
}

static void _eon_element_stack_instance_init(void *o)
{
	Eon_Element_Stack *thiz = o;

	thiz->r = enesim_renderer_compound_new();
}

static void _eon_element_stack_instance_deinit(void *o)
{
	Eon_Element_Stack *thiz;

	thiz = EON_ELEMENT_STACK(o);
	enesim_renderer_unref(thiz->r);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_stack_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_stack);
	return n;
}
