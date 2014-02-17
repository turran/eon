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
#include "eon_drawer_private.h"

#include "eon_drawer_widget.h"
#include "eon_drawer_button.h"
#include "eon_drawer_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_DRAWER_BUTTON_DESCRIPTOR eon_drawer_button_descriptor_get()
#define EON_DRAWER_BUTTON_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,		\
		Eon_Drawer_Button_Class, EON_DRAWER_BUTTON_DESCRIPTOR)
#define EON_DRAWER_BUTTON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Drawer_Button, EON_DRAWER_BUTTON_DESCRIPTOR)

typedef struct _Eon_Drawer_Button
{
	Eon_Drawer_Widget base;
	const Eon_Drawer_Button_Descriptor *d;
	void *data;
} Eon_Drawer_Button;

typedef struct _Eon_Drawer_Button_Class
{
	Eon_Drawer_Widget_Class base;
} Eon_Drawer_Button_Class;

/*----------------------------------------------------------------------------*
 *                              Widget interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_drawer_widget_renderer_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Button *thiz;

	thiz = EON_DRAWER_BUTTON(w);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(w, thiz->data);
	return NULL;
}

static void _eon_drawer_widget_geometry_set(Eon_Drawer_Widget *w, Eina_Rectangle *geom)
{
	Eon_Drawer_Button *thiz;

	thiz = EON_DRAWER_BUTTON(w);
	if (thiz->d->geometry_set)
		thiz->d->geometry_set(w, thiz->data, geom);
}

/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_DRAWER_WIDGET_DESCRIPTOR,
		Eon_Drawer_Button, Eon_Drawer_Button_Class, eon_drawer_button);

static void _eon_drawer_button_class_init(void *k)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS(k);
	klass->renderer_get = _eon_drawer_widget_renderer_get;
	klass->geometry_set = _eon_drawer_widget_geometry_set;
}

static void _eon_drawer_button_instance_init(void *o)
{
}

static void _eon_drawer_button_instance_deinit(void *o)
{
	Eon_Drawer_Button *thiz;

	thiz = EON_DRAWER_BUTTON(o);
	if (thiz->d->free)
		thiz->d->free(EON_DRAWER_WIDGET(o), thiz->data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Drawer_Widget * eon_drawer_button_new(
		const Eon_Drawer_Button_Descriptor *d, void *data)
{
	Eon_Drawer_Button *thiz;
	Eon_Drawer_Widget *w;

	w = ENESIM_OBJECT_INSTANCE_NEW(eon_drawer_button);
	thiz = EON_DRAWER_BUTTON(w);
	thiz->d = d;
	thiz->data = data;

	return w;	
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_drawer_button_data_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Button *thiz;

	thiz = EON_DRAWER_BUTTON(w);
	return thiz->data;
}
