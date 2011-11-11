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

#ifndef _EON_THEME_WIDGET_H_
#define _EON_THEME_WIDGET_H_

/**
 * @{
 */

typedef Enesim_Renderer * (*Eon_Theme_Widget_Renderer_Get)(Enesim_Renderer *r);
typedef Eina_Bool (*Eon_Theme_Widget_Setup)(Enesim_Renderer *r, Enesim_Error **error);
typedef void (*Eon_Theme_Widget_Cleanup)(Enesim_Renderer *r);
typedef Eina_Bool (*Eon_Theme_Widget_Has_Changed)(Enesim_Renderer *r);

EAPI void eon_theme_widget_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_height_get(Enesim_Renderer *r, double *width);

/**
 * @}
 */

#endif
