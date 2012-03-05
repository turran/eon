/* EON - Stack and Toolkit library
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
#include "Eon.h"
#include "eon_private.h"
/**
 * @todo Remove the last_expand property and add a weight property per child
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_STACK_CHILD_VERTICAL_ALIGNMENT;
static Ender_Property *EON_STACK_CHILD_HORIZONTAL_ALIGNMENT;
static Ender_Property *EON_STACK_DIRECTION;
static Ender_Property *EON_STACK_LAST_EXPAND;
static Ender_Property *EON_STACK_HOMOGENEOUS;

typedef struct _Eon_Stack_Child
{
	Ender_Element *ender;
	double old_x;
	double old_y;
	double curr_x;
	double curr_y;
	Eon_Horizontal_Alignment halign;
	Eon_Vertical_Alignment valign;
} Eon_Stack_Child;

typedef struct _Eon_Stack_State
{
	Eon_Stack_Direction direction;
	Eina_Bool homogeneous;
	Eina_Bool last_expand;
} Eon_Stack_State;

typedef struct _Eon_Stack
{
	Eina_List *children;
	Eina_Bool needs_setup : 1;
	Eon_Stack_State old, curr;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Stack;

typedef struct _Eon_Stack_Damage_Data
{
	double x;
	double y;
	Enesim_Renderer_Damage_Cb real_cb;
	void *real_data;
} Eon_Stack_Damage_Data;

static inline Eon_Stack * _eon_stack_get(Enesim_Renderer *r)
{
	Eon_Stack *thiz;

	thiz = eon_layout_data_get(r);
	return thiz;
}

static Eina_Bool _stack_damage_cb(Enesim_Renderer *child_r, Eina_Rectangle *rect, Eina_Bool past, void *data)
{
	Eon_Stack_Damage_Data *ddata = data;

	/* TODO the previous data should be added with the previous x, y */
	rect->x += ddata->x;
	rect->y += ddata->y;

	ddata->real_cb(child_r, rect, past, ddata->real_data);
	return EINA_TRUE;
}

static double _stack_vertical_min_width(Eon_Stack *thiz)
{
	Eon_Stack_Child *ech;
	Eina_List *l;
	double min_width = 0;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		double w;

		eon_element_min_width_get(ech->ender, &w);
		if (w > min_width)
			min_width = w;
	}
	return min_width;
}

static double _stack_vertical_min_height(Eon_Stack *thiz)
{
	Eon_Stack_Child *ech;
	Eina_List *l;
	double min_height = 0;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		double h;

		eon_element_min_height_get(ech->ender, &h);
		min_height += h;
	}
	return min_height;
}

static double _stack_horizontal_min_height(Eon_Stack *thiz)
{
	Eon_Stack_Child *ech;
	Eina_List *l;
	double min_height = 0;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		double h;

		eon_element_min_height_get(ech->ender, &h);
		if (h > min_height)
			min_height = h;
	}
	return min_height;
}

static double _stack_horizontal_min_width(Eon_Stack *thiz)
{
	Eon_Stack_Child *ech;
	Eina_List *l;
	double min_width = 0;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		double w;

		eon_element_min_width_get(ech->ender, &w);
		min_width += w;
	}
	return min_width;
}

