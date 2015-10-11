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
#include "eon_stock.h"
#include "eon_element_button.h"
#include "eon_element_label_stock.h"
#include "eon_element_button_stock.h"

#include "eon_stock_private.h"
#include "eon_widget_drawer_private.h"
#include "eon_widget_proxy_private.h"
#include "eon_theme_document_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_BUTTON_STOCK_DESCRIPTOR eon_element_button_stock_descriptor_get()
#define EON_ELEMENT_BUTTON_STOCK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Button_Stock, EON_ELEMENT_BUTTON_STOCK_DESCRIPTOR)

typedef struct _Eon_Element_Button_Stock
{
	Eon_Widget_Proxy base;
	/* attributes */
	Egueb_Dom_Node *stock;
	/* private */
	Egueb_Dom_Node *button;
	Egueb_Dom_Node *label;
} Eon_Element_Button_Stock;

typedef struct _Eon_Element_Button_Stock_Class
{
	Eon_Widget_Proxy_Class base;
} Eon_Element_Button_Stock_Class;

static Egueb_Dom_Node * _eon_element_button_stock_theme_instance_create(
		Eon_Widget_Drawer *wd, Egueb_Dom_Node *theme_document,
		void *data)
{
	Eon_Element_Button_Stock *thiz = data;
	Eon_Stock stock;
	Egueb_Dom_Node *ret;
	Egueb_Dom_String *s;
	char *name = NULL;

	thiz = EON_ELEMENT_BUTTON_STOCK(data);
	if (!egueb_dom_attr_final_string_get(thiz->stock, &s))
		return NULL;
	/* prepend the button */
	if (asprintf(&name, "button-%s", egueb_dom_string_string_get(s)) < 0)
	{
		egueb_dom_string_unref(s);
		return NULL;
	}

	ret = eon_theme_document_instance_new(theme_document, name, NULL);
	free(name);

	return ret;
}

/*----------------------------------------------------------------------------*
 *                          Widget Proxy interface                            *
 *----------------------------------------------------------------------------*/
static void _eon_element_button_stock_init(Eon_Widget_Proxy *wp)
{
	Eon_Element_Button_Stock *thiz;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_BUTTON_STOCK(wp);
	n = (EON_ELEMENT(wp))->n;

	thiz->stock = eon_stock_attr_new();
	egueb_dom_attr_set(thiz->stock, EGUEB_DOM_ATTR_TYPE_DEFAULT, EON_STOCK_OK);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->stock), NULL);
}

static Egueb_Dom_Node * _eon_element_button_stock_proxied_get(
		Eon_Widget_Proxy *wp)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(wp);
	return egueb_dom_node_ref(thiz->button);
}

/* We need to propagate the 'stock' value because it will change the hints
 * of the element
 */
static void _eon_element_button_stock_geometry_propagate(Eon_Widget_Proxy *wp)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(wp);
	if (egueb_dom_attr_has_changed(thiz->stock))
	{
		Eon_Stock stock;

		egueb_dom_attr_final_get(thiz->stock, &stock);
		egueb_dom_element_attribute_masked_set(thiz->label, EON_STOCK,
				EGUEB_DOM_ATTR_TYPE_BASE, stock);
		/* FIXME mark it as processed */
	}
}

/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_element_button_stock_pre_process(Eon_Widget *w)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(w);
	return EINA_TRUE;
}
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
	Eon_Widget_Class *w_klass;
	Eon_Widget_Proxy_Class *wp_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_button_stock_tag_name_get;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->pre_process = _eon_element_button_stock_pre_process;

	wp_klass = EON_WIDGET_PROXY_CLASS(k);
	wp_klass->init = _eon_element_button_stock_init;
	wp_klass->proxied_get = _eon_element_button_stock_proxied_get;
	wp_klass->geometry_propagate = _eon_element_button_stock_geometry_propagate;
}

static void _eon_element_button_stock_instance_init(void *o)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(o);
	thiz->button = eon_element_button_new();
	thiz->label = eon_element_label_stock_new();
	egueb_dom_node_child_append(thiz->button, egueb_dom_node_ref(thiz->label), NULL);

	eon_widget_drawer_theme_instance_create_set(thiz->button, _eon_element_button_stock_theme_instance_create, thiz);
}

static void _eon_element_button_stock_instance_deinit(void *o)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(o);
	egueb_dom_node_unref(thiz->stock);
	egueb_dom_node_unref(thiz->button);
	if (thiz->label)
	{
		egueb_dom_node_unref(thiz->label);
		thiz->label = NULL;
	}
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

EAPI void eon_element_button_stock_stock_set(Egueb_Dom_Node *n,
		Eon_Stock stock)
{

}

EAPI Eon_Stock eon_element_button_stock_stock_get(Egueb_Dom_Node *n)
{

}

