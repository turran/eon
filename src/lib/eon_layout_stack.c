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
#include "eon_private.h"
#include "eon_main.h"

#include "eon_renderable.h"
#include "eon_renderable_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _eon_layout_stack_get_weight(Egueb_Dom_Node *n)
{
	Egueb_Dom_Node *weight_attr;
	int weight = 0;

	weight_attr = egueb_dom_element_attribute_node_get(n, EON_ATTR_WEIGHT);
	if (weight_attr)
	{
		egueb_dom_attr_final_get(weight_attr, &weight);
		egueb_dom_node_unref(weight_attr);
	}
	return weight;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_layout_stack_vertical_size_hints_get(Egueb_Dom_Node *r,
		Eon_Renderable_Size *size, int *min_height, int *weights)
{
	Egueb_Dom_Node *child;
	int ret = 0;

	/* iterate over the children */
	*weights = 0;
	child = egueb_dom_element_child_first_get(r);
	while (child)
	{
		Egueb_Dom_Node *tmp;
		Eon_Renderable_Size child_size;
		int weight = 0;
		int child_size_flags;

		if (!eon_is_renderable(child))
			goto next;

		/* get the weights */
		*weights += _eon_layout_stack_get_weight(child);

		/* the algo */
		child_size_flags = eon_renderable_size_hints_get(child, &child_size);
		if (child_size_flags & EON_RENDERABLE_HINT_MIN_MAX)
		{
			/* the height is the sum of every child height */
			size->min_height += child_size.min_height;
			if (child_size.max_height < 0)
				size->max_height = -1;
			else if (size->max_height >= 0)
				size->max_height += child_size.max_height;
				
			/* the width works as a frame layout */
			size->min_width = MAX(size->min_width, child_size.min_width);
			if (child_size.max_width >= 0)
				size->max_width = MIN(size->max_width, child_size.max_width);
		}

		if (child_size_flags & EON_RENDERABLE_HINT_PREFERRED)
		{
			if (child_size.pref_height >= 0)
			{
				if (size->pref_height < 0)
					size->pref_height = child_size.pref_height;
				else
					size->pref_height += child_size.pref_height;
			}
			size->pref_width = MAX(size->pref_width, child_size.pref_width);
		}

		ret |= child_size_flags;

next:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
	/* make sure the pref height is in the min range, it might be possible
	 * that some children does not provide a preferred height and some children do
	 */
	if (size->pref_height >= 0)
		size->pref_height = MAX(size->pref_height, size->min_height);

	*min_height = size->min_height;

	return ret;
}

void eon_layout_stack_vertical_size_geometry_set(Egueb_Dom_Node *r,
		Eina_Rectangle *g, int min_height, int weights)
{
	Egueb_Dom_Node *child;
	int extra = 0;
	int empty = 0;
	int ret = 0;

	/* get the difference between the min size and the geometry
	 * then, use the weight to calculate how much space should
	 * we allocate for every child min size
	 */
	empty = g->h - min_height;
	if (weights)
		extra = empty/weights;
	/* iterate over the children */
	child = egueb_dom_element_child_first_get(r);
	while (child)
	{
		Egueb_Dom_Node *tmp;
		Egueb_Dom_Node *weight_attr;
		Eon_Renderable_Size child_size;
		Eina_Rectangle child_g = *g;
		Eina_Rectangle child_final_g;
		int child_size_flags;
		int weight = 0;

		if (!eon_is_renderable(child))
			goto next_size;

		child_size_flags = eon_renderable_size_hints_get(child, &child_size);
		weight_attr = egueb_dom_element_attribute_node_get(child, EON_ATTR_WEIGHT);
		if (weight_attr)
		{
			egueb_dom_attr_final_get(weight_attr, &weight);
			egueb_dom_node_unref(weight_attr);
		}

		/* we start from its minimum size and give space based on the weight */
		child_g.h = child_size.min_height;
		child_g.h += extra * weight;

		eon_renderable_geometry_solve(child, &child_g, &child_final_g);
		eon_renderable_geometry_set(child, &child_final_g);

		/* Finally process */
		egueb_dom_element_process(child);

		g->y += child_g.h;
next_size:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
}

int eon_layout_stack_horizontal_size_hints_get(Egueb_Dom_Node *r,
		Eon_Renderable_Size *size, int *min_width, int *weights)
{
	Egueb_Dom_Node *child;
	int ret = 0;

	/* iterate over the children */
	*weights = 0;
	child = egueb_dom_element_child_first_get(r);
	while (child)
	{
		Egueb_Dom_Node *tmp;
		Eon_Renderable_Size child_size;
		int weight = 0;
		int child_size_flags;

		if (!eon_is_renderable(child))
			goto next;

		/* get the weights */
		*weights += _eon_layout_stack_get_weight(child);

		/* the algo */
		child_size_flags = eon_renderable_size_hints_get(child, &child_size);
		if (child_size_flags & EON_RENDERABLE_HINT_MIN_MAX)
		{
			/* the width is the sum of every child width */
			size->min_width += child_size.min_width;
			if (child_size.max_width < 0)
				size->max_width = -1;
			else if (size->max_width >= 0)
				size->max_width += child_size.max_width;
				
			/* the height works as a frame layout */
			size->min_height = MAX(size->min_height, child_size.min_height);
			if (child_size.max_height >= 0)
				size->max_height = MIN(size->max_height, child_size.max_height);
		}

		if (child_size_flags & EON_RENDERABLE_HINT_PREFERRED)
		{
			if (child_size.pref_width >= 0)
			{
				if (size->pref_width < 0)
					size->pref_width = child_size.pref_width;
				else
					size->pref_width += child_size.pref_width;
			}
			size->pref_height = MAX(size->pref_height, child_size.pref_height);
		}

		ret |= child_size_flags;

next:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
	/* make sure the pref width is in the min range, it might be possible
	 * that some children does not provide a preferred width and some children do
	 */
	if (size->pref_width >= 0)
		size->pref_width = MAX(size->pref_width, size->min_width);

	*min_width = size->min_width;

	return ret;
}

void eon_layout_stack_horizontal_size_geometry_set(Egueb_Dom_Node *r,
		Eina_Rectangle *g, int min_width, int weights)
{
	Egueb_Dom_Node *child;
	int extra = 0;
	int empty = 0;
	int ret = 0;

	/* get the difference between the min size and the geometry
	 * then, use the weight to calculate how much space should
	 * we allocate for every child min size
	 */
	empty = g->w - min_width;
	if (weights)
		extra = empty/weights;

	/* iterate over the children */
	child = egueb_dom_element_child_first_get(r);
	while (child)
	{
		Egueb_Dom_Node *tmp;
		Egueb_Dom_Node *weight_attr;
		Eon_Renderable_Size child_size;
		Eina_Rectangle child_g = *g;
		Eina_Rectangle child_final_g;
		int child_size_flags;
		int weight = 0;

		if (!eon_is_renderable(child))
			goto next_size;

		child_size_flags = eon_renderable_size_hints_get(child, &child_size);
		weight_attr = egueb_dom_element_attribute_node_get(child, EON_ATTR_WEIGHT);
		if (weight_attr)
		{
			egueb_dom_attr_final_get(weight_attr, &weight);
			egueb_dom_node_unref(weight_attr);
		}

		/* we start from its minimum size and give space based on the weight */
		child_g.w = child_size.min_width;
		child_g.w += extra * weight;

		eon_renderable_geometry_solve(child, &child_g, &child_final_g);
		eon_renderable_geometry_set(child, &child_final_g);
		/* Finally process */
		egueb_dom_element_process(child);

		g->x += child_g.w;
next_size:
		tmp = egueb_dom_element_sibling_next_get(child);
		egueb_dom_node_unref(child);
		child = tmp;
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

