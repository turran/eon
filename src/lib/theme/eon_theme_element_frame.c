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
#include "eon_theme_element_frame.h"
#include "eon_theme_main_private.h"
#include "eon_theme_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_FRAME_DESCRIPTOR eon_theme_element_frame_descriptor_get()
#define EON_THEME_ELEMENT_FRAME(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Frame, EON_THEME_ELEMENT_FRAME_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Frame
{
	Eon_Theme_Renderable base;
	/* attributes */
	Egueb_Dom_Node *font;
	Egueb_Dom_Node *color;
	/* private */
	const Eon_Theme_Element_Frame_Descriptor *d;
	void *data;
} Eon_Theme_Element_Frame;

typedef struct _Eon_Theme_Element_Frame_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Element_Frame_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_frame_renderer_get(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(r);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}

static Eina_Bool _eon_theme_element_frame_process(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(r);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_theme_element_frame_init(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Frame *thiz;

	/* attributes */
	thiz->font = egueb_css_attr_font_new(NULL, EINA_TRUE, EINA_TRUE, EINA_FALSE);
	thiz->color = egueb_css_attr_color_new(
			egueb_dom_string_ref(EON_NAME_COLOR), NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(e->n, egueb_dom_node_ref(thiz->color), NULL);
	egueb_dom_element_attribute_node_set(e->n, egueb_dom_node_ref(thiz->font), NULL);
}

static Egueb_Dom_Node * _eon_theme_element_frame_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static Egueb_Dom_String * _eon_theme_element_frame_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(e);
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
		Eon_Theme_Element_Frame, Eon_Theme_Element_Frame_Class, eon_theme_element_frame);

static void _eon_theme_element_frame_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_frame_renderer_get;
	klass->process = _eon_theme_element_frame_process;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->init = _eon_theme_element_frame_init;
	e_klass->ctor = _eon_theme_element_frame_ctor;
	e_klass->tag_name_get = _eon_theme_element_frame_tag_name_get;
}

static void _eon_theme_element_frame_instance_init(void *o)
{
}

static void _eon_theme_element_frame_instance_deinit(void *o)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
	/* attributes */
	egueb_dom_node_unref(thiz->font);
	egueb_dom_node_unref(thiz->color);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_element_frame_min_size_get(Egueb_Dom_Node *n, int *w, int *h)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(egueb_dom_element_external_data_get(n));
	thiz->d->min_size_get(thiz->data, w, h);
}

void eon_theme_element_frame_title_set(Egueb_Dom_Node *n, Egueb_Dom_String *s)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(egueb_dom_element_external_data_get(n));
	thiz->d->title_set(thiz->data, egueb_dom_string_chars_get(s));
	egueb_dom_string_unref(s);
}

void eon_theme_element_frame_padding_get(Egueb_Dom_Node *n, Eon_Box *padding)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(egueb_dom_element_external_data_get(n));
	if (thiz->d->padding_get)
		thiz->d->padding_get(thiz->data, padding);
}

void eon_theme_element_frame_content_set(Egueb_Dom_Node *n, Enesim_Renderer *r)
{
	Eon_Theme_Element_Frame *thiz;

	thiz = EON_THEME_ELEMENT_FRAME(egueb_dom_element_external_data_get(n));
	if (thiz->d->content_set)
		thiz->d->content_set(thiz->data, r);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new frame theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_frame_new(
		const Eon_Theme_Element_Frame_Descriptor *d, void *data)
{
	Eon_Theme_Element_Frame *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_frame);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

