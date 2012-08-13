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
#include "eon_private_layout_frame.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Layout_Frame_Geometry_Set_Data
{
	Eon_Layout_Frame_Descriptor *d;
	Eon_Geometry *g;
} Eon_Layout_Frame_Geometry_Set_Data;

typedef struct _Eon_Layout_Frame_Hints_Get_Data
{
	Eon_Layout_Frame_Descriptor *d;
	Eon_Hints *hints;
} Eon_Layout_Frame_Hints_Get_Data;

/* TODO when setting the geometry we need to take into
 * account the previously generated min/max/preferred
 * per child
 */
static void _geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Frame_Geometry_Set_Data *data = user_data;
	Eon_Margin cmargin;
	Eon_Geometry cg;

	data->d->child_padding_get(ref, child, &cmargin);
	cg = *data->g;
	cg.x += cmargin.left;
	cg.width -= cmargin.right + cmargin.left;
	cg.y += cmargin.top;
	cg.height -= cmargin.top + cmargin.bottom;
	printf("frame: setting geometry %g %g %g %g\n", cg.x, cg.y, cg.width, cg.height);
	data->d->child_geometry_set(ref, child, &cg);
}

static void _hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Frame_Hints_Get_Data *data = user_data;
	Eon_Hints hints;
	Eon_Margin cmargin;
	double h;
	double v;

	eon_hints_initialize(&hints);
	data->d->child_hints_get(ref, child, &hints);
	data->d->child_padding_get(ref, child, &cmargin);

	//printf("child max %g %g %g %g\n", cmax.width, cmax.height, cmin.width, cmin.height);

	h = cmargin.left + cmargin.right;
	v = cmargin.top + cmargin.bottom;

	if (hints.min.width < DBL_MAX)
		hints.min.width += h;
	if (hints.min.height < DBL_MAX)
		hints.min.height += v;
	if (hints.max.width < DBL_MAX)
		hints.max.width += h;
	if (hints.max.height < DBL_MAX)
		hints.max.height += v;
	if (hints.max.width < DBL_MAX && hints.max.width > 0)
		hints.preferred.width += h;
	if (hints.max.height < DBL_MAX && hints.max.height > 0)
		hints.preferred.height += v;

	/* the min size is the max of every child min size */
	data->hints->min.width = MAX(data->hints->min.width, hints.min.width);
	data->hints->min.height = MAX(data->hints->min.height, hints.min.height);
	/* the max size is the min of every child max size */
	data->hints->max.width = MIN(data->hints->max.width, hints.max.width);
	data->hints->max.height = MIN(data->hints->max.height, hints.max.height);
	/* the preferred size is the max of every child preferred size */
	data->hints->preferred.width = MAX(data->hints->preferred.width, hints.preferred.width);
	data->hints->preferred.height = MAX(data->hints->preferred.height, hints.preferred.height);

	printf("frame: min %g %g max %g %g preferred %g %g\n",
		data->hints->min.width, data->hints->min.height,
		data->hints->max.width, data->hints->max.height,
		data->hints->preferred.width, data->hints->preferred.height);
}
/*----------------------------------------------------------------------------*
 *                        The Eon's layout interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_layout_frame_geometry_set(void *descriptor, void *ref,
		Eon_Geometry *g)
{
	Eon_Layout_Frame_Geometry_Set_Data data;
	Eon_Layout_Frame_Descriptor *d = descriptor;

	data.d = d;
	data.g = g;
	d->child_foreach(ref, _geometry_set_cb, &data);
}

static void _eon_layout_frame_hints_get(void *descriptor, void *ref,
		Eon_Hints *hints)
{
	Eon_Layout_Frame_Hints_Get_Data data;
	Eon_Layout_Frame_Descriptor *d = descriptor;

	data.d = d;
	data.hints = hints;
	d->child_foreach(ref, _hints_get_cb, &data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Layout eon_layout_frame = {
	/* .geometry_set 	= */ _eon_layout_frame_geometry_set,
	/* .hints_get 		= */ _eon_layout_frame_hints_get,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
