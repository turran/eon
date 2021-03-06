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
#include "eon_stock.h"
#include "eon_element_button.h"
#include "eon_element_label_stock.h"
#include "eon_element_button_stock.h"

#include "eon_widget_private.h"
#include "eon_stock_private.h"
#include "eon_theme_document_private.h"
#include "eon_feature_proxy_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_BUTTON_STOCK_DESCRIPTOR eon_element_button_stock_descriptor_get()
#define EON_ELEMENT_BUTTON_STOCK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Button_Stock, EON_ELEMENT_BUTTON_STOCK_DESCRIPTOR)

typedef struct _Eon_Element_Button_Stock
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *stock;
	/* private */
	Egueb_Dom_Feature *proxy;
	Egueb_Dom_Node *button;
	Egueb_Dom_Node *label;
} Eon_Element_Button_Stock;

typedef struct _Eon_Element_Button_Stock_Class
{
	Eon_Widget_Class base;
} Eon_Element_Button_Stock_Class;

/* Whenever the 'stock' attribute is modifed be sure to invalidate the
 * geometry. A new 'stock' value implies a new geometry
 */
static void _eon_element_button_stock_attr_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Button_Stock *thiz = data;
	Egueb_Dom_Node *attr;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;

	attr = egueb_dom_event_mutation_related_get(e);
	if (thiz->stock == attr)
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Stock changed, "
				"invalidating geometry");
		eon_renderable_invalidate_geometry((EON_ELEMENT(thiz))->n);
	}
	egueb_dom_node_unref(attr);
}

/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_button_stock_init(Eon_Widget *w)
{
	Eon_Element_Button_Stock *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_BUTTON_STOCK(w);
	n = (EON_ELEMENT(w))->n;

	/* private */
	thiz->proxy = eon_feature_proxy_add(n, thiz->button);

	/* attributes */
	thiz->stock = eon_stock_attr_new();
	egueb_dom_attr_set(thiz->stock, EGUEB_DOM_ATTR_TYPE_DEFAULT, EON_STOCK_OK);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->stock), NULL);

	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_button_stock_attr_modified_cb,
			EINA_FALSE, thiz);
}

static Eina_Bool _eon_element_button_stock_pre_process(Eon_Widget *w)
{
	Eon_Element_Button_Stock *thiz;
	Eon_Renderable *r;
	Eon_Element *proxied;

	thiz = EON_ELEMENT_BUTTON_STOCK(w);
	proxied = egueb_dom_element_external_data_get(thiz->button);

	/* element attributes */
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

	if (egueb_dom_attr_has_changed(r->hexpand))
	{
		Eon_Renderable *p_r;
		Eina_Bool hexpand;

		p_r = EON_RENDERABLE(proxied);
		egueb_dom_attr_final_get(r->hexpand, &hexpand);
		/* FIXME freeze the node to not send any event, this will skip the mutation */
		egueb_dom_attr_set(p_r->valign, EGUEB_DOM_ATTR_TYPE_BASE, hexpand);
		/* FIXME thaw back */
		/* FIXME mark it as processed */
	}

	if (egueb_dom_attr_has_changed(r->vexpand))
	{
		Eon_Renderable *p_r;
		Eina_Bool vexpand;

		p_r = EON_RENDERABLE(proxied);
		egueb_dom_attr_final_get(r->vexpand, &vexpand);
		/* FIXME freeze the node to not send any event, this will skip the mutation */
		egueb_dom_attr_set(p_r->valign, EGUEB_DOM_ATTR_TYPE_BASE, vexpand);
		/* FIXME thaw back */
		/* FIXME mark it as processed */
	}
	/* widget attributes */

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_button_stock_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(r);
	return eon_renderable_renderer_get(thiz->button);
}

static int _eon_element_button_stock_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(r);
	/* We need to propagate the 'stock' value because it will change the
	 * hints of the element
	 */
	if (egueb_dom_attr_has_changed(thiz->stock))
	{
		Eon_Stock stock;
		Egueb_Dom_String *s;
		Eon_Element *e;
		char *name = NULL;

		/* propagate the attributes */
		egueb_dom_attr_final_get(thiz->stock, &stock);
		egueb_dom_element_attribute_masked_set(thiz->label, EON_STOCK,
				EGUEB_DOM_ATTR_TYPE_BASE, stock);
		egueb_dom_attr_final_string_get(thiz->stock, &s);

		/* clear the list of theme-ids and set the new one */
		e = EON_ELEMENT(egueb_dom_element_external_data_get(thiz->button));
		egueb_dom_attr_string_list_clear(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT);
		egueb_dom_attr_string_list_prepend(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_BUTTON));
		egueb_dom_attr_string_list_prepend(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_BUTTON_STOCK));
		/* prepend the stock */
		if (asprintf(&name, "stockButton-%s", egueb_dom_string_chars_get(s)) < 0)
		{
			egueb_dom_string_unref(s);
			return 0;
		}
		egueb_dom_string_unref(s);
		s = egueb_dom_string_steal(name);
		egueb_dom_attr_string_list_prepend(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT, s);
	}
	return eon_renderable_size_hints_get(thiz->button, size);
}

static Eina_Bool _eon_element_button_stock_process(Eon_Renderable *r)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(r);
	eon_renderable_geometry_set(thiz->button, &r->geometry);
	egueb_dom_element_process(thiz->button);
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_button_stock_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_BUTTON_STOCK);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR,
		Eon_Element_Button_Stock, Eon_Element_Button_Stock_Class,
		eon_element_button_stock);

static void _eon_element_button_stock_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_button_stock_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_button_stock_renderer_get;
	r_klass->size_hints_get = _eon_element_button_stock_size_hints_get;
	r_klass->process = _eon_element_button_stock_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_button_stock_init;
	w_klass->pre_process = _eon_element_button_stock_pre_process;
}

static void _eon_element_button_stock_instance_init(void *o)
{
	Eon_Element_Button_Stock *thiz;
	Eon_Element *e;

	thiz = EON_ELEMENT_BUTTON_STOCK(o);
	thiz->button = eon_element_button_new();
	/* prepend the 'stockButton' theme id */
	e = EON_ELEMENT(egueb_dom_element_external_data_get(thiz->button));
	egueb_dom_attr_string_list_prepend(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_BUTTON_STOCK));

	thiz->label = eon_element_label_stock_new();
	egueb_dom_node_child_append(thiz->button, egueb_dom_node_ref(thiz->label), NULL);
}

static void _eon_element_button_stock_instance_deinit(void *o)
{
	Eon_Element_Button_Stock *thiz;

	thiz = EON_ELEMENT_BUTTON_STOCK(o);
	egueb_dom_feature_unref(thiz->proxy);
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

