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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Layout_Stack_Geometry_Set_Data
{
	Eon_Geometry g;
	Eon_Orientation orientation;
	Eon_Layout_Stack_Descriptor *d;
	int weights;
	double empty;
} Eon_Layout_Stack_Geometry_Set_Data;

typedef struct _Eon_Layout_Stack_Hints_Get_Data
{
	Eon_Hints *hints;
	Eon_Orientation orientation;
	Eon_Layout_Stack_Descriptor *d;
	int count;
} Eon_Layout_Stack_Hints_Get_Data;

static void _weight_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Geometry_Set_Data *data = user_data;
	data->weights += data->d->child_weight_get(ref, child);
}

static void _no_homogeneous_vertical_geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Geometry_Set_Data *data = user_data;
	Eon_Hints hints;
	int weight;

	eon_hints_initialize(&hints);
	data->d->child_hints_get(ref, child, &hints);

	data->g.height = hints.min.height;
	weight = data->d->child_weight_get(ref, child);
	if (weight != 0)
		data->g.height += (data->empty / data->weights) * weight;
	printf("stack: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.y += data->g.height;
}

static void _no_homogeneous_horizontal_geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Geometry_Set_Data *data = user_data;
	Eon_Hints hints;
	int weight;

	eon_hints_initialize(&hints);
	data->d->child_hints_get(ref, child, &hints);

	data->g.width = hints.min.width;
	weight = data->d->child_weight_get(ref, child);
	if (weight != 0)
		data->g.width += (data->empty / data->weights) * weight;
	printf("stack: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.x += data->g.width;
}

static void _homogeneous_horizontal_geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Geometry_Set_Data *data = user_data;
	printf("stack: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.x += data->g.width;
}

static void _homogeneous_vertical_geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Geometry_Set_Data *data = user_data;
	printf("stack: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.y += data->g.height;
}

static void _homogeneous_hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Hints_Get_Data *data = user_data;
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

static void _no_homogeneous_horizontal_hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Hints_Get_Data *data = user_data;
	Eon_Hints hints;
	double min = 0;

	eon_hints_initialize(&hints);
	data->d->child_hints_get(ref, child, &hints);

	/* the width is the sum of every child width */
	if (hints.min.width == DBL_MAX)
		data->hints->min.width = DBL_MAX;
	if (data->hints->min.width != DBL_MAX)
	{
		min = hints.min.width;
		data->hints->min.width += min;
	}

	if (hints.max.width == DBL_MAX)
		data->hints->max.width = DBL_MAX;
	if (data->hints->max.width != DBL_MAX)
		data->hints->max.width += hints.max.width;

	if (hints.preferred.width < 0)
		data->hints->preferred.width += min;
	if (data->hints->preferred.width != DBL_MAX)
		data->hints->preferred.width += hints.preferred.width;

	/* the height should follow the normal min/max scheme */
	data->hints->min.height = MAX(data->hints->min.height, hints.min.height);
	data->hints->max.height = MIN(data->hints->max.height, hints.max.height);
	data->hints->preferred.height = MAX(data->hints->preferred.height, hints.preferred.height);

	eon_hints_sanitize(data->hints);
	printf("stack nhh: min %g %g max %g %g preferred %g %g\n",
		data->hints->min.width, data->hints->min.height,
		data->hints->max.width, data->hints->max.height,
		data->hints->preferred.width, data->hints->preferred.height);
}

