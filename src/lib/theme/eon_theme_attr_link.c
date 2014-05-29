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
#include "Ender.h"
#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Attr_Link
{
	Eon_Theme_Attr_Link_Set set;
	Eon_Theme_Attr_Link_Get get;
	Egueb_Dom_String *s;
} Eon_Theme_Attr_Link;
/*----------------------------------------------------------------------------*
 *                        The exernal attr interface                           *
 *----------------------------------------------------------------------------*/
static void * _eon_theme_attr_link_init(Egueb_Dom_Node *n)
{
	Eon_Theme_Attr_Link *thiz;

	thiz = calloc(1, sizeof(Eon_Theme_Attr_Link));
	return thiz;
}

static void _eon_theme_attr_link_deinit(Egueb_Dom_Node *n, void *data)
{
	free(data);
}

static Eina_Bool _eon_theme_attr_link_value_get(Egueb_Dom_Node *n, void *data,
		Egueb_Dom_Attr_Type type, Egueb_Dom_Value *value)
{
	Eon_Theme_Attr_Link *thiz = data;
	Egueb_Dom_Node *owner;
	void *o;

	owner = egueb_dom_attr_owner_get(n);
	if (!owner) return EINA_FALSE;

	o = eon_theme_element_instance_object_get(owner);
	value->data.d = thiz->get(o);
	egueb_dom_node_unref(owner);

	return EINA_TRUE;
}

static Eina_Bool _eon_theme_attr_link_value_set(Egueb_Dom_Node *n, void *data,
		Egueb_Dom_Attr_Type type, Egueb_Dom_Value *value)
{
	Eon_Theme_Attr_Link *thiz = data;
	Egueb_Dom_Node *owner;
	void *o;

	owner = egueb_dom_attr_owner_get(n);
	if (!owner) return EINA_FALSE;

	o = eon_theme_element_instance_object_get(owner);
	thiz->set(o, value->data.d);
	egueb_dom_node_unref(owner);

	return EINA_TRUE;
}

static const Egueb_Dom_Value_Descriptor * _eon_theme_attr_link_value_descriptor_get(
		Egueb_Dom_Node *n, void *data)
{
	return egueb_dom_value_dom_string_get();
}

static Egueb_Dom_Attr_External_Descriptor _descriptor = {
	/* init 		= */ _eon_theme_attr_link_init,
	/* deinit 		= */ _eon_theme_attr_link_deinit,
	/* value_descriptor_get	= */ _eon_theme_attr_link_value_descriptor_get,
	/* value_get 		= */ _eon_theme_attr_link_value_get,
	/* value_set	 	= */ _eon_theme_attr_link_value_set,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_theme_attr_link_new(const char *name,
		Eon_Theme_Attr_Link_Get get, Eon_Theme_Attr_Link_Set set)
{
	Eon_Theme_Attr_Link *thiz;
	Egueb_Dom_String *s;
	Egueb_Dom_Node *n;

	n = egueb_dom_attr_external_new(&_descriptor);
	s = egueb_dom_string_new_with_static_string(name);
	egueb_dom_attr_init(n, s, EINA_TRUE, EINA_TRUE,
			EINA_TRUE);
	thiz = egueb_dom_attr_external_data_get(n);
	thiz->set = set;
	thiz->get = get;
	
	return n;
}


