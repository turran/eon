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
typedef struct _Eon_Theme_Widget Eon_Theme_Widget;

/* widget descriptor functions */

typedef Enesim_Renderer * (*Eon_Theme_Widget_Renderer_Get)(Eon_Theme_Widget *t);
typedef void (*Eon_Theme_Widget_X_Set)(Eon_Theme_Widget *t, double x);
typedef void (*Eon_Theme_Widget_Y_Set)(Eon_Theme_Widget *t, double y);
typedef void (*Eon_Theme_Widget_Width_Set)(Eon_Theme_Widget *t, double width);
typedef void (*Eon_Theme_Widget_Height_Set)(Eon_Theme_Widget *t, double height);
typedef void (*Eon_Theme_Widget_Free)(Eon_Theme_Widget *t);

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
 * size (preferred, min, max, whatever) might have changed
 */
typedef Eina_Bool (*Eon_Theme_Widget_Informs_Setup)(Enesim_Renderer *r);

typedef struct _Eon_Theme_Widget_Descriptor
{
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_X_Set x_set;
	Eon_Theme_Widget_Y_Set y_set;
	Eon_Theme_Widget_Width_Set width_set;
	Eon_Theme_Widget_Height_Set height_set;
	Eon_Theme_Widget_Free free;
} Eon_Theme_Widget_Descriptor;

Eon_Theme_Widget * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor, void *data);
void * eon_theme_widget_data_get(Eon_Theme_Widget *t);
void eon_theme_widget_informs_setup_set(Eon_Theme_Widget *t, Eon_Theme_Widget_Informs_Setup setup);

/**
 * @}
 */

#endif
