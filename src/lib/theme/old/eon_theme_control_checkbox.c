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
#include "Eon.h"

#include "eon_theme_widget.h"
#include "eon_theme_control_checkbox.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Control_Checkbox_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Control_Checkbox_Size_Get size_get;
} Eon_Theme_Control_Checkbox_Descriptor_Internal;

typedef struct _Eon_Theme_Control_Checkbox
{
	Eon_Theme_Control_Checkbox_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Control_Checkbox;

static inline Eon_Theme_Control_Checkbox * _eon_theme_control_checkbox_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Control_Checkbox *thiz;

	thiz = eon_theme_widget_data_get(t);
	return thiz;
}

static void _eon_theme_control_checkbox_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Control_Checkbox *thiz;

	thiz = _eon_theme_control_checkbox_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
EAPI void eon_theme_control_checkbox_size_get(Eon_Theme_Widget *t, Eon_Size *size)
{
	Eon_Theme_Control_Checkbox *thiz;

	thiz = _eon_theme_control_checkbox_get(t);
	if (thiz->descriptor.size_get)
		thiz->descriptor.size_get(t, size);
	else
	{
		size->width = 0;
		size->height = 0;
	}	
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_theme_control_checkbox_new(Eon_Theme_Control_Checkbox_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Control_Checkbox *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;

	thiz = calloc(1, sizeof(Eon_Theme_Control_Checkbox));
	thiz->descriptor.size_get = descriptor->size_get;
	thiz->descriptor.free = _eon_theme_control_checkbox_free;
	thiz->data = data;
	/* widget descriptor */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = NULL;
	pdescriptor.height_set = NULL;
	pdescriptor.free = _eon_theme_control_checkbox_free;

	return eon_theme_widget_new(&pdescriptor, thiz);
}

EAPI void * eon_theme_control_checkbox_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Control_Checkbox *thiz;

	thiz = _eon_theme_control_checkbox_get(t);
	return thiz->data;
}
