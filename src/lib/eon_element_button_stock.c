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

/* Create an enum for every stock type button, i.e
 * - OK
 * - CANCEL
 * - HELP
 * - APPLY
 * - ...
 * Define how are we going to handle the meta-widgets, that is
 * widgets that use others for drawing/interacting.
 * Theme propagation?
 * How to make a specific owned widget use a specific theme?
 */
#include "eon_private.h"
#include "eon_main.h"
#include "eon_element_button.h"
#include "eon_element_label.h"

#include "eon_widget_drawer_private.h"
#include "eon_widget_proxy_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_BUTTON_STOCK_DESCRIPTOR eon_element_button_stock_descriptor_get()
#define EON_ELEMENT_BUTTON_STOCK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Button_Stock, EON_ELEMENT_BUTTON_STOCK_DESCRIPTOR)

typedef struct _Eon_Element_Button_Stock
{
	Eon_Widget_Proxy base;
	Egueb_Dom_Node *button;
	Egueb_Dom_Node *label;
	Egueb_Dom_Node *text;
} Eon_Element_Button_Stock;

typedef struct _Eon_Element_Button_Stock_Class
{
	Eon_Widget_Proxy_Class base;
} Eon_Element_Button_Stock_Class;

static Egueb_Dom_Node * _eon_element_button_stock_theme_instance_create(
		Eon_Widget_Drawer *thiz, Egueb_Dom_Node *theme_document)
{
	Egueb_Dom_Node *ret;

	ret = ender_document_instance_new(theme_document, "buttonOk", NULL);
	return ret;
}

/*----------------------------------------------------------------------------*
 *                          Widget Proxy interface                            *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_element_button_stock_proxied_get(
		Eon_Widget_Proxy *wp)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(wp);
	return egueb_dom_node_ref(thiz->button);
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_button_stock_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_BUTTON_STOCK);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_PROXY_DESCRIPTOR,
		Eon_Element_Button_Stock, Eon_Element_Button_Stock_Class,
		eon_element_button_stock);

static void _eon_element_button_stock_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Widget_Proxy_Class *wp_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_button_stock_tag_name_get;

	wp_klass = EON_WIDGET_PROXY_CLASS(k);
	wp_klass->proxied_get = _eon_element_button_stock_proxied_get;
}

static void _eon_element_button_stock_instance_init(void *o)
{
	Eon_Element_Button_Stock *thiz;
	Eon_Widget_Drawer *wd;

	thiz = EON_ELEMENT_BUTTON_STOCK(o);
	thiz->button = eon_element_button_new();
	thiz->label = eon_element_label_new();
	thiz->text = egueb_dom_text_new();
	egueb_dom_node_child_append(thiz->button, egueb_dom_node_ref(thiz->label), NULL);
	egueb_dom_node_child_append(thiz->label, egueb_dom_node_ref(thiz->text), NULL);

	wd = EON_WIDGET_DRAWER(egueb_dom_element_external_data_get(thiz->button));
	wd->theme_instance_create = _eon_element_button_stock_theme_instance_create;
	/* TODO set the theme instance create callback */
}

static void _eon_element_button_stock_instance_deinit(void *o)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(o);
	egueb_dom_node_unref(thiz->button);
	egueb_dom_node_unref(thiz->label);
	egueb_dom_node_unref(thiz->text);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_button_stock_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_button_stock);
	return n;
}

