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

#ifndef _EON_THEME_BIN_H_
#define _EON_THEME_BIN_H_

/**
 * @{
 */

typedef void (*Eon_Theme_Bin_Child_Set)(Eon_Theme_Widget *t, Enesim_Renderer *child);
typedef void (*Eon_Theme_Bin_Child_Get)(Eon_Theme_Widget *t, Enesim_Renderer **child);

typedef struct _Eon_Theme_Bin_Descriptor
{
	/* widget descriptor */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_X_Set x_set;
	Eon_Theme_Widget_Y_Set y_set;
	Eon_Theme_Widget_Width_Set width_set;
	Eon_Theme_Widget_Height_Set height_set;
	Eon_Theme_Widget_Free free;
	/* bin descriptor */
	Eon_Theme_Bin_Child_Set child_set;
	Eon_Theme_Bin_Child_Get child_get;
} Eon_Theme_Bin_Descriptor;

/* private */
EAPI Eon_Theme_Widget * eon_theme_bin_new(Eon_Theme_Bin_Descriptor *descriptor, void *data);
EAPI void * eon_theme_bin_data_get(Eon_Theme_Widget *t);

/**
 * @}
 */
#endif

