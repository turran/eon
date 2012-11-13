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

	Eon_Geometry track_area;
	double thumb_length;

	/* needed for the layout */
	double min_length;

	Eina_Bool thumb_dragging;
	double offset_dragging;
} Eon_Scrollbar;

static inline Eon_Scrollbar * _eon_scrollbar_get(Eon_Element *ee)
{
	Eon_Scrollbar *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}

static inline double _eon_scrollbar_value_sanitize(Eon_Scrollbar *thiz, double value)
{
	if (value < thiz->min)
		value = thiz->min;
	if (value > thiz->max)
		value = thiz->max;
	return value;
}

static double _eon_scrollbar_thumb_length(Eon_Scrollbar *thiz, double length, double min, double max)
{
	double size;

	size = thiz->page_size * length;
	if (size < min)
		size = min;
	if (size > max)
		size = max;

	return size;
}

static double _eon_scrollbar_thumb_position(Eon_Scrollbar *thiz, double s, double length)
{
	double percent;

	percent = thiz->value / (thiz->max - thiz->min);
	return s + ((length - thiz->thumb_length) * percent);
}

static void _eon_scrollbar_thumb_geometry_get(Eon_Scrollbar *thiz, Eon_Geometry *g, Eon_Geometry *dst)
{
	Eon_Size min, max;

	eon_theme_control_scroll_thumb_sizes_get(thiz->t_thumb->object, &min, &max);

	/* TODO center the thumb */
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		thiz->thumb_length = _eon_scrollbar_thumb_length(thiz, g->width, min.width, max.width);
		dst->x = _eon_scrollbar_thumb_position(thiz, g->x, g->width);
		dst->y = g->y;
		dst->width = thiz->thumb_length;
		dst->height = g->height;
	}
	else
	{
		thiz->thumb_length = _eon_scrollbar_thumb_length(thiz, g->height, min.height, max.height);
		dst->x = g->x;
		dst->y = _eon_scrollbar_thumb_position(thiz, g->y, g->height);
		dst->width = g->width;
		dst->height = thiz->thumb_length;
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
 *                            The arrows layout                               *
 *----------------------------------------------------------------------------*/
static void _arrow_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = 2;
	margin->top = 2;
	margin->bottom = 2;
	margin->right = 2;
}

static void _arrow_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Theme_Instance *child_thiz = child;
	Eon_Geometry dst;
	Eon_Size size;
	Eon_Hints hints;

	/* make the arrow be centered */
	eon_theme_control_scroll_arrow_size_get(child_thiz->object, &size);
	eon_hints_sizes_values_set(&hints, size.width, size.height, size.width, size.height, size.width, size.height);
	dst = *g;
	eon_hints_geometry_align(&hints, &dst, EON_HORIZONTAL_ALIGNMENT_CENTER,
				EON_VERTICAL_ALIGNMENT_CENTER);
	eon_theme_instance_property_set(child_thiz, "x", dst.x, NULL);
	eon_theme_instance_property_set(child_thiz, "y", dst.y, NULL);
	eon_theme_instance_property_set(child_thiz, "width", dst.width, NULL);
	eon_theme_instance_property_set(child_thiz, "height", dst.height, NULL);
}

static void _arrow_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Theme_Instance *child_thiz = child;
	Eon_Size size;

	eon_theme_control_scroll_arrow_size_get(child_thiz->object, &size);
	printf("arrow size %g %g\n", size.width, size.height);
	hints->min = size;
	hints->max = size;
	hints->preferred = size;
}

static void _increment_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Scrollbar *thiz = ref;
	cb(ref, thiz->t_increment, data);
}

static void _decrement_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Scrollbar *thiz = ref;
	cb(ref, thiz->t_decrement, data);
}

static Eon_Layout_Frame_Descriptor _increment_layout = {
	/* .child_padding_get 	= */ _arrow_layout_child_padding_get,
	/* .child_foreach 	= */ _increment_layout_child_foreach,
	/* .child_hints_get 	= */ _arrow_layout_child_hints_get,
	/* .child_geometry_set 	= */ _arrow_layout_child_geometry_set,
};

