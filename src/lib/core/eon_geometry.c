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

#include "eon_main.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_GEOMETRY_X;
static Ender_Property *EON_GEOMETRY_Y;
static Ender_Property *EON_GEOMETRY_WIDTH;
static Ender_Property *EON_GEOMETRY_HEIGHT;

#define _eon_geometry_new NULL
#define _eon_geometry_delete NULL
#define _eon_geometry_x_set NULL
#define _eon_geometry_x_get NULL
#define _eon_geometry_y_set NULL
#define _eon_geometry_y_get NULL
#define _eon_geometry_width_set NULL
#define _eon_geometry_width_get NULL
#define _eon_geometry_height_set NULL
#define _eon_geometry_height_get NULL
#include "eon_generated_geometry.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

