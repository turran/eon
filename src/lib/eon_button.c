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
/* TODO
 * Add a simple
 * eon_button_new_with_label(char *label);
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Button
{
} Eon_Button;

static inline Eon_Button * _eon_button_get(Enesim_Renderer *r)
{
	Eon_Button *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_button_free(Enesim_Renderer *r)
{
	Eon_Button *thiz;

	thiz = _eon_button_get(r);
	free(thiz);
}

static void _eon_button_initialize(Ender_Element *e)
{
	/* register every event needed for a button
	 * like: mouse_in, mouse_down, mouse_up, mouse_out, etc
	 */
}

static Eon_Container_Descriptor _descriptor = {
	.initialize = _eon_button_initialize,
	.free = _eon_button_free,
	.name = "button",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_button_new(void)
{
	Eon_Button *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Button));
	if (!thiz) return NULL;

	r = eon_container_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_button.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_button_new(void)
{
	return ender_element_new("button");
}
