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
typedef struct _Eon_Label
{
} Eon_Label;

static inline Eon_Label * _eon_label_get(Enesim_Renderer *r)
{
	Eon_Label *thiz;

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
EAPI Enesim_Renderer * eon_label_new(void)
{
	Eon_Label *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Label));
	if (!thiz) return NULL;

	r = eon_widget_new("label", thiz);
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
EAPI void eon_label_text_set(Enesim_Renderer *r, const char *text)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "text", text);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_get(Enesim_Renderer *r, const char **text)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(r);
	if (!thiz) return;

	eon_widget_property_get(r, "text", text);
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_ellipsize_set(Enesim_Renderer *r, Eina_Bool enable)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(r);

	eon_widget_property_set(r, "ellipsize", enable);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_ellipsize_get(Enesim_Renderer *r, Eina_Bool *enabled)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(r);

	eon_widget_property_get(r, "ellipsize", enabled);
}

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_direction_set(Enesim_Renderer *r, Etex_Direction direction)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(r);
	eon_widget_property_set(r, "direction", direction);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_direction_get(Enesim_Renderer *r, Etex_Direction *direction)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(r);
	eon_widget_property_get(r, "direction", direction);
}
#endif
