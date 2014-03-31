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

/* Create an enum for every stock type label, i.e
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
#include "eon_stock.h"
#include "eon_element_label.h"
#include "eon_element_label.h"
#include "eon_element_label_stock.h"

#include "eon_stock_private.h"
#include "eon_widget_drawer_private.h"
#include "eon_widget_proxy_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_LABEL_STOCK_DESCRIPTOR eon_element_label_stock_descriptor_get()
#define EON_ELEMENT_LABEL_STOCK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Label_Stock, EON_ELEMENT_LABEL_STOCK_DESCRIPTOR)

typedef struct _Eon_Element_Label_Stock
{
	Eon_Widget_Proxy base;
	/* attributes */
	Egueb_Dom_Node *stock;
	/* private */
	Egueb_Dom_Node *label;
	Egueb_Dom_Node *text;
} Eon_Element_Label_Stock;

typedef struct _Eon_Element_Label_Stock_Class
{
	Eon_Widget_Proxy_Class base;
} Eon_Element_Label_Stock_Class;

static Egueb_Dom_Node * _eon_element_label_stock_theme_instance_create(
		Eon_Widget_Drawer *wd, Egueb_Dom_Node *theme_document,
		void *data)
{
	Eon_Element_Label_Stock *thiz = data;
	Eon_Stock stock;
	Egueb_Dom_Node *ret;
	Egueb_Dom_String *s;
	char *name = NULL;

	thiz = EON_ELEMENT_LABEL_STOCK(data);
	if (!egueb_dom_attr_final_string_get(thiz->stock, &s))
		return NULL;
	/* prepend the label */
	if (asprintf(&name, "label-%s", egueb_dom_string_string_get(s)) < 0)
	{
		egueb_dom_string_unref(s);
		return NULL;
	}

	ret = ender_document_instance_new(theme_document, name, NULL);
	free(name);

	return ret;
}

/*----------------------------------------------------------------------------*
 *                          Widget Proxy interface                            *
 *----------------------------------------------------------------------------*/
static void _eon_element_label_stock_init(Eon_Widget_Proxy *wp)
{
	Eon_Element_Label_Stock *thiz;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_LABEL_STOCK(wp);
	n = (EON_ELEMENT(wp))->n;

	thiz->stock = eon_stock_attr_new();
	egueb_dom_attr_set(thiz->stock, EGUEB_DOM_ATTR_TYPE_DEFAULT, EON_STOCK_OK);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->stock), NULL);
}

static Egueb_Dom_Node * _eon_element_label_stock_proxied_get(
		Eon_Widget_Proxy *wp)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(wp);
	return egueb_dom_node_ref(thiz->label);
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_element_label_stock_pre_process(Eon_Widget *w)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(w);
	/* in case the stock has changed, use the new label */
	if (egueb_dom_attr_has_changed(thiz->stock))
	{
		egueb_dom_character_data_append_data_inline(thiz->text, "hello", NULL);
		printf(">>>>>>>>> changed\n");
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_label_stock_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_LABEL_STOCK);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_PROXY_DESCRIPTOR,
		Eon_Element_Label_Stock, Eon_Element_Label_Stock_Class,
		eon_element_label_stock);

static void _eon_element_label_stock_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Widget_Class *w_klass;
	Eon_Widget_Proxy_Class *wp_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_label_stock_tag_name_get;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->pre_process = _eon_element_label_stock_pre_process;

	wp_klass = EON_WIDGET_PROXY_CLASS(k);
	wp_klass->init = _eon_element_label_stock_init;
	wp_klass->proxied_get = _eon_element_label_stock_proxied_get;
}

static void _eon_element_label_stock_instance_init(void *o)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(o);
	thiz->label = eon_element_label_new();
	thiz->text = egueb_dom_text_new();
	egueb_dom_node_child_append(thiz->label, egueb_dom_node_ref(thiz->text), NULL);

	eon_widget_drawer_theme_instance_create_set(thiz->label, _eon_element_label_stock_theme_instance_create, thiz);
}

static void _eon_element_label_stock_instance_deinit(void *o)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(o);
	egueb_dom_node_unref(thiz->stock);
	egueb_dom_node_unref(thiz->label);
	egueb_dom_node_unref(thiz->text);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_label_stock_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_label_stock);
	return n;
}

EAPI void eon_element_label_stock_type_set(Egueb_Dom_Node *n,
		Eon_Stock stock)
{

}

EAPI Eon_Stock eon_element_label_stock_type_get(Egueb_Dom_Node *n)
{

}

