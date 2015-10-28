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
#include "eon_theme_element_label_private.h"
#include "eon_theme_main_private.h"
#include "eon_theme_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_LABEL_DESCRIPTOR eon_theme_element_label_descriptor_get()
#define EON_THEME_ELEMENT_LABEL(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Label, EON_THEME_ELEMENT_LABEL_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Label
{
	Eon_Theme_Element base;
	/* properties */
	Egueb_Dom_Node *font;
} Eon_Theme_Element_Label;

typedef struct _Eon_Theme_Element_Label_Class
{
	Eon_Theme_Element_Class base;
} Eon_Theme_Element_Label_Class;
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_theme_element_label_ctor(Eon_Theme_Element *e)
{
	return eon_theme_element_label_new();
}

static void _eon_theme_element_label_init(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Label *thiz;
	Egueb_Dom_Node *n;

	thiz = EON_THEME_ELEMENT_LABEL(e);
	/* create the attributes */
	thiz->font = egueb_css_attr_font_new(NULL, EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(e->n, egueb_dom_node_ref(thiz->font), NULL);
}

static Eina_Bool _eon_theme_element_label_process(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Label *thiz;
	thiz = EON_THEME_ELEMENT_LABEL(e);
	return EINA_TRUE;
}

static Egueb_Dom_String * _eon_theme_element_label_tag_name_get(
		Eon_Theme_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_LABEL);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_THEME_ELEMENT_DESCRIPTOR,
		Eon_Theme_Element_Label, Eon_Theme_Element_Label_Class, eon_theme_element_label);

static void _eon_theme_element_label_class_init(void *k)
{
	Eon_Theme_Element_Class *klass;

	klass = EON_THEME_ELEMENT_CLASS(k);
	klass->ctor = _eon_theme_element_label_ctor;
	klass->init = _eon_theme_element_label_init;
	klass->tag_name_get = _eon_theme_element_label_tag_name_get;
	klass->process = _eon_theme_element_label_process;
}

static void _eon_theme_element_label_instance_init(void *o)
{
}

static void _eon_theme_element_label_instance_deinit(void *o)
{
	Eon_Theme_Element_Label *thiz;

	thiz = EON_THEME_ELEMENT_LABEL(o);
	egueb_dom_node_unref(thiz->font);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_element_label_new(void)
{
	Eon_Theme_Element_Label *thiz;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_label);
	thiz = egueb_dom_element_external_data_get(n);
	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
