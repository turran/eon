/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
#define EON_ELEMENT_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_WIDGET_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_WIDGET_MAGIC);\
	} while(0)

#define EON_ELEMENT_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_ELEMENT_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_ELEMENT_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Element
{
	EINA_MAGIC;
	Eon_Element_Descriptor *descriptor;
	double width;
	double height;
	double max_width;
	double max_height;
	double min_width;
	double min_height;
	double actual_width;
	double actual_height;
	void *data;
} Eon_Element;

static inline Eon_Element * _eon_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_ELEMENT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_initialize(Ender *ender)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(ender);
	thiz = _eon_element_get(r);
	if (thiz->descriptor->initialize)
		thiz->descriptor->initialize(ender);
}

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *edescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Element));
	EINA_MAGIC_SET(thiz, EON_ELEMENT_MAGIC);
	thiz->data = data;
	thiz->descriptor = edescriptor;
	thiz->actual_width = -1;
	thiz->actual_height = -1;
	thiz->max_width = thiz->max_height = DBL_MAX;
	thiz->min_width = thiz->min_height = 0;
	thiz->width = -1;
	thiz->height = -1;

	r = enesim_renderer_new(descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_element_data_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->data;
}

void eon_element_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	if (thiz->descriptor->actual_width_set)
		thiz->descriptor->actual_width_set(r, width);
}

void eon_element_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_height = height;
	if (thiz->descriptor->actual_height_set)
		thiz->descriptor->actual_height_set(r, height);
}

void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	thiz->actual_height = height;
	if (thiz->descriptor->actual_width_set)
		thiz->descriptor->actual_width_set(r, width);
	if (thiz->descriptor->actual_height_set)
		thiz->descriptor->actual_height_set(r, height);
}

void eon_element_actual_size_get(Enesim_Renderer *r, double *width, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->actual_width;
	*height = thiz->actual_height;
}

void eon_element_real_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double rw;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->width;
	eon_element_min_width_get(r, &min);
	eon_element_max_width_get(r, &max);
	rw = set > max ? max : set;
	rw = rw < min ? min : rw;

	//printf("real width = %g (%g %g %g)\n", rw, min, set, max);
	*width = rw;
}

void eon_element_real_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double rh;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;


	thiz = _eon_element_get(r);
	set = thiz->height;
	eon_element_min_width_get(r, &min);
	eon_element_max_width_get(r, &max);
	rh = set > max ? max : set;
	rh = rh < min ? min : rh;

	//printf("real height = %g (%g %g %g)\n", rh, min, set, max);
	*height = rh;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_element(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	if (!thiz) return EINA_FALSE;
	if (!EINA_MAGIC_CHECK(thiz, EON_ELEMENT_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->actual_height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->actual_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double v = 0;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->min_width_get)
		v = thiz->descriptor->min_width_get(r);
	*width = v > thiz->min_width ? v : thiz->min_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double v = 0;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->min_height_get)
		v = thiz->descriptor->min_height_get(r);
	*height = v > thiz->min_height ? v : thiz->min_height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->max_width_get)
		v = thiz->descriptor->max_width_get(r);
	*width = v < thiz->max_width ? v : thiz->max_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->max_height_get)
		v = thiz->descriptor->max_height_get(r);
	*height = v < thiz->max_height ? v : thiz->max_height;
}
