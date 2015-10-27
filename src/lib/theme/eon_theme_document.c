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
#include "eon_theme_namespace_private.h"
#include "eon_theme_element_eot_private.h"
#include "eon_theme_element_label_private.h"
#include "eon_theme_element_instance_private.h"
#include "eon_theme_element_object_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Document
{
} Eon_Theme_Document;

static Eina_Hash *_docs = NULL;
/*----------------------------------------------------------------------------*
 *                     The exernal document interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_theme_document_element_create(Egueb_Dom_Node *n,
		void *data, const char *ns_uri, const char *name)
{
	/* we can only create states/state/set/animate */
	/* any other object needs to be created using the topmost ender */
	DBG("Creating element '%s' with ns '%s'", name, ns_uri);
	if (!strcmp(name, "eot"))
	{
		return eon_theme_element_eot_new();
	}
	else if (!strcmp(name, "label"))
	{
		return eon_theme_element_label_new();
	}
	else if (!strcmp(name, "set"))
	{
		return egueb_smil_set_new();
	}
	else if (!strcmp(name, "animate"))
	{
		return egueb_smil_animate_new();
	}
	else if (ns_uri)
	{
		Eon_Theme_Namespace *ns;
		ns = eon_theme_namespace_find(ns_uri);
		if (!ns)
		{
			WRN("No namespace '%s' found", ns_uri);
			return NULL;
		}
		return eon_theme_namespace_element_ctor(ns, name);
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
	ret = egueb_dom_string_is_equal(name, EON_THEME_NAME_ELEMENT_EOT);
	egueb_dom_string_unref(name);
	return ret;
}

static Egueb_Dom_Document_External_Descriptor _descriptor = {
	/* version 		= */ EGUEB_DOM_DOCUMENT_EXTERNAL_DESCRIPTOR_VERSION,
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

void eon_theme_document_init(void)
{
	_docs = eina_hash_string_superfast_new(EINA_FREE_CB(egueb_dom_node_unref));
}

void eon_theme_document_shutdown(void)
{
	eina_hash_free(_docs);
}

Egueb_Dom_Node * eon_theme_document_load(Egueb_Dom_String *name)
{
	Egueb_Dom_Node *ret = NULL;
	Enesim_Stream *s;
	const char *theme_path;
	char path[PATH_MAX];

	ret = eina_hash_find(_docs, egueb_dom_string_string_get(name));
	if (ret) return egueb_dom_node_ref(ret);
		
	/* load the file based from the fs */
	theme_path = getenv("EON_THEME_PATH");
	if (!theme_path)
	{
		theme_path = "/usr/local/share/eon/themes";
	}
	snprintf(path, PATH_MAX, "%s/%s.eot", theme_path, egueb_dom_string_string_get(name));
	DBG("Loading theme at '%s'", path);
	s = enesim_stream_file_new(path, "r");
	if (!s) return NULL;

	ret = eon_theme_document_new();
	egueb_dom_parser_parse(s, &ret);
	eina_hash_add(_docs, egueb_dom_string_string_get(name),
			egueb_dom_node_ref(ret));
	return ret;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

