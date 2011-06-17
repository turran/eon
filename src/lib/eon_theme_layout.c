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
typedef struct _Eon_Theme_Layout
{
	EINA_MAGIC;
	/* properties */
	/* private */
	Eon_Theme_Layout_Child_Add child_add;
	Eon_Theme_Layout_Child_Remove child_remove;
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Layout;

static inline Eon_Theme_Layout * _eon_theme_layout_get(Enesim_Renderer *r)
{
	Eon_Theme_Layout *thiz;

	thiz = eon_theme_element_data_get(r);

	return thiz;
}

static void _eon_theme_layout_free(Enesim_Renderer *r)
{
	Eon_Theme_Layout *thiz;

	thiz = _eon_theme_layout_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_layout_new(Eon_Theme_Layout_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Layout *thiz;
	Eon_Theme_Element_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Layout));
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->child_add = descriptor->child_add;
	thiz->child_remove = descriptor->child_remove;

	pdescriptor.max_width_get = descriptor->max_width_get;
	pdescriptor.min_width_get = descriptor->min_width_get;
	pdescriptor.max_height_get = descriptor->max_height_get;
	pdescriptor.min_height_get = descriptor->min_height_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_layout_free;

	r = eon_theme_element_new(&pdescriptor, thiz);
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
EAPI void * eon_theme_layout_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Layout *thiz;

	thiz = _eon_theme_layout_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_layout(Enesim_Renderer *r)
{

}

EAPI void eon_theme_layout_child_add(Enesim_Renderer *r, Ender_Element *e)
{
	Eon_Theme_Layout *thiz;

	thiz = _eon_theme_layout_get(r);
	if (thiz->child_add)
		thiz->child_add(r, e);
}

EAPI void eon_theme_layout_child_remove(Enesim_Renderer *r, Ender_Element *e)
{
	Eon_Theme_Layout *thiz;

	thiz = _eon_theme_layout_get(r);
	if (thiz->child_remove)
		thiz->child_remove(r, e);
}
