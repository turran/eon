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

#ifndef _EON_THEME_LAYOUT_H_
#define _EON_THEME_LAYOUT_H_

/**
 * @{
 */

typedef void (*Eon_Theme_Container_Child_Add)(Eon_Theme_Widget *r,  Enesim_Renderer *child);
typedef void (*Eon_Theme_Container_Child_Remove)(Eon_Theme_Widget *r, Enesim_Renderer *child);

typedef struct _Eon_Theme_Container_Descriptor
{
	/* widget needed functions */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_X_Set x_set;
	Eon_Theme_Widget_Y_Set y_set;
	Eon_Theme_Widget_Width_Set width_set;
	Eon_Theme_Widget_Height_Set height_set;
	Eon_Theme_Widget_Free free;
	/* container needed functions */
	Eon_Theme_Container_Child_Add child_add;
	Eon_Theme_Container_Child_Remove child_remove;
} Eon_Theme_Container_Descriptor;

EAPI Eon_Theme_Widget * eon_theme_container_new(Eon_Theme_Container_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_layout(Eon_Theme_Widget *r);
EAPI void * eon_theme_container_data_get(Eon_Theme_Widget *r);
EAPI void eon_theme_container_child_add(Eon_Theme_Widget *r, Enesim_Renderer *child);
EAPI void eon_theme_container_child_remove(Eon_Theme_Widget *r, Enesim_Renderer *child);

/**
 * @}
 */

#endif
