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
#include "eon_drawer_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Drawer_Widget,
		Eon_Drawer_Widget_Class, eon_drawer_widget);

static void _eon_drawer_widget_class_init(void *k)
{
}

static void _eon_drawer_widget_instance_init(void *o)
{
}

static void _eon_drawer_widget_instance_deinit(void *o)
{
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#if 0
void * eon_drawer_widget_data_get(Eon_Drawer_Widget *thiz)
{
	return thiz->data;
}
#endif

void eon_drawer_widget_geometry_set(Eon_Drawer_Widget *thiz, Eina_Rectangle *g)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS_GET(thiz);
	if (klass->geometry_set)
		klass->geometry_set(thiz, g);
}

Enesim_Renderer * eon_drawer_widget_renderer_get(Eon_Drawer_Widget *thiz)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS_GET(thiz);
	if (klass->renderer_get)
		return klass->renderer_get(thiz);
	return NULL;
}

void eon_drawer_widget_ender_populate(Egueb_Dom_Node *n, Eon_Drawer_Widget *thiz)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS_GET(thiz);
	if (klass->ender_populate)
		klass->ender_populate(thiz, n);
}

Eina_Bool eon_drawer_widget_ender_process(Egueb_Dom_Node *n, Eon_Drawer_Widget *thiz)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS_GET(thiz);
	if (klass->ender_process)
		klass->ender_process(thiz, n);
	return EINA_TRUE;
}

#if 0
void eon_drawer_widget_size_changed_cb_set(Eon_Drawer_Widget *thiz, Eon_Drawer_Widget_Size_Changed_Cb cb, void *data)
{
	thiz->size_changed = cb;
	thiz->size_changed_data = data;
}
#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_drawer_widget_free(Eon_Drawer_Widget *thiz)
{
	enesim_object_instance_free(ENESIM_OBJECT_INSTANCE(thiz));
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_drawer_widget_request_process(Eon_Drawer_Widget *thiz)
{
#if 0
	if (thiz->size_changed)
		return thiz->size_changed(thiz->size_changed_data);
#endif
}
