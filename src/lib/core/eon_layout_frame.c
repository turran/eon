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

} Eon_Layout_Frame_Geometry_Set_Data;

typedef struct _Eon_Layout_Frame_Hints_Get_Data
{

} Eon_Layout_Frame_Hints_Get_Data;

static void _geometry_set_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Frame_Geometry_Set_Data *data = user_data;
}

static void _hints_get_cb(void *ref, void *child, void *user_data)
{
	Eon_Layout_Frame_Hints_Get_Data *data = user_data;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's layout interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_layout_frame_geometry_set(void *descriptor, void *ref,
		Enesim_Rectangle *g)
{
	Eon_Layout_Frame_Geometry_Set_Data data;
	Eon_Layout_Frame_Descriptor *d = descriptor;

	d->child_foreach(ref, _geometry_set_cb, &data);
}

static void _eon_layout_frame_hints_get(void *descriptor, void *ref,
		Eon_Size *min, Eon_Size *max, Eon_Size *preffered)
{
	Eon_Layout_Frame_Hints_Get_Data data;
	Eon_Layout_Frame_Descriptor *d = descriptor;

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
