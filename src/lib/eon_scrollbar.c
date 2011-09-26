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
	Eon_Orientation orientation;
	double max;
	double min;
	double page_increment;
	double page_size;
	double step_increment;
	double value;
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
	printf("scrollbar clicked at %g %g\n", ev->rel_x, ev->rel_y);
	/* calculate the real value */
	/* FIXME */
	eon_scrollbar_value_set(e, 10);
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

static Eina_Bool _eon_scrollbar_setup(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Eon_Size size;
	double length;
	double blength;
	double bsize;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	eon_element_actual_size_get(r, &size);
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		length = size.width;
	else
		length = size.height;
	blength = length / (thiz->max - thiz->min);
	/* do the size of the thumb */
	eon_scrollbar_page_size_get(e, &bsize);
	eon_theme_scrollbar_thumb_size_set(theme_r, bsize);
	/* FIXME */
	eon_theme_scrollbar_thumb_percent_set(theme_r, 10);
	printf("bar length = %g (%g %g %g) %g\n", blength, length, thiz->max, thiz->min, bsize);
	/* calculate the real size of the thumb */
	//eon_theme_scrollbar_thumb_min_size_get(theme_r, &min);
	//eon_theme_scrollbar_thumb_max_size_get(theme_r, &max);
	/* set the new size on the thumb and its position */
	return EINA_TRUE;
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
	.setup = _eon_scrollbar_setup,
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
	/* default values */
	thiz->value = 0;
	thiz->max = 100;
	thiz->min = 0;
	thiz->step_increment = 1;
	thiz->page_increment = 10;
	thiz->page_size = 10;
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
	thiz->orientation = orientation;
	eon_widget_property_set(r, "orientation", orientation, NULL);
}

static void _eon_scrollbar_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*orientation = thiz->orientation;
}

static void _eon_scrollbar_max_set(Enesim_Renderer *r, double max)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	thiz->max = max;
}

static void _eon_scrollbar_max_get(Enesim_Renderer *r, double *max)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*max = thiz->max;
}

static void _eon_scrollbar_min_set(Enesim_Renderer *r, double min)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	thiz->min = min;
}

static void _eon_scrollbar_min_get(Enesim_Renderer *r, double *min)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*min = thiz->min;
}

static void _eon_scrollbar_page_increment_set(Enesim_Renderer *r, double page_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	thiz->page_increment = page_increment;
}

static void _eon_scrollbar_page_increment_get(Enesim_Renderer *r, double *page_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*page_increment = thiz->page_increment;
}

static void _eon_scrollbar_page_size_set(Enesim_Renderer *r, double page_size)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	thiz->page_size = page_size;
}

static void _eon_scrollbar_page_size_get(Enesim_Renderer *r, double *page_size)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *theme_r;
	double max, min, final;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	final = thiz->page_size;
	theme_r = eon_widget_theme_renderer_get(r);

	eon_theme_scrollbar_thumb_max_size_get(theme_r, &max);
	if (final > max) final = max;
	eon_theme_scrollbar_thumb_min_size_get(theme_r, &min);
	if (final < min) final = min;
	*page_size = final;
}

static void _eon_scrollbar_step_increment_set(Enesim_Renderer *r, double step_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	thiz->step_increment = step_increment;
}

static void _eon_scrollbar_step_increment_get(Enesim_Renderer *r, double *step_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*step_increment = thiz->step_increment;
}

static void _eon_scrollbar_value_set(Enesim_Renderer *r, double value)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	thiz->value = value;
}

static void _eon_scrollbar_value_get(Enesim_Renderer *r, double *value)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*value = thiz->value;
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
EAPI void eon_scrollbar_page_size_set(Ender_Element *e, double page_size)
{
	ender_element_value_set(e, "page_size", page_size, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_size_get(Ender_Element *e, double *page_size)
{
	ender_element_value_get(e, "page_size", page_size, NULL);
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

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_value_set(Ender_Element *e, double value)
{
	ender_element_value_set(e, "value", value, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_value_get(Ender_Element *e, double *value)
{
	ender_element_value_get(e, "value", value, NULL);
}

