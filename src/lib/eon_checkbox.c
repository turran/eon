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
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Checkbox
{
	/* properties */
	Eina_Bool selected;
} Eon_Checkbox;

static inline Eon_Checkbox * _eon_checkbox_get(Enesim_Renderer *r)
{
	Eon_Checkbox *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static void _checkbox_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eina_Bool selected;

	printf("called\n");
	eon_checkbox_selected_get(e, &selected);
	eon_checkbox_selected_set(e, !selected);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's container interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_checkbox_initialize(Ender_Element *ender)
{
	/* register every needed callback */
	ender_event_listener_add(ender, "MouseClick", _checkbox_mouse_click, NULL);
}

static Eon_Container_Descriptor _descriptor = {
	.initialize = _eon_checkbox_initialize,
	.name = "checkbox",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_checkbox_new(void)
{
	Eon_Checkbox *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Checkbox));
	if (!thiz) return NULL;

	r = eon_container_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_checkbox_selected_get(Enesim_Renderer *r, Eina_Bool *selected)
{
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(r);
	*selected = thiz->selected;
}

static void _eon_checkbox_selected_set(Enesim_Renderer *r, Eina_Bool selected)
{
	Eon_Checkbox *thiz;
	Escen_Ender *theme;
	Escen_State *new_state;
	
	thiz = _eon_checkbox_get(r);
	/* first set the property internally */
	thiz->selected = selected;
	/* now set the state */
	theme = eon_element_theme_ender_get(r);
	if (selected)
		new_state = escen_ender_state_get(theme, "selected");
	else
		new_state = escen_ender_state_get(theme, "unselected");
	if (new_state)
	{
		Escen_Instance *eei;
		eei = eon_element_theme_instance_get(r);
		escen_instance_state_set(eei, new_state);
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_checkbox.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_checkbox_new(void)
{
	return ender_element_new("checkbox");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_checkbox_selected_set(Ender_Element *e, Eina_Bool selected)
{
	ender_element_value_set(e, "selected", selected, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_checkbox_selected_get(Ender_Element *e, Eina_Bool *selected)
{
	ender_element_value_get(e, "selected", selected, NULL);
}


