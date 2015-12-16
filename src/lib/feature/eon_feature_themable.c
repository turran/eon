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
#include "eon_theme_name.h"

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
	Eon_Theme_Name last_theme;
} Eon_Feature_Themable;

typedef struct _Eon_Feature_Themable_Class
{
	Enesim_Object_Class base;
} Eon_Feature_Themable_Class;

typedef struct _Eon_Feature_Themable_Theme_Id_Foreach_Data
{
	Eon_Feature_Themable *thiz;
	Egueb_Dom_Node *theme_doc;
	Egueb_Dom_Node *theme_element;
} Eon_Feature_Themable_Theme_Id_Foreach_Data;

static Eina_Bool _eon_feature_themable_load_theme_id_cb(void *data, void *user_data)
{
	Eon_Feature_Themable_Theme_Id_Foreach_Data *fdata = user_data;
	Eon_Feature_Themable *thiz = fdata->thiz;
	Egueb_Dom_Node *theme_doc = fdata->theme_doc;
	Egueb_Dom_Node *theme_element;
	Egueb_Dom_String *theme_id = data;

	theme_element = egueb_dom_document_element_get_by_id(theme_doc, theme_id, NULL);
	DBG_ELEMENT(thiz->n, "Loading theme id '%s'",
			egueb_dom_string_string_get(theme_id));
	if (theme_element)
	{
		DBG_ELEMENT(thiz->n, "Theme element for id '%s' found",
				egueb_dom_string_string_get(theme_id));
		fdata->theme_element = theme_element;
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

static void _eon_feature_themable_event_propagate_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Feature_Themable *thiz = data;

	if (!thiz->theme_element)
		return;
	Egueb_Dom_String *s;
	s = egueb_dom_event_type_get(e);
	egueb_dom_node_event_propagate(thiz->theme_element, e);
}

/*
 * Some events need to arrive to the topmost element
 * The smil timeline event to make the topmost element provide a timeline
 * The mutation events in case we need to process again the element
 * The request process to process again the element
 */
static void _eon_feature_themable_event_monitor_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Feature_Themable *thiz = data;

	if (egueb_smil_event_is_timeline(e))
	{
		DBG("Theme element requesting a timeline");
		egueb_dom_node_event_propagate(thiz->n, e);
		return;
	}
	else if (egueb_dom_event_is_mutation(e))
	{
		Egueb_Dom_String *type;
		type = egueb_dom_event_type_get(e);
		if (type == EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED ||
				type == EGUEB_DOM_EVENT_MUTATION_CHARACTER_DATA_MODIFIED)
		{
			DBG("Theme element triggered a mutation event");
			egueb_dom_node_event_propagate(thiz->n, e);
		}
		egueb_dom_string_unref(type);
	}
	else if (egueb_dom_event_is_process(e))
	{
		DBG("Theme element triggered a process event");
		egueb_dom_node_event_propagate(thiz->n, e);
		return;
	}
#if 0
	else if (eon_event_is_geometry_invalidate(e))
	{
		DBG("Theme element requests a size invalidation");

	}
#endif
}
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
	egueb_dom_node_unref(thiz->theme_element);
	eon_theme_name_reset(&thiz->last_theme);
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
	Egueb_Dom_Event_Target *et;

	/* create the feature */
	thiz = ENESIM_OBJECT_INSTANCE_NEW(eon_feature_themable);
	thiz->n = n;
	thiz->f = egueb_dom_feature_external_new(&_descriptor, thiz);
	/* assign it */
	egueb_dom_node_feature_add(n, NULL, NULL, egueb_dom_feature_ref(thiz->f));
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	/* TODO in case a node has been added/removed from a document make
	 * sure to set/unset the new/old document too
	 */
	/* add events for every 'state' that needs to be propagated
	 * like mousein, mouseodown, click, focusin, etc
	 */
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_CLICK,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_DOWN,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_UP,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_OVER,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_MOVE,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MOUSE_OUT,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_FOCUS_IN,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_FOCUS_OUT,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
	/* return it */
	return thiz->f;
}

