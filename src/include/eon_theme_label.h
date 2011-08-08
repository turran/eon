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
typedef double (*Eon_Theme_Label_Max_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Label_Max_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Label_Min_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Label_Min_Width_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Label_Preferred_Height_Get)(Enesim_Renderer *r);
typedef double (*Eon_Theme_Label_Preferred_Width_Get)(Enesim_Renderer *r);

/* font name */
typedef void (*Eon_Theme_Label_Font_Set)(Enesim_Renderer *r, const char *font);
typedef const char * (*Eon_Theme_Label_Font_Get)(Enesim_Renderer *r);
/* font size */
typedef void (*Eon_Theme_Label_Size_Set)(Enesim_Renderer *r, int size);
typedef int (*Eon_Theme_Label_Size_Get)(Enesim_Renderer *r);
/* text */
typedef void (*Eon_Theme_Label_Text_Set)(Enesim_Renderer *r, const char *str);
typedef const char * (*Eon_Theme_Label_Text_Get)(Enesim_Renderer *r);
/* ellipsize */
typedef void (*Eon_Theme_Label_Ellipsize_Set)(Enesim_Renderer *r, Eina_Bool enable);
typedef Eina_Bool (*Eon_Theme_Label_Ellipsize_Get)(Enesim_Renderer *r);
/* height ascender + ascender */
typedef unsigned int (*Eon_Theme_Label_Height_Get)(Enesim_Renderer *r);
typedef unsigned int (*Eon_Theme_Label_Width_Get)(Enesim_Renderer *r);

typedef struct _Eon_Theme_Label_Descriptor
{
	/* the label needed functions */
	Eon_Theme_Label_Font_Set font_set;
	Eon_Theme_Label_Font_Get font_get;
	Eon_Theme_Label_Size_Set size_set;
	Eon_Theme_Label_Size_Get size_get;
	Eon_Theme_Label_Text_Set text_set;
	Eon_Theme_Label_Text_Get text_get;
	Eon_Theme_Label_Ellipsize_Set ellipsize_set;
	Eon_Theme_Label_Ellipsize_Get ellipsize_get;
	Eon_Theme_Label_Height_Get height_get;
	Eon_Theme_Label_Width_Get width_get;
	Eon_Theme_Label_Max_Width_Get max_width_get;
	Eon_Theme_Label_Min_Width_Get min_width_get;
	Eon_Theme_Label_Max_Height_Get max_height_get;
	Eon_Theme_Label_Min_Height_Get min_height_get;
	Eon_Theme_Label_Preferred_Width_Get preferred_width_get;
	Eon_Theme_Label_Preferred_Height_Get preferred_height_get;
	/* renderer needed functions */
	Enesim_Renderer_Sw_Setup sw_setup;
	Enesim_Renderer_Sw_Cleanup sw_cleanup;
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


