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
static Ender_Property *EON_WRAPPER_WRAPPED;

typedef struct _Eon_Wrapper
{
	/* properties */
	Ender_Element *wrapped;
	/* private */
	Eina_Bool changed : 1;
	Enesim_Renderer *clipper;
	Enesim_Renderer *compound;
	Enesim_Renderer *fallback;
	Enesim_Renderer *wrapped_renderer;
	Enesim_Renderer_Sw_Fill fill;
} Eon_Wrapper;

static inline Eon_Wrapper * _eon_wrapper_get(Eon_Element *ee)
{
	Eon_Wrapper *thiz;

	thiz = eon_element_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       The Eon's element interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_wrapper_free(Eon_Element *ee)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(ee);
	free(thiz);
}

static Eina_Bool _eon_wrapper_needs_setup(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);

	return thiz->changed;
}

static double _eon_wrapper_preferred_height_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;
	Eina_Rectangle rect;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	if (!thiz->wrapped_renderer) return 0;

	enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);
	//printf("wrapper preferred height %d\n", rect.h);
	return rect.h;
}

static double _eon_wrapper_preferred_width_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;
	Eina_Rectangle rect;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	if (!thiz->wrapped_renderer) return 0;

	enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);
	//printf("wrapper preferred width %d\n", rect.w);
	return rect.w;
}

/* The min and max hint functions must check if the wrapped renderer supports
 * the affine transformation, if so we can just return inifinite for both values
 */

static double _eon_wrapper_min_width_get(Ender_Element *e)
{
	return 1;
}

static double _eon_wrapper_max_width_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;
	Eina_Rectangle rect;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	if (!thiz->wrapped) return DBL_MAX;
	enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);

	//printf("wrapper max width %d\n", rect.w);
	return DBL_MAX;
	//return rect.w;
}

static double _eon_wrapper_min_height_get(Ender_Element *e)
{
	return 1;
}

static double _eon_wrapper_max_height_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;
	Eina_Rectangle rect;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	if (!thiz->wrapped) return DBL_MAX;
	enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);

	//printf("wrapper max height %d\n", rect.h);
	return DBL_MAX;
	//return rect.h;
}

static Eina_Bool _eon_wrapper_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;
	double ox, oy;
	double aw, ah;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	if (!thiz->wrapped) return EINA_FALSE;

	ox = state->actual_position.x;
	oy = state->actual_position.y;
	aw = state->actual_size.width;
	ah = state->actual_size.height;
	//printf("setting wrapper @ %g %g - %g %g\n", ox, oy, aw, ah);

	enesim_renderer_origin_set(thiz->compound, ox, oy);
	enesim_renderer_clipper_width_set(thiz->clipper, aw);
	enesim_renderer_clipper_height_set(thiz->clipper, ah);

	/* set the scale factor, this should be more complex, as we should check if we are using
	 * some transformation matrix or not
	 * we should also add a scale property, to know whenever the use wants the scale the content
	 * or not
	 */
	if (thiz->wrapped)
	{
		Eina_Rectangle rect;
		double sx;
		double sy;

		enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);
		sx = aw / rect.w;
		sy = ah / rect.h;
		//printf("scaling %g %g\n", sx, sy);
		enesim_renderer_scale_set(thiz->wrapped_renderer, sx, sy);

	}
	thiz->changed = EINA_FALSE;

	return EINA_TRUE;
}

static Enesim_Renderer * _eon_wrapper_renderer_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	return thiz->compound;
}

static void _eon_wrapper_initialize(Ender_Element *ender)
{
	/* we should register for every event and whenever one is received
	 * just forward it to the wrapped element?
	 */
}

static Eon_Element_Descriptor _descriptor = {
	.min_width_get = _eon_wrapper_min_width_get,
	.max_width_get = _eon_wrapper_max_width_get,
	.min_height_get = _eon_wrapper_min_height_get,
	.max_height_get = _eon_wrapper_max_height_get,
	.preferred_width_get = _eon_wrapper_preferred_width_get,
	.preferred_height_get = _eon_wrapper_preferred_height_get,
	.setup = _eon_wrapper_setup,
	.renderer_get = _eon_wrapper_renderer_get,
	.needs_setup = _eon_wrapper_needs_setup,
	.initialize = _eon_wrapper_initialize,
	.free = _eon_wrapper_free,
	.name = "wrapper",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_wrapper_new(void)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Wrapper));

	r = enesim_renderer_background_new();
	thiz->fallback = r;
	enesim_renderer_background_color_set(r, 0xff00ffff);
	enesim_renderer_rop_set(r, ENESIM_FILL);

	r = enesim_renderer_clipper_new();
	thiz->clipper = r;
	enesim_renderer_clipper_content_set(r, thiz->fallback);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	r = enesim_renderer_compound_new();
	enesim_renderer_compound_layer_add(r, thiz->clipper);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->compound = r;

	ee = eon_element_new(&_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	enesim_renderer_unref(thiz->fallback);
	enesim_renderer_unref(thiz->clipper);
	enesim_renderer_unref(thiz->compound);
	free(thiz);
	return NULL;
}

static void _eon_wrapper_wrapped_set(Eon_Element *ee, Ender_Element *wrapped)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(ee);
	/* FIXME check that the wrapped element does support the origin property */
	thiz->wrapped = wrapped;
	thiz->wrapped_renderer = NULL;
	if (!thiz->wrapped)
		return;
	thiz->wrapped_renderer = ender_element_object_get(wrapped);
	enesim_renderer_clipper_content_set(thiz->clipper, thiz->wrapped_renderer);
	thiz->changed = EINA_TRUE;
}

static void _eon_wrapper_wrapped_get(Eon_Element *ee, Ender_Element **wrapped)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(ee);
	*wrapped = thiz->wrapped;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_wrapper_delete NULL
#include "eon_generated_wrapper.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_wrapper_new(void)
{
	return EON_ELEMENT_NEW("wrapper");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_wrapper_wrapped_set(Ender_Element *e, Ender_Element *wrapped)
{
	ender_element_value_set(e, "wrapped", wrapped, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_wrapper_wrapped_get(Ender_Element *e, Ender_Element **wrapped)
{
	ender_element_value_get(e, "wrapped", wrapped, NULL);
}
