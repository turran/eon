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
#define EON_THEME_ELEMENT_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_THEME_ELEMENT_MAGIC)
#define EON_THEME_ELEMENT_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_THEME_ELEMENT_MAGIC, ret)

typedef struct _Eon_Theme_Element
{
	EINA_MAGIC
	/* properties */
	double width;
	double height;
	/* private */
	Eon_Theme_Element_Descriptor *descriptor;
	void *data;
} Eon_Theme_Element;

static inline Eon_Theme_Element * _eon_theme_element_get(Enesim_Renderer *r)
{
	Eon_Theme_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_THEME_ELEMENT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_element_new(Eon_Theme_Element_Descriptor *twdescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Element *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Element));
	EINA_MAGIC_SET(thiz, EON_THEME_ELEMENT_MAGIC);
	thiz->data = data;
	thiz->descriptor = twdescriptor;

	r = enesim_renderer_new(descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_element(Enesim_Renderer *r)
{
	Eon_Theme_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	if (!thiz) return EINA_FALSE;
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_ELEMENT_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_element_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->min_width_get)
		*width = thiz->descriptor->min_width_get(r);
	else
		*width = 0;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Element *thiz;

	if (!height) return;
	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->min_height_get)
		*height = thiz->descriptor->min_height_get(r);
	else
		*height = 0;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->max_width_get)
		*width = thiz->descriptor->max_width_get(r);
	else
		*width = DBL_MAX;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Element *thiz;

	if (!height) return;
	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	if (thiz->descriptor->max_height_get)
		*height = thiz->descriptor->max_height_get(r);
	else
		*height = DBL_MAX;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_width_set(Enesim_Renderer *r, double width)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	thiz->width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	*width = thiz->width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_height_set(Enesim_Renderer *r, double height)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	thiz->height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_element_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Element *thiz;

	thiz = _eon_theme_element_get(r);
	if (!thiz) return;
	*height = thiz->height;
}


