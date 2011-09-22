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
	double max;
	double min;
	double page_increment;
	double step_increment;
	/* private */
} Eon_Scrollbar;

static inline Eon_Scrollbar * _eon_scrollbar_get(Enesim_Renderer *r)
{
	Eon_Scrollbar *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}

static void _eon_scrollbar_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Click *ev = event_data;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle geometry;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_scrollbar_thumb_geometry_get(theme_r, &geometry);

	/* FIXME double check that the thumb geometry is actually inside
	 * the object geometry
	 */
	printf("scrollbar clicked\n");
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_scrollbar_initialize(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);

	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK], _eon_scrollbar_mouse_click, NULL);
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

static void _eon_scrollbar_max_set(Enesim_Renderer *r, double max)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "max", max, NULL);
}

static void _eon_scrollbar_max_get(Enesim_Renderer *r, double *max)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	eon_widget_property_get(r, "max", max, NULL);
}

static void _eon_scrollbar_min_set(Enesim_Renderer *r, double min)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "min", min, NULL);
}

static void _eon_scrollbar_min_get(Enesim_Renderer *r, double *min)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	eon_widget_property_get(r, "min", min, NULL);
}

static void _eon_scrollbar_page_increment_set(Enesim_Renderer *r, double page_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "page_increment", page_increment, NULL);
}

static void _eon_scrollbar_page_increment_get(Enesim_Renderer *r, double *page_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	eon_widget_property_get(r, "page_increment", page_increment, NULL);
}

static void _eon_scrollbar_step_increment_set(Enesim_Renderer *r, double step_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;

	eon_widget_property_set(r, "step_increment", step_increment, NULL);
}

static void _eon_scrollbar_step_increment_get(Enesim_Renderer *r, double *step_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	eon_widget_property_get(r, "step_increment", step_increment, NULL);
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
EAPI Ender_Element * eon_hscrollbar_new(void)
{
	Ender_Element *e;

	e = eon_scrollbar_new();
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_vscrollbar_new(void)
{
	Ender_Element *e;

	e = eon_scrollbar_new();
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_VERTICAL);
	return e;
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

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_max_set(Ender_Element *e, double max)
{
	ender_element_value_set(e, "max", max, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_max_get(Ender_Element *e, double *max)
{
	ender_element_value_get(e, "max", max, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_min_set(Ender_Element *e, double min)
{
	ender_element_value_set(e, "min", min, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_min_get(Ender_Element *e, double *min)
{
	ender_element_value_get(e, "min", min, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_increment_set(Ender_Element *e, double page_increment)
{
	ender_element_value_set(e, "page_increment", page_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_increment_get(Ender_Element *e, double *page_increment)
{
	ender_element_value_get(e, "page_increment", page_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_step_increment_set(Ender_Element *e, double step_increment)
{
	ender_element_value_set(e, "step_increment", step_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_step_increment_get(Ender_Element *e, double *step_increment)
{
	ender_element_value_get(e, "step_increment", step_increment, NULL);
}
