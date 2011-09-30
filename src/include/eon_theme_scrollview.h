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

#ifndef _EON_THEME_SCROLLVIEW_H_
#define _EON_THEME_SCROLLVIEW_H_

/**
 * @{
 */

typedef struct _Eon_Theme_Scrollview_Descriptor
{
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
	Enesim_Renderer_Delete free;
} Eon_Theme_Scrollview_Descriptor;

EAPI Enesim_Renderer * eon_theme_scrollview_new(Eon_Theme_Scrollview_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_scrollview(Enesim_Renderer *r);
EAPI void * eon_theme_scrollview_data_get(Enesim_Renderer *r);
EAPI void eon_theme_scrollview_hbar_set(Enesim_Renderer *r, Enesim_Renderer *hbar);
EAPI void eon_theme_scrollview_hbar_get(Enesim_Renderer *r, Enesim_Renderer **hbar);
EAPI void eon_theme_scrollview_vbar_set(Enesim_Renderer *r, Enesim_Renderer *vbar);
EAPI void eon_theme_scrollview_vbar_get(Enesim_Renderer *r, Enesim_Renderer **vbar);
EAPI void eon_theme_scrollview_offset_set(Enesim_Renderer *r, Eon_Position *position);
EAPI void eon_theme_scrollview_offset_get(Enesim_Renderer *r, Eon_Position *position);

/**
 * @}
 */

#endif
