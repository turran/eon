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
#include "eon_theme_private.h"
#include "eon_theme_element_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Theme_Element,
		Eon_Theme_Element_Class, eon_theme_element);

static void _eon_theme_element_class_init(void *k)
{
}

static void _eon_theme_element_instance_init(void *o)
{
}

static void _eon_theme_element_instance_deinit(void *o)
{
}
/*----------------------------------------------------------------------------*
 *                      The exernal element interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_theme_element_init(Egueb_Dom_Node *n, void *data)
{
}

static void _eon_theme_element_deinit(Egueb_Dom_Node *n, void *data)
{
	enesim_object_instance_free(data);
}

static Egueb_Dom_String * _eon_theme_element_tag_name_get(
		Egueb_Dom_Node *node, void *data)
{
	Eon_Theme_Element *thiz = data;
	Eon_Theme_Element_Class *klass;

	klass = EON_THEME_ELEMENT_CLASS_GET(data);
	if (klass->tag_name_get)
		return klass->tag_name_get(thiz);
}

static Eina_Bool _eon_theme_element_child_appendable(Egueb_Dom_Node *n,
		void *data, Egueb_Dom_Node *child)
{
	Eon_Theme_Element *thiz = data;
	Eon_Theme_Element_Class *klass;

	klass = EON_THEME_ELEMENT_CLASS_GET(data);
	if (klass->child_appendable)
		return klass->child_appendable(thiz, child);
	return EINA_FALSE;
}

static Eina_Bool _eon_theme_element_process(Egueb_Dom_Node *n, void *data)
{
	Eon_Theme_Element *thiz = data;
	Eon_Theme_Element_Class *klass;

	klass = EON_THEME_ELEMENT_CLASS_GET(data);

	if (klass->process)
		return klass->process(thiz);
	return EINA_TRUE;
}

static Egueb_Dom_Element_External_Descriptor _descriptor = {
	/* .version		= */ EGUEB_DOM_ELEMENT_EXTERNAL_DESCRIPTOR_VERSION,
	/* init 		= */ _eon_theme_element_init,
	/* deinit 		= */ _eon_theme_element_deinit,
	/* tag_name_get		= */ _eon_theme_element_tag_name_get,
	/* child_appendable 	= */ _eon_theme_element_child_appendable,
	/* process 		= */ _eon_theme_element_process,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool eon_theme_is_element(Egueb_Dom_Node *n)
{
	void *data;

	if (!n) return EINA_FALSE;
	if (!egueb_dom_element_is_external(n))
		return EINA_FALSE;
	data = egueb_dom_element_external_data_get(n);
	if (!enesim_object_instance_inherits(ENESIM_OBJECT_INSTANCE(data),
			EON_THEME_ELEMENT_DESCRIPTOR))
		return EINA_FALSE;
	return EINA_TRUE;
}

Egueb_Dom_Node * eon_theme_element_new(Enesim_Object_Descriptor *descriptor,
		Enesim_Object_Class *klass)
{
	Eon_Theme_Element *thiz;
	Eon_Theme_Element_Class *k;
	Egueb_Dom_Node *n;
	void *object;

	thiz = enesim_object_descriptor_instance_new(descriptor, klass);
	n = egueb_dom_element_external_new(&_descriptor, thiz);
	thiz->n = n;

	/* add the attributes */
	thiz->id = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EON_NAME_ID), NULL, NULL,
			EINA_FALSE, EINA_FALSE, EINA_FALSE);

	/* finally initialize */
	k = EON_THEME_ELEMENT_CLASS_GET(thiz);

	if (k->init)
		k->init(thiz);
	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