static void _no_homogeneous_vertical_hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Stack_Hints_Get_Data *data = user_data;
	Eon_Hints hints;
	double min = 0;

	eon_hints_initialize(&hints);
	data->d->child_hints_get(ref, child, &hints);

	/* the height is the sum of every child height */
	if (hints.min.height == DBL_MAX)
		data->hints->min.height = DBL_MAX;
	if (data->hints->min.height != DBL_MAX)
	{
		min = hints.min.height;
		data->hints->min.height += min;
	}

	if (hints.max.height == DBL_MAX)
		data->hints->max.height = DBL_MAX;
	if (data->hints->max.height != DBL_MAX)
		data->hints->max.height += hints.max.height;

	if (hints.preferred.height < 0)
		data->hints->preferred.height += min;
	if (data->hints->preferred.height != DBL_MAX)
		data->hints->preferred.height += hints.preferred.height;
	/* the width should follow the normal min/max scheme */
	data->hints->min.width = MAX(data->hints->min.width, hints.min.width);
	data->hints->max.width = MIN(data->hints->max.width, hints.max.width);
	data->hints->preferred.width = MAX(data->hints->preferred.width, hints.preferred.width);

	eon_hints_sanitize(data->hints);
	printf("stack nhv: min %g %g max %g %g preferred %g %g\n",
		data->hints->min.width, data->hints->min.height,
		data->hints->max.width, data->hints->max.height,
		data->hints->preferred.width, data->hints->preferred.height);
}
/*----------------------------------------------------------------------------*
 *                        The Eon's layout interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_layout_stack_geometry_set(void *descriptor, void *ref,
		Eon_Geometry *g)
{
	Eon_Layout_Stack_Geometry_Set_Data data;
	Eon_Layout_Stack_Descriptor *d = descriptor;
	Eina_Bool homogeneous;
	int count;

	homogeneous = d->is_homogeneous(ref);

	data.d = d;
	data.orientation = d->orientation_get(ref);
	data.weights = 0;
	if (homogeneous)
	{
		count = d->child_count_get(ref);

		data.g.x = g->x;
		data.g.y = g->y;
		if (data.orientation == EON_ORIENTATION_HORIZONTAL)
		{
			data.g.width = g->width / count;
			data.g.height = g->height;
			d->child_foreach(ref, _homogeneous_horizontal_geometry_set_cb, &data);
		}
		else
		{
			data.g.width = g->width;
			data.g.height = g->height / count;
			d->child_foreach(ref, _homogeneous_vertical_geometry_set_cb, &data);
		}
	}
	else
	{
		double min;

		/* get the difference between the min size and the geometry
		 * then, use the weight to calculate how much space should
		 * we allocate for every child min size
		 */
		d->min_length_get(ref, &min);
		d->child_foreach(ref, _weight_get_cb, &data);
		data.g.x = g->x;
		data.g.y = g->y;

		printf("min size is %g weight is %d\n", min, data.weights);
		/* get all the weights */
		if (data.orientation == EON_ORIENTATION_HORIZONTAL)
		{
			data.empty = g->width - min;
			printf("empty is %g\n", data.empty);
			data.g.height = g->height;
			d->child_foreach(ref, _no_homogeneous_horizontal_geometry_set_cb, &data);
		}
		else
		{
			data.empty = g->height - min;
			printf("empty is %g\n", data.empty);
			data.g.width = g->width;
			d->child_foreach(ref, _no_homogeneous_vertical_geometry_set_cb, &data);
		}
	}
}

static void _eon_layout_stack_hints_get(void *descriptor, void *ref,
		Eon_Hints *hints)
{
	Eon_Layout_Stack_Hints_Get_Data data;
	Eon_Layout_Stack_Descriptor *d = descriptor;
	Eina_Bool homogeneous;

	data.d = d;
	data.count = 0;
	data.hints = hints;
	data.orientation = d->orientation_get(ref);
	homogeneous = d->is_homogeneous(ref);
	if (homogeneous)
	{
		d->child_foreach(ref, _homogeneous_hints_get_cb, &data);

		if (data.orientation == EON_ORIENTATION_HORIZONTAL) {
			hints->min.width *= data.count;
			hints->preferred.height *= data.count;
		} else {
			hints->min.height *= data.count;
			hints->preferred.height *= data.count;
		}
		d->child_count_set(ref, data.count);
	}
	else
	{
		if (data.orientation == EON_ORIENTATION_HORIZONTAL)
		{
			d->child_foreach(ref, _no_homogeneous_horizontal_hints_get_cb, &data);
			d->min_length_set(ref, data.hints->min.width);
		}
		else
		{
			d->child_foreach(ref, _no_homogeneous_vertical_hints_get_cb, &data);
			d->min_length_set(ref, data.hints->min.height);
		}
	}
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

