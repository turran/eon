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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_scrollbar.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"

#include "eon_private_layout.h"
#include "eon_private_layout_stack.h"
#include "eon_private_layout_frame.h"

#include "theme/eon_theme_widget.h"
#include "theme/eon_theme_scrollbar.h"
#include "theme/eon_theme_control_scroll_arrow.h"
#include "theme/eon_theme_control_scroll_thumb.h"
#include "theme/eon_theme_control_scroll_thumb_area.h"
/* The scrollbar is composed of differnt theme instances. One
 * for the thumb container (the thumb and the scrolling area)
 * and two for the different arrows. All of it is
 * wrapped inside a stack layout to order theme
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_SCROLLBAR_MAX;
static Ender_Property *EON_SCROLLBAR_MIN;
static Ender_Property *EON_SCROLLBAR_VALUE;
static Ender_Property *EON_SCROLLBAR_ORIENTATION;
static Ender_Property *EON_SCROLLBAR_PAGE_SIZE;
static Ender_Property *EON_SCROLLBAR_PAGE_INCREMENT;
static Ender_Property *EON_SCROLLBAR_STEP_INCREMENT;

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
	Eon_Theme_Instance *t_increment;
	Eon_Theme_Instance *t_decrement;
	Eon_Theme_Instance *t_thumb;
	Eon_Theme_Instance *t_thumb_area;
	/* needed for the layout */
	double min_length;

	Eina_Bool changed : 1;
	Eina_Bool thumb_dragging;
	double offset_dragging;
} Eon_Scrollbar;

static inline Eon_Scrollbar * _eon_scrollbar_get(Eon_Element *ee)
{
	Eon_Scrollbar *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}

static inline double _eon_scrollbar_value_check(Eon_Scrollbar *thiz, double v)
{
	if (v > thiz->max - thiz->page_size) v = thiz->max - thiz->page_size;
	if (v < thiz->min) v = thiz->min;

	return v;
}

static double _eon_scrollbar_thumb_length(Eon_Scrollbar *thiz, double length, double min, double max)
{
	double size;

	size = (thiz->page_size / (thiz->max - thiz->min)) * length;
	if (size < min)
		size = min;
	if (size > max)
		size = max;
	return size;
}

static void _eon_scrollbar_thumb_geometry_get(Eon_Scrollbar *thiz, Eon_Geometry *g, Eon_Geometry *dst)
{
	Eon_Size min, max;
	double thumb_size;

	eon_theme_control_scroll_thumb_sizes_get(thiz->t_thumb->object, &min, &max);

	dst->x = g->x;
	dst->y = g->y;
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		thumb_size = _eon_scrollbar_thumb_length(thiz, g->width, min.width, max.width);
		dst->width = thumb_size;
		dst->height = g->height;
	}
	else
	{
		thumb_size = _eon_scrollbar_thumb_length(thiz, g->height, min.height, max.height);
		dst->width = g->width;
		dst->height = thumb_size;
	}
}

static void _eon_scrollbar_thumb_area_hints_get(Eon_Scrollbar *thiz, Eon_Hints *hints)
{
	Eon_Size min, max;

	eon_theme_control_scroll_thumb_area_sizes_get(thiz->t_thumb_area->object, &min, &max);
	hints->min = min;
	hints->max = max;
	hints->preferred.width = hints->preferred.height = -1;
	printf("thumb area size %g %g %g %g\n", min.width, min.height, max.width, max.height);
}
/*----------------------------------------------------------------------------*
 *                             The thumb layout                               *
 *----------------------------------------------------------------------------*/
