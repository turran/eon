/* EON - Canvas and Toolkit library
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
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "eon_private.h"
#include "eon_main.h"
#include "eon_attr_theme_name_private.h"
#include "eon_element_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Element,
		Eon_Element_Class, eon_element);

static void _eon_element_class_init(void *k)
{
}

static void _eon_element_instance_init(void *o)
{
}

static void _eon_element_instance_deinit(void *o)
{
	Eon_Element *thiz = o;

	if (thiz->theme)
		egueb_dom_node_unref(thiz->theme);
}
/*----------------------------------------------------------------------------*
 *                      The exernal element interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_element_ctor(void *data)
{
	return NULL;
}

static void _eon_element_init(Egueb_Dom_Node *n, void *data)
{
}

static void _eon_element_deinit(Egueb_Dom_Node *n, void *data)
{
	enesim_object_instance_free(data);
}

static Egueb_Dom_String * _eon_element_tag_name_get(
		Egueb_Dom_Node *node, void *data)
{
	Eon_Element *thiz = data;
	Eon_Element_Class *klass;

	klass = EON_ELEMENT_CLASS_GET(data);
	if (klass->tag_name_get)
		return klass->tag_name_get(thiz);
}

static Eina_Bool _eon_element_child_appendable(Egueb_Dom_Node *n,
		void *data, Egueb_Dom_Node *child)
{
	Eon_Element *thiz = data;
	Eon_Element_Class *klass;

	klass = EON_ELEMENT_CLASS_GET(data);
	if (klass->child_appendable)
		return klass->child_appendable(thiz, child);
	return EINA_FALSE;
}

static Eina_Bool _eon_element_process(Egueb_Dom_Node *n, void *data)
{
	Eon_Element *thiz = data;
	Eon_Element_Class *klass;
	Egueb_Dom_Node *rel;

	klass = EON_ELEMENT_CLASS_GET(data);

	INFO_ELEMENT(n, "Processing");
	/* FIXME do we actually need this? */
	/* process the inheritable attributes */
	rel = egueb_dom_node_parent_get(n);
	/* check the topmost */
	if (rel && egueb_dom_node_type_get(rel) != EGUEB_DOM_NODE_TYPE_ELEMENT)
	{
		egueb_dom_node_unref(rel);
		rel = NULL;
	}
	if (rel)
	{
		Eon_Element *other;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(rel));
		egueb_dom_attr_inheritable_process(thiz->theme, other->theme);
		egueb_dom_node_unref(rel);
	}
	if (klass->process)
		return klass->process(thiz);
	return EINA_TRUE;
}

static Egueb_Dom_Element_External_Descriptor _descriptor = {
	/* .version		= */ EGUEB_DOM_ELEMENT_EXTERNAL_DESCRIPTOR_VERSION,
	/* ctor 		= */ _eon_element_ctor,
	/* init 		= */ _eon_element_init,
	/* deinit 		= */ _eon_element_deinit,
	/* tag_name_get		= */ _eon_element_tag_name_get,
	/* child_appendable 	= */ _eon_element_child_appendable,
	/* process 		= */ _eon_element_process,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_element_new(Enesim_Object_Descriptor *descriptor,
		Enesim_Object_Class *klass)
{
	Eon_Element *thiz;
	Eon_Element_Class *k;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *theme;

	thiz = enesim_object_descriptor_instance_new(descriptor, klass);
	n = egueb_dom_element_external_new(&_descriptor, thiz);
	thiz->n = n;

	/* add the attributes */
	thiz->theme = eon_attr_theme_name_new();
	thiz->theme_id = egueb_dom_attr_string_list_new(
			egueb_dom_string_ref(EON_NAME_THEME_ID),
			NULL, NULL, EINA_FALSE, EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->theme), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->theme_id), NULL);

	/* finally initialize */
	k = EON_ELEMENT_CLASS_GET(thiz);

	if (k->init)
		k->init(thiz);

	return n;
}

void eon_element_attrs_propagate(Egueb_Dom_Node *n, Egueb_Dom_Node *other_n)
{
	Eon_Element *thiz;
	Eon_Element *other;

	thiz = EON_ELEMENT(egueb_dom_element_external_data_get(n));
	other = EON_ELEMENT(egueb_dom_element_external_data_get(other_n));
	if (egueb_dom_attr_has_changed(thiz->theme))
	{
		Egueb_Dom_String *theme;

		egueb_dom_attr_final_get(thiz->theme, &theme);
		egueb_dom_attr_set(other->theme, EGUEB_DOM_ATTR_TYPE_BASE, theme);
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_element_theme_set(Egueb_Dom_Node *n, Egueb_Dom_String *theme)
{
	
}
