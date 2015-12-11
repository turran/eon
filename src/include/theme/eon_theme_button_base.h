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

#ifndef _EON_THEME_BUTTON_BASE_H_
#define _EON_THEME_BUTTON_BASE_H_

/**
 * @{
 */

/* FIXME
 * typedef void (*Eon_Theme_Button_Base_Margin_Get)(Enesim_Renderer *r, Eon_Margin *margin);
 * typedef void (*Eon_Theme_Button_Base_Min_Size_Get)(Enesim_Renderer *r, Eon_Size *size);
 *
 */

typedef void (*Eon_Theme_Button_Base_Margin_Get)(Enesim_Renderer *r, Eon_Margin *margin);
typedef void (*Eon_Theme_Button_Base_Position_Get)(Enesim_Renderer *r, Eon_Size *size, Eon_Position *position);

/**
 * @}
 */

#endif


