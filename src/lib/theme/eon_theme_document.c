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
#include "eon_theme_document_private.h"
#include "eon_theme_element_instance_private.h"
#include "eon_theme_element_object_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Document
{
} Eon_Theme_Document;

/*----------------------------------------------------------------------------*
 *                     The exernal document interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_theme_document_element_create(Egueb_Dom_Node *n,
		void *data, const char *name)
{
	/* we can only create states/state/set/animate */
	/* any other object needs to be created using the topmost ender */
	DBG("Creating element '%s'", name);
	if (!strcmp(name, "states"))
	{
		return eon_theme_element_states_new();
	}
	else if (!strcmp(name, "state"))
	{
		return eon_theme_element_state_new();
	}
	else if (!strcmp(name, "object"))
	{
		return eon_theme_element_object_new();
	}
	else if (!strcmp(name, "ender"))
	{
		return eon_theme_element_eon_theme_new();
	}
	else if (!strcmp(name, "instance"))
	{
		return eon_theme_element_instance_new();
	}
	else if (!strcmp(name, "scene"))
	{
		return eon_theme_element_scene_new();
	}
	else if (!strcmp(name, "set"))
	{
		return egueb_smil_set_new();
	}
	else if (!strcmp(name, "animate"))
	{
		return egueb_smil_animate_new();
	}

	return NULL;
}

static void _eon_theme_document_init(Egueb_Dom_Node *n, void *data)
{
}

static void _eon_theme_document_deinit(Egueb_Dom_Node *n, void *data)
{
	Eon_Theme_Document *thiz = data;

	free(thiz);
}

static Eina_Bool _eon_theme_document_child_appendable(Egueb_Dom_Node *n,
		void *data, Egueb_Dom_Node *child)
{
	Egueb_Dom_String *name;
	Eina_Bool ret;

	name = egueb_dom_node_name_get(child);
	ret = egueb_dom_string_is_equal(name, EON_THEME_ELEMENT_EON_THEME);
	egueb_dom_string_unref(name);
	return ret;
}

static Egueb_Dom_Document_External_Descriptor _descriptor = {
	/* init 		= */ _eon_theme_document_init,
	/* deinit 		= */ _eon_theme_document_deinit,
	/* element_create	= */ _eon_theme_document_element_create,
	/* child_appendable 	= */ _eon_theme_document_child_appendable,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_document_new(void)
{
	Eon_Theme_Document *thiz;

	thiz = calloc(1, sizeof(Eon_Theme_Document));

	return egueb_dom_document_external_new(&_descriptor, thiz);
}

Egueb_Dom_Node * eon_theme_document_instance_new(Egueb_Dom_Node *n,
		const char *id, Eina_Error *err)
{
	Egueb_Dom_Node *ret;
	Egueb_Dom_Node *rel;
	Egueb_Dom_Node *topmost;
	Egueb_Dom_String *sid;
	
	sid = egueb_dom_string_new_with_static_string(id);
	rel = egueb_dom_document_element_get_by_id(n, sid, err);
	egueb_dom_string_unref(sid);
	if (!rel) return NULL;

	if (!eon_theme_element_is_object(rel))
	{
		egueb_dom_node_unref(rel);
		return NULL;
	}

	ret = egueb_dom_document_element_create(n, EON_THEME_ELEMENT_INSTANCE, err);
	if (!ret)
	{
		egueb_dom_node_unref(rel);
		return NULL;
	}

	topmost = egueb_dom_document_document_element_get(n);
	egueb_dom_node_child_append(topmost, egueb_dom_node_ref(ret), NULL);
	eon_theme_element_instance_relative_set(ret, rel);
	egueb_dom_node_unref(topmost);

	return ret;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

