/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
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
#include "eon_theme_private.h"
#include "eon_theme_element_entry.h"
#include "eon_theme_main_private.h"
#include "eon_theme_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_ENTRY_DESCRIPTOR eon_theme_element_entry_descriptor_get()
#define EON_THEME_ELEMENT_ENTRY(o) ENESIM_OBJECT_INSTANCE_CHECK(o,		\
		Eon_Theme_Element_Entry, EON_THEME_ELEMENT_ENTRY_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Entry
{
	Eon_Theme_Widget base;
	/* attributes */
	Egueb_Dom_Node *font;
	Egueb_Dom_Node *color;
	/* private */
	const Eon_Theme_Element_Entry_Descriptor *d;
	void *data;
} Eon_Theme_Element_Entry;

typedef struct _Eon_Theme_Element_Entry_Class
{
	Eon_Theme_Widget_Class base;
} Eon_Theme_Element_Entry_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_entry_renderer_get(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(r);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_theme_element_entry_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static void _eon_theme_element_entry_init(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(e);
	/* attributes */
	thiz->font = egueb_css_attr_font_new(NULL, EINA_TRUE, EINA_TRUE, EINA_FALSE);
	thiz->color = egueb_css_attr_color_new(
			egueb_dom_string_ref(EON_NAME_COLOR), NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(e->n, egueb_dom_node_ref(thiz->color), NULL);
	egueb_dom_element_attribute_node_set(e->n, egueb_dom_node_ref(thiz->font), NULL);
}


static Eina_Bool _eon_theme_element_entry_process(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(e);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

static Egueb_Dom_String * _eon_theme_element_entry_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(e);
	if (thiz->d->tag_name_get)
	{
		const char *name;
		name = thiz->d->tag_name_get();
		return egueb_dom_string_new_with_static_string(name);
	}
	return NULL;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_THEME_WIDGET_DESCRIPTOR,
		Eon_Theme_Element_Entry, Eon_Theme_Element_Entry_Class, eon_theme_element_entry);

static void _eon_theme_element_entry_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_entry_renderer_get;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->ctor = _eon_theme_element_entry_ctor;
	e_klass->tag_name_get = _eon_theme_element_entry_tag_name_get;
	e_klass->process = _eon_theme_element_entry_process;
}

static void _eon_theme_element_entry_instance_init(void *o)
{
}

static void _eon_theme_element_entry_instance_deinit(void *o)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
	/* attributes */
	egueb_dom_node_unref(thiz->font);
	egueb_dom_node_unref(thiz->color);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_element_entry_padding_get(Egueb_Dom_Node *n, Eon_Box *padding)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(egueb_dom_element_external_data_get(n));
	if (thiz->d->padding_get)
		thiz->d->padding_get(thiz->data, padding);
}

void eon_theme_element_entry_content_set(Egueb_Dom_Node *n, Enesim_Renderer *r)
{
	Eon_Theme_Element_Entry *thiz;

	thiz = EON_THEME_ELEMENT_ENTRY(egueb_dom_element_external_data_get(n));
	if (thiz->d->content_set)
		thiz->d->content_set(thiz->data, r);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new entry theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_entry_new(
		const Eon_Theme_Element_Entry_Descriptor *d, void *data)
{
	Eon_Theme_Element_Entry *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_entry);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

/**
 * Gets the private data of a entry
 * @param w The entry to get the data from
 * @return The private data
 */
EAPI void * eon_theme_element_entry_data_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Entry *thiz;
	Eon_Theme_Element *e;

	e = EON_THEME_ELEMENT(egueb_dom_element_external_data_get(n));
	thiz = EON_THEME_ELEMENT_ENTRY(e);

	return thiz->data;
}

