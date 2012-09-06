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
#include "eon_private_layout_grid.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Layout_Grid_Geometry_Set_Data
{
	Eon_Geometry g;
	Eon_Orientation orientation;
	Eon_Layout_Grid_Descriptor *d;
	double x;
	int current_col;
	int cols;
} Eon_Layout_Grid_Geometry_Set_Data;

typedef struct _Eon_Layout_Grid_Hints_Get_Data
{
	Eon_Hints *hints;
	Eon_Orientation orientation;
	Eon_Layout_Grid_Descriptor *d;
	int count;
} Eon_Layout_Grid_Hints_Get_Data;

static void _geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Grid_Geometry_Set_Data *data = user_data;

	printf("grid: setting geometry %g %g %g %g\n", data->g.x, data->g.y, data->g.width, data->g.height);
	data->d->child_geometry_set(ref, child, &data->g);
	data->g.x += data->g.width;
	data->current_col++;
	if (data->current_col == data->cols)
	{
		data->g.x = data->x;
		data->g.y += data->g.height;
		data->current_col = 0;
	}
}

static void _hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Grid_Hints_Get_Data *data = user_data;
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
static void _eon_layout_grid_geometry_set(void *descriptor, void *ref,
		Eon_Geometry *g)
{
	Eon_Layout_Grid_Geometry_Set_Data data;
	Eon_Layout_Grid_Descriptor *d = descriptor;
	int num_columns;
	int rows;
	int cols;

	data.d = d;
	data.g.x = g->x;
	data.g.y = g->y;
	data.x = g->x;

	num_columns = d->num_columns_get(ref);
	if (num_columns <= 0)
	{
		/* TODO from the size, calculate the real number of columns and rows */
		//d->child_foreach(ref, _geometry_set_cb, &data);
	}
	else
	{

		d->rows_columns_get(ref, &rows, &cols);

		data.g.width = g->width / cols;
		data.g.height = g->height / rows;

		data.cols = cols;
		data.current_col = 0;
		d->child_foreach(ref, _geometry_set_cb, &data);
	}
}

static void _eon_layout_grid_hints_get(void *descriptor, void *ref,
		Eon_Hints *hints)
{
	Eon_Layout_Grid_Hints_Get_Data data;
	Eon_Layout_Grid_Descriptor *d = descriptor;
	int num_columns;
	int rows = 0;
	int cols = 0;

	data.d = d;
	data.count = 0;
	data.hints = hints;
	d->child_foreach(ref, _hints_get_cb, &data);

	num_columns = d->num_columns_get(ref);
	printf("grid: num columns = %d\n", num_columns);

	/* FIXME handle the case where the num columns are automatic */
	if (num_columns <= 0)
	{
		hints->type = EON_HINTS_TYPE_DYNAMIC;
		hints->max.width = hints->min.width * data.count;
		hints->max.height = hints->min.height * data.count;
	}
	else
	{
		/* in case the number of columns requested is greater
		 * than the number of elements, just use the number
		 * of elements
		 */
		cols = num_columns;
		if (cols > data.count)
			cols = data.count;

		rows = data.count / cols;

		printf("grid: rows %d cols %d\n", rows, cols);
		hints->type = EON_HINTS_TYPE_FIXED;
		hints->min.width *= cols;
		hints->min.height *= rows;
		hints->preferred.height *= cols;
		hints->preferred.height *= rows;

	}
	d->rows_columns_set(ref, rows, cols);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Layout eon_layout_grid = {
	/* .geometry_set 	= */ _eon_layout_grid_geometry_set,
	/* .hints_get 		= */ _eon_layout_grid_hints_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
