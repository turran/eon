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
/*
 * This is funcion defines whenever the widget has changed some "visual" property
 * that requires the widget to redraw again, but not do the setup again. This is
 * only valid for visual properties and no layout properties
 */
typedef Eina_Bool (*Eon_Theme_Widget_Has_Changed)(Enesim_Renderer *r);
/* 
 * This functions defines a way for the widget to know whenever there is a
 * change on the theme that requires the widget to do its own setup again.
 *
 * The widget in order to know whenever it needs to do it's own setup
 * based on some property change of itself also checks whenever the theme
 * renderer associated with the widget has changed some property that requires
 * the widget to do it's own setup again. A simple usage would be a font size
 * change on the label theme, if that happens, in case a button has such label
 * as the content, the buttons needs to do it's own setup again given that
 * size (preferred, min, max, whaever) might changed
 */
typedef Eina_Bool (*Eon_Theme_Widget_Informs_Setup)(Enesim_Renderer *r);

EAPI void eon_theme_widget_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_widget_height_get(Enesim_Renderer *r, double *width);

/**
 * @}
 */

#endif