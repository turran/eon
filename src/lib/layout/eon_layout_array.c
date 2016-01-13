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

#include "eon_renderable_private.h"
#include "eon_layout_private.h"
#include "eon_layout_array_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LAYOUT_ARRAY_DESCRIPTOR eon_layout_array_descriptor_get()
#define EON_LAYOUT_ARRAY(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Layout_Array, EON_LAYOUT_ARRAY_DESCRIPTOR)

typedef struct _Eon_Layout_Array
{
	Eon_Layout base;
	/* properties */
	int spacing;
	Eon_Box padding;
	Eon_Orientation orientation;
	Eina_Bool homogeneous;
	/* private */
	int count;
	/* the external interface */
	Eon_Layout_Array_Descriptor *descriptor;
	void *data;
} Eon_Layout_Array;

typedef struct _Eon_Layout_Array_Class
{
	Eon_Layout_Class base;
} Eon_Layout_Array_Class;

static int _eon_layout_array_homogeneous_size_hints_get(Eon_Layout_Array *thiz,
		Eon_Renderable_Size *size)
{
	Eon_Box chp;
	Eon_Renderable_Size chs;
	int chm;
	int ret = 0;
	int count = 0;
	void *child = NULL;

	/* iterate over the children */
	while ((child = thiz->descriptor->next(child, &chp, &chm, &chs, NULL)))
	{
		if (chm & EON_RENDERABLE_HINT_MIN_MAX)
		{
			ret |= EON_RENDERABLE_HINT_MIN_MAX;

			if (chs.min_width >= 0)
				size->min_width = MAX(chs.min_width + chp.left + chp.right, size->min_width);
			if (size->max_width >= 0)
			{
				if (chs.max_width >= 0)
					size->max_width = MAX(chs.max_width + chp.left + chp.right, size->max_width);
				else
					size->max_width = -1;
			}

			if (chs.min_height >= 0)
				size->min_height = MAX(chs.min_height + chp.top + chp.bottom, size->min_height);
			if (size->max_height >= 0)
			{
				if (chs.max_height >= 0)
					size->max_height = MAX(chs.max_height + chp.top + chp.bottom, size->max_height);
				else
					size->max_height = -1;
			}
		}

		if (chm & EON_RENDERABLE_HINT_PREFERRED)
		{
			ERR("TODO: Missing preferred calcs");
		}
		count++;
	}

	thiz->count = count;
	return ret;
}

static int _eon_layout_array_horizontal_homogeneous_size_hints_get(Eon_Layout_Array *thiz,
		Eon_Renderable_Size *size)
{
	int ret;

	ret = _eon_layout_array_homogeneous_size_hints_get(thiz, size);

	/* add the general padding and the inner spacing */
	if (ret & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (size->min_width >= 0)
		{
			size->min_width = size->min_width * thiz->count;
			size->min_width += thiz->spacing * (thiz->count - 1);
			size->min_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->max_width >= 0)
		{
			size->max_width = size->max_width * thiz->count;
			size->max_width += thiz->spacing * (thiz->count - 1);
			size->max_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->min_height >= 0)
		{
			size->min_height += thiz->padding.top + thiz->padding.bottom;
		}
		if (size->max_height >= 0)
		{
			size->max_height += thiz->padding.top + thiz->padding.bottom;
		}
	}

	return ret;
}

static int _eon_layout_array_vertical_homogeneous_size_hints_get(Eon_Layout_Array *thiz,
		Eon_Renderable_Size *size)
{
	int ret;

	ret = _eon_layout_array_homogeneous_size_hints_get(thiz, size);

	/* add the general padding and the inner spacing */
	if (ret & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (size->min_width >= 0)
		{
			size->min_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->max_width >= 0)
		{
			size->max_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->min_height >= 0)
		{
			size->min_height = size->min_height * thiz->count;
			size->min_height += thiz->spacing * (thiz->count - 1);
			size->min_height += thiz->padding.top + thiz->padding.bottom;
		}
		if (size->max_height >= 0)
		{
			size->max_height = size->max_height * thiz->count;
			size->max_height += thiz->spacing * (thiz->count - 1);
			size->max_height += thiz->padding.top + thiz->padding.bottom;
		}
	}

	return ret;
}

