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
typedef struct _Eon_Scrollbar
{
	/* properties */
	/* private */
} Eon_Scrollbar;

static inline Eon_Scrollbar * _eon_scrollbar_get(Enesim_Renderer *r)
{
	Eon_Scrollbar *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}
#if 0
static void _eon_scrollbar_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Down *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	eis = _eon_scrollbar_input_state_get(thiz, e, ev->input);
	printf("passing mouse down\n");
	eon_input_state_feed_mouse_down(eis);
}
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_scrollbar_initialize(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
}

static void _eon_scrollbar_free(Enesim_Renderer *r)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	free(thiz);
}

static double _eon_scrollbar_min_width_get(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_min_width_get(theme_r, &v);

	return v;
}

static double _eon_scrollbar_max_width_get(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_max_width_get(theme_r, &v);

	return v;
}

static double _eon_scrollbar_min_height_get(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_min_height_get(theme_r, &v);

	return v;
}

static double _eon_scrollbar_max_height_get(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);


	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_max_height_get(theme_r, &v);

	return v;
}

static double _eon_scrollbar_preferred_width_get(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_preferred_width_get(theme_r, &v);

	return v;
}

static double _eon_scrollbar_preferred_height_get(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_preferred_height_get(theme_r, &v);

	return v;
}


static Eon_Widget_Descriptor _eon_scrollbar_widget_descriptor = {
	.name = "scrollbar",
	.initialize = _eon_scrollbar_initialize,
	.free = _eon_scrollbar_free,
	.min_width_get = _eon_scrollbar_min_width_get,
	.max_width_get = _eon_scrollbar_max_width_get,
	.min_height_get = _eon_scrollbar_min_height_get,
	.max_height_get = _eon_scrollbar_max_height_get,
	.preferred_width_get = _eon_scrollbar_preferred_width_get,
	.preferred_height_get = _eon_scrollbar_preferred_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_scrollbar_new(void)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Scrollbar));
	if (!thiz) return NULL;

	r = eon_widget_new(&_eon_scrollbar_widget_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_scrollbar_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "orientation", orientation, NULL);
}

static void _eon_scrollbar_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	eon_widget_property_get(r, "orientation", orientation, NULL);
}

#include "eon_generated_scrollbar.c"
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
EAPI Ender_Element * eon_scrollbar_new(void)
{
	return ender_element_new_with_namespace("scrollbar", "eon");
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_orientation_set(Ender_Element *e, Eon_Orientation orientation)
{
	ender_element_value_set(e, "orientation", orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_orientation_get(Ender_Element *e, Eon_Orientation *orientation)
{
	ender_element_value_get(e, "orientation", orientation, NULL);
}
