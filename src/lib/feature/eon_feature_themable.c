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

#include "eon_element_private.h"
#include "eon_renderable_private.h"
#include "eon_feature_themable_private.h"
#include "eon_theme_document_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_FEATURE_THEMABLE_DESCRIPTOR eon_feature_themable_descriptor_get()
#define EON_FEATURE_THEMABLE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Feature_Themable, EON_FEATURE_THEMABLE_DESCRIPTOR)

typedef struct _Eon_Feature_Themable
{
	Enesim_Object_Instance base;
	Egueb_Dom_Node *n;
	Egueb_Dom_Feature *f;
	Egueb_Dom_Node *theme_element;
	Egueb_Dom_String *last_theme;
	Egueb_Dom_String *last_theme_id;
} Eon_Feature_Themable;

typedef struct _Eon_Feature_Themable_Class
{
	Enesim_Object_Class base;
} Eon_Feature_Themable_Class;
/*----------------------------------------------------------------------------*
 *                              Feature interface                             *
 *----------------------------------------------------------------------------*/
static int _eon_feature_themable_version_get(void *data)
{
	return EGUEB_DOM_FEATURE_EXTERNAL_DESCRIPTOR_VERSION;
}

static void _eon_feature_themable_init(void *data)
{

}

static void _eon_feature_themable_deinit(void *data)
{
	Eon_Feature_Themable *thiz;

	thiz = EON_FEATURE_THEMABLE(data);
	if (thiz->theme_element)
		egueb_dom_node_unref(thiz->theme_element);
}

static Egueb_Dom_Feature_External_Descriptor _descriptor = {
	/* .version_get		= */ _eon_feature_themable_version_get,
	/* .init 		= */ _eon_feature_themable_init,
	/* .deinit 		= */ _eon_feature_themable_deinit,
	/* .type_get		= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Feature_Themable,
		Eon_Feature_Themable_Class, eon_feature_themable);

static void _eon_feature_themable_class_init(void *k)
{
}

static void _eon_feature_themable_instance_init(void *o)
{
	Eon_Feature_Themable *thiz;

	thiz = EON_FEATURE_THEMABLE(o);
}

static void _eon_feature_themable_instance_deinit(void *o)
{
	Eon_Feature_Themable *thiz;

	thiz = EON_FEATURE_THEMABLE(o);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Feature * eon_feature_themable_add(Egueb_Dom_Node *n)
{
	Eon_Feature_Themable *thiz;
	Egueb_Dom_Feature *f;

	/* create the feature */
	thiz = ENESIM_OBJECT_INSTANCE_NEW(eon_feature_themable);
	thiz->f = egueb_dom_feature_external_new(&_descriptor, thiz);
	/* assign it */
	egueb_dom_node_feature_add(n, NULL, NULL, egueb_dom_feature_ref(thiz->f));
	/* TODO in case a node has been added/removed from a document make
	 * sure to seti/unset the new/old document too
	 */
	/* TODO add events for every 'state' that needs to be propagated
	 * like mousein, mouseodown, click, focusin, etc
	 */
	/* return it */
	return thiz->f;
}

Egueb_Dom_Node * eon_feature_themable_load(Egueb_Dom_Feature *f)
{
	Eon_Feature_Themable *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *ret = NULL;
	Egueb_Dom_String *curr_theme = NULL;
	Egueb_Dom_String *curr_theme_id = NULL;
	Eina_Bool theme_changed;

	thiz = EON_FEATURE_THEMABLE(egueb_dom_feature_external_data_get(f));
	e = EON_ELEMENT(egueb_dom_element_external_data_get(thiz->n));

	/* check if the theme has changed */
	egueb_dom_attr_final_get(e->theme, &curr_theme);
	egueb_dom_attr_final_get(e->theme_id, &curr_theme_id);
	if (!egueb_dom_string_is_equal(curr_theme, thiz->last_theme))
	{
		theme_changed = EINA_TRUE;
		goto changed;
	}

	if (!egueb_dom_string_is_equal(curr_theme_id, thiz->last_theme_id))
	{
		theme_changed = EINA_TRUE;
		goto changed;
	}
	
changed:
	if (theme_changed)
	{
		Egueb_Dom_Node *theme_doc;
		Egueb_Dom_Node *theme_element;

		/* for element that are renderable, make sure to invalidate
		 * the geometry
		 */
		if (eon_is_renderable(thiz->n))
		{
			eon_renderable_invalidate_geometry(thiz->n);
		}


		if (thiz->theme_element)
		{
			egueb_dom_node_unref(thiz->theme_element);
			thiz->theme_element = NULL;
		}

		/* swap */
		egueb_dom_string_unref(thiz->last_theme);
		thiz->last_theme = egueb_dom_string_dup(curr_theme);

		egueb_dom_string_unref(thiz->last_theme_id);
		thiz->last_theme_id = egueb_dom_string_dup(curr_theme_id);

		/* get the theme node */
		theme_doc = eon_theme_document_load(curr_theme);
		if (!theme_doc)
		{
			ERR_ELEMENT(thiz->n, "Theme '%s' does not exist", egueb_dom_string_string_get(curr_theme));
			goto done;
		}
		theme_element = egueb_dom_document_element_get_by_id(theme_doc, curr_theme_id, NULL);
		egueb_dom_node_unref(theme_doc);
		if (!theme_element)
		{
			ERR_ELEMENT(thiz->n, "Theme id '%s' does not exist", egueb_dom_string_string_get(curr_theme_id));
			goto done;
		}
		/* clone it */
		DBG_ELEMENT(thiz->n, "Loading theme '%s' with id '%s'",
				egueb_dom_string_string_get(curr_theme),
				egueb_dom_string_string_get(curr_theme_id));
		thiz->theme_element = egueb_dom_node_clone(theme_element, EINA_TRUE, EINA_FALSE, NULL);
		/* TODO adopt it to the new document */
	}
done:
	egueb_dom_string_unref(curr_theme);
	egueb_dom_string_unref(curr_theme_id);
	return egueb_dom_node_ref(thiz->theme_element);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
