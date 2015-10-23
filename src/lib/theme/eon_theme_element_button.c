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
#include "eon_theme_element_button.h"
#include "eon_theme_main_private.h"
#include "eon_theme_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_BUTTON_DESCRIPTOR eon_theme_element_button_descriptor_get()
#define EON_THEME_ELEMENT_BUTTON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Button, EON_THEME_ELEMENT_BUTTON_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Button
{
	Eon_Theme_Renderable base;
	const Eon_Theme_Element_Button_Descriptor *d;
	void *data;
} Eon_Theme_Element_Button;

typedef struct _Eon_Theme_Element_Button_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Element_Button_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_button_renderer_get(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(r);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}

static void _eon_theme_element_button_geometry_set(Eon_Theme_Renderable *r, Eina_Rectangle *geom)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(r);
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_theme_element_button_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static Eina_Bool _eon_theme_element_button_process(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(e);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

static Egueb_Dom_String * _eon_theme_element_button_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(e);
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
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_THEME_RENDERABLE_DESCRIPTOR,
		Eon_Theme_Element_Button, Eon_Theme_Element_Button_Class, eon_theme_element_button);

static void _eon_theme_element_button_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_button_renderer_get;
	klass->geometry_set = _eon_theme_element_button_geometry_set;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->ctor = _eon_theme_element_button_ctor;
	e_klass->tag_name_get = _eon_theme_element_button_tag_name_get;
	e_klass->process = _eon_theme_element_button_process;
}

static void _eon_theme_element_button_instance_init(void *o)
{
}

static void _eon_theme_element_button_instance_deinit(void *o)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_element_button_padding_get(Egueb_Dom_Node *n, Eon_Box *padding)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(egueb_dom_element_external_data_get(n));
	if (thiz->d->padding_get)
		thiz->d->padding_get(thiz->data, padding);
}

void eon_theme_element_button_content_set(Egueb_Dom_Node *n, Enesim_Renderer *r)
{
	Eon_Theme_Element_Button *thiz;

	thiz = EON_THEME_ELEMENT_BUTTON(egueb_dom_element_external_data_get(n));
	if (thiz->d->content_set)
		thiz->d->content_set(thiz->data, r);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new button theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_button_new(
		const Eon_Theme_Element_Button_Descriptor *d, void *data)
{
	Eon_Theme_Element_Button *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_button);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

/**
 * Gets the private data of a button
 * @param w The button to get the data from
 * @return The private data
 */
EAPI void * eon_theme_element_button_data_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Button *thiz;
	Eon_Theme_Element *e;

	e = EON_THEME_ELEMENT(egueb_dom_element_external_data_get(n));
	thiz = EON_THEME_ELEMENT_BUTTON(e);

	return thiz->data;
}