static void _stack_horizontal_arrange(Ender_Element *e, Eon_Stack *thiz, double aw, double ah, Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Stack_Child *ech;
	Eon_Stack_Child *last_ech;
	Eon_Size size;
	Enesim_Renderer *r;
	Eina_List *l;
	double last_x = 0;

	r = ender_element_object_get(e);
	last_ech = eina_list_data_get(eina_list_last(thiz->children));
	size.width = aw;
	size.height = ah;
	//printf("H stack initial size %g %g\n", aw, ah);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *child_r;
		Enesim_Renderer *child_rr;
		Eon_Size child_size;
		double y = 0;

		if (size.width <= 0)
			break;
		child_r = ender_element_object_get(ech->ender);

		eon_element_real_relative_size_get(ech->ender, &size, &child_size);
		if (!thiz->curr.last_expand || ech != last_ech)
		{
			eon_element_real_width_get(ech->ender, &child_size.width);
			if (child_size.width > size.width)
				child_size.width = size.width;
		}
		if (child_size.height != size.height)
		{
			switch (ech->valign)
			{
				case EON_VERTICAL_ALIGNMENT_TOP:
				y = 0;
				break;

				case EON_VERTICAL_ALIGNMENT_BOTTOM:
				y = size.height - child_size.height;
				break;

				case EON_VERTICAL_ALIGNMENT_CENTER:
				y = (size.height / 2) - (child_size.height / 2);
				break;
			}
		}
		{
			const char *name;
			const char *child_name;

			enesim_renderer_name_get(r, &name);
			enesim_renderer_name_get(child_r, &child_name);
			//printf("H setting child %s %s %g %g %g %g (aw, ah %g %g)\n", name, child_name, last_x, y, child_size.width, child_size.height, aw, ah);
		}
		eon_element_actual_size_set(child_r, child_size.width, child_size.height);
		eon_element_actual_position_set(child_r, last_x, y);
		eon_element_setup(ech->ender, s, err);
		child_rr = eon_element_renderer_get(ech->ender);
		/* now add the renderer associated with the widget into the theme */
		eon_widget_property_add(r, "child", child_rr, NULL);
		ech->curr_x = last_x;
		ech->curr_y = y;
		last_x += child_size.width;
		size.width -= child_size.width;
	}
}

static void _stack_vertical_arrange(Ender_Element *e, Eon_Stack *thiz, double aw, double ah, Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Stack_Child *ech;
	Eon_Stack_Child *last_ech;
	Eon_Size size;
	Enesim_Renderer *r;
	Eina_List *l;
	double last_y = 0;

	r = ender_element_object_get(e);
	last_ech = eina_list_data_get(eina_list_last(thiz->children));
	size.width = aw;
	size.height = ah;
	//printf("V stack initial size %g %g\n", aw, ah);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *child_r;
		Enesim_Renderer *child_rr;
		Eon_Size child_size;
		double x = 0;

		if (size.height <= 0)
			break;

		child_r = ender_element_object_get(ech->ender);

		eon_element_real_relative_size_get(ech->ender, &size, &child_size);
		{
			const char *name;
			const char *child_name;

			//printf("V child %s %s relative size %g %g -> %g %g\n", name, child_name, size.width, size.height, child_size.width, child_size.height);
		}
		if (!thiz->curr.last_expand || ech != last_ech)
		{
			eon_element_real_height_get(ech->ender, &child_size.height);
			if (child_size.height > size.height)
				child_size.height = size.height;
		}
		if (child_size.width != size.width)
		{
			switch (ech->halign)
			{
				case EON_HORIZONTAL_ALIGNMENT_LEFT:
				x = 0;
				break;

				case EON_HORIZONTAL_ALIGNMENT_RIGHT:
				x = size.width - child_size.width;
				break;

				case EON_HORIZONTAL_ALIGNMENT_CENTER:
				x = (size.width / 2) - (child_size.width / 2);
				break;
			}
		}
		{
			const char *name;
			const char *child_name;

			enesim_renderer_name_get(r, &name);
			enesim_renderer_name_get(child_r, &child_name);
			//printf("V setting child %s %s %g %g %g %g (aw,ah %g %g)\n", name, child_name, x, last_y, child_size.width, child_size.height, aw, ah);
		}
		eon_element_actual_size_set(child_r, child_size.width, child_size.height);
		eon_element_actual_position_set(child_r, x, last_y);
		eon_element_setup(ech->ender, s, err);
		/* now add the renderer associated with the widget into the theme */
		child_rr = eon_element_renderer_get(ech->ender);
		eon_widget_property_add(r, "child", child_rr, NULL);
		ech->curr_x = x;
		ech->curr_y = last_y;
		last_y += child_size.height;
		size.height -= child_size.height;
	}
}

