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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_surface.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_element.h"
/* TODO use the backend to get the surface pool */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Surface
{
	/* properties */
	/* private */
	Enesim_Renderer *image;
	Enesim_Surface *s;
} Eon_Surface;

static inline Eon_Surface * _eon_surface_get(Eon_Element *ee)
{
	Eon_Surface *thiz;

	thiz = eon_element_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_surface_free(Eon_Element *e)
{
	Eon_Surface *thiz;

	thiz = _eon_surface_get(e);
	if (thiz->image)
		enesim_renderer_unref(thiz->image);
	if (thiz->s)
		enesim_surface_unref(thiz->s);
	free(thiz);
}

static void _eon_surface_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Surface *thiz;

	thiz = _eon_surface_get(e);
	enesim_renderer_image_position_set(thiz->image, g->x, g->y);
	enesim_renderer_image_width_set(thiz->image, g->width);
	enesim_renderer_image_height_set(thiz->image, g->height);
}

static void _eon_surface_hints_get(Eon_Element *e, Eon_Hints *hints)
{
	Eon_Surface *thiz;
	int sw = 0;
	int sh = 0;

	thiz = _eon_surface_get(e);
	if (thiz->s)
	{
		enesim_surface_size_get(thiz->s, &sw, &sh);
	}
	eon_hints_sizes_values_set(hints, sw, sh, sw, sh, sw, sh);
}

static Enesim_Renderer * _eon_surface_renderer_get(Ender_Element *e)
{
	Eon_Surface *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_surface_get(ee);
	return thiz->image;
}

static Eon_Element_Descriptor _eon_surface_descriptor = {
	/* .initialize 		= */ NULL,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .renderer_get 	= */ _eon_surface_renderer_get,
	/* .hints_get 		= */ _eon_surface_hints_get,
	/* .geometry_set 	= */ _eon_surface_geometry_set,
	/* .is_focusable	= */ NULL,
	/* .free		= */ _eon_surface_free,
	/* .name 		= */ "surface",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_surface_new(void)
{
	Eon_Surface *thiz;
	Eon_Element *ee;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Surface));

	r = enesim_renderer_image_new();
	thiz->image = r;

	ee = eon_element_new(&_eon_surface_descriptor, thiz);
	if (!ee) goto base_err;

	return ee;

base_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_surface_delete NULL
#include "eon_generated_surface.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_surface_new(void)
{
	return EON_ELEMENT_NEW("surface");
}

EAPI void eon_surface_source_set(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Surface *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_surface_get(ee);
	thiz->s = s;
	enesim_renderer_image_src_set(thiz->image, thiz->s);
}

EAPI void eon_surface_damage_add(Ender_Element *e, Eina_Rectangle *area)
{
	Eon_Surface *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_surface_get(ee);
	enesim_renderer_image_damage_add(thiz->image, area);
}

