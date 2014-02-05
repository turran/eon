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
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_renderable_init(Eon_Element *e)
{
	Eon_Renderable *thiz;
	Eon_Renderable_Class *klass;

 	thiz = EON_RENDERABLE(e);
	/* add the attributes */
	thiz->min_width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MIN_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->min_width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->min_height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MIN_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->min_height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->max_width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MAX_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->max_width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->max_height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_MAX_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->max_height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->width = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_WIDTH), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->width, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);
	thiz->height = egueb_dom_attr_int_new(
			egueb_dom_string_ref(EON_HEIGHT), EINA_FALSE,
			EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(thiz->height, EGUEB_DOM_ATTR_TYPE_DEFAULT, -1);

	klass = EON_RENDERABLE_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

static Eina_Bool _eon_renderable_process(Eon_Element *e)
{
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR, Eon_Renderable,
		Eon_Renderable_Class, eon_renderable);

static void _eon_renderable_class_init(void *k)
{
	Eon_Element_Class *klass = k;
	klass->init = _eon_renderable_init;
	klass->process = _eon_renderable_process;
}

static void _eon_renderable_instance_init(void *o)
{
}

static void _eon_renderable_instance_deinit(void *o)
{
	Eon_Renderable *thiz = o;

	egueb_dom_node_unref(thiz->min_width);
	egueb_dom_node_unref(thiz->min_height);
	egueb_dom_node_unref(thiz->max_width);
	egueb_dom_node_unref(thiz->max_height);
	egueb_dom_node_unref(thiz->width);
	egueb_dom_node_unref(thiz->height);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