static Eina_Bool _stack_relayout(Ender_Element *e,
		Eon_Stack *thiz,
		const Eon_Size *size,
		Enesim_Surface *s,
		Enesim_Error **err)
{
	Enesim_Renderer *r;

	/* the idea on a layout setup is the set the actual width and height
	 * of every child before calling the setup of each child
	 */
	r = ender_element_object_get(e);
	eon_widget_property_clear(r, "child");
	if (thiz->curr.direction == EON_STACK_DIRECTION_HORIZONTAL)
		_stack_horizontal_arrange(e, thiz, size->width, size->height, s, err);
	else
		_stack_vertical_arrange(e, thiz, size->width, size->height, s, err);
	/* FIXME */
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_stack_free(Enesim_Renderer *r)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	free(thiz);
}

static void _eon_stack_damage(Ender_Element *e, Enesim_Renderer_Damage_Cb cb, void *data)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;
	double x;
	double y;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);

	eon_element_actual_position_get(r, &x, &y);
	/* if we have changed then just return our size */
	if (thiz->needs_setup)
whole:
	{
		Eina_Rectangle area;
		double w;
		double h;

		eon_element_actual_width_get(e, &w);
		eon_element_actual_height_get(e, &h);
		area.x = x;
		area.y = y;
		area.w = (int)w;
		area.h = (int)h;

		cb(r, &area, EINA_FALSE, data);
		return;
	}
	/* if not, return the children's */
	else
	{
		Eon_Stack_Damage_Data ddata;
		Eon_Stack_Child *ech;
		Eina_List *l;

		ddata.x = x;
		ddata.y = y;
		ddata.real_cb = cb;
		ddata.real_data = data;

		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			/* in case a children needs a setup given that the size allocation is relative to others
			 * we need to do the setup again on the whole stack*/
			if (eon_element_needs_setup(ech->ender))
				goto whole;
			eon_element_damages_get(ech->ender, _stack_damage_cb, &ddata);
		}
	}
}

static Eina_Bool _eon_stack_needs_setup(Ender_Element *e)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Enesim_Renderer *r;
	Eina_List *l;
	Eina_Bool ret;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);

	ret = thiz->needs_setup;
	if (ret) return EINA_TRUE;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		ret = eon_element_needs_setup(ech->ender);
		if (ret) break;
	}
	return ret;
}

static Eina_Bool _eon_stack_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	/* TODO we should only relayout whenever our properties have changed or a child
	 * has changed properties, if not just leave it as is. Also we should not
	 * check *every* property
	 */
	/* if the actual size has changed, then relayout
	 * if some child preferred/min/max size has changed then call the relayout
	 */
	return _stack_relayout(e, thiz, &state->actual_size, s, err);
}

static void _eon_stack_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Enesim_Renderer *r;
	Eina_List *l;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		eon_element_cleanup(ech->ender, s);
	}
	thiz->needs_setup = EINA_FALSE;
}

static double _eon_stack_min_width_get(Ender_Element *e)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;
	double min_width;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	if (!thiz) return 0;

	if (thiz->curr.direction == EON_STACK_DIRECTION_HORIZONTAL)
		min_width = _stack_horizontal_min_width(thiz);
	else
		min_width = _stack_vertical_min_width(thiz);

	return min_width;
}

static double _eon_stack_min_height_get(Ender_Element *e)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;
	double min_height;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	if (!thiz) return 0;

	if (thiz->curr.direction == EON_STACK_DIRECTION_HORIZONTAL)
		min_height = _stack_horizontal_min_height(thiz);
	else
		min_height = _stack_vertical_min_height(thiz);

	return min_height;
}

static double _eon_stack_preferred_width_get(Ender_Element *e)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;
	double preferred_width = -1;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	if (!thiz) return preferred_width;

	if (thiz->curr.direction == EON_STACK_DIRECTION_HORIZONTAL)
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		preferred_width = 0;
		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double pw;

			eon_element_preferred_width_get(ech->ender, &pw);
			if (pw < 0) continue;
			preferred_width += pw;
		}
	}
	else
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double pw;

			eon_element_preferred_width_get(ech->ender, &pw);
			preferred_width = MAX(pw, preferred_width);
		}

	}
	return preferred_width;
}

