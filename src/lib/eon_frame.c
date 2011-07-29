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
 * eon_frame_new_with_label(char *label);
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Frame
{
	Ender_Element *description;
} Eon_Frame;

static inline Eon_Frame * _eon_frame_get(Enesim_Renderer *r)
{
	Eon_Frame *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_frame_free(Enesim_Renderer *r)
{
	Eon_Frame *thiz;

	thiz = _eon_frame_get(r);
	free(thiz);
}

static Eon_Container_Descriptor _descriptor = {
	.free = _eon_frame_free,
	.name = "frame",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_frame_new(void)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Frame));
	if (!thiz) return NULL;

	r = eon_container_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_frame_description_get(Enesim_Renderer *r, Ender_Element **e)
{
	Eon_Frame *thiz;

	thiz = _eon_frame_get(r);
	*e = thiz->description;
}

static void _eon_frame_description_set(Enesim_Renderer *r, Ender_Element *e)
{
	Eon_Frame *thiz;

	thiz = _eon_frame_get(r);
	thiz->description = e;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_frame.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_frame_new(void)
{
	return ender_element_new_with_namespace("frame", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_frame_description_get(Ender_Element *e, Ender_Element **description)
{
	ender_element_value_get(e, "description", description, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_frame_description_set(Ender_Element *e, Ender_Element *description)
{
	ender_element_value_set(e, "description", description, NULL);
}
