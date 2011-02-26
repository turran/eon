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
typedef struct _Eon_Radio
{
	char *text;
	Ender *external;
} Eon_Radio;

static inline Eon_Radio * _eon_radio_get(Enesim_Renderer *r)
{
	Eon_Radio *thiz;

	thiz = eon_widget_data_get(r);
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
EAPI Enesim_Renderer * eon_radio_new(void)
{
	Eon_Radio *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Radio));
	if (!thiz) return NULL;

	r = eon_widget_new("radio", thiz);
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
EAPI void eon_radio_label_set(Enesim_Renderer *r, const char *text)
{
	Eon_Radio *thiz;
	Ender_Value value;

	thiz = _eon_radio_get(r);
	if (!thiz) return;

	value.string = text;
	eon_widget_property_set(r, "label", &value);
}