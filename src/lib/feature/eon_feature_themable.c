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

#include "eon_feature_themable_private.h"
/* TODO
 * - Add a callback on the request_process function of the theme
 */
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
	Eina_Bool theme_changed;
} Eon_Feature_Themable;

typedef struct _Eon_Feature_Themable_Class
{
	Enesim_Object_Class base;
} Eon_Feature_Themable_Class;

#if 0
static Eina_Bool _eon_feature_themable_load_theme(Eon_Feature_Themable *thiz)
{
	Eon_Element *e;
	Eon_Widget *w;
	Egueb_Dom_String *theme = NULL;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *topmost;
	Egueb_Dom_Node *rel_theme = NULL;
	Eina_Bool ret = EINA_TRUE;

	w = EON_WIDGET(thiz);
	e = EON_ELEMENT(w);
	n = e->n;

	if (!thiz->theme_changed)
	{
		if (!thiz->theme_widget)
		{
			ERR_ELEMENT(n, "Theme does not exist");
			return EINA_FALSE;
		}
		return EINA_TRUE;
	}

	if (thiz->theme_instance)
	{
		egueb_dom_node_unref(thiz->theme_instance);
		thiz->theme_instance = NULL;
	}

	if (thiz->theme_document)
	{
		egueb_dom_node_unref(thiz->theme_document);
		thiz->theme_document = NULL;
	}
	thiz->theme_widget = NULL;

	parent = egueb_dom_node_parent_get(n);
	if (parent && (egueb_dom_node_type_get(parent) == EGUEB_DOM_NODE_TYPE_ELEMENT))
	{
		Eon_Element *other;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(parent));
		rel_theme = egueb_dom_node_ref(other->theme);
		if (w->last_parent_theme)
			egueb_dom_string_unref(w->last_parent_theme);
		egueb_dom_attr_final_get(other->theme, &theme);
		w->last_parent_theme = egueb_dom_string_dup(theme);
		// FIX this egueb_dom_string_unref(theme);
	}
	egueb_dom_attr_inheritable_process(e->theme, rel_theme);

	egueb_dom_node_unref(rel_theme);
	egueb_dom_node_unref(parent);

	egueb_dom_attr_final_get(e->theme, &theme);
	DBG_ELEMENT(n, "Last theme was '%s', new one is '%s'",
			egueb_dom_string_string_get(w->last_theme),
			egueb_dom_string_string_get(theme));
	/* check if the theme has changed, if so, request a new theme */
	if (!egueb_dom_string_is_equal(theme, w->last_theme))
	{
		if (w->last_theme)
			egueb_dom_string_unref(w->last_theme);
		/* ask for the theme */
		/* get the tag name and fetch the instance from it */
		DBG_ELEMENT(n, "Theme is '%s'", egueb_dom_string_string_get(theme));
		w->last_theme = egueb_dom_string_dup(theme);
	}
	// FIX this egueb_dom_string_unref(theme);

	/* load the theme */
	doc = egueb_dom_node_owner_document_get(n);
	if (!doc)
	{
		WARN_ELEMENT(n, "Node does not have a document");
		ret = EINA_FALSE;
		goto no_document;
	}

	topmost = egueb_dom_document_document_element_get(doc);
	if (!topmost)
	{
		WARN_ELEMENT(n, "Document does not have topmost element");
		ret = EINA_FALSE;
		goto no_topmost;
	}

	INFO_ELEMENT(n, "Loading theme");
	thiz->theme_document = eon_element_eon_theme_load(topmost, theme);
	if (thiz->theme_document)
	{
		Eon_Feature_Themable_Class *klass;
		Egueb_Dom_Node *theme_instance = NULL;

		klass = EON_FEATURE_THEMABLE_CLASS_GET(thiz);
		if (thiz->theme_instance_create)
		{
			theme_instance = thiz->theme_instance_create(thiz,
					thiz->theme_document, thiz->theme_instance_create_data);
		}

		if (!theme_instance && klass->theme_instance_create)
		{
			theme_instance = klass->theme_instance_create(thiz,
					thiz->theme_document);
		}

		if (!theme_instance)
		{
			Egueb_Dom_String *tag_name;

			tag_name = egueb_dom_element_tag_name_get(n);
			theme_instance = eon_theme_document_instance_new(
				thiz->theme_document,
				egueb_dom_string_string_get(tag_name),
				NULL);
		}
		if (theme_instance)
		{
			Enesim_Renderer *r;
			int enabled;

			INFO_ELEMENT(n, "Instance fetched correctly");
			thiz->theme_instance = theme_instance;
			/* get the drawer widget */
			thiz->theme_widget = eon_theme_element_instance_object_get(thiz->theme_instance);
			/* set the renderer */
			r = eon_drawer_widget_renderer_get(thiz->theme_widget);
			enesim_renderer_proxy_proxied_set(thiz->proxy, r);
			/* call the class method */
			if (klass->theme_instance_created)
				klass->theme_instance_created(thiz);
			/* Set the default state first */
			egueb_dom_attr_final_get(w->enabled, &enabled);
			eon_theme_element_instance_state_set(thiz->theme_instance,
					enabled ? "default" : "disabled", NULL);
		}
		else
		{
			ret = EINA_FALSE;
			goto no_theme;
		}
	}
	else
	{
		ret = EINA_FALSE;
		goto no_theme;
	}

no_theme:
	egueb_dom_node_unref(topmost);
no_topmost:
	egueb_dom_node_unref(doc);
no_document:
	egueb_dom_string_unref(theme);
	thiz->theme_changed = EINA_FALSE;

	return ret;
}
#endif
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
	thiz->theme_changed = EINA_TRUE;
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
	/* in case a node has been added/removed from a document make
	 * sure to seti/unset the new/old document too
	 */
	/* return it */
	return thiz->f;
}

/* TODO The theme id is set on the widget itself */
void eon_feature_themable_id_clear(Egueb_Dom_Feature *f)
{

}


void eon_feature_themable_id_add(Egueb_Dom_Feature *f)
{

}

Egueb_Dom_Node * eon_feature_themable_load(Egueb_Dom_Feature *f)
{
	/* process the theme attribute */
	/* check if the theme has changed */
	/* get the node theme */
	/* clone it */
	/* adopt it to the new document */
	/* return the newly created element */
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
