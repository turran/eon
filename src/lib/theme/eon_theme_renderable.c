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
#include "eon_theme_main_private.h"
#include "eon_theme_renderable_private.h"
#include "eon_event_geometry_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_THEME_ELEMENT_DESCRIPTOR, Eon_Theme_Renderable,
		Eon_Theme_Renderable_Class, eon_theme_renderable);

static void _eon_theme_renderable_class_init(void *k)
{
}

static void _eon_theme_renderable_instance_init(void *o)
{
}

static void _eon_theme_renderable_instance_deinit(void *o)
{
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_renderable_geometry_set(Egueb_Dom_Node *n, Eina_Rectangle *geometry)
{
	Eon_Theme_Renderable *thiz;

	thiz = EON_THEME_RENDERABLE(egueb_dom_element_external_data_get(n));
	thiz->geometry = *geometry;
}

Enesim_Renderer * eon_theme_renderable_renderer_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Renderable *thiz;
	Eon_Theme_Renderable_Class *klass;

	thiz = EON_THEME_RENDERABLE(egueb_dom_element_external_data_get(n));
	klass = EON_THEME_RENDERABLE_CLASS_GET(thiz);
	if (klass->renderer_get)
		return klass->renderer_get(thiz);
	return NULL;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_theme_renderable_geometry_get(Egueb_Dom_Node *n, Eina_Rectangle *geom)
{
	Eon_Theme_Renderable *thiz;

	thiz = EON_THEME_RENDERABLE(egueb_dom_element_external_data_get(n));
	*geom = thiz->geometry;
}

/*
 * In case the theme element needs to inform the corresponding element that
 * the size hints of the owner element must change, call this function
 * An example use is whenever a font attribute changes on a label theme. On
 * such case the geometry of the label element must be recalculated.
 */
EAPI void eon_theme_renderable_geometry_invalidate(Egueb_Dom_Node *n)
{
	Eon_Theme_Renderable *thiz;
	Egueb_Dom_Event *ev;
	Egueb_Dom_Event_Target *et;

	thiz = EON_THEME_RENDERABLE(egueb_dom_element_external_data_get(n));
	DBG("Invalidating geometry");

	ev = eon_event_geometry_invalidate_new();
	et = EGUEB_DOM_EVENT_TARGET((EON_THEME_ELEMENT(thiz))->n);
	egueb_dom_event_target_event_dispatch(et, ev, NULL, NULL);
}
