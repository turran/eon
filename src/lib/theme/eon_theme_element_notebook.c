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
#include "eon_theme_element_notebook.h"
#include "eon_theme_main_private.h"
#include "eon_theme_renderable_private.h"
/*
 * A notebook is composed of:
 * An area where the tabs lay
 * An area of the content of the tab
 * 
 * The renderable notebook must add/remove/promote/demote tab title renderers
 * The renderable notebook must set the content renderer
 * TODO:
 * enesim: add api to make a compound layer be promoted/demoted
 * enesim: add api to make a compound layer be sent top top or to bottom
 * eon: create the notebook element
 * eon: create the notebookTab element (widget)
 * eon: create the notebookContent element (renderable)
 * eon: create the themes associated with this
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_NOTEBOOK_DESCRIPTOR eon_theme_element_notebook_descriptor_get()
#define EON_THEME_ELEMENT_NOTEBOOK(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Notebook, EON_THEME_ELEMENT_NOTEBOOK_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Notebook
{
	Eon_Theme_Renderable base;
	/* attributes */
	Egueb_Dom_Node *tabs_expand;
	/* private */
	const Eon_Theme_Element_Notebook_Descriptor *d;
	void *data;
} Eon_Theme_Element_Notebook;

typedef struct _Eon_Theme_Element_Notebook_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Element_Notebook_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_notebook_renderer_get(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Notebook *thiz;

	thiz = EON_THEME_ELEMENT_NOTEBOOK(r);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_theme_element_notebook_init(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Notebook *thiz;

	/* attributes */
	thiz->tabs_expand = egueb_dom_attr_boolean_new(egueb_dom_string_ref(EON_ATTR_ENABLED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(e->n, egueb_dom_node_ref(thiz->font), NULL);
}

static Egueb_Dom_Node * _eon_theme_element_notebook_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Notebook *thiz;

	thiz = EON_THEME_ELEMENT_NOTEBOOK(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static Eina_Bool _eon_theme_element_notebook_process(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Notebook *thiz;

	thiz = EON_THEME_ELEMENT_NOTEBOOK(e);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

static Egueb_Dom_String * _eon_theme_element_notebook_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Notebook *thiz;

	thiz = EON_THEME_ELEMENT_NOTEBOOK(e);
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
		Eon_Theme_Element_Notebook, Eon_Theme_Element_Notebook_Class, eon_theme_element_notebook);

static void _eon_theme_element_notebook_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_notebook_renderer_get;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->init = _eon_theme_element_notebook_init;
	e_klass->ctor = _eon_theme_element_notebook_ctor;
	e_klass->tag_name_get = _eon_theme_element_notebook_tag_name_get;
	e_klass->process = _eon_theme_element_notebook_process;
}

static void _eon_theme_element_notebook_instance_init(void *o)
{
}

static void _eon_theme_element_notebook_instance_deinit(void *o)
{
	Eon_Theme_Element_Notebook *thiz;

	thiz = EON_THEME_ELEMENT_NOTEBOOK(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
	/* attributes */
	egueb_dom_node_unref(thiz->tabs_expand);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new notebook theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_notebook_new(
		const Eon_Theme_Element_Notebook_Descriptor *d, void *data)
{
	Eon_Theme_Element_Notebook *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_notebook);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

