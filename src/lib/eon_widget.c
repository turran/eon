/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_WIDGET_MAGIC)
#define EON_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_WIDGET_MAGIC, ret)

typedef struct _Eon_Widget
{
	EINA_MAGIC
	Eon_Element_Initialize initialize;
	Enesim_Renderer_Delete free;
	void *data;
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	if (!r) return NULL;
	thiz = eon_element_data_get(r);
	EON_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _widget_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Enesim_Renderer *r;
	Escen_State *new_state;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;

	r = ender_element_renderer_get(e);
	theme_ender = eon_element_theme_ender_get(r);
	new_state = escen_ender_state_get(theme_ender, "mouse_in");
	if (!new_state) return;

	theme_instance = eon_element_theme_instance_get(r);
	escen_instance_state_set(theme_instance, new_state);
}

static void _widget_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Enesim_Renderer *r;
	Escen_State *new_state;
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;

	r = ender_element_renderer_get(e);
	theme_instance = eon_element_theme_instance_get(r);
	if (escen_instance_current_state_finalized(theme_instance))
	{
		printf("state finalized, setting the new one\n");
	}
	else
	{
		printf("state didnt finalize, not setting a new one\n");
		return;
	}
	theme_ender = eon_element_theme_ender_get(r);
	new_state = escen_ender_state_get(theme_ender, "mouse_out");
	if (!new_state) return;

	escen_instance_state_set(theme_instance, new_state);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_widget_free(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

static void _eon_widget_initialize(Ender_Element *ender)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(ender);
	thiz = _eon_widget_get(r);
	/* register every needed callback */
	ender_event_listener_add(ender, "MouseIn", _widget_mouse_in, NULL);
	ender_event_listener_add(ender, "MouseOut", _widget_mouse_out, NULL);
	if (thiz->initialize)
		thiz->initialize(ender);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_widget_new(Eon_Widget_Descriptor *descriptor, void *data)
{
	Eon_Widget *thiz;
	Eon_Element_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Widget));
	EINA_MAGIC_SET(thiz, EON_WIDGET_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;

	pdescriptor.initialize = _eon_widget_initialize;
	pdescriptor.free = _eon_widget_free;
	pdescriptor.name = descriptor->name;

	r = eon_element_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_widget_data_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = _eon_widget_get(r);
	if (!e) return NULL;

	return e->data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_widget(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = eon_element_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_WIDGET_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}
