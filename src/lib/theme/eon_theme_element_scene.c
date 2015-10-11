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
#include "eon_theme_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                     The exernal document interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_theme_element_scene_tag_name_get(
		Egueb_Dom_Node *node, void *data)
{
	return egueb_dom_string_ref(EON_THEME_ELEMENT_SCENE);
}

static Eina_Bool _eon_theme_element_scene_child_appendable(Egueb_Dom_Node *n,
		void *data, Egueb_Dom_Node *child)
{
	Egueb_Dom_String *name;
	Eina_Bool ret = EINA_FALSE;

	if (egueb_dom_node_type_get(child) != EGUEB_DOM_NODE_TYPE_ELEMENT)
		return ret;

	name = egueb_dom_element_name_get(child);
	if (name == EON_THEME_ELEMENT_OBJECT)
		ret = EINA_TRUE;
	egueb_dom_string_unref(name);

	return ret;
}

static Egueb_Dom_Element_External_Descriptor _descriptor = {
	/* version 		= */ EGUEB_DOM_ELEMENT_EXTERNAL_DESCRIPTOR_VERSION,
	/* init 		= */ NULL,
	/* deinit 		= */ NULL,
	/* tag_name_get		= */ _eon_theme_element_scene_tag_name_get,
	/* child_appendable 	= */ _eon_theme_element_scene_child_appendable,
	/* process 		= */ NULL,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_theme_element_scene_new(void)
{
	return egueb_dom_element_external_new(&_descriptor, NULL);
}

