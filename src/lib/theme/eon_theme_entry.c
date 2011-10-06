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
typedef struct _Eon_Theme_Entry
{
	void *data;
	Enesim_Renderer_Delete free;
} Eon_Theme_Entry;

static inline Eon_Theme_Entry * _eon_theme_entry_get(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = eon_theme_widget_data_get(r);

	return thiz;
}

static void _eon_theme_entry_free(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
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
EAPI Enesim_Renderer * eon_theme_entry_new(Eon_Theme_Entry_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Entry *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Entry));
	thiz->data = data;
	thiz->free = descriptor->free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
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
EAPI Eina_Bool eon_is_theme_entry(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_entry_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Entry *thiz;

	thiz = _eon_theme_entry_get(r);
	return thiz->data;
}
