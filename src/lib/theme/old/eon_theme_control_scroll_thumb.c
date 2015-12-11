/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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

#include "eon_theme_widget.h"
#include "eon_theme_control_scroll_thumb.h"

#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Control_Scroll_Thumb_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Control_Scroll_Thumb_Sizes_Get sizes_get;
	Eon_Theme_Control_Scroll_Thumb_Orientation_Set orientation_set;
} Eon_Theme_Control_Scroll_Thumb_Descriptor_Internal;

typedef struct _Eon_Theme_Control_Scroll_Thumb
{
	Eon_Theme_Control_Scroll_Thumb_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Control_Scroll_Thumb;

static inline Eon_Theme_Control_Scroll_Thumb * _eon_theme_control_scroll_thumb_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Control_Scroll_Thumb *thiz;

	thiz = eon_theme_widget_data_get(t);
	return thiz;
}

static void _eon_theme_control_scroll_thumb_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_control_scroll_thumb_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_theme_control_scroll_thumb_new(Eon_Theme_Control_Scroll_Thumb_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Control_Scroll_Thumb *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;

	thiz = calloc(1, sizeof(Eon_Theme_Control_Scroll_Thumb));
	thiz->descriptor.sizes_get = descriptor->sizes_get;
	thiz->descriptor.orientation_set = descriptor->orientation_set;
	thiz->descriptor.free = _eon_theme_control_scroll_thumb_free;
	thiz->data = data;
	/* widget descriptor */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_control_scroll_thumb_free;

	return eon_theme_widget_new(&pdescriptor, thiz);
}

EAPI void * eon_theme_control_scroll_thumb_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_control_scroll_thumb_get(t);
	return thiz->data;
}

EAPI void eon_theme_control_scroll_thumb_sizes_get(Eon_Theme_Widget *t, Eon_Size *min, Eon_Size *max)
{
	Eon_Theme_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_control_scroll_thumb_get(t);
	if (thiz->descriptor.sizes_get)
		thiz->descriptor.sizes_get(t, min, max);
	else
	{
		min->width = min->height = 0;
		max->width = max->height = DBL_MAX;
	}	
}

EAPI void eon_theme_control_scroll_thumb_orientation_set(Eon_Theme_Widget *t, Eon_Orientation orientation)
{
	Eon_Theme_Control_Scroll_Thumb *thiz;

	thiz = _eon_theme_control_scroll_thumb_get(t);
	if (thiz->descriptor.orientation_set)
		thiz->descriptor.orientation_set(t, orientation);
}

