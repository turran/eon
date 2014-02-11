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
#include "eon_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_BUTTON_DESCRIPTOR eon_element_button_descriptor_get()
#define EON_ELEMENT_BUTTON_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,		\
		Eon_Element_Button_Class, EON_ELEMENT_BUTTON_DESCRIPTOR)
#define EON_ELEMENT_BUTTON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Button, EON_ELEMENT_BUTTON_DESCRIPTOR)

typedef struct _Eon_Element_Button
{
	Eon_Widget base;
} Eon_Element_Button;

typedef struct _Eon_Element_Button_Class
{
	Eon_Widget_Class base;
} Eon_Element_Button_Class;
/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static int _eon_element_button_size_hints_get(Eon_Widget *w,
		Eon_Renderable_Size *size)
{
	printf("size hints get\n");
	/* in case it has a child get the size hints of the content */
	/* otherwise get it from the theme */
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_button_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_ELEMENT_BUTTON);
}

/* a process is called whenever we want to set a new geometry */
static Eina_Bool _eon_element_button_process(Eon_Element *e)
{
	Eon_Element_Button *thiz;
	Egueb_Dom_Node *n = e->n;

	thiz = EON_ELEMENT_BUTTON(e);

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Button,
		Eon_Element_Button_Class, eon_element_button);

static void _eon_element_button_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_button_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->size_hints_get = _eon_element_button_size_hints_get;
}

static void _eon_element_button_instance_init(void *o)
{
}

static void _eon_element_button_instance_deinit(void *o)
{
	Eon_Element_Button *thiz;

	thiz = EON_ELEMENT_BUTTON(o);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_button_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_button);
	return n;
}

