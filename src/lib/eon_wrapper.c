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
} Eon_Wrapper;

static inline Eon_Wrapper * _eon_wrapper_get(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = eon_element_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       The Eon's element interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_wrapper_free(Enesim_Renderer *r)
{
}

static void _eon_wrapper_initialize(Ender_Element *ender)
{
	/* we should register for every event and whenever one is received
	 * just forward it to the wrapped element
	 */
}

static double _eon_wrapper_min_width_get(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return 0;

	return 0;
}

static double _eon_wrapper_max_width_get(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return 0;

	return 0;
}

static double _eon_wrapper_min_height_get(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return 0;

	return 0;
}

static double _eon_wrapper_max_height_get(Enesim_Renderer *r)
{
	Eon_Wrapper *thiz;

	thiz = _eon_wrapper_get(r);
	if (!thiz->wrapped) return 0;

	return 0;
}

static Eina_Bool _eon_wrapper_setup(Ender_Element *e)
{
	Eon_Wrapper *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_wrapper_get(r);
	if (thiz->wrapped)
	{
	}

	return EINA_TRUE;
}

static Eon_Element_Descriptor _descriptor = {
	.min_width_get = _eon_wrapper_min_width_get,
	.max_width_get = _eon_wrapper_max_width_get,
	.min_height_get = _eon_wrapper_min_height_get,
	.max_height_get = _eon_wrapper_max_height_get,
	.setup = _eon_wrapper_setup,
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
	thiz->wrapped = wrapped;
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
EAPI Ender_Element * eon_wrapper_new(void)
{
	return ender_element_new("wrapper");
}

EAPI void eon_wrapper_wrapped_set(Ender_Element *e, Ender_Element *wrapped)
{
	ender_element_value_set(e, "wrapped", wrapped, NULL);
}

EAPI void eon_wrapper_wrapped_get(Ender_Element *e, Ender_Element **wrapped)
{
	ender_element_value_get(e, "wrapped", wrapped, NULL);
}
