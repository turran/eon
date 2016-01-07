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

/*
 * A layout must be a generic interface to create layout algorithms.
 * A possible interface would be:
 * eon_layout_size_hints_get(Eon_Layout *thiz, Eon_Renderable *r);
 * eon_layout_geometry_set(Eon_Layout, Eon_Renderable *r, Eina_Rectangle *area);
 * with a possible interface like:
 *
 * typedef Egueb_Dom_Node * (*Eon_Layout_Array_Child_Next)(Egueb_Dom_Node *current,
 *   Eon_Box *padding);
 * typedef void (*Eon_Layout_Array_Child_Process)(Egueb_Dom_Node *child,
 *   Eina_Rectangle *area);
 *
 * typedef void (*Eon_Layout_Array_Child_Process)(void *child,
 *   Eina_Rectangle *area);
 * typedef void * (*Eon_Layout_Array_Child_Next)(void *current,
 *   Eon_Box *padding, Eon_Layout_Array_Child_Process *process);
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