static Eon_Layout_Frame_Descriptor _decrement_layout = {
	/* .child_padding_get 	= */ _arrow_layout_child_padding_get,
	/* .child_foreach 	= */ _decrement_layout_child_foreach,
	/* .child_hints_get 	= */ _arrow_layout_child_hints_get,
	/* .child_geometry_set 	= */ _arrow_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                             The thumb layout                               *
 *----------------------------------------------------------------------------*/
static void _thumb_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	Eon_Scrollbar *thiz = ref;

	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		margin->left = 10;
		margin->top = 2;
		margin->bottom = 2;
		margin->right = 10;
	}
	else
	{
		margin->left = 2;
		margin->top = 10;
		margin->bottom = 10;
		margin->right = 2;
	}
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
		/* get the new geometry of the thumb position */
		_eon_scrollbar_thumb_geometry_get(thiz, g, &dst);
		printf("real thumb size is %g %g %g %g\n", dst.x, dst.y, dst.width, dst.height); 
	}
	else
	{
		Eon_Hints hints;

		dst = *g;
		/* store the tracking area */
		thiz->track_area = dst;
		/* make the thumb area be centered */
		_eon_scrollbar_thumb_area_hints_get(thiz, &hints);
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
	Eon_Layout_Child_Data *data = child;

	eon_layout_geometry_set(data->layout, data->descriptor, thiz, g);
}

static void _scrollbar_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Scrollbar *thiz = ref;
	Eon_Layout_Child_Data *data = child;

	eon_layout_hints_get(data->layout, data->descriptor, thiz, hints);
}

static void _scrollbar_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Layout_Child_Data child;
	Eon_Scrollbar *thiz = ref;

	/* call them in order */
	/* TODO later we can make the user provides the order
	 * so the controls are ordered like:
	 * arrow - arrow - thumb,
	 * arrow - thumb - arrow,
	 * etc
	 */
	child.layout =  &eon_layout_frame;
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		child.descriptor = &_decrement_layout;
		cb(ref, &child, data);
		child.descriptor = &_thumb_layout;
		cb(ref, &child, data);
		child.descriptor = &_increment_layout;
		cb(ref, &child, data);
	}
	else
	{
		child.descriptor = &_increment_layout;
		cb(ref, &child, data);
		child.descriptor = &_thumb_layout;
		cb(ref, &child, data);
		child.descriptor = &_decrement_layout;
		cb(ref, &child, data);
	}
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

static int _scrollbar_layout_child_weight_get(void *ref, void *child)
{
	Eon_Layout_Child_Data *data = child;

	/* the thumb should take all the space */
	if (data->descriptor == &_thumb_layout)
		return 1;
	else
		return 0;
}

static Eon_Layout_Stack_Descriptor _scrollbar_layout = {
	/* .orientation_get 	= */ _scrollbar_layout_orientation_get,
	/* .min_length_get 	= */ _scrollbar_layout_min_length_get,
	/* .min_length_get 	= */ _scrollbar_layout_min_length_set,
	/* .child_weight_get 	= */ _scrollbar_layout_child_weight_get,
	/* .child_foreach 	= */ _scrollbar_layout_child_foreach,
	/* .child_hints_get 	= */ _scrollbar_layout_child_hints_get,
	/* .child_geometry_set 	= */ _scrollbar_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                                Event handlers                              *
 *----------------------------------------------------------------------------*/
static void _eon_scrollbar_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz = data;
	Eon_Event_Mouse_Move *ev = event_data;
	double x, y;
	double c;
	double cmin;
	double cmax;
	double v;
	double percent;

	if (!thiz->thumb_dragging) return;

	x = ev->x;
	y = ev->y;

	/* get the absolute position of the event */
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		cmin = thiz->track_area.x;
		cmax = cmin + thiz->track_area.width - thiz->thumb_length;
		c = x - thiz->offset_dragging;
	}
	else
	{
		cmin = thiz->track_area.y;
		cmax = cmin + thiz->track_area.height - thiz->thumb_length;
		c = y - thiz->offset_dragging;
	}
	/* normalize the coordinate */
	if (c < cmin)
		c = cmin;
	if (c > cmax)
		c = cmax;
	percent = (c - cmin) / (cmax - cmin);
	/* get the value matching the coordinate */
	v = thiz->min + ((thiz->max - thiz->min) * percent);

	v = _eon_scrollbar_value_sanitize(thiz, v);
	if (thiz->value != v)
		eon_scrollbar_value_set(e, v);
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
	Eon_Scrollbar *thiz = data;
	Eon_Event_Mouse_Drag_Start *ev = event_data;
	Eon_Geometry g;
	double x, y;

	x = ev->x;
	y = ev->y;

	/* keep the coordinates relative to the thumb */
	eon_theme_widget_geometry_get(thiz->t_thumb->object, &g);
	if (!eon_geometry_is_inside(&g, x, y)) return;
	
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		thiz->offset_dragging = x - g.x;
	else
		thiz->offset_dragging = y - g.y;
	thiz->thumb_dragging = EINA_TRUE;
}

