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
#include "Eon.h"

#include "eon_theme_widget.h"
#include "eon_theme_bin.h"
#include "eon_theme_radio.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Radio_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Radio_Control_Set control_set;
} Eon_Theme_Radio_Descriptor_Internal;

typedef struct _Eon_Theme_Radio
{
	Eon_Theme_Radio_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Radio;

static inline Eon_Theme_Radio * _eon_theme_radio_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Radio *thiz;

	thiz = eon_theme_bin_data_get(t);
	return thiz;
}

static void _eon_theme_radio_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Radio *thiz;

	thiz = _eon_theme_radio_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_theme_radio_new(Eon_Theme_Radio_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Radio *thiz;
	Eon_Theme_Bin_Descriptor pdescriptor;
	Eon_Theme_Widget *t;

	thiz = calloc(1, sizeof(Eon_Theme_Radio));
	thiz->descriptor.control_set = descriptor->control_set;
	thiz->descriptor.free = descriptor->free;
	thiz->data = data;

	/* widget descriptor */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_radio_free;
	pdescriptor.child_get = descriptor->child_get;
	pdescriptor.child_set = descriptor->child_set;

	t = eon_theme_bin_new(&pdescriptor, thiz);
	return t;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_radio_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Radio *thiz;

	thiz = _eon_theme_radio_get(t);
	return thiz->data;
}

EAPI void eon_theme_radio_control_set(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Radio *thiz;

	thiz = _eon_theme_radio_get(t);
	if (thiz->descriptor.control_set)
		thiz->descriptor.control_set(t, r);
}

