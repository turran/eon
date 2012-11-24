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
#include "eon_wrapper.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_wrapper_log

static int _eon_wrapper_log = -1;

static Ender_Property *EON_WRAPPER_WRAPPED;

typedef struct _Eon_Wrapper
{
	/* properties */
	Enesim_Renderer *wrapped;
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

static Enesim_Renderer * _eon_wrapper_renderer_get(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	return thiz->compound;
}

static void _eon_wrapper_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	/* TODO set the position */
}

static void _eon_wrapper_hints_get(Eon_Element *e, Eon_Hints *hints)
{
	Eon_Wrapper *thiz;
	Eina_Rectangle bounds;

	thiz = _eon_wrapper_get(e);
	if (!thiz->wrapped) return;
	enesim_renderer_destination_boundings(thiz->wrapped, &bounds, 0, 0);
	eon_hints_sizes_values_set(hints, bounds.w, bounds.h, bounds.w, bounds.h, bounds.w, bounds.h);
}

static Eon_Element_Descriptor _descriptor = {
	/* .initialize 		= */ NULL,
	/* .backend_added	= */ NULL,
	/* .backend_removed	= */ NULL,
	/* .renderer_get 	= */ _eon_wrapper_renderer_get,
	/* .hints_get 		= */ NULL,
	/* .geometry_set 	= */ _eon_wrapper_geometry_set,
	/* .is_focusable	= */ NULL,
	/* .free 		= */ _eon_wrapper_free,
	/* .name 		= */ "wrapper",
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

static void _eon_wrapper_wrapped_set(Eon_Element *ee, Enesim_Renderer *wrapped)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(ee);
	if (thiz->wrapped)
	{
		enesim_renderer_unref(thiz->wrapped);
		thiz->wrapped = NULL;
	}
	thiz->wrapped = wrapped;
	thiz->changed = EINA_TRUE;
}

static void _eon_wrapper_wrapped_get(Eon_Element *ee, Ender_Element **wrapped)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(ee);
	*wrapped = thiz->wrapped;
}

#define _eon_wrapper_delete NULL
#include "eon_generated_wrapper.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_wrapper_init(void)
{
	_eon_wrapper_log = eina_log_domain_register("eon_wrapper", EON_LOG_COLOR_DEFAULT);
	if (_eon_wrapper_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_wrapper_init();
}

void eon_wrapper_shutdown(void)
{
	if (_eon_wrapper_log < 0)
		return;
	_eon_wrapper_shutdown();
	eina_log_domain_unregister(_eon_wrapper_log);
	_eon_wrapper_log = -1;
}
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
EAPI void eon_wrapper_wrapped_set(Ender_Element *e, Enesim_Renderer *wrapped)
{
	ender_element_property_value_set(e, EON_WRAPPER_WRAPPED, wrapped, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_wrapper_wrapped_get(Ender_Element *e, Enesim_Renderer **wrapped)
{
	Eon_Wrapper *thiz;
	Eon_Element *ee;

	if (!wrapped) return;
	ee = ender_element_object_get(e);
	thiz = _eon_wrapper_get(ee);
	*wrapped = thiz->wrapped;
}
