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
typedef struct _Eon_Scrollview
{
	/* properties */
	double x_position;
	double y_position;
	/* private */
} Eon_Scrollview;

static inline Eon_Button * _eon_button_get(Enesim_Renderer *r)
{
	Eon_Button *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_scrollview_initialize(Ender_Element *e)
{
	/* register every event needed for a scrollview
	 * like: mouse_in, mouse_down, mouse_up, mouse_out, etc
	 */
}

static Eon_Widget_Descriptor _eon_scrollview_widget_descriptor = {
	.initialize = _eon_scrollview_initialize,
	.name = "scrollview",
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
EAPI Enesim_Renderer * eon_scrollview_new(void)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Scrollview));
	if (!thiz) return NULL;

	r = eon_container_new(&_eon_scrollview_widget_descriptor, thiz);
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
EAPI void eon_scrollview_x_position_set(Enesim_Renderer *r, double x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	thiz->x_position = x;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_get(Enesim_Renderer *r, double *x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*x = thiz->x_position;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_set(Enesim_Renderer *r, double y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	thiz->y_position = y;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_get(Enesim_Renderer *r, double *y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*y = thiz->y_position;
}