static int _eon_layout_array_horizontal_size_hints_get(Eon_Layout_Array *thiz,
		Eon_Renderable_Size *size)
{
	Eon_Box chp;
	Eon_Renderable_Size chs;
	int chm;
	int ret = 0;
	int count = 0;
	void *child = NULL;

	/* iterate over the children */
	while ((child = thiz->descriptor->next(child, &chp, &chm, &chs, NULL)))
	{
		if (chm & EON_RENDERABLE_HINT_MIN_MAX)
		{
			ret |= EON_RENDERABLE_HINT_MIN_MAX;

			if (chs.min_width >= 0)
				size->min_width += chs.min_width + chp.left + chp.right;
			if (size->max_width >= 0)
			{
				if (chs.max_width >= 0)
					size->max_width += chs.max_width + chp.left + chp.right;
				else
					size->max_width = -1;
			}

			if (chs.min_height >= 0)
				size->min_height = MAX(chs.min_height + chp.top + chp.bottom, size->min_height);
			if (size->max_height >= 0)
			{
				if (chs.max_height >= 0)
					size->max_height = MAX(chs.max_height + chp.top + chp.bottom, size->max_height);
				else
					size->max_height = -1;
			}
		}

		if (chm & EON_RENDERABLE_HINT_PREFERRED)
		{
			ERR("TODO: Missing preferred calcs");
		}
		count++;
	}

	/* add the general padding and the inner spacing */
	if (ret & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (size->min_width >= 0)
		{
			size->min_width += thiz->spacing * (count - 1);
			size->min_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->max_width >= 0)
		{
			size->max_width += thiz->spacing * (count - 1);
			size->max_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->min_height >= 0)
		{
			size->min_height += thiz->padding.top + thiz->padding.bottom;
		}
		if (size->max_height >= 0)
		{
			size->max_height += thiz->padding.top + thiz->padding.bottom;
		}
	}
	thiz->count = count;

	return ret;
}

static int _eon_layout_array_vertical_size_hints_get(Eon_Layout_Array *thiz,
		Eon_Renderable_Size *size)
{
	Eon_Box chp;
	Eon_Renderable_Size chs;
	int chm;
	int ret = 0;
	int count = 0;
	void *child = NULL;

	/* FIXME we are setting the initial hints to -1 for max width/height */
	//size->max_width = size->max_height = 0;
	/* iterate over the children */
	while ((child = thiz->descriptor->next(child, &chp, &chm, &chs, NULL)))
	{
		if (chm & EON_RENDERABLE_HINT_MIN_MAX)
		{
			ret |= EON_RENDERABLE_HINT_MIN_MAX;

			if (chs.min_width >= 0)
				size->min_width = MAX(chs.min_width + chp.left + chp.right, size->min_width);
			if (size->max_width >= 0)
			{
				if (chs.max_width >= 0)
					size->max_width = MAX(chs.max_width + chp.left + chp.right, size->max_width);
				else
					size->max_width = -1;
			}

			if (chs.min_height >= 0)
				size->min_height += chs.min_height + chp.top + chp.bottom;
			if (size->max_height >= 0)
			{
				if (chs.max_height >= 0)
					size->max_height += chs.max_height + chp.top + chp.bottom;
				else
					size->max_height = -1;
			}
		}

		if (chm & EON_RENDERABLE_HINT_PREFERRED)
		{
			ERR("TODO: Missing preferred calcs");
		}
		count++;
	}

	/* add the general padding and the inner spacing */
	if (ret & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (size->min_width >= 0)
		{
			size->min_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->max_width >= 0)
		{
			size->max_width += thiz->padding.left + thiz->padding.right;
		}
		if (size->min_height >= 0)
		{
			size->min_height += thiz->spacing * (count - 1);
			size->min_height += thiz->padding.top + thiz->padding.bottom;
		}
		if (size->max_height >= 0)
		{
			size->max_height += thiz->spacing * (count - 1);
			size->max_height += thiz->padding.top + thiz->padding.bottom;
		}
	}
	thiz->count = count;

	return ret;
}

static void _eon_layout_array_horizontal_homogeneous_geometry_set(Eon_Layout_Array *thiz,
		Eina_Rectangle *area)
{
	Eon_Box chp;
	Eon_Layout_Array_Child_Geometry_Set_Cb geometry_set;
	int ret = 0;
	int x, width;
	void *child = NULL;

	/* remove the padding */
	area->x += thiz->padding.left;
	area->w -= thiz->padding.left + thiz->padding.right;
	area->y += thiz->padding.top;
	area->h -= thiz->padding.top + thiz->padding.bottom;

	width = (area->w - ((thiz->count - 1) * thiz->spacing))/(thiz->count);
	x = area->x;

	/* iterate over the children */
	while ((child = thiz->descriptor->next(child, &chp, NULL, NULL, &geometry_set)))
	{
		Eina_Rectangle cha;

		cha.x = x + chp.left;
		cha.y = area->y + chp.top;
		cha.w = width - chp.left - chp.right;
		cha.h = area->h - chp.top - chp.bottom;
		geometry_set(child, &cha);
		x += width;
	}
}

