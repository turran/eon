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

#ifndef _EON_THEME_CONTROL_SCROLL_ARROW_H_
#define _EON_THEME_CONTROL_SCROLL_ARROW_H_

/**
 * @{
 */

typedef void (*Eon_Theme_Control_Scroll_Arrow_Size_Get)(Eon_Theme_Widget *t, Eon_Size *size);
typedef void (*Eon_Theme_Control_Scroll_Arrow_Direction_Set)(Eon_Theme_Widget *t, Eon_Direction direction);

typedef struct _Eon_Theme_Control_Scroll_Arrow_Descriptor
{
	/* widget descriptor */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_X_Set x_set;
	Eon_Theme_Widget_Y_Set y_set;
	Eon_Theme_Widget_Free free;
	/* control_scroll_arrow descriptor */
	Eon_Theme_Control_Scroll_Arrow_Size_Get size_get;
	Eon_Theme_Control_Scroll_Arrow_Direction_Set direction_set;
} Eon_Theme_Control_Scroll_Arrow_Descriptor;

EAPI Eon_Theme_Widget * eon_theme_control_scroll_arrow_new(Eon_Theme_Control_Scroll_Arrow_Descriptor *descriptor, void *data);
EAPI void * eon_theme_control_scroll_arrow_data_get(Eon_Theme_Widget *t);

/**
 * @}
 */
#endif

