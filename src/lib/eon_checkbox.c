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
	char *group_name;
} Eon_Checkbox;

static inline Eon_Checkbox * _eon_checkbox_get(Enesim_Renderer *r)
{
	Eon_Checkbox *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static void _checkbox_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{

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
EAPI Enesim_Renderer * eon_checkbox_new(void)
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