static void _thumb_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	//if (thiz->t_thumb == thiz_child)
	margin->left = 2;
	margin->top = 2;
	margin->bottom = 2;
	margin->right = 2;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _thumb_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Scrollbar *thiz = ref;
	Eon_Theme_Instance *child_thiz = child;
	Eon_Geometry dst;

	printf("thumb layout setting %g %g %g %g\n", g->x, g->y, g->width, g->height);
	if (child_thiz == thiz->t_thumb)
	{
		_eon_scrollbar_thumb_geometry_get(thiz, g, &dst);
		printf("real thumb size is %g %g %g %g\n", dst.x, dst.y, dst.width, dst.height); 
		/* FIXME now set the correct position */
		
	}
	else
	{
		Eon_Hints hints;

		/* make the thumb area be centered */
		_eon_scrollbar_thumb_area_hints_get(thiz, &hints);
		dst = *g;
		eon_hints_geometry_align(&hints, &dst, EON_HORIZONTAL_ALIGNMENT_CENTER,
				EON_VERTICAL_ALIGNMENT_CENTER);
	}

	eon_theme_instance_property_set(child_thiz, "x", dst.x, NULL);
	eon_theme_instance_property_set(child_thiz, "y", dst.y, NULL);
	eon_theme_instance_property_set(child_thiz, "width", dst.width, NULL);
	eon_theme_instance_property_set(child_thiz, "height", dst.height, NULL);
}

static void _thumb_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Scrollbar *thiz = ref;
	Eon_Theme_Instance *child_thiz = child;

	if (thiz->t_thumb == child_thiz)
	{
		Eon_Size min, max;

		/* the thumb size should be the real size for the area we want to display
		 * using the theme constraints
		 */
		eon_theme_control_scroll_thumb_sizes_get(child_thiz->object, &min, &max);
		hints->min = min;
		hints->max = max;
		hints->preferred.width = hints->preferred.height = -1;
		printf("thumb size %g %g %g %g\n", min.width, min.height, max.width, max.height);
	}
	else
	{
		_eon_scrollbar_thumb_area_hints_get(thiz, hints);
	}
}

static void _thumb_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Scrollbar *thiz = ref;

	/* first the area then the thumb */
	cb(ref, thiz->t_thumb_area, data);
	cb(ref, thiz->t_thumb, data);
}

static Eon_Layout_Frame_Descriptor _thumb_layout = {
	/* .child_padding_get 	= */ _thumb_layout_child_padding_get,
	/* .child_foreach 	= */ _thumb_layout_child_foreach,
	/* .child_hints_get 	= */ _thumb_layout_child_hints_get,
	/* .child_geometry_set 	= */ _thumb_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                       The Eon's layout descriptors                         *
 *----------------------------------------------------------------------------*/
static void _scrollbar_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Scrollbar *thiz = ref;
	Eon_Theme_Instance *child_thiz = child;

	if (child_thiz)
	{
		eon_theme_instance_property_set(child_thiz, "x", g->x, NULL);
		eon_theme_instance_property_set(child_thiz, "y", g->y, NULL);
		eon_theme_instance_property_set(child_thiz, "width", g->width, NULL);
		eon_theme_instance_property_set(child_thiz, "height", g->height, NULL);
	}
	else
	{
		printf("scrollbar thumb layout of size %g %g %g %g\n", g->x, g->y, g->width, g->height);
		eon_layout_geometry_set(&eon_layout_frame, &_thumb_layout, thiz, g);
	}
}

static void _scrollbar_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Scrollbar *thiz = ref;
	Eon_Theme_Instance *child_thiz = child;

	if (child_thiz)
	{
		Eon_Size size;

		eon_theme_control_scroll_arrow_size_get(child_thiz->object, &size);
		printf("arrow size %g %g\n", size.width, size.height);
		hints->min = size;
		hints->max = size;
		hints->preferred = size;
	}
	else
	{
		eon_layout_hints_get(&eon_layout_frame, &_thumb_layout, thiz, hints);
	}
}