static double _eon_stack_preferred_height_get(Ender_Element *e)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;
	double preferred_height = -1;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	if (!thiz) return preferred_height;

	if (thiz->curr.direction == EON_STACK_DIRECTION_VERTICAL)
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		preferred_height = 0;
		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double ph;

			eon_element_preferred_height_get(ech->ender, &ph);
			if (ph < 0) continue;
			preferred_height += ph;
		}
	}
	else
	{
		Eon_Stack_Child *ech;
		Eina_List *l;

		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			double ph;

			eon_element_preferred_height_get(ech->ender, &ph);
			preferred_height = MAX(ph, preferred_height);
		}
		//printf("preferred height %g\n", preferred_height);
	}

	return preferred_height;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's layout interface                         *
 *----------------------------------------------------------------------------*/
/* FIXME we need to decide how are we going to get find an element on the scene
 * so far we were intersecting the input coordinates with the object layout
 * geometry, for things like comboboxes we might need to intersect against
 * the displayed geometry, given that a combobox display somehting outside
 * the layout tree but the user should interact with it normally
 */
static Ender_Element * _eon_stack_child_at(Ender_Element *e, double x, double y)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Ender_Element *child = NULL;
	Enesim_Renderer *r;
	Eina_List *l;

	r = ender_element_object_get(e);
	thiz = _eon_stack_get(r);
	if (!thiz) return NULL;
#if 0
	{
		char *name;
		enesim_renderer_name_get(r, &name);
		printf("stack %s child at %g %g \n", name, x, y);
	}
#endif

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *rchild;
		Eon_Size child_size;
		double child_x, child_y;

		child_x = x - ech->curr_x;
		if (child_x < 0) continue;
		child_y = y - ech->curr_y;
		if (child_y < 0) continue;

		/* TODO still need the width and height */
		rchild = ender_element_object_get(ech->ender);
		if (eon_is_element(rchild))
		{
			eon_element_actual_size_get(rchild, &child_size);
		}
		else
		{
			Enesim_Rectangle bounds;
			enesim_renderer_boundings(rchild, &bounds);
			child_size.width = bounds.w;
			child_size.height = bounds.h;
		}
		if (child_x <= child_size.width && child_y <= child_size.height)
		{
			if (enesim_renderer_is_inside(rchild, child_x, child_y))
			{
				child = ech->ender;
				break;
			}
		}
	}
	//printf("returning %p\n", child);

	return child;
}

static void _eon_stack_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;

	thiz = _eon_stack_get(r);
	thiz_child = calloc(1, sizeof(Eon_Stack_Child));
	thiz_child->ender = child;
	thiz->children = eina_list_append(thiz->children, thiz_child);
	thiz->needs_setup = EINA_TRUE;

	ender_element_value_set(child, "rop", ENESIM_BLEND, NULL);
}

static void _eon_stack_child_remove(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;
	Eina_List *l, *l_next;
	Eina_Bool found = EINA_FALSE;

	thiz = _eon_stack_get(r);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, thiz_child)
	{
		if (thiz_child->ender == child)
		{
			thiz->children = eina_list_remove_list(thiz->children, l);
			found = EINA_TRUE;
			break;
		}
	}
	if (found)
		thiz->needs_setup = EINA_TRUE;
}

static void _eon_stack_child_clear(Enesim_Renderer *r)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;
	Eina_List *l, *l_next;

	thiz = _eon_stack_get(r);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, thiz_child)
	{
		thiz->children = eina_list_remove_list(thiz->children, l);
	}
	eon_widget_property_clear(r, "child");
	thiz->needs_setup = EINA_TRUE;
}

static Eon_Layout_Descriptor _descriptor = {
	.child_add = _eon_stack_child_add,
	.child_clear = _eon_stack_child_clear,
	.child_remove = _eon_stack_child_remove,
	.child_at = _eon_stack_child_at,
	.min_width_get = _eon_stack_min_width_get,
	.min_height_get = _eon_stack_min_height_get,
	.preferred_width_get = _eon_stack_preferred_width_get,
	.preferred_height_get = _eon_stack_preferred_height_get,
	.damage = _eon_stack_damage,
	.needs_setup = _eon_stack_needs_setup,
	.cleanup = _eon_stack_cleanup,
	.setup = _eon_stack_setup,
	.free = _eon_stack_free,
	.name = "stack",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_stack_new(void)
{
	Eon_Stack *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Stack));
	if (!thiz) return NULL;

	r = eon_layout_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_stack_direction_set(Enesim_Renderer *r, Eon_Stack_Direction direction)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	thiz->curr.direction = direction;
	thiz->needs_setup = EINA_TRUE;
}

