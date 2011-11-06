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

#ifndef _EON_THEME_SPLITTER_H_
#define _EON_THEME_SPLITTER_H_

/**
 * @{
 */
typedef double (*Eon_Theme_Splitter_Min_Length_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Splitter_Thickness_Get)(Enesim_Renderer *r);

typedef struct _Eon_Theme_Splitter_Descriptor
{
	Eon_Theme_Splitter_Min_Length_Get min_length_get;
	Eon_Theme_Splitter_Thickness_Get thickness_get;
	/* renderer needed functions */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Eon_Theme_Widget_Cleanup cleanup;
	Enesim_Renderer_Name name;
	Enesim_Renderer_Delete free;
} Eon_Theme_Splitter_Descriptor;

EAPI Enesim_Renderer * eon_theme_splitter_new(Eon_Theme_Splitter_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_splitter(Enesim_Renderer *r);
EAPI void * eon_theme_splitter_data_get(Enesim_Renderer *r);
EAPI void eon_theme_splitter_position_get(Enesim_Renderer *r, double *position);
EAPI void eon_theme_splitter_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation);

void eon_theme_splitter_position_set(Enesim_Renderer *r, double position);
void eon_theme_splitter_thickness_get(Enesim_Renderer *r, double *thickness);
void eon_theme_splitter_min_length_get(Enesim_Renderer *r, double *length);
void eon_theme_splitter_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation);

/**
 * @}
 */


#endif