static void _scrollbar_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Scrollbar *thiz = ref;

	/* call them in order */
	/* TODO later we can make the user provides the order
	 * so the controls are ordered like:
	 * arrow - arrow - thumb,
	 * arrow - thumb - arrow,
	 * etc
	 */
	cb(ref, thiz->t_increment, data);
	/* for the thumb we send NULL as a child to inform that we need
	 * the stack layout
	 */
	cb(ref, NULL, data);
	cb(ref, thiz->t_decrement, data);
}

static Eina_Bool _scrollbar_layout_is_homogeneous(void *ref)
{
	return EINA_FALSE;
}

static Eon_Orientation _scrollbar_layout_orientation_get(void *ref)
{
	Eon_Scrollbar *thiz = ref;
	return thiz->orientation;
}

static void _scrollbar_layout_min_length_get(void *ref, double *min)
{
	Eon_Scrollbar *thiz = ref;
	*min = thiz->min_length;
}

static void _scrollbar_layout_min_length_set(void *ref, double min)
{
	Eon_Scrollbar *thiz = ref;
	thiz->min_length = min;
}

static int _scrollbar_layout_child_gravity_get(void *ref, void *child)
{
	/* the thumb should take all the space */
	if (!child)
		return 1;
	else
		return 0;
}

static Eon_Layout_Stack_Descriptor _scrollbar_layout = {
	/* .is_homogeneous 	= */ _scrollbar_layout_is_homogeneous,
	/* .orientation_get 	= */ _scrollbar_layout_orientation_get,
	/* .min_length_get 	= */ _scrollbar_layout_min_length_get,
	/* .min_length_get 	= */ _scrollbar_layout_min_length_set,
	/* .child_gravity_get 	= */ _scrollbar_layout_child_gravity_get,
	/* .child_count_get 	= */ NULL,
	/* .child_count_set 	= */ NULL,
	/* .child_foreach 	= */ _scrollbar_layout_child_foreach,
	/* .child_hints_get 	= */ _scrollbar_layout_child_hints_get,
	/* .child_geometry_set 	= */ _scrollbar_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                                Event handlers                              *
 *----------------------------------------------------------------------------*/
#if 0
static void _eon_scrollbar_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Size size;
	Eon_Element *ee;
	double c;
	double v;
	double length;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);
	if (!thiz->thumb_dragging) return;

	eon_element_actual_size_get(ee, &size);
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
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);
	thiz->thumb_dragging = EINA_FALSE;
}

static void _eon_scrollbar_mouse_drag_start(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz;
	Eon_Event_Mouse_Drag_Start *ev = event_data;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle tg;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);
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
	Eon_Element *ee;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle ig, dg, tg;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);

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
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_scrollbar_initialize(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);

#if 0
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK], _eon_scrollbar_mouse_click, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_START], _eon_scrollbar_mouse_drag_start, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_STOP], _eon_scrollbar_mouse_drag_stop, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_scrollbar_mouse_move, NULL);
#endif
}

#if 0
static Eina_Bool _eon_scrollbar_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Scrollbar *thiz;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;
	double percent;
	double max, min;
	double thumb_size;
	double length;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);

	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		length = state->actual_size.width;
	else
		length = state->actual_size.height;

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

	thiz->changed = EINA_FALSE;
	return EINA_TRUE;
}
#endif

static void _eon_scrollbar_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(e);
	eon_layout_geometry_set(&eon_layout_stack, &_scrollbar_layout, thiz, g);
}

static void _eon_scrollbar_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(e);
	eon_layout_hints_get(&eon_layout_stack, &_scrollbar_layout, thiz, hints);
}

static void _eon_scrollbar_free(Eon_Element *e)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(e);
	free(thiz);
}


