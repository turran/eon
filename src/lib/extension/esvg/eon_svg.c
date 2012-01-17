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
typedef struct _Eon_Svg
{
} Eon_Svg;

static inline Eon_Svg * _eon_svg_get(Enesim_Renderer *r)
{
	Eon_Svg *thiz;

	thiz = eon_element_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
/* FIXME add the click event here instead of the svg itself, as every
 * inherited eleement of this type must handle this
 */
static void _eon_svg_initialize(Ender_Element *e)
{
	Eon_Svg *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_svg_get(r);
#if 0
	ender_event_listener_add(e, "MouseIn", _svg_mouse_in, NULL);
	ender_event_listener_add(e, "MouseOut", _svg_mouse_out, NULL);
	ender_event_listener_add(e, "MouseDown", _svg_mouse_down, NULL);
	ender_event_listener_add(e, "MouseUp", _svg_mouse_up, NULL);
	if (thiz->initialize)
		thiz->initialize(e);
#endif
}

static double _eon_svg_min_width_get(Ender_Element *e)
{
	return DBL_MAX;
}

static double _eon_svg_min_height_get(Ender_Element *e)
{
	return DBL_MAX;
}
static double _eon_svg_preferred_width_get(Ender_Element *e)
{
	return DBL_MAX;
}

static double _eon_svg_preferred_height_get(Ender_Element *e)
{
	return DBL_MAX;
}

static double _eon_svg_max_width_get(Ender_Element *e)
{
	return DBL_MAX;
}

static double _eon_svg_max_height_get(Ender_Element *e)
{
	return DBL_MAX;
}

static Eina_Bool _eon_svg_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Svg *thiz;
	Enesim_Renderer *r;
	Ender_Element *content;

	r = ender_element_renderer_get(e);
	thiz = _eon_svg_get(r);
	return EINA_TRUE;
}

static void _eon_svg_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Svg *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_svg_get(r);
}

static Enesim_Renderer * _eon_svg_renderer_get(Ender_Element *e)
{
	Eon_Svg *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_svg_get(r);
	return NULL;
}

static Eina_Bool _eon_svg_needs_setup(Ender_Element *e)
{
	Eon_Svg *thiz;
	Enesim_Renderer *r;
	Eina_Bool ret;

	r = ender_element_renderer_get(e);
	thiz = _eon_svg_get(r);

	return EINA_TRUE;
}

static void _eon_svg_free(Enesim_Renderer *r)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(r);
	free(thiz);
}

static Eon_Element_Descriptor _descriptor = {
	.min_width_get = _eon_svg_min_width_get,
	.max_width_get = _eon_svg_max_width_get,
	.min_height_get = _eon_svg_min_height_get,
	.max_height_get = _eon_svg_max_height_get,
	.preferred_width_get = _eon_svg_preferred_width_get,
	.preferred_height_get = _eon_svg_preferred_height_get,
	.setup = _eon_svg_setup,
	.cleanup = _eon_svg_cleanup,
	.renderer_get = _eon_svg_renderer_get,
	.needs_setup = _eon_svg_needs_setup,
	.initialize = _eon_svg_initialize,
	.free = _eon_svg_free,
	.name = "svg",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_svg_new(void)
{
	Eon_Svg *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Svg));
	if (!thiz) return NULL;

	r = eon_element_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#if 0
#include "eon_generated_svg.c"
#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_svg_new(void)
{
	return ender_element_new_with_namespace("svg", "eon");
}

