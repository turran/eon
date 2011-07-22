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

#ifndef _EON_THEME_CONTAINER_H_
#define _EON_THEME_CONTAINER_H_

/**
 * @{
 */

typedef double (*Eon_Theme_Container_Decoration_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Container_Decoration_Height_Get)(Enesim_Renderer *r);
typedef void (*Eon_Theme_Container_Content_Position_Get)(Enesim_Renderer *r, Enesim_Renderer *content, double *x, double *y);

EAPI void eon_theme_container_content_set(Enesim_Renderer *r, Enesim_Renderer *content);
EAPI void eon_theme_container_content_get(Enesim_Renderer *r, Enesim_Renderer **content);

/**
 * @}
 */

#endif

