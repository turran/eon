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
typedef struct _Eon_Element
{
	EINA_MAGIC;
	unsigned int min_width;
	unsigned int min_height;
	unsigned int max_width;
	unsigned int max_height;
	void *data;
} Eon_Element;

static inline Eon_Element * _eon_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);

	return thiz;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_element_new(Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Element));
	thiz->data = data;

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
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_get(Enesim_Renderer *r, unsigned int width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_set(Enesim_Renderer *r, unsigned int *width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->min_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_set(Enesim_Renderer *r, unsigned int height)
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
EAPI void eon_element_min_height_get(Enesim_Renderer *r, unsigned int *height)
{
	Eon_Element *thiz;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->min_height;
}
