/* EON - Stack and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Stack_Child
{
	Ender *ender;
	double old_x;
	double old_y;
	Eon_Horizontal_Alignment halign;
	Eon_Vertical_Alignment valign;
} Eon_Stack_Child;

typedef struct _Eon_Stack_State
{
	unsigned int width;
	unsigned int height;
	Eon_Stack_Direction direction;
} Eon_Stack_State;

typedef struct _Eon_Stack
{
	Eina_List *children;
	Eon_Stack_State old, curr;
	Enesim_Renderer *background;
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Stack;

static inline Eon_Stack * _eon_stack_get(Enesim_Renderer *r)
{
	Eon_Stack *e;

	e = eon_layout_data_get(r);
	return e;
}

static void _stack_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Stack *e;

	e = _eon_stack_get(r);
	/* just iterate over the list of dirty rectangles and intersect against the span */
	/* if it intersects render the child that is on that span from bottom to top */
	e->fill_func(e->compound, x, y, len, dst);
}

static void _stack_horizontal_arrange(Eon_Stack *thiz)
{
	Eon_Stack_Child *ech;
	Eina_List *l;
	double last_x = 0;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *renderer;
		Enesim_Matrix matrix;
		Enesim_Matrix_Type matrix_type;
		Eina_Rectangle boundings;
		double y;
		double x;

		renderer = ender_element_renderer_get(ech->ender);
		enesim_renderer_transformation_get(renderer, &matrix);
		matrix_type = enesim_matrix_type_get(&matrix);
		enesim_renderer_boundings(renderer, &boundings);
		enesim_renderer_origin_get(renderer, &ech->old_x, &ech->old_y);

		x = -boundings.x + last_x;
		switch (ech->valign)
		{
			case EON_VERTICAL_ALIGNMENT_TOP:
			y = -boundings.y;
			break;

			case EON_VERTICAL_ALIGNMENT_BOTTOM:
			y = -boundings.y + thiz->curr.height - boundings.h;
			break;

			case EON_VERTICAL_ALIGNMENT_CENTER:
			y = -boundings.y + (thiz->curr.height / 2) - (boundings.h / 2);
			break;

		}
		enesim_renderer_origin_set(renderer, x, y);
		last_x += boundings.w;
	}
}

static void _stack_vertical_arrange(Eon_Stack *thiz)
{
	Eon_Stack_Child *ech;
	Eina_List *l;
	double last_y = 0;

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *renderer;
		Enesim_Matrix matrix;
		Enesim_Matrix_Type matrix_type;
		Eina_Rectangle boundings;
		double y;
		double x;

		renderer = ender_element_renderer_get(ech->ender);
		enesim_renderer_transformation_get(renderer, &matrix);
		matrix_type = enesim_matrix_type_get(&matrix);
		enesim_renderer_boundings(renderer, &boundings);
		enesim_renderer_origin_get(renderer, &ech->old_x, &ech->old_y);

		y = -boundings.y  + last_y;
		switch (ech->halign)
		{
			case EON_HORIZONTAL_ALIGNMENT_LEFT:
			x = -boundings.x;
			break;

			case EON_HORIZONTAL_ALIGNMENT_RIGHT:
			x = -boundings.x + thiz->curr.width - boundings.w;
			break;

			case EON_HORIZONTAL_ALIGNMENT_CENTER:
			x = -boundings.x + (thiz->curr.width / 2) - (boundings.w / 2);
			break;

		}
		enesim_renderer_origin_set(renderer, x, y);
		last_y += boundings.h;
	}
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static void _eon_stack_cleanup(Enesim_Renderer *r)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *ech;
	Eina_List *l;
	double ox, oy;

	thiz = _eon_stack_get(r);
	if (!thiz) return;

	/* restore the coordinates on every child */
	enesim_renderer_sw_cleanup(thiz->compound);

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *renderer;
		Enesim_Matrix matrix;
		Enesim_Matrix_Type matrix_type;

		renderer = ender_element_renderer_get(ech->ender);
		//enesim_renderer_transformation_get(renderer, &matrix);
		//matrix_type = enesim_matrix_type_get(&matrix);
		enesim_renderer_origin_set(renderer, ech->old_x, ech->old_y);
	}
}

