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
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_widget_renderer_get(Eon_Renderable *r)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(r);
	return enesim_renderer_ref(thiz->proxy);
}

static Eina_Bool _eon_widget_pre_setup(Eon_Renderable *r)
{

}

static Eina_Bool _eon_widget_setup(Eon_Renderable *r)
{

}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_widget_init(Eon_Element *e)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(e);
	/* instantiate the theme element */
	thiz->proxy = enesim_renderer_proxy_new();
}

/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Widget,
		Eon_Widget_Class, eon_widget);

static void _eon_widget_class_init(void *k)
{
	Eon_Element_Class *klass = EON_ELEMENT_CLASS(k);
	Eon_Renderable_Class *r_klass;

	klass->init = _eon_widget_init;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_widget_renderer_get;
}

static void _eon_widget_instance_init(void *o)
{
}

static void _eon_widget_instance_deinit(void *o)
{
	Eon_Widget *thiz;

	thiz = EON_WIDGET(o);
	if (thiz->theme)
		egueb_dom_node_unref(thiz->theme);
	if (thiz->proxy)
		enesim_renderer_unref(thiz->proxy);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
