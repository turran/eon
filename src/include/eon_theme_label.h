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

#ifndef _EON_THEME_LABEL_H_
#define _EON_THEME_LABEL_H_

/**
 * @{
 */

typedef Eina_Bool (*Eon_Theme_Label_Setup)(Enesim_Renderer *r, Enesim_Renderer *text, Enesim_Error **err);
typedef void (*Eon_Theme_Label_Cleanup)(Enesim_Renderer *r);
typedef Enesim_Renderer * (*Eon_Theme_Label_Renderer_Get)(Enesim_Renderer *r, Enesim_Renderer *text);
typedef void (*Eon_Theme_Label_Padding_Get)(Enesim_Renderer *r, Eon_Margin padding);

typedef struct _Eon_Theme_Label_Descriptor
{
	/* the label needed functions */
	Eon_Theme_Label_Renderer_Get renderer_get;
	Eon_Theme_Label_Setup setup;
	Eon_Theme_Label_Cleanup cleanup;
	Eon_Theme_Label_Padding_Get padding_get;
	Eina_Bool can_ellipsize;
	/* renderer needed functions */
	Enesim_Renderer_Delete free;
} Eon_Theme_Label_Descriptor;

EAPI Enesim_Renderer * eon_theme_label_new(Eon_Theme_Label_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_label(Enesim_Renderer *r);
EAPI void * eon_theme_label_data_get(Enesim_Renderer *r);
EAPI void eon_theme_label_font_set(Enesim_Renderer *r, const char *str);
EAPI void eon_theme_label_font_get(Enesim_Renderer *r, const char **str);
EAPI void eon_theme_label_size_set(Enesim_Renderer *r, int size);
EAPI void eon_theme_label_size_get(Enesim_Renderer *r, int *size);
EAPI void eon_theme_label_text_set(Enesim_Renderer *r, const char *text);
EAPI void eon_theme_label_text_get(Enesim_Renderer *r, const char **text);
EAPI void eon_theme_label_ellipsize_set(Enesim_Renderer *r, Eina_Bool enable);
EAPI void eon_theme_label_ellipsize_get(Enesim_Renderer *r, Eina_Bool *enable);
EAPI void eon_theme_label_max_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_label_max_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_label_min_width_get(Enesim_Renderer *r, double *width);
EAPI void eon_theme_label_min_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_label_preferred_height_get(Enesim_Renderer *r, double *height);
EAPI void eon_theme_label_preferred_width_get(Enesim_Renderer *r, double *width);


/**
 * @}
 */
#endif


