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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Bool _eon_widget_load_theme(Eon_Widget *thiz)
{
	Eon_Element *e;
	Egueb_Dom_String *theme = NULL;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *parent;

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

static Eina_Bool _eon_widget_pre_setup(Eon_Renderable *r)
{
	/* in case the theme has changed, request the new one */
	return EINA_TRUE;
}

static int _eon_widget_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;

	thiz = EON_WIDGET(r);
	/* when requesting the size hints be sure to load the theme instance */
	_eon_widget_load_theme(thiz);
	klass = EON_WIDGET_CLASS_GET(thiz);

	if (klass->size_hints_get)
		return klass->size_hints_get(thiz, size);
	return 0;
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
	if (thiz->proxy)
		enesim_renderer_unref(thiz->proxy);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
