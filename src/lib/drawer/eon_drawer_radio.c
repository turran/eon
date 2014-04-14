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
#include "eon_drawer_radio.h"
#include "eon_drawer_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_DRAWER_RADIO_DESCRIPTOR eon_drawer_radio_descriptor_get()
#define EON_DRAWER_RADIO(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Drawer_Radio, EON_DRAWER_RADIO_DESCRIPTOR)

typedef struct _Eon_Drawer_Radio
{
	Eon_Drawer_Widget base;
	const Eon_Drawer_Radio_Descriptor *d;
	void *data;
} Eon_Drawer_Radio;

typedef struct _Eon_Drawer_Radio_Class
{
	Eon_Drawer_Widget_Class base;
} Eon_Drawer_Radio_Class;

/*----------------------------------------------------------------------------*
 *                              Widget interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_drawer_widget_renderer_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(w, thiz->data);
	return NULL;
}

static void _eon_drawer_widget_geometry_set(Eon_Drawer_Widget *w, Eina_Rectangle *geom)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	if (thiz->d->geometry_set)
		thiz->d->geometry_set(w, thiz->data, geom);
}

static void _eon_drawer_widget_ender_populate(Eon_Drawer_Widget *w, Egueb_Dom_Node *n)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	if (thiz->d->ender_populate)
		thiz->d->ender_populate(w, thiz->data, n);
}

static Eina_Bool _eon_drawer_widget_ender_process(Eon_Drawer_Widget *w, Egueb_Dom_Node *n)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	if (thiz->d->ender_process)
		return thiz->d->ender_process(w, thiz->data, n);
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_DRAWER_WIDGET_DESCRIPTOR,
		Eon_Drawer_Radio, Eon_Drawer_Radio_Class, eon_drawer_radio);

static void _eon_drawer_radio_class_init(void *k)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS(k);
	klass->renderer_get = _eon_drawer_widget_renderer_get;
	klass->geometry_set = _eon_drawer_widget_geometry_set;
	klass->ender_populate = _eon_drawer_widget_ender_populate;
	klass->ender_process = _eon_drawer_widget_ender_process;
}

static void _eon_drawer_radio_instance_init(void *o)
{
}

static void _eon_drawer_radio_instance_deinit(void *o)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(o);
	if (thiz->d->free)
		thiz->d->free(EON_DRAWER_WIDGET(o), thiz->data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_drawer_radio_padding_get(Eon_Drawer_Widget *w, Eon_Box *padding)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	if (thiz->d->padding_get)
		thiz->d->padding_get(w, thiz->data, padding);
}

void eon_drawer_radio_content_set(Eon_Drawer_Widget *w, Enesim_Renderer *r)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	if (thiz->d->content_set)
		thiz->d->content_set(w, thiz->data, r);
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new radio drawer
 * @param d The set of functions the drawer implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Eon_Drawer_Widget * eon_drawer_radio_new(
		const Eon_Drawer_Radio_Descriptor *d, void *data)
{
	Eon_Drawer_Radio *thiz;
	Eon_Drawer_Widget *w;

	w = ENESIM_OBJECT_INSTANCE_NEW(eon_drawer_radio);
	thiz = EON_DRAWER_RADIO(w);
	thiz->d = d;
	thiz->data = data;

	return w;	
}

/**
 * Gets the private data of a radio
 * @param w The radio to get the data from
 * @return The private data
 */
EAPI void * eon_drawer_radio_data_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Radio *thiz;

	thiz = EON_DRAWER_RADIO(w);
	return thiz->data;
}

/**
 * @brief Register the radio implementation on the ender namespace
 * @param ns The ender namespace registered on the drawer imeplementation
 * @param ctor The function used to create a drawer implementation radio
 */
EAPI void eon_drawer_radio_ender_register(Ender_Namespace *ns,
		Ender_Instance_Descriptor_Ctor ctor)
{
	Ender_Instance_Descriptor d;

	d.ctor = ctor;
	d.dtor = ENDER_INSTANCE_DESCRIPTOR_DTOR(eon_drawer_widget_free);
	d.populate = ENDER_INSTANCE_DESCRIPTOR_POPULATE(eon_drawer_widget_ender_populate);
	d.process = ENDER_INSTANCE_DESCRIPTOR_PROCESS(eon_drawer_widget_ender_process);
	d.child_appendable = NULL;
	d.child_added = NULL;
	d.child_removed = NULL;

	ender_namespace_instance_register(ns, &d, "radio");
}