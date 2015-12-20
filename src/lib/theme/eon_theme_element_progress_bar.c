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
#include "eon_theme_element_progress_bar.h"
#include "eon_theme_main_private.h"
#include "eon_theme_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_ELEMENT_PROGRESS_BAR_DESCRIPTOR eon_theme_element_progress_bar_descriptor_get()
#define EON_THEME_ELEMENT_PROGRESS_BAR(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Theme_Element_Progress_Bar, EON_THEME_ELEMENT_PROGRESS_BAR_DESCRIPTOR)

typedef struct _Eon_Theme_Element_Progress_Bar
{
	Eon_Theme_Renderable base;
	/* attributes */
	/* private */
	Eon_Orientation orientation;
	double progression;
	const Eon_Theme_Element_Progress_Bar_Descriptor *d;
	void *data;
} Eon_Theme_Element_Progress_Bar;

typedef struct _Eon_Theme_Element_Progress_Bar_Class
{
	Eon_Theme_Renderable_Class base;
} Eon_Theme_Element_Progress_Bar_Class;

/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_theme_element_progress_bar_renderer_get(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(r);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(thiz->data);
	return NULL;
}

static Eina_Bool _eon_theme_element_progress_bar_process(Eon_Theme_Renderable *r)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(r);
	if (thiz->d->process)
		return thiz->d->process(thiz->data);
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_theme_element_progress_bar_init(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	/* attributes */
}

static Egueb_Dom_Node * _eon_theme_element_progress_bar_ctor(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(e);
	if (thiz->d->ctor)
		return thiz->d->ctor();
	return NULL;
}

static Egueb_Dom_String * _eon_theme_element_progress_bar_tag_name_get(Eon_Theme_Element *e)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(e);
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
		Eon_Theme_Element_Progress_Bar, Eon_Theme_Element_Progress_Bar_Class, eon_theme_element_progress_bar);

static void _eon_theme_element_progress_bar_class_init(void *k)
{
	Eon_Theme_Renderable_Class *klass;
	Eon_Theme_Element_Class *e_klass;

	klass = EON_THEME_RENDERABLE_CLASS(k);
	klass->renderer_get = _eon_theme_element_progress_bar_renderer_get;
	klass->process = _eon_theme_element_progress_bar_process;

	e_klass = EON_THEME_ELEMENT_CLASS(k);
	e_klass->init = _eon_theme_element_progress_bar_init;
	e_klass->ctor = _eon_theme_element_progress_bar_ctor;
	e_klass->tag_name_get = _eon_theme_element_progress_bar_tag_name_get;
}

static void _eon_theme_element_progress_bar_instance_init(void *o)
{
}

static void _eon_theme_element_progress_bar_instance_deinit(void *o)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(o);
	if (thiz->d->dtor)
		thiz->d->dtor(thiz->data);
	/* attributes */
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_theme_element_progress_bar_thickness_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Progress_Bar *thiz;

	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(egueb_dom_element_external_data_get(n));
	return thiz->d->thickness_get(thiz->data);
}

void eon_theme_element_progress_bar_orientation_set(Egueb_Dom_Node *n, Eon_Orientation o)
{
	Eon_Theme_Element_Progress_Bar *thiz;
	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(egueb_dom_element_external_data_get(n));
	thiz->orientation = o;
}

void eon_theme_element_progress_bar_progression_set(Egueb_Dom_Node *n, double progression)
{
	Eon_Theme_Element_Progress_Bar *thiz;
	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(egueb_dom_element_external_data_get(n));
	thiz->progression = progression;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new progress_bar theme
 * @param d The set of functions the theme implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Egueb_Dom_Node * eon_theme_element_progress_bar_new(
		const Eon_Theme_Element_Progress_Bar_Descriptor *d, void *data)
{
	Eon_Theme_Element_Progress_Bar *thiz;
	Eon_Theme_Element *e;
	Egueb_Dom_Node *n;

	n = EON_THEME_ELEMENT_NEW(eon_theme_element_progress_bar);
	thiz = egueb_dom_element_external_data_get(n);
	thiz->d = d;
	thiz->data = data;

	e = EON_THEME_ELEMENT(thiz);
	return e->n;
}

EAPI Eon_Orientation eon_theme_element_progress_bar_orientation_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Progress_Bar *thiz;
	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(egueb_dom_element_external_data_get(n));
	return thiz->orientation;
}

EAPI double eon_theme_element_progress_bar_progression_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Element_Progress_Bar *thiz;
	thiz = EON_THEME_ELEMENT_PROGRESS_BAR(egueb_dom_element_external_data_get(n));
	return thiz->progression;
}

