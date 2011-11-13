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
typedef struct _Eon_Scrollbar_State
{
	Eon_Orientation orientation;
	double max;
	double min;
	double page_size;
	double value;
} Eon_Scrollbar_State;

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
	Eina_Bool changed : 1;
	Eina_Bool thumb_dragging;
	double offset_dragging;
} Eon_Scrollbar;

static inline Eon_Scrollbar * _eon_scrollbar_get(Enesim_Renderer *r)
{
	Eon_Scrollbar *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}

static inline double _eon_scrollbar_value_check(Eon_Scrollbar *thiz, double v)
{
	if (v > thiz->max - thiz->page_size) v = thiz->max - thiz->page_size;
	if (v < thiz->min) v = thiz->min;

	return v;
}

static void _eon_scrollbar_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Size size;
	Enesim_Renderer *r;
	double c;
	double v;
	double length;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	if (!thiz->thumb_dragging) return;

	eon_element_actual_size_get(r, &size);
	/* get the absolute position of the event */
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		c = ev->x - thiz->offset_dragging;
		length = size.width;
	}
	else
	{
		c = ev->y - thiz->offset_dragging;
		length = size.height;
	}
	if (c > length) c = length;
	v = c / length;
	v = (thiz->max - thiz->min) * v;
	eon_scrollbar_value_set(e, _eon_scrollbar_value_check(thiz, v));
}

static void _eon_scrollbar_mouse_drag_stop(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	thiz->thumb_dragging = EINA_FALSE;
}

static void _eon_scrollbar_mouse_drag_start(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Drag_Start *ev = event_data;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle tg;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollbar_thumb_geometry_get(theme_r, &tg);
	if (!enesim_rectangle_is_inside(&tg, ev->rel_x, ev->rel_y))
		return;
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		thiz->offset_dragging = ev->x - tg.x;
	else
		thiz->offset_dragging = ev->y - tg.y;
	thiz->thumb_dragging = EINA_TRUE;
}

static void _eon_scrollbar_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Click *ev = event_data;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle ig, dg, tg;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	eon_theme_scrollbar_decrement_arrow_geometry_get(theme_r, &ig);
	eon_theme_scrollbar_increment_arrow_geometry_get(theme_r, &dg);
	eon_theme_scrollbar_thumb_geometry_get(theme_r, &tg);

	if (enesim_rectangle_is_inside(&dg, ev->rel_x, ev->rel_y))
	{
		eon_scrollbar_value_set(e, _eon_scrollbar_value_check(thiz, thiz->value - thiz->step_increment));
	}
	else if (enesim_rectangle_is_inside(&ig, ev->rel_x, ev->rel_y))
	{
		eon_scrollbar_value_set(e, _eon_scrollbar_value_check(thiz, thiz->value + thiz->step_increment));
	}
	/* the thumb case */
	else
	{
		double dt, it;
		double c;

		if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		{
			dt = tg.x;
			it = tg.x + tg.w;
			c = ev->rel_x;
		}
		else
		{
			dt = tg.y;
			it = tg.y + tg.h;
			c = ev->rel_y;
		}
		/* decrement side */
		if (dt - c > DBL_EPSILON)
		{
			eon_scrollbar_value_set(e, _eon_scrollbar_value_check(thiz, thiz->value - thiz->page_increment));
		}
		else if (c - it > DBL_EPSILON)
		{
			eon_scrollbar_value_set(e, _eon_scrollbar_value_check(thiz, thiz->value + thiz->page_increment));
		}
	}
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
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_START], _eon_scrollbar_mouse_drag_start, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_STOP], _eon_scrollbar_mouse_drag_stop, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_scrollbar_mouse_move, NULL);
}

static Eina_Bool _eon_scrollbar_setup(Ender_Element *e, Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Scrollbar *thiz;
	Eon_Size size;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double percent;
	double max, min;
	double thumb_size;
	double length;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	eon_element_actual_size_get(r, &size);
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		length = size.width;
	else
		length = size.height;

	/* first set the size of the thumb */
	thumb_size = (thiz->page_size / (thiz->max - thiz->min)) * length;
	eon_theme_scrollbar_thumb_max_size_get(theme_r, &max);
	if (thumb_size > max) thumb_size = max;
	eon_theme_scrollbar_thumb_min_size_get(theme_r, &min);
	if (thumb_size < min) thumb_size = min;
	eon_theme_scrollbar_thumb_size_set(theme_r, thumb_size);
	/* now set the percent of the thumb */

	percent = thiz->value / (thiz->max - thiz->min);
	eon_theme_scrollbar_thumb_percent_set(theme_r, percent);

	return EINA_TRUE;
}

static void _eon_scrollbar_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	thiz->changed = EINA_FALSE;
}

static Eina_Bool _eon_scrollbar_has_changed(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollbar_get(r);
	return thiz->changed;
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
	.cleanup = _eon_scrollbar_cleanup,
	.has_changed = _eon_scrollbar_has_changed,
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
	/* default values */
	thiz->value = 0;
	thiz->max = 100;
	thiz->min = 0;
	thiz->step_increment = 1;
	thiz->page_increment = 10;
	thiz->page_size = 10;

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
	thiz->changed = EINA_TRUE;

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
	thiz->changed = EINA_TRUE;
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
	thiz->changed = EINA_TRUE;
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
	thiz->changed = EINA_TRUE;
}

static void _eon_scrollbar_page_size_get(Enesim_Renderer *r, double *page_size)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(r);
	if (!thiz) return;
	*page_size = thiz->page_size;
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
	if (value > thiz->max - thiz->page_size) value = thiz->max - thiz->page_size;
	if (value < thiz->min) value = thiz->min;
	thiz->value = value;
	thiz->changed = EINA_TRUE;
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

