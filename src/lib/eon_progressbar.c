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
typedef struct _Eon_Progressbar
{
	/* properties */
	/* private */
} Eon_Progressbar;

static inline Eon_Progressbar * _eon_progressbar_get(Enesim_Renderer *r)
{
	Eon_Progressbar *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_progressbar_initialize(Ender_Element *ender)
{
	/* register every needed callback */
}

static Eon_Widget_Descriptor _eon_progressbar_widget_descriptor = {
	.initialize = _eon_progressbar_initialize,
	.name = "progressbar",
};
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
EAPI Enesim_Renderer * eon_progressbar_new(void)
{
	Eon_Progressbar *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Progressbar));
	if (!thiz) return NULL;

	r = eon_widget_new(&_eon_progressbar_widget_descriptor, thiz);
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
EAPI void eon_progressbar_progression_get(Enesim_Renderer *r, double *progression)
{
	Eon_Progressbar *thiz;

	thiz = _eon_progressbar_get(r);
	if (!thiz) return;

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_progressbar_progression_set(Enesim_Renderer *r, double progression)
{
	Eon_Progressbar *thiz;

	thiz = _eon_progressbar_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "progression", progression, NULL);
}
