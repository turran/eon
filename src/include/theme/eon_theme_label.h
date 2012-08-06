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

typedef void (*Eon_Theme_Label_Text_Set)(Eon_Theme_Widget *t, const char *str);
typedef void (*Eon_Theme_Label_Text_Get)(Eon_Theme_Widget *t, const char **str);
typedef void (*Eon_Theme_Label_Font_Set)(Eon_Theme_Widget *t, const char *str);
typedef void (*Eon_Theme_Label_Font_Get)(Eon_Theme_Widget *t, const char **str);
typedef void (*Eon_Theme_Label_Font_Size_Set)(Eon_Theme_Widget *t, int str);
typedef void (*Eon_Theme_Label_Font_Size_Get)(Eon_Theme_Widget *t, int *str);

typedef struct _Eon_Theme_Label_Descriptor
{
	/* widget descriptor */
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_X_Set x_set;
	Eon_Theme_Widget_Y_Set y_set;
	Eon_Theme_Widget_Width_Set width_set;
	Eon_Theme_Widget_Height_Set height_set;
	Eon_Theme_Widget_Free free;
	/* the label needed functions */
	Eon_Theme_Label_Text_Set text_set;
	Eon_Theme_Label_Text_Get text_get;
	Eon_Theme_Label_Font_Size_Get font_size_get;
	Eon_Theme_Label_Font_Size_Set font_size_set;
	Eon_Theme_Label_Font_Get font_get;
	Eon_Theme_Label_Font_Set font_set;
} Eon_Theme_Label_Descriptor;

EAPI Eon_Theme_Widget * eon_theme_label_new(Eon_Theme_Label_Descriptor *descriptor,
		void *data);
EAPI Eina_Bool eon_is_theme_label(Eon_Theme_Widget *t);
EAPI void * eon_theme_label_data_get(Eon_Theme_Widget *t);
EAPI void eon_theme_label_font_set(Eon_Theme_Widget *t, const char *str);
EAPI void eon_theme_label_font_get(Eon_Theme_Widget *t, const char **str);
EAPI void eon_theme_label_font_size_set(Eon_Theme_Widget *t, int size);
EAPI void eon_theme_label_font_size_get(Eon_Theme_Widget *t, int *size);
EAPI void eon_theme_label_text_set(Eon_Theme_Widget *t, const char *text);
EAPI void eon_theme_label_text_get(Eon_Theme_Widget *t, const char **text);

/**
 * @}
 */
#endif


