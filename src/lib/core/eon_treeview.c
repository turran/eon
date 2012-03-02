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
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Treeview
{
} Eon_Treeview;

static inline Eon_Treeview * _eon_treeview_get(Enesim_Renderer *r)
{
	Eon_Treeview *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static double _eon_treeview_min_width_get(Ender_Element *e)
{
	Eon_Treeview *thiz;

	r = ender_element_object_get(e);
	thiz = _eon_treeview_get(r);
}

static double _eon_treeview_max_width_get(Ender_Element *e)
{
	Eon_Treeview *thiz;

	r = ender_element_object_get(e);
	thiz = _eon_treeview_get(r);
}

static double _eon_treeview_min_height_get(Ender_Element *e)
{
	Eon_Treeview *thiz;

	r = ender_element_object_get(e);
	thiz = _eon_treeview_get(r);
}

static double _eon_treeview_max_height_get(Ender_Element *e)
{
	Eon_Treeview *thiz;

	r = ender_element_object_get(e);
	thiz = _eon_treeview_get(r);
}

static double _eon_treeview_preferred_width_get(Ender_Element *e)
{
	Eon_Treeview *thiz;

	r = ender_element_object_get(e);
	thiz = _eon_treeview_get(r);
}

static double _eon_treeview_preferred_height_get(Ender_Element *e)
{
	Eon_Treeview *thiz;

	r = ender_element_object_get(e);
	thiz = _eon_treeview_get(r);
}

static Eon_Widget_Descriptor _eon_treeview_widget_descriptor = {
	.name = "treeview",
	.min_width_get = _eon_treeview_min_width_get,
	.max_width_get = _eon_treeview_max_width_get,
	.min_height_get = _eon_treeview_min_height_get,
	.max_height_get = _eon_treeview_max_height_get,
	.preferred_width_get = _eon_treeview_preferred_width_get,
	.preferred_height_get = _eon_treeview_preferred_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_treeview_new(void)
{
	Eon_Treeview *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Treeview));
	if (!thiz) return NULL;

	r = eon_widget_new(&_eon_treeview_widget_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_treeview.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

