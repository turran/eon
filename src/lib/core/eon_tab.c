/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
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
#include "Eon.h"
#include "eon_private.h"
#include "Eon_Theme.h"
#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_TAB_CHILD;
static Ender_Property *EON_TAB_CHILD_CONTENT;

typedef struct _Eon_Tab_Child
{
	Eina_Bool changed : 1;
	Ender_Element *e;
	Ender_Element *content;
} Eon_Tab_Child;

typedef struct _Eon_Tab
{
	/* properties */
	Eina_List *children;
	/* private */
	Eina_Bool changed : 1;
} Eon_Tab;

static inline Eon_Tab * _eon_tab_get(Eon_Element *ee)
{
	Eon_Tab *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}

static void _eon_tab_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	/* whenever we click on it we should change the theme content based on what tab we have clicked */
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_tab_initialize(Ender_Element *e)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK], _eon_tab_mouse_click, NULL);
}

static Eina_Bool _eon_tab_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Tab *thiz;
	Eon_Tab_Child *ech;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);

	EINA_LIST_FOREACH(thiz->children, l, ech)
	{
		/* we should set the size of every descriptor based on its min/max/preferred size */
		/* while iterating if the current tab is selected set the content of the theme too */
	}
	thiz->changed = EINA_FALSE;

	return EINA_TRUE;
}

static Eina_Bool _eon_tab_needs_setup(Ender_Element *e)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);
	return thiz->changed;
}

static void _eon_tab_free(Eon_Element *ee)
{
	Eon_Tab *thiz;

	thiz = _eon_tab_get(ee);
	free(thiz);
}

static double _eon_tab_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	return 0;
}

static double _eon_tab_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	return 0;
}

static double _eon_tab_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	return 0;
}

static double _eon_tab_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	return 0;
}

static double _eon_tab_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	return 0;
}

static double _eon_tab_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_tab_get(ee);

	return 0;
}

static Eon_Widget_Descriptor _eon_tab_widget_descriptor = {
	.name = "tab",
	.initialize = _eon_tab_initialize,
	.free = _eon_tab_free,
	.setup = _eon_tab_setup,
	.needs_setup = _eon_tab_needs_setup,
	.min_width_get = _eon_tab_min_width_get,
	.max_width_get = _eon_tab_max_width_get,
	.min_height_get = _eon_tab_min_height_get,
	.max_height_get = _eon_tab_max_height_get,
	.preferred_width_get = _eon_tab_preferred_width_get,
	.preferred_height_get = _eon_tab_preferred_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_tab_new(void)
{
	Eon_Tab *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Tab));
	if (!thiz) return NULL;
	/* default values */

	ee = eon_widget_new(&_eon_tab_widget_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_tab_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Tab *thiz;
	Eon_Tab_Child *ech;
	Eina_List *l, *l_next;

	thiz = _eon_tab_get(ee);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, ech)
	{
		if (ech->e == child)
		{
			thiz->children = eina_list_remove_list(thiz->children, l);
			thiz->changed = EINA_TRUE;
			break;
		}
	}
}

static void _eon_tab_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Tab *thiz;
	Eon_Tab_Child *ech;

	thiz = _eon_tab_get(ee);
	ech = calloc(1, sizeof(Eon_Tab_Child));
	ech->e = child;

	thiz->children = eina_list_append(thiz->children, ech);
	thiz->changed = EINA_TRUE;
}

static void _eon_tab_child_clear(Eon_Element *ee)
{
	Eon_Tab *thiz;

	thiz = _eon_tab_get(ee);
	thiz->changed = EINA_TRUE;
}

static void _eon_tab_child_content_set(Eon_Element *ee, Ender_Element *child,
		Ender_Element *content)
{
	Eon_Tab *thiz;
	Eon_Tab_Child *ech;
	Eina_List *l;

	thiz = _eon_tab_get(ee);
	EINA_LIST_FOREACH(thiz->children, l, ech)
	{
		if (ech->e == child)
		{
			ech->content = content;
			ech->changed = EINA_TRUE;
			break;
		}
	}
}

static void _eon_tab_child_selected_set(Eon_Element *ee, Ender_Element *child,
		Eina_Bool selected)
{
	Eon_Tab *thiz;
	Eon_Tab_Child *ech;
	Eina_List *l;

	thiz = _eon_tab_get(ee);
	EINA_LIST_FOREACH(thiz->children, l, ech)
	{
		if (ech->e == child)
		{
			break;
		}
	}
}

#define _eon_tab_child_get NULL
#define _eon_tab_child_set NULL
#define _eon_tab_child_content_get NULL
#include "eon_generated_tab.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_tab_new(void)
{
	return ender_element_new_with_namespace("tab", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_tab_child_add_with_label(Ender_Element *e, const char *text)
{
	Ender_Element *child;

	child = eon_label_new_with_text(text);
	eon_tab_child_add(e, child);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_tab_child_add(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_add(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_tab_child_remove(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_remove(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_tab_child_clear(Ender_Element *e)
{
	ender_element_value_clear(e, "child");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_tab_child_content_set(Ender_Element *e, Ender_Element *child, Ender_Element *content)
{
	ender_element_value_set(child, "child_content", content, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_tab_child_selected_set(Ender_Element *e, Ender_Element *child, Eina_Bool selected)
{
	ender_element_value_set(child, "child_selected", selected, NULL);
}

