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

#ifndef _EON_THEME_CONTROL_SCROLL_THUMB_H_
#define _EON_THEME_CONTROL_SCROLL_THUMB_H_

/**
 * @{
 */

typedef void (*Eon_Theme_Control_Scroll_Thumb_Sizes_Get)(Eon_Theme_Widget *t, Eon_Size *min, Eon_Size *max);
typedef void (*Eon_Theme_Control_Scroll_Thumb_Orientation_Set)(Eon_Theme_Widget *t, Eon_Orientation orientation);

typedef struct _Eon_Theme_Control_Scroll_Thumb_Descriptor
{
	/* widget descriptor */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_X_Set x_set;
	Eon_Theme_Widget_Y_Set y_set;
	Eon_Theme_Widget_Width_Set width_set;
	Eon_Theme_Widget_Height_Set height_set;
	Eon_Theme_Widget_Free free;
	/* control_scroll_thumb descriptor */
	Eon_Theme_Control_Scroll_Thumb_Sizes_Get sizes_get;
	Eon_Theme_Control_Scroll_Thumb_Orientation_Set orientation_set;
} Eon_Theme_Control_Scroll_Thumb_Descriptor;

EAPI Eon_Theme_Widget * eon_theme_control_scroll_thumb_new(Eon_Theme_Control_Scroll_Thumb_Descriptor *descriptor, void *data);
EAPI void * eon_theme_control_scroll_thumb_data_get(Eon_Theme_Widget *t);

EAPI void eon_theme_control_scroll_thumb_sizes_get(Eon_Theme_Widget *t, Eon_Size *min, Eon_Size *max);
EAPI void eon_theme_control_scroll_thumb_orientation_set(Eon_Theme_Widget *t, Eon_Orientation orientation);

/**
 * @}
 */
#endif


