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
typedef struct _Eon_Wrapper
{
	Ender_Element *wrapped;
	Enesim_Renderer *clipper;
	Enesim_Renderer *compound;
	Enesim_Renderer *fallback;
	Enesim_Renderer *wrapped_renderer;
	Enesim_Renderer_Sw_Fill fill;
} Eon_Wrapper;

static inline Eon_Wrapper * _eon_wrapper_get(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = eon_element_data_get(r);
	return thiz;
}

static void _wrapped_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Enesim_Renderer *r = data;
	Ender_Element *element;

	element = ender_element_renderer_from(r);
	eon_element_changed_set(element, EINA_TRUE);
}
/*----------------------------------------------------------------------------*
 *                       The Eon's element interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_wrapper_free(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	free(thiz);
}

static Eina_Bool _eon_wrapper_has_changed(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;
	Eina_Bool ret;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
	ret = enesim_renderer_has_changed(thiz->wrapped_renderer);

	if (ret)
	{
		printf("wrapped changed\n");
	}
	return ret;
}

static double _eon_wrapper_preferred_height_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;
	Eina_Rectangle rect;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped_renderer) return 0;

	enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);
	//printf("wrapper preferred height %d\n", rect.h);
	return rect.h;
}

static double _eon_wrapper_preferred_width_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;
	Eina_Rectangle rect;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
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
	Enesim_Renderer *r;
	Eina_Rectangle rect;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
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
	Enesim_Renderer *r;
	Eina_Rectangle rect;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return DBL_MAX;
	enesim_renderer_destination_boundings(thiz->wrapped_renderer, &rect, 0, 0);

	//printf("wrapper max height %d\n", rect.h);
	return DBL_MAX;
	//return rect.h;
}

static Eina_Bool _eon_wrapper_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;
	double ox, oy;
	double aw, ah;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return EINA_FALSE;

	eon_element_actual_position_get(r, &ox, &oy);
	eon_element_actual_width_get(e, &aw);
	eon_element_actual_height_get(e, &ah);
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

	return EINA_TRUE;
}

static void _eon_wrapper_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
}

static Enesim_Renderer * _eon_wrapper_renderer_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
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
	.cleanup = _eon_wrapper_cleanup,
	.renderer_get = _eon_wrapper_renderer_get,
	.has_changed = _eon_wrapper_has_changed,
	.initialize = _eon_wrapper_initialize,
	.free = _eon_wrapper_free,
	.name = "wrapper",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_wrapper_new(void)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Wrapper));

	r = enesim_renderer_background_new();
	thiz->fallback = r;
	enesim_renderer_background_color_set(r, 0xff00ffff);

	r = enesim_renderer_clipper_new();
	thiz->clipper = r;
	enesim_renderer_clipper_content_set(r, thiz->fallback);

	r = enesim_renderer_compound_new();
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->clipper);

	r = eon_element_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_wrapper_wrapped_set(Enesim_Renderer *r, Ender_Element *wrapped)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	/* FIXME check that the wrapped element does support the origin property */
	//if (thiz->wrapped)
	//	ender_event_listener_remove(thiz->wrapped, "Mutation", _wrapped_changed, r);
	thiz->wrapped = wrapped;
	thiz->wrapped_renderer = NULL;
	if (!thiz->wrapped)
		return;
	thiz->wrapped_renderer = ender_element_renderer_get(wrapped);
	ender_event_listener_add(thiz->wrapped, "Mutation", _wrapped_changed, r);

	enesim_renderer_clipper_content_set(thiz->clipper, thiz->wrapped_renderer);
	/* FIXME this should be part of the renderer itself */
	enesim_renderer_rop_set(thiz->wrapped_renderer, ENESIM_BLEND);
}

static void _eon_wrapper_wrapped_get(Enesim_Renderer *r, Ender_Element **wrapped)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	*wrapped = thiz->wrapped;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
	return ender_element_new_with_namespace("wrapper", "eon");
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