static void _eon_layout_array_vertical_homogeneous_geometry_set(Eon_Layout_Array *thiz,
		Eina_Rectangle *area)
{
	Eon_Box chp;
	Eon_Layout_Array_Child_Geometry_Set_Cb geometry_set;
	int ret = 0;
	int y, height;
	void *child = NULL;

	/* remove the padding */
	area->x += thiz->padding.left;
	area->w -= thiz->padding.left + thiz->padding.right;
	area->y += thiz->padding.top;
	area->h -= thiz->padding.top + thiz->padding.bottom;

	height = (area->h - ((thiz->count - 1) * thiz->spacing))/(thiz->count);
	y = area->y;

	/* iterate over the children */
	while ((child = thiz->descriptor->next(child, &chp, NULL, NULL, &geometry_set)))
	{
		Eina_Rectangle cha;

		cha.x = area->x + chp.left;
		cha.w = area->w - chp.left - chp.right;
		cha.y = y + chp.top;
		cha.h = height - chp.top - chp.bottom;
		geometry_set(child, &cha);
		y += height;
	}
}

static void _eon_layout_array_horizontal_geometry_set(Eon_Layout_Array *thiz,
		Eina_Rectangle *area)
{
}

static void _eon_layout_array_vertical_geometry_set(Eon_Layout_Array *thiz,
		Eina_Rectangle *area)
{
	/* set every min height */
	/* add the missing to a list */
	/* calculate the free space */
	/* give the free space on every child */
	/* do not give more space that it can receive */
	/* calculate again the free space */
	/* and go processing it */
}
/*----------------------------------------------------------------------------*
 *                              Layout interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_layout_array_size_hints_get(Eon_Layout *l, Eon_Renderable_Size *size)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		if (thiz->homogeneous)
			return _eon_layout_array_horizontal_homogeneous_size_hints_get(thiz, size);
		else
			return _eon_layout_array_horizontal_size_hints_get(thiz, size);
	}
	else
	{
		if (thiz->homogeneous)
			return _eon_layout_array_vertical_homogeneous_size_hints_get(thiz, size);
		else
			return _eon_layout_array_vertical_size_hints_get(thiz, size);
	}
}

static void _eon_layout_array_geometry_set(Eon_Layout *l, Eina_Rectangle *area)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	if (!thiz->count)
		return;

	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
	{
		if (thiz->homogeneous)
			_eon_layout_array_horizontal_homogeneous_geometry_set(thiz, area);
		else
			_eon_layout_array_horizontal_geometry_set(thiz, area);
	}
	else
	{
		if (thiz->homogeneous)
			_eon_layout_array_vertical_homogeneous_geometry_set(thiz, area);
		else
			_eon_layout_array_vertical_geometry_set(thiz, area);
	}
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_LAYOUT_DESCRIPTOR,
		Eon_Layout_Array, Eon_Layout_Array_Class,
		eon_layout_array);

static void _eon_layout_array_class_init(void *k)
{
	Eon_Layout_Class *klass;

	klass = EON_LAYOUT_CLASS(k);
	klass->size_hints_get = _eon_layout_array_size_hints_get;
	klass->geometry_set = _eon_layout_array_geometry_set;
}

static void _eon_layout_array_instance_init(void *o)
{
}

static void _eon_layout_array_instance_deinit(void *o)
{
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Layout * eon_layout_array_new(Eon_Layout_Array_Descriptor *d, void *data)
{
	Eon_Layout_Array *thiz;
	Eon_Layout *l;

	l = ENESIM_OBJECT_INSTANCE_NEW(eon_layout_array);
	thiz = EON_LAYOUT_ARRAY(l);
	thiz->descriptor = d;
	thiz->data = data;

	return l;
}

void eon_layout_array_inner_spacing_set(Eon_Layout *l, int spacing)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->spacing = spacing;
}

void eon_layout_array_padding_set(Eon_Layout *l, Eon_Box *padding)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->padding = *padding;
}

void eon_layout_array_orientation_set(Eon_Layout *l, Eon_Orientation orientation)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->orientation = orientation;
}

void eon_layout_array_homogeneous_set(Eon_Layout *l, Eina_Bool homogeneous)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->homogeneous = homogeneous;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

