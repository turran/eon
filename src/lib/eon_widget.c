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
#include "eon_widget.h"

#include "eon_widget_private.h"
#include "eon_element_eon_private.h"
#include "eon_theme_document_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_widget_init(Eon_Renderable *r)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_WIDGET(r);
	n = (EON_ELEMENT(r))->n;

	/* private */
	/* attributes */
	thiz->enabled = egueb_dom_attr_boolean_new(egueb_dom_string_ref(EON_ATTR_ENABLED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->enabled, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_TRUE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->enabled), NULL);

	/* events */
	klass = EON_WIDGET_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

static Eina_Bool _eon_widget_pre_process(Eon_Renderable *r)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;

	thiz = EON_WIDGET(r);
	klass = EON_WIDGET_CLASS_GET(r);

	if (klass->pre_process)
	{
		if (!klass->pre_process(thiz))
			return EINA_FALSE;
	}
	return EINA_TRUE;
}

static Egueb_Dom_Node * _eon_widget_element_at(Eon_Renderable *r, Eina_Rectangle *cursor)
{
	Eon_Widget *thiz;
	Eon_Widget_Class *klass;
	Egueb_Dom_Node *n;
	int enabled;

	thiz = EON_WIDGET(r);
	egueb_dom_attr_final_get(thiz->enabled, &enabled);
	if (!enabled)
		return NULL;
	klass = EON_WIDGET_CLASS_GET(r);
	if (klass->element_at)
		return klass->element_at(thiz, cursor);

	n = (EON_ELEMENT(r))->n;	
	return egueb_dom_node_ref(n);
}

static Eina_Bool _eon_widget_is_focusable(Eon_Renderable *r)
{
	Eon_Widget *thiz;
	int enabled;

	thiz = EON_WIDGET(r);
	egueb_dom_attr_final_get(thiz->enabled, &enabled);
	if (!enabled)
		return EINA_FALSE;
	else
		return EINA_TRUE;
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
	r_klass->init = _eon_widget_init;
	r_klass->pre_process = _eon_widget_pre_process;
	r_klass->element_at = _eon_widget_element_at;
	r_klass->is_focusable = _eon_widget_is_focusable;
}

static void _eon_widget_instance_init(void *o)
{
}

static void _eon_widget_instance_deinit(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);

	/* attributes */
	egueb_dom_node_unref(thiz->enabled);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_widget_enabled_set(Egueb_Dom_Node *n, Eina_Bool enabled)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->enabled, EGUEB_DOM_ATTR_TYPE_BASE, enabled);
}

EAPI Eina_Bool eon_widget_enabled_get(Egueb_Dom_Node *n)
{
	Eon_Widget *thiz;
	Eina_Bool ret;

	thiz = EON_WIDGET(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->enabled, &ret);
	return ret;
}
