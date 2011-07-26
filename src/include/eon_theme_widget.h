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
typedef double (*Eon_Theme_Widget_Max_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Widget_Max_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Widget_Min_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Widget_Min_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Widget_Preferred_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Widget_Preferred_Width_Get)(Enesim_Renderer *r);

EAPI void eon_theme_widget_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_height_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_max_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_max_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_widget_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_min_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_widget_preferred_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_widget_preferred_width_get(Enesim_Renderer *r, double *width);

/**
 * @}
 */

#endif
