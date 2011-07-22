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
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Canvas_Child
{
	Ender_Element *ender;
	double old_x;
	double old_y;
	double x;
	double y;
} Eon_Canvas_Child;

typedef struct _Eon_Canvas_State
{
	unsigned int width;
	unsigned int height;
	Enesim_Color color;
} Eon_Canvas_State;

typedef struct _Eon_Canvas
{
	Eina_Tiler *tiler;
	Eina_List *children;
	Eon_Canvas_State old, curr;
	Enesim_Renderer *background;
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Canvas;

static inline Eon_Canvas * _eon_canvas_get(Enesim_Renderer *r)
{
	Eon_Canvas *thiz;

	thiz = eon_layout_data_get(r);
	return thiz;
}

#if 0
static void _canvas_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	/* just iterate over the list of dirty rectangles and intersect against the span */
	/* if it intersects render the child that is on that span from bottom to top */
	e->fill_func(e->compound, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_canvas_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(r);
	if (!thiz) return EINA_FALSE;
	if (thiz->curr.width == 0 || thiz->curr.height == 0)
	{
		DBG("Invalid size %dx%d", thiz->curr.width, thiz->curr.height);
		return EINA_FALSE;
	}

	if (thiz->curr.width != thiz->old.width || thiz->curr.height != thiz->old.height)
	{
		if (thiz->tiler) eina_tiler_free(thiz->tiler);
		/* create a new tiler */
		thiz->tiler = eina_tiler_new(thiz->curr.width, thiz->curr.height);
	}
	/* TODO handle the background */
	/* set the coordinates on every child */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Enesim_Renderer *renderer;
		Enesim_Matrix matrix;
		Enesim_Matrix_Type matrix_type;
		Enesim_Rectangle boundings;

		renderer = ender_element_renderer_get(ech->ender);
		enesim_renderer_transformation_get(renderer, &matrix);
		matrix_type = enesim_matrix_type_get(&matrix);
		enesim_renderer_boundings(renderer, &boundings);
		if (matrix_type == ENESIM_MATRIX_IDENTITY)
		{
			/* just translate the origin, do a matrix set?
			 * matrix compose? origin set?
			 */
			enesim_renderer_origin_get(renderer, &ech->old_x, &ech->old_y);
			enesim_renderer_origin_set(renderer, ech->x, ech->y);
		}
		else
		{
			Enesim_Matrix translate;

			/* multiply the current matrix to translate it to the final destination */
			enesim_matrix_translate(&translate, -ech->x, -ech->y);
			enesim_matrix_compose(&matrix, &translate, &matrix);
			enesim_renderer_transformation_set(renderer, &matrix);
		}
	}
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		DBG("Cannot setup the compound renderer");
		return EINA_FALSE;
	}
	thiz->fill_func = enesim_renderer_sw_fill_get(thiz->compound);
	*fill = _canvas_draw;

	return EINA_TRUE;
}

static void _eon_canvas_cleanup(Enesim_Renderer *r)
{
	Eon_Canvas *e;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	e = _eon_canvas_get(r);
	if (!e) return;

	/* remove every dirty rectangle? */
	if (e->tiler) eina_tiler_clear(e->tiler);
	/* restore the coordinates on every child */
	EINA_LIST_FOREACH (e->children, l, ech)
	{
		Enesim_Renderer *renderer;
		Enesim_Matrix matrix;
		Enesim_Matrix_Type matrix_type;

		renderer = ender_element_renderer_get(ech->ender);
		enesim_renderer_transformation_get(renderer, &matrix);
		matrix_type = enesim_matrix_type_get(&matrix);
		if (matrix_type == ENESIM_MATRIX_IDENTITY)
		{
			enesim_renderer_origin_set(renderer, ech->old_x, ech->old_y);
		}
		else
		{
			Enesim_Matrix translate;

			/* multiply the current matrix to translate it to the final destination */
		}
	}
}

static void _eon_canvas_free(Enesim_Renderer *r)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	if (!e) return;

	free(e);
}

static void _eon_canvas_boundings(Enesim_Renderer *r, Enesim_Rectangle *rect)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	if (!e) return;

	rect->x = 0;
	rect->y = 0;
	rect->w = e->curr.width;
	rect->h = e->curr.height;
}

static Enesim_Renderer_Descriptor _eon_canvas_descriptor = {
	.sw_setup = _eon_canvas_setup,
	.sw_cleanup = _eon_canvas_cleanup,
	.boundings = _eon_canvas_boundings,
	.free = _eon_canvas_free,
};
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's layout interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_canvas_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;

	thiz = _eon_canvas_get(r);
	ech = calloc(1, sizeof(Eon_Canvas_Child));
	ech->ender = child;
	thiz->children = eina_list_append(thiz->children, ech);
	enesim_renderer_compound_layer_add(thiz->compound, ender_element_renderer_get(child));
}

static void _eon_canvas_child_remove(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Canvas *thiz;

	thiz = _eon_canvas_get(r);
}

static Eon_Layout_Descriptor _descriptor = {
	.child_add = _eon_canvas_child_add,
	.child_remove = _eon_canvas_child_remove,
	.name = "canvas",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_canvas_new(void)
{
	Eon_Canvas *e;
	Enesim_Renderer *thiz;
	Enesim_Renderer *r;

	e = calloc(1, sizeof(Eon_Canvas));
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

	thiz = eon_layout_new(&_descriptor, e);
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

static void _eon_canvas_child_x_set(Enesim_Renderer *r, Ender_Element *child, double x)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(r);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->x = x;
		}
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
static void _eon_canvas_child_y_set(Enesim_Renderer *r, Ender_Element *child, double y)
{
	Eon_Canvas *thiz;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	thiz = _eon_canvas_get(r);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->y = y;
		}
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_canvas_child_x_get NULL
#define _eon_canvas_child_y_get NULL
#include "eon_generated_canvas.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_canvas_new(void)
{
	return ender_element_new_with_namespace("canvas", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_x_set(Ender_Element *e, Ender_Element *child, double x)
{
	ender_element_value_set(child, "x", x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_y_set(Ender_Element *e, Ender_Element *child, double y)
{
	ender_element_value_set(child, "y", y, NULL);
}