static Eina_Bool _eon_stack_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Eon_Stack *thiz;
	double ox, oy;

	thiz = _eon_stack_get(r);
	if (!thiz) return EINA_FALSE;
	if (!eon_layout_state_setup(r, thiz->curr.width, thiz->curr.height))
		return EINA_FALSE;
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	/* set the coordinates on every child */
	/* the way to setting the actual size is based on min-size, max-size
	 * and the boundings for an eon element or only the boundings
	 * for an enesim renderer
	 */
	if (thiz->curr.direction == EON_STACK_DIRECTION_HORIZONTAL)
		_stack_horizontal_arrange(thiz);
	else
		_stack_vertical_arrange(thiz);

	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		DBG("Cannot setup the compound renderer");
		_eon_stack_cleanup(r);
		return EINA_FALSE;
	}
	thiz->fill_func = enesim_renderer_sw_fill_get(thiz->compound);
	*fill = _stack_draw;

	return EINA_TRUE;
}

static void _eon_stack_free(Enesim_Renderer *r)
{
	Eon_Stack *e;

	e = _eon_stack_get(r);
	if (!e) return;

	free(e);
}

static void _eon_stack_boundings(Enesim_Renderer *r, Eina_Rectangle *rect)
{
	Eon_Stack *e;

	e = _eon_stack_get(r);
	if (!e) return;

	rect->x = 0;
	rect->y = 0;
	rect->w = e->curr.width;
	rect->h = e->curr.height;
}

static Enesim_Renderer_Descriptor _eon_stack_renderer_descriptor = {
	.sw_setup = _eon_stack_setup,
	.sw_cleanup = _eon_stack_cleanup,
	.boundings = _eon_stack_boundings,
	.free = _eon_stack_free,
};
/*----------------------------------------------------------------------------*
 *                         The Eon's layout interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_stack_child_add(Enesim_Renderer *r, Ender *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;

	thiz = _eon_stack_get(r);
	thiz_child = calloc(1, sizeof(Eon_Stack_Child));
	thiz_child->ender = child;
	thiz->children = eina_list_append(thiz->children, thiz_child);
	enesim_renderer_compound_layer_add(thiz->compound, ender_element_renderer_get(child));
	/* TODO whenever a child is added, register a callback for a property
	 * change, if it is called then we need to do the setup again
	 */
}

static void _eon_stack_child_remove(Enesim_Renderer *r, Ender *child)
{
	Eon_Stack *thiz;
	Eon_Stack_Child *thiz_child;
}

static Eon_Layout_Descriptor _eon_stack_layout_descriptor = {
	.child_add = _eon_stack_child_add,
	.child_remove = _eon_stack_child_remove,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_stack_new(void)
{
	Eon_Stack *e;
	Enesim_Renderer *thiz;
	Enesim_Renderer *r;

	e = calloc(1, sizeof(Eon_Stack));
	if (!e) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	e->compound = r;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_background_color_set(r, 0xffffffff);
	enesim_renderer_compound_layer_add(e->compound, r);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	e->background = r;

	thiz = eon_layout_new(&_eon_stack_layout_descriptor,
			&_eon_stack_renderer_descriptor, e);
	if (!thiz) goto renderer_err;

	return thiz;

renderer_err:
	enesim_renderer_delete(e->background);
background_err:
	enesim_renderer_delete(e->compound);
compound_err:
	free(e);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_width_set(Enesim_Renderer *r, unsigned int width)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	thiz->curr.width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_height_set(Enesim_Renderer *r, unsigned int height)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	thiz->curr.height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_direction_set(Enesim_Renderer *r, Eon_Stack_Direction direction)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	thiz->curr.direction = direction;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_direction_get(Enesim_Renderer *r, Eon_Stack_Direction *direction)
{
	Eon_Stack *thiz;

	thiz = _eon_stack_get(r);
	if (direction) *direction = thiz->curr.direction;

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_horizontal_alignment_set(Enesim_Renderer *r, Ender *child,
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
		}
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_stack_child_vertical_alignment_set(Enesim_Renderer *r, Ender *child,
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
		}
	}
}