static void _eon_stack_direction_get(Enesim_Renderer *r, Eon_Stack_Direction *direction)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	if (direction) *direction = thiz->curr.direction;

}

static void _eon_stack_child_horizontal_alignment_set(Enesim_Renderer *r, Ender_Element *child,
		Eon_Horizontal_Alignment alignment)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Eina_List *l;

	thiz = _eon_stack_get(r);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->halign = alignment;
			thiz->needs_setup = EINA_TRUE;
		}
	}
}

static void _eon_stack_child_vertical_alignment_set(Enesim_Renderer *r, Ender_Element *child,
		Eon_Vertical_Alignment alignment)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Eina_List *l;

	thiz = _eon_stack_get(r);
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->valign = alignment;
			thiz->needs_setup = EINA_TRUE;
		}
	}
}

static void _eon_stack_last_expand_set(Enesim_Renderer *r, Eina_Bool last_expand)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	thiz->curr.last_expand = last_expand;
	thiz->needs_setup = EINA_TRUE;
}

static void _eon_stack_last_expand_get(Enesim_Renderer *r, Eina_Bool *last_expand)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	*last_expand = thiz->curr.last_expand;
}

static void _eon_stack_homogeneous_set(Enesim_Renderer *r, Eina_Bool homogeneous)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	thiz->curr.homogeneous = homogeneous;
	thiz->needs_setup = EINA_TRUE;
}

static void _eon_stack_homogeneous_get(Enesim_Renderer *r, Eina_Bool *homogeneous)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	*homogeneous = thiz->curr.homogeneous;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_stack_child_horizontal_alignment_get NULL
#define _eon_stack_child_vertical_alignment_get NULL
#include "eon_generated_stack.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_stack_new(void)
{
	return ender_element_new_with_namespace("stack", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_hstack_new(void)
{
	Ender_Element *e;

	e = ender_element_new_with_namespace("stack", "eon");
	eon_stack_direction_set(e, EON_STACK_DIRECTION_HORIZONTAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_vstack_new(void)
{
	Ender_Element *e;

	e = ender_element_new_with_namespace("stack", "eon");
	eon_stack_direction_set(e, EON_STACK_DIRECTION_VERTICAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_direction_set(Ender_Element *e, Eon_Stack_Direction direction)
{
	ender_element_value_set(e, "direction", direction, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_direction_get(Ender_Element *e, Eon_Stack_Direction *direction)
{
	ender_element_value_get(e, "direction", direction, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_horizontal_alignment_set(Ender_Element *e, Ender_Element *child,
		Eon_Horizontal_Alignment alignment)
{
	ender_element_value_set(child, "child_horizontal_alignment", alignment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_vertical_alignment_set(Ender_Element *e, Ender_Element *child,
		Eon_Vertical_Alignment alignment)
{
	ender_element_value_set(child, "child_vertical_alignment", alignment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_last_expand_set(Ender_Element *e, Eina_Bool expand)
{
	ender_element_value_set(e, "last_expand", expand, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_last_expand_get(Ender_Element *e, Eina_Bool *expand)
{
	ender_element_value_get(e, "last_expand", expand, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_homogeneous_set(Ender_Element *e, Eina_Bool homogeneous)
{
	ender_element_value_set(e, "homogeneous", homogeneous, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_homogeneous_get(Ender_Element *e, Eina_Bool *homogeneous)
{
	ender_element_value_get(e, "homogeneous", homogeneous, NULL);
}

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_weight_set(Ender_Element *e, Ender_Element *child,
		int weight)
{
	ender_element_value_set(child, "weight", weight, NULL);
}
#endif
