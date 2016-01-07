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
#include "eon_element_scale.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_theme_element_scale_private.h"
#include "eon_orientation_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_SCALE_DESCRIPTOR eon_element_scale_descriptor_get()
#define EON_ELEMENT_SCALE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Scale, EON_ELEMENT_SCALE_DESCRIPTOR)

typedef struct _Eon_Element_Scale
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *orientation;
	Egueb_Dom_Node *value;
	Egueb_Dom_Node *min;
	Egueb_Dom_Node *max;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *proxy;
} Eon_Element_Scale;

typedef struct _Eon_Element_Scale_Class
{
	Eon_Widget_Class base;
} Eon_Element_Scale_Class;

/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_scale_init(Eon_Widget *w)
{
	Eon_Element_Scale *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_SCALE(w);

	/* attributes */
	n = (EON_ELEMENT(w))->n;
	thiz->orientation = eon_orientation_attr_new();
	egueb_dom_attr_set(thiz->orientation, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			EON_ORIENTATION_HORIZONTAL);
	thiz->value = egueb_dom_attr_double_new(EON_NAME_ATTR_VALUE,
			NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	thiz->min = egueb_dom_attr_double_new(EON_NAME_ATTR_MIN,
			NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->min, EGUEB_DOM_ATTR_TYPE_DEFAULT, 0.0);
	thiz->max = egueb_dom_attr_double_new(EON_NAME_ATTR_MAX,
			NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->max, EGUEB_DOM_ATTR_TYPE_DEFAULT, 1.0);
	egueb_dom_element_attribute_node_set(n,
			egueb_dom_node_ref(thiz->orientation), NULL);
	egueb_dom_element_attribute_node_set(n,
			egueb_dom_node_ref(thiz->value), NULL);
	egueb_dom_element_attribute_node_set(n,
			egueb_dom_node_ref(thiz->min), NULL);
	egueb_dom_element_attribute_node_set(n,
			egueb_dom_node_ref(thiz->max), NULL);

	/* private */
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_SCALE));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_scale_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Scale *thiz;

	thiz = EON_ELEMENT_SCALE(r);
	return enesim_renderer_ref(thiz->proxy);
}

static int _eon_element_scale_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Scale *thiz;
	Eon_Orientation orientation;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int min_length = 0;
	int thickness = 0;
	int ret = 0;

	thiz = EON_ELEMENT_SCALE(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}

	/* Our own information */
	egueb_dom_attr_final_get(thiz->orientation, &orientation);

	/* Get the theme information */
	thickness = eon_theme_element_scale_thickness_get(theme_element);
	min_length = eon_theme_element_scale_min_length_get(theme_element);

	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		size->min_width = min_length;
		size->max_width = -1;
		size->min_height = size->max_height = thickness;
	}
	else
	{
		size->min_width = size->max_width = thickness;
		size->min_height = min_length;
		size->max_height = -1;
	}

	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_scale_process(Eon_Renderable *r)
{
	Eon_Element_Scale *thiz;
	Eon_Orientation orientation;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	double value;
	double min;
	double max;

	thiz = EON_ELEMENT_SCALE(r);
	theme_element = eon_feature_themable_element_get(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}
	/* Propagate the attributes */
	egueb_dom_attr_final_get(thiz->orientation, &orientation);
	egueb_dom_attr_final_get(thiz->value, &value);
	egueb_dom_attr_final_get(thiz->min, &min);
	egueb_dom_attr_final_get(thiz->max, &max);

	n = (EON_ELEMENT(r))->n;
	DBG_ELEMENT(n, "Min: %g, Current: %g, Max: %g", min, value, max);
	eon_theme_element_scale_orientation_set(theme_element, orientation);
	/* sanitize the value */
	if (value < min)
		value = min;
	if (value > max)
		value = max;

	if (min == max)
		value = 0;
	else
		value = (value - min)/(max - min);
	DBG_ELEMENT(n, "Finally setting %f", value);
	eon_theme_element_scale_value_set(theme_element, value);
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
static Egueb_Dom_String * _eon_element_scale_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_SCALE);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Scale,
		Eon_Element_Scale_Class, eon_element_scale);

static void _eon_element_scale_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_scale_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_scale_renderer_get;
	r_klass->size_hints_get = _eon_element_scale_size_hints_get;
	r_klass->process = _eon_element_scale_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_scale_init;
}

static void _eon_element_scale_instance_init(void *o)
{
}

static void _eon_element_scale_instance_deinit(void *o)
{
	Eon_Element_Scale *thiz;

	thiz = EON_ELEMENT_SCALE(o);
	/* attributes */
	egueb_dom_node_unref(thiz->orientation);
	egueb_dom_node_unref(thiz->value);
	egueb_dom_node_unref(thiz->min);
	egueb_dom_node_unref(thiz->max);
	/* private */
	enesim_renderer_unref(thiz->proxy);
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_scale_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_scale);
	return n;
}

EAPI void eon_element_scale_value_set(Egueb_Dom_Node *n, double v)
{
	Eon_Element_Scale *thiz;

	thiz = EON_ELEMENT_SCALE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->value, EGUEB_DOM_ATTR_TYPE_BASE, v);
}

EAPI double eon_element_scale_value_get(Egueb_Dom_Node *n)
{
	Eon_Element_Scale *thiz;
	double v;

	thiz = EON_ELEMENT_SCALE(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->value, &v);
	return v;
}
