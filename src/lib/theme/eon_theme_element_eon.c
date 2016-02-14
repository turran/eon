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
#include "eon_theme_private.h"
#include "eon_theme_element_eon.h"
#include "eon_theme_main_private.h"
#include "eon_theme_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_EON_DESCRIPTOR eon_theme_element_eon_descriptor_get()
#define EON_THEME_ELEMENT_EON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Eon, EON_THEME_ELEMENT_EON_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Eon
{
	Eon_Theme_Renderable base;
	const Eon_Theme_Element_Eon_Descriptor *d;
	void *data;
} Eon_Theme_Element_Eon;

typedef struct _Eon_Theme_Element_Eon_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Element_Eon_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_eon_renderer_get(Eon_Theme_Renderable *w)
{
	Eon_Theme_Element_Eon *thiz;

	thiz = EON_THEME_ELEMENT_EON(w);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}

static Eina_Bool _eon_theme_element_eon_process(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Eon *thiz;

	thiz = EON_THEME_ELEMENT_EON(r);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                            Element  interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_theme_element_eon_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Eon *thiz;

	thiz = EON_THEME_ELEMENT_EON(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static Egueb_Dom_String * _eon_theme_element_eon_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Eon *thiz;

	thiz = EON_THEME_ELEMENT_EON(e);
	if (thiz->d->tag_name_get)
	{
		const char *name;
		name = thiz->d->tag_name_get();
		return egueb_dom_string_new_with_static_chars(name);
	}
	return NULL;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/

ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_THEME_RENDERABLE_DESCRIPTOR,
		Eon_Theme_Element_Eon, Eon_Theme_Element_Eon_Class, eon_theme_element_eon);

static void _eon_theme_element_eon_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_eon_renderer_get;
	klass->process = _eon_theme_element_eon_process;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->ctor = _eon_theme_element_eon_ctor;
	e_klass->tag_name_get = _eon_theme_element_eon_tag_name_get;
}

static void _eon_theme_element_eon_instance_init(void *o)
{
}

static void _eon_theme_element_eon_instance_deinit(void *o)
{
	Eon_Theme_Element_Eon *thiz;

	thiz = EON_THEME_ELEMENT_EON(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new eon theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_eon_new(
		const Eon_Theme_Element_Eon_Descriptor *d, void *data)
{
	Eon_Theme_Element_Eon *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_eon);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

/**
 * Gets the private data of a eon
 * @param w The eon to get the data from
 * @return The private data
 */
EAPI void * eon_theme_element_eon_data_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Eon *thiz;
	Eon_Theme_Element *e;

	e = EON_THEME_ELEMENT(egueb_dom_element_external_data_get(n));
	thiz = EON_THEME_ELEMENT_EON(e);

	return thiz->data;
}

