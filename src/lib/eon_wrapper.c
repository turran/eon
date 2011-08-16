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
/**
 * @todo
 * Given that we always need to return a renderer whenever the eon_element
 * interface requires, we need to create another object, maybe a clipper+background
 * or better a compound+clipper+background and whenever the wrapped property
 * is set, we just swap the element inside the compound and everything will
 * continue working :)
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Wrapper
{
	Ender_Element *wrapped;
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

static void _wrapper_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	thiz->fill(thiz->wrapped_renderer, x, y, len, dst);
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
static Eina_Bool _eon_wrapper_sw_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Eon_Wrapper *thiz;
	Enesim_Rectangle rect;

	thiz = _eon_wrapper_get(r);

	/* in case the renderer has no transformation matrix, just set the origin
	 * with the actual_x,y values
	 */
	//enesim_renderer_boundings(r, &rect);
	//enesim_renderer_origin_set(thiz->compound, rect.x, rect.y);
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("the theme can not setup yet\n");
		return EINA_FALSE;
	}

	/* get the ender from the escen ender and get the fill function */
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _wrapper_draw;

	return EINA_TRUE;
}

static void _eon_wrapper_sw_cleanup(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _eon_wrapper_free(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	free(thiz);
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
	return rect.w;
}

/* The min and max hint functions must check if the wrapped renderer supports
 * the affine transformation, if so we can just return inifinite for both values
 */

static double _eon_wrapper_min_width_get(Ender_Element *e)
{
	return 0;
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

	return rect.w;
}

static double _eon_wrapper_min_height_get(Ender_Element *e)
{
	return 0;
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

	return rect.h;
}

static Eina_Bool _eon_wrapper_setup(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;
	double ox, oy;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return EINA_FALSE;

	eon_element_actual_position_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	return EINA_TRUE;
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
	.sw_setup = _eon_wrapper_sw_setup,
	.sw_cleanup = _eon_wrapper_sw_cleanup,
	.setup = _eon_wrapper_setup,
	.renderer_get = _eon_wrapper_renderer_get,
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

	r = enesim_renderer_compound_new();
	thiz->compound = r;
	enesim_renderer_compound_layer_add(r, thiz->fallback);

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

	enesim_renderer_compound_layer_clear(thiz->compound);
	enesim_renderer_compound_layer_add(thiz->compound, thiz->wrapped_renderer);
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
