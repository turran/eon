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
#include "eon_element_separator.h"

#include "eon_widget_private.h"
#include "eon_feature_themable_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_separator_private.h"
#include "eon_orientation_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_SEPARATOR_DESCRIPTOR eon_element_separator_descriptor_get()
#define EON_ELEMENT_SEPARATOR_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,		\
		Eon_Element_Separator_Class, EON_ELEMENT_SEPARATOR_DESCRIPTOR)
#define EON_ELEMENT_SEPARATOR(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Separator, EON_ELEMENT_SEPARATOR_DESCRIPTOR)

typedef struct _Eon_Element_Separator
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *orientation;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *proxy;
} Eon_Element_Separator;

typedef struct _Eon_Element_Separator_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Separator_Class;

/*----------------------------------------------------------------------------*
 *                            Renderable interface                            *
 *----------------------------------------------------------------------------*/
static void _eon_element_separator_init(Eon_Renderable *r)
{
	Eon_Element_Separator *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_SEPARATOR(r);
	/* private */
	e = EON_ELEMENT(r);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_SEPARATOR));
	thiz->proxy = enesim_renderer_proxy_new();

	/* attributes */
	n = (EON_ELEMENT(r))->n;
	thiz->orientation = eon_orientation_attr_new();
	egueb_dom_attr_set(thiz->orientation, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			EON_ORIENTATION_HORIZONTAL);
	egueb_dom_element_attribute_node_set(n,
			egueb_dom_node_ref(thiz->orientation), NULL);
	/* features */
	thiz->theme_feature = eon_feature_themable_add(n);
	/* events */
}

static Enesim_Renderer * _eon_element_separator_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Separator *thiz;

	thiz = EON_ELEMENT_SEPARATOR(r);
	return enesim_renderer_ref(thiz->proxy);
}

static int _eon_element_separator_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Separator *thiz;
	Eon_Orientation orientation;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int thickness = 0;
	int ret = 0;

	thiz = EON_ELEMENT_SEPARATOR(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}

	thickness = eon_theme_element_separator_thickness_get(theme_element);
	egueb_dom_attr_final_get(thiz->orientation, &orientation);
	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	size->min_width = size->max_width = -1;
	size->min_height = size->max_height = -1;
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		size->min_height = thickness;
	}
	else
	{
		size->min_width = thickness;
	}

	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_separator_process(Eon_Renderable *r)
{
	Eon_Element_Separator *thiz;
	Egueb_Dom_Node *theme_element;

	thiz = EON_ELEMENT_SEPARATOR(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}
	/* Set the geometry on the child */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);
	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);
	
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_separator_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_SEPARATOR);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Separator,
		Eon_Element_Separator_Class, eon_element_separator);

static void _eon_element_separator_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_separator_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_separator_renderer_get;
	r_klass->size_hints_get = _eon_element_separator_size_hints_get;
	r_klass->process = _eon_element_separator_process;
	r_klass->init = _eon_element_separator_init;
}

static void _eon_element_separator_instance_init(void *o)
{
}

static void _eon_element_separator_instance_deinit(void *o)
{
	Eon_Element_Separator *thiz;

	thiz = EON_ELEMENT_SEPARATOR(o);
	/* attributes */
	egueb_dom_node_unref(thiz->orientation);
	/* private */
	egueb_dom_feature_unref(thiz->theme_feature);
	enesim_renderer_unref(thiz->proxy);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_separator_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_separator);
	return n;
}
