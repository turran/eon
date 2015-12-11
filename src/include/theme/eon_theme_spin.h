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

#ifndef _EON_THEME_SPIN_H_
#define _EON_THEME_SPIN_H_

/**
 * @{
 */
typedef void (*Eon_Theme_Spin_Margin_Get)(Enesim_Renderer *r, Eon_Margin *margin);
typedef void (*Eon_Theme_Spin_Entry_Set)(Enesim_Renderer *r, Enesim_Renderer *entry, Enesim_Renderer *old_entry);
typedef void (*Eon_Theme_Spin_Arrows_Is_Inside)(Enesim_Renderer *r, Eon_Position *cursor, Eina_Bool *inc, Eina_Bool *dec);

typedef struct _Eon_Theme_Spin_Descriptor
{
	Eon_Theme_Spin_Margin_Get margin_get;
	Eon_Theme_Spin_Entry_Set entry_set;
	Eon_Theme_Spin_Arrows_Is_Inside arrows_is_inside;
	/* renderer needed functions */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Spin_Descriptor;

EAPI Enesim_Renderer * eon_theme_spin_new(Eon_Theme_Spin_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_spin(Enesim_Renderer *r);
EAPI void * eon_theme_spin_data_get(Enesim_Renderer *r);

/**
 * @}
 */


#endif