static void _eon_scrollbar_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollbar *thiz = data;
	Eon_Event_Mouse_Click *ev = event_data;
	Eon_Geometry g;
	double x, y;
	double v;

	x = ev->x;
	y = ev->y;

	printf("clicked! at %g %g\n", x, y);
	eon_theme_widget_geometry_get(thiz->t_increment->object, &g);
	if (eon_geometry_is_inside(&g, x, y))
	{
		v = thiz->value + thiz->step_increment;
		goto set;
	}

	eon_theme_widget_geometry_get(thiz->t_decrement->object, &g);
	if (eon_geometry_is_inside(&g, x, y))
	{
		v = thiz->value - thiz->step_increment;
		goto set;
	}

	eon_theme_widget_geometry_get(thiz->t_thumb->object, &g);
	if (eon_geometry_is_inside(&g, x, y))
	{
		printf("thumb!\n");
		return;
	}
	else
	{
		double c, l, ic;

		printf("page\n");
		if (!eon_geometry_is_inside(&thiz->track_area, x, y)) return;

		if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		{
			ic = x;
			c = g.x;
			l = g.width;
		}
		else
		{
			ic = y;
			c = g.y;
			l = g.height;
		}

		if (ic > c + l)
		{
			v = thiz->value + thiz->page_increment;
		}
		else
		{
			v = thiz->value - thiz->page_increment;
		}
		goto set;
	}

	return;
set:
	v = _eon_scrollbar_value_sanitize(thiz, v);
	if (thiz->value != v)
		eon_scrollbar_value_set(e, v);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_scrollbar_initialize(Ender_Element *e)
{
	Eon_Scrollbar *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollbar_get(ee);

	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK], _eon_scrollbar_mouse_click, thiz);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_START], _eon_scrollbar_mouse_drag_start, thiz);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_STOP], _eon_scrollbar_mouse_drag_stop, thiz);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_scrollbar_mouse_move, thiz);
}

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
	/* .initialize 		= */ _eon_scrollbar_initialize,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
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
	thiz->page_size = 0.5;

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
	Eon_Geometry dst;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;

	if (page_size < 0)
		page_size = 0;
	if (page_size > 1)
		page_size = 1;

	thiz->page_size = page_size;

	if (eon_element_has_changed(ee))
		return;

	_eon_scrollbar_thumb_geometry_get(thiz, &thiz->track_area, &dst);
	eon_theme_instance_property_set(thiz->t_thumb, "x", dst.x, NULL);
	eon_theme_instance_property_set(thiz->t_thumb, "y", dst.y, NULL);
	eon_theme_instance_property_set(thiz->t_thumb, "width", dst.width, NULL);
	eon_theme_instance_property_set(thiz->t_thumb, "height", dst.height, NULL);
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

	value = _eon_scrollbar_value_sanitize(thiz, value);

	thiz->value = value;
	if (eon_element_has_changed(ee))
		return;

	/* set the thumb position */
	printf("setting value!!! %g\n", value);
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double p;

		p = _eon_scrollbar_thumb_position(thiz, thiz->track_area.x, thiz->track_area.width);
		eon_theme_instance_property_set(thiz->t_thumb, "x", p, NULL);
	}
	else
	{
		double p;

		p = _eon_scrollbar_thumb_position(thiz, thiz->track_area.y, thiz->track_area.height);
		eon_theme_instance_property_set(thiz->t_thumb, "y", p, NULL);
	}
}

static void _eon_scrollbar_value_get(Eon_Element *ee, double *value)
{
	Eon_Scrollbar *thiz;

	thiz = _eon_scrollbar_get(ee);
	if (!thiz) return;
	*value = thiz->value;
}

#define _eon_scrollbar_delete NULL
#include "eon_generated_scrollbar.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_scrollbar_init(void)
{
	_eon_scrollbar_init();
}

void eon_scrollbar_shutdown(void)
{
	_eon_scrollbar_shutdown();
}
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
 * This is between 0 and 1. This range is relative to the area specified for the scrollarea
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

