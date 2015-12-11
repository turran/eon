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
#include "eon_theme_element_private.h"
#include "eon_theme_element_eot_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Element_Eot
{
	Eon_Theme_Element base;
} Eon_Theme_Element_Eot;

typedef struct _Eon_Theme_Element_Eot_Class
{
	Eon_Theme_Element_Class base;
} Eon_Theme_Element_Eot_Class;

static void _eon_theme_element_eot_process_cb(Egueb_Dom_Event *e,
		void *data)
{
	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_BUBBLING)
		return;

	DBG("Preventing a process on a child element");
	egueb_dom_event_stop_propagation(e);
}

/* a mutation is being triggered from one of our children
 * or a children of our children, prevent the process
 */
static void _eon_theme_element_eot_mutation_cb(Egueb_Dom_Event *e,
		void *data)
{
	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_BUBBLING)
		return;

	DBG("Preventing a process on a child element");
	egueb_dom_event_mutation_process_prevent(e);
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static void _eon_theme_element_eot_init(Eon_Theme_Element *e)
{
	Egueb_Dom_Event_Target *evt;

	evt = EGUEB_DOM_EVENT_TARGET_CAST(e->n);
	egueb_dom_event_target_event_listener_add(evt,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_theme_element_eot_mutation_cb,
			EINA_FALSE, NULL);
	egueb_dom_event_target_event_listener_add(evt,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_eon_theme_element_eot_mutation_cb,
			EINA_FALSE, NULL);
	egueb_dom_event_target_event_listener_add(evt,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_eon_theme_element_eot_mutation_cb,
			EINA_FALSE, NULL);
	egueb_dom_event_target_event_listener_add(evt,
			EGUEB_DOM_EVENT_PROCESS,
			_eon_theme_element_eot_process_cb,
			EINA_FALSE, NULL);
}

static Egueb_Dom_String * _eon_theme_element_eot_tag_name_get(
		Eon_Theme_Element *e)
{
	return egueb_dom_string_ref(EON_THEME_NAME_ELEMENT_EOT);
}

static Eina_Bool _eon_theme_element_eot_child_appendable(Eon_Theme_Element *e,
		Egueb_Dom_Node *child)
{	
	if (eon_theme_is_element(child))
		return EINA_TRUE;
	return EINA_FALSE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_THEME_ELEMENT_DESCRIPTOR,
		Eon_Theme_Element_Eot, Eon_Theme_Element_Eot_Class,
		eon_theme_element_eot);

static void _eon_theme_element_eot_class_init(void *k)
{
	Eon_Theme_Element_Class *klass = k;

	klass->init = _eon_theme_element_eot_init;
	klass->child_appendable = _eon_theme_element_eot_child_appendable;
	klass->tag_name_get = _eon_theme_element_eot_tag_name_get;
}

static void _eon_theme_element_eot_instance_init(void *o)
{
}

static void _eon_theme_element_eot_instance_deinit(void *o)
{
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool eon_theme_element_is_eot(Egueb_Dom_Node *n)
{
	void *data;

	if (!n) return EINA_FALSE;
	if (!egueb_dom_element_is_external(n))
		return EINA_FALSE;
	data = egueb_dom_element_external_data_get(n);
	if (!enesim_object_instance_inherits(ENESIM_OBJECT_INSTANCE(data),
			eon_theme_element_eot_descriptor_get()))
		return EINA_FALSE;
	return EINA_TRUE;
}

Egueb_Dom_Node * eon_theme_element_eot_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_THEME_ELEMENT_NEW(eon_theme_element_eot);
	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
