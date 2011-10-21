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

#ifndef _EON_THEME_TOGGLE_H_
#define _EON_THEME_TOGGLE_H_

/**
 * @{
 */
typedef struct _Eon_Theme_Toggle_Descriptor
{
	/* container needed functions */
	Eon_Theme_Button_Base_Position_Get position_get;
	Eon_Theme_Button_Base_Margin_Get margin_get;
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Eon_Theme_Widget_Cleanup cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Toggle_Descriptor;

EAPI Enesim_Renderer * eon_theme_toggle_new(Eon_Theme_Toggle_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_toggle(Enesim_Renderer *r);
EAPI void * eon_theme_toggle_data_get(Enesim_Renderer *r);

/**
 * @}
 */
#endif

