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
#include "eon_private_main.h"
#include "eon_private_layout.h"
#include "eon_private_layout_list.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Layout_List_Geometry_Set_Data
{
	Eon_Geometry g;
	Eon_Orientation orientation;
	Eon_Layout_List_Descriptor *d;
	int weights;
	double empty;
} Eon_Layout_List_Geometry_Set_Data;

typedef struct _Eon_Layout_List_Hints_Get_Data
{
	Eon_Hints *hints;
	Eon_Orientation orientation;
	Eon_Layout_List_Descriptor *d;
	int count;
} Eon_Layout_List_Hints_Get_Data;

static void _list_horizontal_geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_List_Geometry_Set_Data *data = user_data;
	printf("list: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.x += data->g.width;
}

static void _list_vertical_geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_List_Geometry_Set_Data *data = user_data;
	printf("list: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.y += data->g.height;
}

static void _list_hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_List_Hints_Get_Data *data = user_data;
	Eon_Hints hints;

	eon_hints_initialize(&hints);
	data->d->child_hints_get(ref, child, &hints);

	/* the min size is the max of every child min size */
	data->hints->min.width = MAX(data->hints->min.width, hints.min.width);
	data->hints->min.height = MAX(data->hints->min.height, hints.min.height);
	/* the max size can be anything */
	/* the preferred size is the max of every child preferred size */
	data->hints->preferred.width = MAX(data->hints->preferred.width, hints.preferred.width);
	data->hints->preferred.height = MAX(data->hints->preferred.height, hints.preferred.height);

	data->count++;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's layout interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_layout_list_geometry_set(void *descriptor, void *ref,
		Eon_Geometry *g)
{
	Eon_Layout_List_Geometry_Set_Data data;
	Eon_Layout_List_Descriptor *d = descriptor;
	int count;

	data.d = d;
	data.orientation = d->orientation_get(ref);
	data.weights = 0;

	count = d->child_count_get(ref);

	data.g.x = g->x;
	data.g.y = g->y;
	if (data.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		data.g.width = g->width / count;
		data.g.height = g->height;
		d->child_foreach(ref, _list_horizontal_geometry_set_cb, &data);
	}
	else
	{
		data.g.width = g->width;
		data.g.height = g->height / count;
		d->child_foreach(ref, _list_vertical_geometry_set_cb, &data);
	}
}

static void _eon_layout_list_hints_get(void *descriptor, void *ref,
		Eon_Hints *hints)
{
	Eon_Layout_List_Hints_Get_Data data;
	Eon_Layout_List_Descriptor *d = descriptor;

	data.d = d;
	data.count = 0;
	data.hints = hints;
	data.orientation = d->orientation_get(ref);

	d->child_foreach(ref, _list_hints_get_cb, &data);

	if (data.orientation == EON_ORIENTATION_HORIZONTAL) {
		hints->min.width *= data.count;
		hints->preferred.height *= data.count;
	} else {
		hints->min.height *= data.count;
		hints->preferred.height *= data.count;
	}
	d->child_count_set(ref, data.count);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Layout eon_layout_list = {
	/* .geometry_set 	= */ _eon_layout_list_geometry_set,
	/* .hints_get 		= */ _eon_layout_list_hints_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


