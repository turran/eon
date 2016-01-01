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
#include "eon_private.h"
#include "eon_main.h"

#include "eon_renderable.h"
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_layout_frame_size_hints_get(Egueb_Dom_Node *r,
		Eon_Renderable_Size *size)
{
	Egueb_Dom_Node *child;
	int ret = 0;

	/* iterate over the children */
	child = egueb_dom_element_child_first_get(r);
	while (child)
	{
		Egueb_Dom_Node *tmp;
		Eon_Renderable_Size child_size;
		int child_size_flags;

		if (!eon_is_renderable(child))
			goto next;

		/* the algo */
		child_size_flags = eon_renderable_size_hints_get(child, &child_size);
		if (child_size_flags & EON_RENDERABLE_HINT_MIN_MAX)
		{
			/* the min size is the max of every child min size */
			size->min_width = MAX(size->min_width, child_size.min_width);
			size->min_height = MAX(size->min_height, child_size.min_height);
			/* the max size is the min of every child max size */
			if (child_size.max_width >= 0)
				size->max_width = MIN(size->max_width, child_size.max_width);
			if (child_size.max_height >= 0)
				size->max_height = MIN(size->max_height, child_size.max_height);
		}

		if (child_size_flags & EON_RENDERABLE_HINT_PREFERRED)
		{
			/* the preferred size is the max of every child preferred size */
			size->pref_width = MAX(size->pref_width, child_size.pref_width);
			size->pref_height = MAX(size->pref_height, child_size.pref_height);
		}

		ret |= child_size_flags;
next:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
	return ret;
}

void eon_layout_frame_size_geometry_set(Egueb_Dom_Node *r, Eina_Rectangle *g)
{
	Egueb_Dom_Node *child;

	/* iterate over the children and set the size */
	child = egueb_dom_element_child_first_get(r);
	while (child)
	{
		Egueb_Dom_Node *tmp;

		if (!eon_is_renderable(child))
			goto next;

		eon_renderable_geometry_set(child, g);
		/* Finally process */
		egueb_dom_element_process(child);
next:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
