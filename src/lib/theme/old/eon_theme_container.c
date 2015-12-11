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
#include "eon_theme_container.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Container_Descriptor_Internal
{
	Eon_Theme_Widget_Free free;
	Eon_Theme_Container_Child_Add child_add;
	Eon_Theme_Container_Child_Remove child_remove;
} Eon_Theme_Container_Descriptor_Internal;

typedef struct _Eon_Theme_Container
{
	EINA_MAGIC;
	/* properties */
	/* private */
	Eon_Theme_Container_Descriptor_Internal descriptor;
	void *data;
} Eon_Theme_Container;

static inline Eon_Theme_Container * _eon_theme_container_get(Eon_Theme_Widget *r)
{
	Eon_Theme_Container *thiz;

	thiz = eon_theme_widget_data_get(r);

	return thiz;
}

static void _eon_theme_container_free(Eon_Theme_Widget *r)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	if (thiz->descriptor.free) thiz->descriptor.free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Theme_Widget * eon_theme_container_new(Eon_Theme_Container_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Container *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Eon_Theme_Widget *r;

	thiz = calloc(1, sizeof(Eon_Theme_Container));
	thiz->data = data;
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.child_add = descriptor->child_add;
	thiz->descriptor.child_remove = descriptor->child_remove;

	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_container_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_container_data_get(Eon_Theme_Widget *r)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_layout(Eon_Theme_Widget *r)
{
	return EINA_TRUE;
}

EAPI void eon_theme_container_child_add(Eon_Theme_Widget *r, Enesim_Renderer *child)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	if (thiz->descriptor.child_add)
		thiz->descriptor.child_add(r, child);
}

EAPI void eon_theme_container_child_remove(Eon_Theme_Widget *r, Enesim_Renderer *child)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	if (thiz->descriptor.child_remove)
		thiz->descriptor.child_remove(r, child);
}
