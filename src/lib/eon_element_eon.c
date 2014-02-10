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
#include "eon_element_private.h"
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_EON_DESCRIPTOR eon_element_eon_descriptor_get()
#define EON_ELEMENT_EON_CLASS_GET(o) EON_ELEMENT_EON_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_ELEMENT_EON_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Element_Eon_Class, EON_ELEMENT_EON_DESCRIPTOR)
#define EON_ELEMENT_EON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Eon, EON_ELEMENT_EON_DESCRIPTOR)

typedef struct _Eon_Element_Eon
{
	Eon_Element base;
	Enesim_Renderer *r;
	Eon_Size size;
	/* the theme system */
	Egueb_Dom_Node *theme;
	Eina_List *extra_themes;
} Eon_Element_Eon;

typedef struct _Eon_Element_Eon_Class
{
	Eon_Element_Class base;
} Eon_Element_Eon_Class;
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_element_eon_init(Eon_Element *e)
{
	Eon_Element_Eon *thiz;
	Egueb_Dom_Node *n;

	thiz = EON_ELEMENT_EON(e);
	thiz->r = enesim_renderer_compound_new();

	n = e->n;
	/* register on the process events, we do not want the events to go
	 * bubbling, is enough to mark the topmost element that it needs to be
	 * processed again
	 */
}

static Egueb_Dom_String * _eon_element_eon_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_EON);
}

static Eina_Bool _eon_element_eon_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *our_child;

	/* only accept one child */
	if (!eon_is_renderable(child))
		return EINA_FALSE;

	/* check if we already have one child */
	n = e->n;
	our_child = egueb_dom_node_child_first_get(n);
	if (our_child)
	{
		WARN("Only one child supported");
		egueb_dom_node_unref(our_child);
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_element_eon_process(Eon_Element *e)
{
	Eon_Element_Eon *thiz;
	Eon_Renderable_Size size;
	Egueb_Dom_Node *n = e->n;
	Egueb_Dom_Node *child;
	int size_hints;

	thiz = EON_ELEMENT_EON(e);
	child = egueb_dom_element_child_first_get(n);
	if (!child) return EINA_TRUE;

	/* request the hints on every renderable object */
	size_hints = eon_renderable_size_hints_get(child, &size);
	if (size_hints)
	{
		/* set the geometry on every renderable */
		printf("set the geometry\n");
	}
	egueb_dom_node_unref(child);

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_ELEMENT_DESCRIPTOR, Eon_Element_Eon,
		Eon_Element_Eon_Class, eon_element_eon);

static void _eon_element_eon_class_init(void *k)
{
	Eon_Element_Class *klass = k;

	klass->init = _eon_element_eon_init;
	klass->process = _eon_element_eon_process;
	klass->tag_name_get = _eon_element_eon_tag_name_get;
	klass->child_appendable = _eon_element_eon_child_appendable;
}

static void _eon_element_eon_instance_init(void *o)
{
}

static void _eon_element_eon_instance_deinit(void *o)
{
	Eon_Element_Eon *thiz;

	thiz = EON_ELEMENT_EON(o);
	enesim_renderer_unref(thiz->r);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#if 0
void eon_element_eon_theme_add(Egueb_Dom_Node *n, Egueb_Dom_Node *theme)
{
	Eon_Element_Eon *thiz;

	thiz = egueb_dom_element_external_data_get(n);
	thiz->extra_themes = eina_list_append(thiz->extra_themes, theme);
}
#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_eon_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_eon);
	return n;
}

EAPI void eon_element_eon_size_set(Egueb_Dom_Node *n, Eon_Size *size)
{
	Eon_Element_Eon *thiz;

	thiz = egueb_dom_element_external_data_get(n);
	thiz->size = *size;
	egueb_dom_element_request_process(n);
}

EAPI void eon_element_eon_size_get(Egueb_Dom_Node *n, Eon_Size *size)
{
	Eon_Element_Eon *thiz;

	thiz = egueb_dom_element_external_data_get(n);
	*size = thiz->size;
}

EAPI Eina_Bool eon_element_eon_draw(Egueb_Dom_Node *n,
		Enesim_Surface *s, Enesim_Rop rop, Eina_Rectangle *clip,
		int x, int y, Enesim_Log **error)
{
	Eon_Element_Eon *thiz;
	Eina_Bool ret;

	thiz = egueb_dom_element_external_data_get(n);
	return enesim_renderer_draw(thiz->r, s, rop, clip, x, y, error);
}

EAPI void eon_element_eon_theme_tick(Egueb_Dom_Node *n)
{
	Eon_Element_Eon *thiz;
	Egueb_Dom_Node *theme;
	Eina_List *l;

	thiz = egueb_dom_element_external_data_get(n);
	ender_document_tick(thiz->theme);
	EINA_LIST_FOREACH(thiz->extra_themes, l, theme)
	{
		ender_document_tick(theme);
	}
}

#if 0
EAPI void eon_element_eon_theme_set(Egueb_Dom_Node *n, Egueb_Dom_String *theme)
{
	
}

EAPI void eon_document_theme_set_from_document(Egueb_Dom_Node *n, Egueb_Dom_Node *theme)
{
	/* iterate over every widget, check if the theme set is NULL, if so
	 * update the instance
	 */
}

EAPI void eon_document_theme_set_from_stream(Egueb_Dom_Node *n, Enesim_Stream *s)
{
	Egueb_Dom_Node *theme;

	theme = ender_document_new();
	egueb_dom_parser_parse(stream, theme);
	enesim_stream_unref(stream);

	eon_document_theme_set_from_document(n, theme);
}
#endif