static Eon_Widget_Descriptor _eon_scrollbar_descriptor = {
	/* .initialize 		= */ NULL,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .setup 		= */ NULL,
	/* .needs_setup 	= */ NULL,
	/* .geometry_set 	= */ _eon_scrollbar_geometry_set,
	/* .free		= */ _eon_scrollbar_free,
	/* .name 		= */ "scrollbar",
	/* .hints_get 		= */ _eon_scrollbar_hints_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_scrollbar_new(void)
{
	Eon_Scrollbar *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;
	Eon_Theme_Instance *control;

	theme = eon_theme_instance_new("scrollbar", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Scrollbar));
	/* default values */
	thiz->orientation = EON_ORIENTATION_HORIZONTAL;
	thiz->value = 0;
	thiz->max = 100;
	thiz->min = 0;
	thiz->step_increment = 1;
	thiz->page_increment = 10;
	thiz->page_size = 10;

	control = eon_theme_instance_new("scroll_thumb_area", EINA_TRUE);
	if (!control) goto scroll_thumb_area_err;
	eon_theme_scrollbar_thumb_area_set(theme->object, control->renderer);
	eon_theme_control_scroll_thumb_area_orientation_set(control->object, EON_ORIENTATION_HORIZONTAL);
	thiz->t_thumb_area = control;

	control = eon_theme_instance_new("scroll_thumb", EINA_TRUE);
	if (!control) goto scroll_thumb_err;
	eon_theme_scrollbar_thumb_set(theme->object, control->renderer);
	eon_theme_control_scroll_thumb_orientation_set(control->object, EON_ORIENTATION_HORIZONTAL);
	thiz->t_thumb = control;

	control = eon_theme_instance_new("scroll_arrow_increment", EINA_TRUE);
	if (!control) goto scroll_increment_err;
	eon_theme_scrollbar_arrow_increment_set(theme->object, control->renderer);
	eon_theme_control_scroll_arrow_direction_set(control->object, EON_DIRECTION_RIGHT);
	thiz->t_increment = control;

	control = eon_theme_instance_new("scroll_arrow_decrement", EINA_TRUE);
	eon_theme_scrollbar_arrow_decrement_set(theme->object, control->renderer);
	eon_theme_control_scroll_arrow_direction_set(control->object, EON_DIRECTION_LEFT);
	if (!control) goto scroll_decrement_err;
	thiz->t_decrement = control;

	
	ee = eon_widget_new(theme, &_eon_scrollbar_descriptor, thiz);
	if (!ee) goto base_err;

	return ee;

base_err:
	// eon_theme_instance_free(thiz->t_decrement);
scroll_decrement_err:
	// eon_theme_instance_free(thiz->t_increment);
scroll_increment_err:
	// eon_theme_instance_free(thiz->t_thumb);
scroll_thumb_err:
	// eon_theme_instance_free(thiz->t_thumb_area);
scroll_thumb_area_err:
	// eon_theme_instance_free(theme);
	free(thiz);
	return NULL;
}

static void _eon_scrollbar_orientation_set(Eon_Element *ee, Eon_Orientation orientation)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	thiz->orientation = orientation;
	thiz->changed = EINA_TRUE;
	/* TODO inform of the change */

	if (thiz->orientation == EON_ORIENTATION_VERTICAL)
	{
		eon_theme_control_scroll_arrow_direction_set(thiz->t_increment->object, EON_DIRECTION_TOP);
		eon_theme_control_scroll_thumb_orientation_set(thiz->t_thumb->object, EON_ORIENTATION_VERTICAL);
		eon_theme_control_scroll_thumb_area_orientation_set(thiz->t_thumb_area->object, EON_ORIENTATION_VERTICAL);
		eon_theme_control_scroll_arrow_direction_set(thiz->t_decrement->object, EON_DIRECTION_BOTTOM);
	}
	else
	{
		eon_theme_control_scroll_arrow_direction_set(thiz->t_decrement->object, EON_DIRECTION_LEFT);
		eon_theme_control_scroll_thumb_orientation_set(thiz->t_thumb->object, EON_ORIENTATION_HORIZONTAL);
		eon_theme_control_scroll_thumb_area_orientation_set(thiz->t_thumb_area->object, EON_ORIENTATION_HORIZONTAL);
		eon_theme_control_scroll_arrow_direction_set(thiz->t_increment->object, EON_DIRECTION_RIGHT);
	}
}

