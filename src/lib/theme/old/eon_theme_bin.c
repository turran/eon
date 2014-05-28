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
#include "eon_theme_container.h"
#include "eon_theme_bin.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Bin_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Bin_Child_Set child_set;
	Eon_Theme_Bin_Child_Get child_get;
} Eon_Theme_Bin_Descriptor_Internal;

typedef struct _Eon_Theme_Bin
{
	Eon_Theme_Bin_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Bin;

static inline Eon_Theme_Bin * _eon_theme_bin_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Bin *thiz;

	thiz = eon_theme_container_data_get(t);
	return thiz;
}

static void _eon_theme_bin_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Bin *thiz;

	thiz = _eon_theme_bin_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}

static void _eon_theme_bin_child_add(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Bin *thiz;
	Enesim_Renderer *child = NULL;

	thiz = _eon_theme_bin_get(t);
	if (thiz->descriptor.child_get)
		thiz->descriptor.child_get(t, &child);
	if (!child && thiz->descriptor.child_set)
		thiz->descriptor.child_set(t, r);
}

static void _eon_theme_bin_child_remove(Eon_Theme_Widget *t, Enesim_Renderer *r)
{
	Eon_Theme_Bin *thiz;
	Enesim_Renderer *child = NULL;

	thiz = _eon_theme_bin_get(t);
	if (!child && thiz->descriptor.child_set)
		thiz->descriptor.child_set(t, NULL);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Widget * eon_theme_bin_new(Eon_Theme_Bin_Descriptor *descriptor, void *data)
{
	Eon_Theme_Bin *thiz;
	Eon_Theme_Container_Descriptor pdescriptor;

	thiz = calloc(1, sizeof(Eon_Theme_Bin));
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.child_get = descriptor->child_get;
	thiz->descriptor.child_set = descriptor->child_set;
	thiz->data = data;

	/* widget descriptor */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_bin_free;
	/* container descriptor */
	pdescriptor.child_add = _eon_theme_bin_child_add;
	pdescriptor.child_remove = _eon_theme_bin_child_remove;

	return eon_theme_container_new(&pdescriptor, thiz);
}

EAPI void * eon_theme_bin_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Bin *thiz;

	thiz = _eon_theme_bin_get(t);
	return thiz->data;
}

