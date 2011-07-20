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
typedef struct _Eon_Theme_Widget
{
	/* properties */
	/* private */
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Widget;

static inline Eon_Theme_Widget * _eon_theme_widget_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = eon_theme_element_data_get(r);

	return thiz;
}

static void _eon_theme_widget_free(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Widget *thiz;
	Eon_Theme_Element_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Widget));
	thiz->data = data;
	thiz->free = descriptor->free;

	pdescriptor.max_width_get = descriptor->max_width_get;
	pdescriptor.min_width_get = descriptor->min_width_get;
	pdescriptor.max_height_get = descriptor->max_height_get;
	pdescriptor.min_height_get = descriptor->min_height_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
	pdescriptor.free = _eon_theme_widget_free;

	r = eon_theme_element_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_theme_widget_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	return thiz->data;
}

Eina_Bool eon_is_theme_widget(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	if (!eon_is_theme_element(r))
		return EINA_FALSE;
	thiz = eon_theme_element_data_get(r);
	if (!thiz) return EINA_FALSE;
#if 0
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_ELEMENT_MAGIC))
		return EINA_FALSE;
#endif
	return EINA_TRUE;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

