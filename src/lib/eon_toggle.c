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
typedef struct _Eon_Toggle
{
	Eon_Toggle_Descriptor *descriptor;
	/* private */
	void *data;
} Eon_Toggle;

static inline Eon_Toggle * _eon_toggle_get(Enesim_Renderer *r)
{
	Eon_Toggle *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_toggle_data_get(Enesim_Renderer *r)
{
	Eon_Toggle *thiz;

	thiz = _eon_toggle_get(r);
	if (!thiz) return NULL;
	return thiz->data;
}

Enesim_Renderer * eon_toggle_new(Eon_Widget_Descriptor *descriptor, void *data)
{
	Eon_Toggle *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Toggle));
	if (!thiz) return NULL;
	thiz->data = data;

	r = eon_widget_new(descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_toggle_toggled_set(Enesim_Renderer *r, Ender *toggled)
{
	Eon_Toggle *thiz;
	Enesim_Renderer *rtoggled;

	thiz = _eon_toggle_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "toggled", toggled, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_toggle_toggled_get(Enesim_Renderer *r, const Ender **toggled)
{
	Eon_Toggle *thiz;

	thiz = _eon_toggle_get(r);
	if (!thiz) return;

	eon_widget_property_get(r, "toggled", toggled, NULL);
}


