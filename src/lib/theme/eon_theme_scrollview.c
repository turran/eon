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
#include "eon_theme_scrollview.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Scrollview_Descriptor_Internal
{
	Eon_Theme_Scrollview_HBar_Set hbar_set;
	Eon_Theme_Scrollview_VBar_Set vbar_set;
	Eon_Theme_Widget_Free free;
} Eon_Theme_Scrollview_Descriptor_Internal;

typedef struct _Eon_Theme_Scrollview
{
	Eon_Theme_Scrollview_Descriptor_Internal descriptor;
	/* private */
	void *data;
} Eon_Theme_Scrollview;

static inline Eon_Theme_Scrollview * _eon_theme_scrollview_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Scrollview *thiz;

	thiz = eon_theme_bin_data_get(t);
	return thiz;
}

static void _eon_theme_scrollview_free(Eon_Theme_Widget *t)
{
	Eon_Theme_Scrollview *thiz;

	thiz = _eon_theme_scrollview_get(t);
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
EAPI Eon_Theme_Widget * eon_theme_scrollview_new(Eon_Theme_Scrollview_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Widget *t;
	Eon_Theme_Scrollview *thiz;
	Eon_Theme_Bin_Descriptor pdescriptor = { 0 };

	thiz = calloc(1, sizeof(Eon_Theme_Scrollview));
	thiz->data = data;
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.hbar_set = descriptor->hbar_set;
	thiz->descriptor.vbar_set = descriptor->vbar_set;

	/* bin descriptor */
	pdescriptor.child_set = descriptor->child_set;
	pdescriptor.child_get = descriptor->child_get;
	/* widget descriptor */
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.x_set = descriptor->x_set;
	pdescriptor.y_set = descriptor->y_set;
	pdescriptor.width_set = descriptor->width_set;
	pdescriptor.height_set = descriptor->height_set;
	pdescriptor.free = _eon_theme_scrollview_free;

	t = eon_theme_bin_new(&pdescriptor, thiz);
	if (!t) goto base_err;

	return t;

base_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_scrollview(Eon_Theme_Widget *t)
{
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_scrollview_data_get(Eon_Theme_Widget *t)
{
	Eon_Theme_Scrollview *thiz;

	thiz = _eon_theme_scrollview_get(t);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_scrollview_hbar_set(Eon_Theme_Widget *t, Enesim_Renderer *hbar)
{
	Eon_Theme_Scrollview *thiz;

	thiz = _eon_theme_scrollview_get(t);
	if (thiz->descriptor.hbar_set)
		thiz->descriptor.hbar_set(t, hbar);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_scrollview_vbar_set(Eon_Theme_Widget *t, Enesim_Renderer *vbar)
{
	Eon_Theme_Scrollview *thiz;

	thiz = _eon_theme_scrollview_get(t);
	if (thiz->descriptor.vbar_set)
		thiz->descriptor.vbar_set(t, vbar);
}
