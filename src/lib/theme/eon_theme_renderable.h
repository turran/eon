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

#ifndef _EON_THEME_RENDERABLE_H_
#define _EON_THEME_RENDERABLE_H_

typedef Enesim_Renderer * (*Eon_Theme_Renderable_Descriptor_Renderer_Get_Cb)(void *data);
EAPI void eon_theme_renderable_geometry_get(Egueb_Dom_Node *n, Eina_Rectangle *geom);
EAPI void eon_theme_renderable_geometry_invalidate(Egueb_Dom_Node *n);

#endif
