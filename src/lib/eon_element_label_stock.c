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
#include "eon_element_label.h"
#include "eon_element_label.h"
#include "eon_element_label_stock.h"

#include "eon_stock_private.h"
#include "eon_widget_drawer_private.h"
#include "eon_widget_proxy_private.h"
#include "eon_theme_document_private.h"
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

	ret = eon_theme_document_instance_new(theme_document, name, NULL);
	free(name);

	return ret;
}

/* Whenever the 'stock' attribute is modifed be sure to invalidate the
 * geometry. A new 'stock' value implies a new geometry
 */
static void _eon_element_label_stock_attr_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Label_Stock *thiz = data;
	Egueb_Dom_Node *attr;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;

	attr = egueb_dom_event_mutation_related_get(e);
	if (thiz->stock == attr)
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Stock changed, "
				"invalidating geometry");
		eon_renderable_invalidate_geometry(EON_RENDERABLE(thiz));
	}
	egueb_dom_node_unref(attr);
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
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->stock), NULL);

	egueb_dom_event_target_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_label_stock_attr_modified_cb,
			EINA_FALSE, thiz);
}

static Egueb_Dom_Node * _eon_element_label_stock_proxied_get(
		Eon_Widget_Proxy *wp)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(wp);
	return egueb_dom_node_ref(thiz->label);
}

static void _eon_element_label_stock_geometry_propagate(Eon_Widget_Proxy *wp)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(wp);
	/* in case the stock has changed, use the new label */
	if (egueb_dom_attr_has_changed(thiz->stock))
	{
		Egueb_Dom_String *s;

		egueb_dom_attr_final_string_get(thiz->stock, &s);
		/* FIXME freeze the node to not send any event, this will skip the mutation */
		egueb_dom_character_data_data_append(thiz->text, s, NULL);
		egueb_dom_string_unref(s);
		/* FIXME thaw back */
		/* FIXME mark it as processed */
	}
}
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_element_label_stock_pre_process(Eon_Widget *w)
{
	Eon_Element_Label_Stock *thiz;
	Eon_Renderable *r;
	Eon_Element *proxied;

	thiz = EON_ELEMENT_LABEL_STOCK(w);
	proxied = egueb_dom_element_external_data_get(thiz->label);

	/* renderable attributes */
	r = EON_RENDERABLE(w);
	if (egueb_dom_attr_has_changed(r->halign))
	{
		Eon_Renderable *p_r;
		Eon_Horizontal_Align halign;

		p_r = EON_RENDERABLE(proxied);
		egueb_dom_attr_final_get(r->halign, &halign);
		/* FIXME freeze the node to not send any event, this will skip the mutation */
		egueb_dom_attr_set(p_r->halign, EGUEB_DOM_ATTR_TYPE_BASE, halign);
		/* FIXME thaw back */
		/* FIXME mark it as processed */
	}

	if (egueb_dom_attr_has_changed(r->valign))
	{
		Eon_Renderable *p_r;
		Eon_Vertical_Align valign;

		p_r = EON_RENDERABLE(proxied);
		egueb_dom_attr_final_get(r->valign, &valign);
		/* FIXME freeze the node to not send any event, this will skip the mutation */
		egueb_dom_attr_set(p_r->valign, EGUEB_DOM_ATTR_TYPE_BASE, valign);
		/* FIXME thaw back */
		/* FIXME mark it as processed */
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
	wp_klass->geometry_propagate = _eon_element_label_stock_geometry_propagate;
}

static void _eon_element_label_stock_instance_init(void *o)
{
	Eon_Element_Label_Stock *thiz;

	thiz = EON_ELEMENT_LABEL_STOCK(o);
	thiz->label = eon_element_label_new();
	eon_renderable_halign_set(thiz->label, EON_HORIZONTAL_ALIGN_CENTER);
	eon_renderable_valign_set(thiz->label, EON_VERTICAL_ALIGN_MIDDLE);
	thiz->text = egueb_dom_text_new();

	egueb_dom_node_child_append(thiz->label, egueb_dom_node_ref(thiz->text), NULL);
	/* Set the function to be able to set our own theme */
	eon_widget_drawer_theme_instance_create_set(thiz->label,
			_eon_element_label_stock_theme_instance_create, thiz);
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

