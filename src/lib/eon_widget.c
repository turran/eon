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
#include "eon_widget_private.h"

/* TODO
 * - When processing a widget we need to also process the theme instance
 *   associated with it
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Bool _eon_widget_load_theme(Eon_Widget *thiz)
{
	Eon_Element *e;
	Egueb_Dom_String *theme = NULL;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;

	if (!thiz->theme_changed)
		return;

	e = EON_ELEMENT(thiz);

	n = e->n;
	parent = egueb_dom_node_parent_get(n);
	if (parent)
	{
		Eon_Element *other;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(parent));
		egueb_dom_attr_inheritable_process(e->theme, other->theme);
		egueb_dom_node_unref(parent);
	}

	egueb_dom_attr_final_get(e->theme, &theme);
	printf("last theme is %s %s\n", egueb_dom_string_string_get(theme),
			egueb_dom_string_string_get(thiz->last_theme));
	/* check if the theme has changed, if so, request a new theme */
	if (!egueb_dom_string_is_equal(theme, thiz->last_theme))
	{
		if (thiz->last_theme)
		{
			egueb_dom_string_unref(thiz->last_theme);
		}
		/* ask for the theme */
		/* get the tag name and fetch the instance from it */
		printf("theme is %s\n", egueb_dom_string_string_get(theme));
		thiz->last_theme = egueb_dom_string_ref(theme);
	}
	egueb_dom_string_unref(theme);
	thiz->theme_changed = EINA_FALSE;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_widget_renderer_get(Eon_Renderable *r)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(r);
	return enesim_renderer_ref(thiz->proxy);
}

static void _eon_widget_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	printf("WIDGET geometry set\n");
}

static int _eon_widget_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;

	thiz = EON_WIDGET(r);
	klass = EON_WIDGET_CLASS_GET(thiz);
	_eon_widget_load_theme(thiz);
	if (klass->size_hints_get)
		return klass->size_hints_get(thiz, size);
	return 0;
}

static Eina_Bool _eon_widget_pre_process(Eon_Renderable *r)
{
	Eon_Element *e;
	Eon_Widget *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;

	printf("WIDGET pre process\n");

	thiz = EON_WIDGET(r);
	e = EON_ELEMENT(r);
	/* in case the theme value has been modifed, invalide the geometry */
	if (egueb_dom_attr_has_changed(e->theme))
	{
		ERR("Widget's theme is different");
		thiz->theme_changed = EINA_TRUE;
		eon_renderable_invalidate_geometry(r);
		return;
	}
	/* in case the parent's theme is different than the previous
	 * theme, incalidate it too
	 */
	n = e->n;
	parent = egueb_dom_node_parent_get(n);
	if (parent)
	{
		Eon_Element *other;
		Egueb_Dom_String *theme;

		other = EON_ELEMENT(egueb_dom_element_external_data_get(parent));
		egueb_dom_attr_final_get(other->theme, &theme);
		if (!egueb_dom_string_is_equal(theme, thiz->last_parent_theme))
		{
			ERR("Parent's theme is different than last one");
			thiz->theme_changed = EINA_TRUE;
			eon_renderable_invalidate_geometry(r);
		}
		egueb_dom_string_unref(theme);
		egueb_dom_node_unref(parent);
	}
}

static Eina_Bool _eon_widget_process(Eon_Renderable *r)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(r);
	printf("WIDGET process\n");
	_eon_widget_load_theme(thiz);
	/* propagate the geometry on the theme */
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Widget,
		Eon_Widget_Class, eon_widget);

static void _eon_widget_class_init(void *k)
{
	Eon_Element_Class *klass = EON_ELEMENT_CLASS(k);
	Eon_Renderable_Class *r_klass;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_widget_renderer_get;
	r_klass->size_hints_get = _eon_widget_size_hints_get;
	r_klass->geometry_set = _eon_widget_geometry_set;
	r_klass->pre_process = _eon_widget_pre_process;
	r_klass->process = _eon_widget_process;
}

static void _eon_widget_instance_init(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);
	/* instantiate the theme element */
	thiz->proxy = enesim_renderer_proxy_new();
}

static void _eon_widget_instance_deinit(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);
	if (thiz->last_theme)
		egueb_dom_string_unref(thiz->last_theme);
	if (thiz->last_parent_theme)
		egueb_dom_string_unref(thiz->last_parent_theme);
	if (thiz->proxy)
		enesim_renderer_unref(thiz->proxy);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
