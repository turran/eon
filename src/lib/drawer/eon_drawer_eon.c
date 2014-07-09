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

#include "eon_drawer_instance.h"
#include "eon_drawer_namespace.h"
#include "eon_drawer_widget.h"
#include "eon_drawer_eon.h"

#include "eon_drawer_widget_private.h"
#include "eon_drawer_eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_DRAWER_EON_DESCRIPTOR eon_drawer_eon_descriptor_get()
#define EON_DRAWER_EON(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Drawer_Eon, EON_DRAWER_EON_DESCRIPTOR)

typedef struct _Eon_Drawer_Eon
{
	Eon_Drawer_Widget base;
	const Eon_Drawer_Eon_Descriptor *d;
	void *data;
	/* private */
	Enesim_Renderer *r;
	Eina_Bool background_changed;
} Eon_Drawer_Eon;

typedef struct _Eon_Drawer_Eon_Class
{
	Eon_Drawer_Widget_Class base;
} Eon_Drawer_Eon_Class;
/*----------------------------------------------------------------------------*
 *                              Widget interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_drawer_eon_renderer_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Eon *thiz;

	thiz = EON_DRAWER_EON(w);
	return enesim_renderer_ref(thiz->r);
}

static void _eon_drawer_eon_ender_populate(Eon_Drawer_Widget *w, Egueb_Dom_Node *n)
{
	Eon_Drawer_Eon *thiz;
	Egueb_Dom_Node *attr;
	Egueb_Dom_String *s;

	thiz = EON_DRAWER_EON(w);
	if (thiz->d->ender_populate)
		thiz->d->ender_populate(w, thiz->data, n);
}

static Eina_Bool _eon_drawer_eon_ender_process(Eon_Drawer_Widget *w, Egueb_Dom_Node *n)
{
	Eon_Drawer_Eon *thiz;

	thiz = EON_DRAWER_EON(w);
	if (thiz->background_changed)
	{
		eon_drawer_eon_update_background(w);
		thiz->background_changed = EINA_FALSE;
	}
	if (thiz->d->ender_process)
		return thiz->d->ender_process(w, thiz->data, n);
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_DRAWER_WIDGET_DESCRIPTOR,
		Eon_Drawer_Eon, Eon_Drawer_Eon_Class, eon_drawer_eon);

static void _eon_drawer_eon_class_init(void *k)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS(k);
	klass->renderer_get = _eon_drawer_eon_renderer_get;
	klass->ender_populate = _eon_drawer_eon_ender_populate;
	klass->ender_process = _eon_drawer_eon_ender_process;
}

static void _eon_drawer_eon_instance_init(void *o)
{
	Eon_Drawer_Eon *thiz;

	thiz = EON_DRAWER_EON(o);
	thiz->r = enesim_renderer_compound_new();
}

static void _eon_drawer_eon_instance_deinit(void *o)
{
	Eon_Drawer_Eon *thiz;

	thiz = EON_DRAWER_EON(o);
	enesim_renderer_unref(thiz->r);
	if (thiz->d->free)
		thiz->d->free(EON_DRAWER_WIDGET(o), thiz->data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_drawer_eon_update_background(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Eon *thiz;
	Enesim_Renderer_Compound_Layer *l;
	Enesim_Renderer *r;

	thiz = EON_DRAWER_EON(w);
	if (!thiz->d->background_renderer_get)
		return;

	r = thiz->d->background_renderer_get(w, thiz->data);
	if (!r) return;

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, r);
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(thiz->r, l);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new eon drawer
 * @param d The set of functions the drawer implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Eon_Drawer_Widget * eon_drawer_eon_new(
		const Eon_Drawer_Eon_Descriptor *d, void *data)
{
	Eon_Drawer_Eon *thiz;
	Eon_Drawer_Widget *w;

	w = ENESIM_OBJECT_INSTANCE_NEW(eon_drawer_eon);
	thiz = EON_DRAWER_EON(w);
	thiz->d = d;
	thiz->data = data;
	thiz->background_changed = EINA_TRUE;

	return w;	
}

/**
 * Gets the private data of a eon
 * @param w The eon to get the data from
 * @return The private data
 */
EAPI void * eon_drawer_eon_data_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Eon *thiz;

	thiz = EON_DRAWER_EON(w);
	return thiz->data;
}

/**
 * @brief Register the eon implementation on the ender namespace
 * @param ns The ender namespace registered on the drawer imeplementation
 * @param ctor The function used to create a drawer implementation eon
 */
EAPI void eon_drawer_eon_ender_register(Eon_Drawer_Namespace *ns,
		Eon_Drawer_Instance_Descriptor_Ctor ctor)
{
	Eon_Drawer_Instance_Descriptor d;

	d.ctor = ctor;
	d.dtor = EON_DRAWER_INSTANCE_DESCRIPTOR_DTOR(eon_drawer_widget_free);
	d.populate = EON_DRAWER_INSTANCE_DESCRIPTOR_POPULATE(eon_drawer_widget_ender_populate);
	d.process = EON_DRAWER_INSTANCE_DESCRIPTOR_PROCESS(eon_drawer_widget_ender_process);
	d.child_appendable = NULL;
	d.child_added = NULL;
	d.child_removed = NULL;

	eon_drawer_namespace_instance_register(ns, &d, "eon");
}
