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
#include "eon_theme_element_separator.h"
#include "eon_theme_main_private.h"
#include "eon_theme_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_SEPARATOR_DESCRIPTOR eon_theme_element_separator_descriptor_get()
#define EON_THEME_ELEMENT_SEPARATOR(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Separator, EON_THEME_ELEMENT_SEPARATOR_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Separator
{
	Eon_Theme_Renderable base;
	/* attributes */
	/* private */
	Eon_Orientation orientation;
	const Eon_Theme_Element_Separator_Descriptor *d;
	void *data;
} Eon_Theme_Element_Separator;

typedef struct _Eon_Theme_Element_Separator_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Element_Separator_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_separator_renderer_get(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Separator *thiz;

	thiz = EON_THEME_ELEMENT_SEPARATOR(r);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}

static Eina_Bool _eon_theme_element_separator_process(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Separator *thiz;

	thiz = EON_THEME_ELEMENT_SEPARATOR(r);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_theme_element_separator_init(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Separator *thiz;

	/* attributes */
}

static Egueb_Dom_Node * _eon_theme_element_separator_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Separator *thiz;

	thiz = EON_THEME_ELEMENT_SEPARATOR(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static Egueb_Dom_String * _eon_theme_element_separator_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Separator *thiz;

	thiz = EON_THEME_ELEMENT_SEPARATOR(e);
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
		Eon_Theme_Element_Separator, Eon_Theme_Element_Separator_Class, eon_theme_element_separator);

static void _eon_theme_element_separator_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_separator_renderer_get;
	klass->process = _eon_theme_element_separator_process;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->init = _eon_theme_element_separator_init;
	e_klass->ctor = _eon_theme_element_separator_ctor;
	e_klass->tag_name_get = _eon_theme_element_separator_tag_name_get;
}

static void _eon_theme_element_separator_instance_init(void *o)
{
}

static void _eon_theme_element_separator_instance_deinit(void *o)
{
	Eon_Theme_Element_Separator *thiz;

	thiz = EON_THEME_ELEMENT_SEPARATOR(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
	/* attributes */
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_theme_element_separator_thickness_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Separator *thiz;

	thiz = EON_THEME_ELEMENT_SEPARATOR(egueb_dom_element_external_data_get(n));
	return thiz->d->thickness_get(thiz->data);
}

void eon_theme_element_separator_orientation_set(Egueb_Dom_Node *n, Eon_Orientation o)
{
	Eon_Theme_Element_Separator *thiz;
	thiz = EON_THEME_ELEMENT_SEPARATOR(egueb_dom_element_external_data_get(n));
	thiz->orientation = o;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new separator theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_separator_new(
		const Eon_Theme_Element_Separator_Descriptor *d, void *data)
{
	Eon_Theme_Element_Separator *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_separator);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

EAPI Eon_Orientation eon_theme_element_separator_orientation_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Separator *thiz;
	thiz = EON_THEME_ELEMENT_SEPARATOR(egueb_dom_element_external_data_get(n));
	return thiz->orientation;
}