Egueb_Dom_Node * eon_feature_themable_load(Egueb_Dom_Feature *f)
{
	Eon_Feature_Themable *thiz;
	Eon_Element *e;
	Eon_Theme_Name curr_theme = { 0, 0 };
	Egueb_Dom_Node *ret = NULL;
	Egueb_Dom_List *curr_theme_id = NULL;
	Eina_Bool theme_changed = EINA_FALSE;

	thiz = EON_FEATURE_THEMABLE(egueb_dom_feature_external_data_get(f));
	e = EON_ELEMENT(egueb_dom_element_external_data_get(thiz->n));

	/* check if the theme has changed */
	egueb_dom_attr_final_get(e->theme, &curr_theme);
	egueb_dom_attr_final_get(e->theme_id, &curr_theme_id);
	if (!eon_theme_name_is_equal(&curr_theme, &thiz->last_theme))
	{
		DBG_ELEMENT(thiz->n, "Theme changed"); 
		theme_changed = EINA_TRUE;
		goto changed;
	}

	if (egueb_dom_attr_has_changed(e->theme_id))
	{
		DBG_ELEMENT(thiz->n, "Theme id changed"); 
		theme_changed = EINA_TRUE;
		goto changed;
	}
	
changed:
	if (theme_changed)
	{
		Eon_Feature_Themable_Theme_Id_Foreach_Data theme_id_fdata;
		Egueb_Dom_Node *theme_doc;
		Egueb_Dom_Node *theme_element;
		Egueb_Dom_Node *doc;

		/* Make sure to process the attr to not enter again */
		egueb_dom_attr_process(e->theme_id);
		/* FIXME it might be renderables that must not trigger and invalidate geometry i.e element_eon
		 */
		/* for element that are renderable, make sure to invalidate
		 * the geometry
		 */
		if (eon_is_renderable(thiz->n))
		{
			eon_renderable_invalidate_geometry(thiz->n);
		}


		if (thiz->theme_element)
		{
			egueb_dom_event_target_monitor_remove(
					EGUEB_DOM_EVENT_TARGET(thiz->theme_element),
					_eon_feature_themable_event_monitor_cb, thiz);
			egueb_dom_node_unref(thiz->theme_element);
			thiz->theme_element = NULL;
		}

		/* swap */
		eon_theme_name_reset(&thiz->last_theme);
		eon_theme_name_copy(&curr_theme, &thiz->last_theme, EINA_TRUE);

		/* get the theme node */
		switch (curr_theme.type)
		{
			case EON_THEME_NAME_TYPE_CURRENT_DOCUMENT:
			INFO_ELEMENT(thiz->n, "Loading theme from current document");
			theme_doc = egueb_dom_node_owner_document_get(thiz->n);
			break;

			case EON_THEME_NAME_TYPE_ENVIRONMENT:
			{
				Egueb_Dom_String *theme_name;
				char *default_theme;

				default_theme = getenv("EON_THEME");
				if (!default_theme)
				{
					default_theme = "basic";
				}
				INFO_ELEMENT(thiz->n, "Loading theme from envvar '%s'", default_theme);
				theme_name = egueb_dom_string_new_with_static_string(default_theme);
				theme_doc = eon_theme_document_load(theme_name);
				egueb_dom_string_unref(theme_name);
			}
			break;

			case EON_THEME_NAME_TYPE_CUSTOM:
			INFO_ELEMENT(thiz->n, "Loading custom theme'%s'",
					egueb_dom_string_string_get(curr_theme.custom));
			theme_doc = eon_theme_document_load(curr_theme.custom);
			break;

			default:
			ERR_ELEMENT(thiz->n, "Invalid theme name");
			goto done;
		}
		if (!theme_doc)
		{
			ERR_ELEMENT(thiz->n, "Theme doc does not exist");
			goto done;
		}

		theme_id_fdata.thiz = thiz;
		theme_id_fdata.theme_doc = theme_doc;
		egueb_dom_list_foreach(curr_theme_id, _eon_feature_themable_load_theme_id_cb, &theme_id_fdata);

		egueb_dom_node_unref(theme_doc);
		if (!theme_id_fdata.theme_element)
		{
			ERR_ELEMENT(thiz->n, "Theme id does not exist");
			goto done;
		}
		/* clone it */
		doc = egueb_dom_node_owner_document_get(thiz->n);
		if (!doc)
		{
			WARN_ELEMENT(thiz->n, "No document set");
			egueb_dom_node_unref(theme_id_fdata.theme_element);
			goto done;
		}
		theme_element = egueb_dom_node_clone(theme_id_fdata.theme_element, EINA_FALSE, EINA_TRUE, NULL);
		egueb_dom_node_unref(theme_id_fdata.theme_element);
		if (!theme_element)
		{
			WARN_ELEMENT(thiz->n, "Can not clone");
			goto done;

		}
		/* adopt it to the new document */
		thiz->theme_element = egueb_dom_document_node_adopt(doc, theme_element, NULL);
		if (!thiz->theme_element)
		{
			WARN_ELEMENT(thiz->n, "Can not adopt");
			goto done;

		}
		egueb_dom_element_enqueue(egueb_dom_node_ref(thiz->theme_element));
		egueb_dom_node_unref(doc);
		/* add the monitor */
		egueb_dom_event_target_monitor_add(EGUEB_DOM_EVENT_TARGET(thiz->theme_element),
				_eon_feature_themable_event_monitor_cb, thiz);
	}
done:
	eon_theme_name_reset(&curr_theme);
	egueb_dom_list_unref(curr_theme_id);
	return egueb_dom_node_ref(thiz->theme_element);
}

Egueb_Dom_Node * eon_feature_themable_element_get(Egueb_Dom_Feature *f)
{
	Eon_Feature_Themable *thiz;

	thiz = egueb_dom_feature_external_data_get(f);
	return egueb_dom_node_ref(thiz->theme_element);
}

void eon_feature_themable_event_propagate(Egueb_Dom_Feature *f,
		Egueb_Dom_String *ev)
{
	Eon_Feature_Themable *thiz;
	Egueb_Dom_Event_Target *et;

	thiz = egueb_dom_feature_external_data_get(f);
	et = EGUEB_DOM_EVENT_TARGET(thiz->n);
	egueb_dom_event_target_event_listener_add(et, ev,
			_eon_feature_themable_event_propagate_cb,
			EINA_FALSE, thiz);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
