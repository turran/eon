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
#include "eon_private_main.h"
#include "eon_private_layout.h"
#include "eon_private_layout_stack.h"

/* FIXME We handle only homogeneus space for now */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Layout_Stack_Geometry_Set_Data
{
	Eon_Geometry g;
	Eon_Orientation orientation;
	Eon_Layout_Stack_Descriptor *d;
} Eon_Layout_Stack_Geometry_Set_Data;

typedef struct _Eon_Layout_Stack_Hints_Get_Data
{
	Eon_Size *min;
	Eon_Size *max;
	Eon_Size *preferred;
	Eon_Orientation orientation;
	Eon_Layout_Stack_Descriptor *d;
	int count;
} Eon_Layout_Stack_Hints_Get_Data;

static void _geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Geometry_Set_Data *data = user_data;
	printf("stack: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	if (data->orientation == EON_ORIENTATION_HORIZONTAL)
		data->g.x += data->g.width;
	else
		data->g.y += data->g.height;
}

static void _hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Hints_Get_Data *data = user_data;
	Eon_Size cmin, cmax, cpreferred;

	data->d->child_hints_get(ref, child, &cmin, &cmax, &cpreferred);
	/* the min size is the max of every child min size */
	data->min->width = MAX(data->min->width, cmin.width);
	data->min->height = MAX(data->min->height, cmin.height);
	/* the max size is the min of every child max size */
	data->max->width = MIN(data->max->width, cmax.width);
	data->max->height = MIN(data->max->height, cmax.height);
	/* the preferred size is the max of every child preferred size */
	data->preferred->width = MAX(data->preferred->width, cpreferred.width);
	data->preferred->height = MAX(data->preferred->height, cpreferred.height);

	data->count++;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's layout interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_layout_stack_geometry_set(void *descriptor, void *ref,
		Eon_Geometry *g)
{
	Eon_Layout_Stack_Geometry_Set_Data data;
	Eon_Layout_Stack_Descriptor *d = descriptor;
	int count;

	data.d = d;
	data.orientation = d->orientation_get(ref);
	count = d->child_count_get(ref);

	data.g.x = g->x;
	data.g.y = g->y;
	if (data.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		data.g.width = g->width / count;
		data.g.height = g->height;
	}
	else
	{
		data.g.width = g->width;
		data.g.height = g->height / count;
	}
	d->child_foreach(ref, _geometry_set_cb, &data);
}

static void _eon_layout_stack_hints_get(void *descriptor, void *ref,
		Eon_Size *min, Eon_Size *max, Eon_Size *preferred)
{
	Eon_Layout_Stack_Hints_Get_Data data;
	Eon_Layout_Stack_Descriptor *d = descriptor;

	data.d = d;
	data.count = 0;
	data.min = min;
	data.max = max;
	data.preferred = preferred;
	data.orientation = d->orientation_get(ref);
	d->child_foreach(ref, _hints_get_cb, &data);

	if (data.orientation == EON_ORIENTATION_HORIZONTAL) {
		min->width *= data.count;
		if (max->width < DBL_MAX)
			max->width *= data.count;
		preferred->height *= data.count;
	} else {
		min->height *= data.count;
		if (max->height < DBL_MAX)
			max->height *= data.count;
		preferred->height *= data.count;
	}
	d->child_count_set(ref, data.count);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Layout eon_layout_stack = {
	/* .geometry_set 	= */ _eon_layout_stack_geometry_set,
	/* .hints_get 		= */ _eon_layout_stack_hints_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