static void _eon_scrollbar_orientation_get(Eon_Element *ee, Eon_Orientation *orientation)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*orientation = thiz->orientation;
}

static void _eon_scrollbar_max_set(Eon_Element *ee, double max)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	thiz->max = max;
	thiz->changed = EINA_TRUE;
}

static void _eon_scrollbar_max_get(Eon_Element *ee, double *max)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*max = thiz->max;
}

static void _eon_scrollbar_min_set(Eon_Element *ee, double min)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	thiz->min = min;
	thiz->changed = EINA_TRUE;
}

static void _eon_scrollbar_min_get(Eon_Element *ee, double *min)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*min = thiz->min;
}

static void _eon_scrollbar_page_increment_set(Eon_Element *ee, double page_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	thiz->page_increment = page_increment;
}

static void _eon_scrollbar_page_increment_get(Eon_Element *ee, double *page_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*page_increment = thiz->page_increment;
}

static void _eon_scrollbar_page_size_set(Eon_Element *ee, double page_size)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;

	thiz->page_size = page_size;
	thiz->changed = EINA_TRUE;
}

static void _eon_scrollbar_page_size_get(Eon_Element *ee, double *page_size)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*page_size = thiz->page_size;
}

static void _eon_scrollbar_step_increment_set(Eon_Element *ee, double step_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	thiz->step_increment = step_increment;
}

static void _eon_scrollbar_step_increment_get(Eon_Element *ee, double *step_increment)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*step_increment = thiz->step_increment;
}

static void _eon_scrollbar_value_set(Eon_Element *ee, double value)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	if (value > thiz->max - thiz->page_size) value = thiz->max - thiz->page_size;
	if (value < thiz->min) value = thiz->min;
	thiz->value = value;
	thiz->changed = EINA_TRUE;
}

static void _eon_scrollbar_value_get(Eon_Element *ee, double *value)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*value = thiz->value;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_scrollbar_delete NULL
#include "eon_generated_scrollbar.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_scrollbar_new(void)
{
	return EON_ELEMENT_NEW("scrollbar");
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
	ender_element_property_value_set(e, EON_SCROLLBAR_ORIENTATION, orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_orientation_get(Ender_Element *e, Eon_Orientation *orientation)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_ORIENTATION, orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_max_set(Ender_Element *e, double max)
{
	ender_element_property_value_set(e, EON_SCROLLBAR_MAX, max, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_max_get(Ender_Element *e, double *max)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_MAX, max, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_min_set(Ender_Element *e, double min)
{
	ender_element_property_value_set(e, EON_SCROLLBAR_MIN, min, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_min_get(Ender_Element *e, double *min)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_MIN, min, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_increment_set(Ender_Element *e, double page_increment)
{
	ender_element_property_value_set(e, EON_SCROLLBAR_PAGE_INCREMENT, page_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_increment_get(Ender_Element *e, double *page_increment)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_PAGE_INCREMENT, page_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_size_set(Ender_Element *e, double page_size)
{
	ender_element_property_value_set(e, EON_SCROLLBAR_PAGE_SIZE, page_size, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_page_size_get(Ender_Element *e, double *page_size)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_PAGE_SIZE, page_size, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_step_increment_set(Ender_Element *e, double step_increment)
{
	ender_element_property_value_set(e, EON_SCROLLBAR_STEP_INCREMENT, step_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_step_increment_get(Ender_Element *e, double *step_increment)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_STEP_INCREMENT, step_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_value_set(Ender_Element *e, double value)
{
	ender_element_property_value_set(e, EON_SCROLLBAR_VALUE, value, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_value_get(Ender_Element *e, double *value)
{
	ender_element_property_value_get(e, EON_SCROLLBAR_VALUE, value, NULL);
}

