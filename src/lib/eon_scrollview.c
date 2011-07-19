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

static inline Eon_Scrollview * _eon_scrollview_get(Enesim_Renderer *r)
{
	Eon_Scrollview *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static void _scrollview_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	printf("mouse clicked\n");
}

static void _scrollview_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	printf("mouse move\n");
}

static void _scrollview_mouse_drag(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	printf("mouse drag\n");
}
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_scrollview_initialize(Ender_Element *e)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);
	ender_event_listener_add(e, "MouseClick", _scrollview_mouse_click, NULL);
	ender_event_listener_add(e, "MouseMove", _scrollview_mouse_move, NULL);
	/* register every event needed for a scrollview
	 * like: mouse_in, mouse_down, mouse_up, mouse_out, etc
	 */
}

static Eina_Bool _eon_scrollview_setup(Ender_Element *e)
{
	return EINA_TRUE;
}

static Eon_Container_Descriptor _descriptor = {
	.initialize = _eon_scrollview_initialize,
	.setup = _eon_scrollview_setup,
	.name = "scrollview",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
EAPI Enesim_Renderer * _eon_scrollview_new(void)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Scrollview));
	if (!thiz) return NULL;

	r = eon_container_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_scrollview_x_position_set(Enesim_Renderer *r, double x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	thiz->x_position = x;
}

static void _eon_scrollview_x_position_get(Enesim_Renderer *r, double *x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*x = thiz->x_position;
}

static void _eon_scrollview_y_position_set(Enesim_Renderer *r, double y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	thiz->y_position = y;
}

static void _eon_scrollview_y_position_get(Enesim_Renderer *r, double *y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*y = thiz->y_position;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_scrollview.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_scrollview_new(void)
{
	return ender_element_new("scrollview");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_set(Ender_Element *e, double x)
{
	ender_element_value_set(e, "x_position", x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_get(Ender_Element *e, double *x)
{
	ender_element_value_get(e, "x_position", x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_set(Ender_Element *e, double y)
{
	ender_element_value_set(e, "y_position", y, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_get(Ender_Element *e, double *y)
{
	ender_element_value_get(e, "y_position", y, NULL);
}

