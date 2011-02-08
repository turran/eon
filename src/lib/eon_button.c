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
typedef struct _Eon_Button
{
	char *text;
	Ender *external;
} Eon_Button;

static inline Eon_Button * _eon_button_get(Enesim_Renderer *r)
{
	Eon_Button *thiz;

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
EAPI Enesim_Renderer * eon_button_new(void)
{
	Eon_Button *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Button));
	if (!thiz) return NULL;

	r = eon_widget_new("button", thiz);
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
EAPI void eon_button_label_set(Enesim_Renderer *r, const char *text)
{
	Eon_Button *thiz;
	Ender_Value value;

	thiz = _eon_button_get(r);
	if (!thiz) return;

	value.string = text;
	eon_widget_property_set(r, "label", &value);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_button_image_set(Enesim_Renderer *r, const char *file)
{
	Eon_Button *thiz;

	thiz = _eon_button_get(r);
	if (!thiz) return;

	/* we should set the property to the escen */
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_button_font_set(Enesim_Renderer *r, const char *file)
{
	Eon_Button *thiz;
	Ender_Value value;

	thiz = _eon_button_get(r);
	if (!thiz) return;

	value.string = file;
	eon_widget_property_set(r, "font", &value);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_button_font_size_set(Enesim_Renderer *r, int size)
{
	Eon_Button *thiz;
	Ender_Value value;

	thiz = _eon_button_get(r);
	if (!thiz) return;

	value.int32 = size;
	eon_widget_property_set(r, "font_size", &value);
}
