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

#ifndef _EON_COLOR_H_
#define _EON_COLOR_H_

/**
 * @defgroup Eon_Color Color
 * @ingroup Eon_Button_Base
 * @{
 */

EAPI Ender_Element * eon_color_new(void);
EAPI void eon_color_value_set(Ender_Element *e, Enesim_Argb value);
EAPI void eon_color_value_get(Ender_Element *e, Enesim_Argb *value);

/**
 * @}
 */

#endif


