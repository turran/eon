#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Canvas_Child
{
	Ender *ender;
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
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill_func;
} Eon_Canvas;

static inline Eon_Canvas * _eon_canvas_get(Enesim_Renderer *r)
{
	Eon_Canvas *e;

	e = eon_layout_data_get(r);
	return e;
}

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
	Eon_Canvas *e;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	e = _eon_canvas_get(r);
	if (!e) return EINA_FALSE;
	if (e->curr.width == 0 || e->curr.height == 0)
	{
		DBG("Invalid size %dx%d", e->curr.width, e->curr.height);
		return EINA_FALSE;
	}

	if (e->curr.width != e->old.width || e->curr.height != e->old.height)
	{
		if (e->tiler) eina_tiler_free(e->tiler);
		/* create a new tiler */
		e->tiler = eina_tiler_new(e->curr.width, e->curr.height);
	}

	/* set the coordinates on every child */
	EINA_LIST_FOREACH (e->children, l, ech)
	{
		Enesim_Renderer *renderer;
		Enesim_Matrix matrix;
		Enesim_Matrix_Type matrix_type;
		Eina_Rectangle boundings;

		renderer = ender_renderer_get(ech->ender);
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
	if (!enesim_renderer_sw_setup(e->compound))
	{
		DBG("Cannot setup the compound renderer");
		return EINA_FALSE;
	}
	e->fill_func = enesim_renderer_sw_fill_get(e->compound);
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

		renderer = ender_renderer_get(ech->ender);
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

static void _eon_canvas_boundings(Enesim_Renderer *r, Eina_Rectangle *rect)
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
EAPI Enesim_Renderer * eon_canvas_new(void)
{
	Eon_Canvas *e;
	Enesim_Renderer *thiz;
	Enesim_Renderer *compound;

	compound = enesim_renderer_compound_new();
	if (!compound) goto compound_err;
	enesim_renderer_rop_set(compound, ENESIM_BLEND);

	e = calloc(1, sizeof(Eon_Canvas));
	if (!e) goto alloc_err;

	e->compound = compound;

	thiz = eon_layout_new(&_eon_canvas_descriptor, e);
	if (!thiz) goto renderer_err;

	return thiz;

renderer_err:
	free(e);
alloc_err:
	enesim_renderer_delete(compound);
compound_err:
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_width_set(Enesim_Renderer *r, unsigned int width)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	e->curr.width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_height_set(Enesim_Renderer *r, unsigned int height)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	e->curr.height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_add(Enesim_Renderer *r, Ender *child)
{
	Eon_Canvas *e;
	Eon_Canvas_Child *ech;

	e = _eon_canvas_get(r);
	ech = calloc(1, sizeof(Eon_Canvas_Child));
	ech->ender = child;
	e->children = eina_list_append(e->children, ech);
	enesim_renderer_compound_layer_add(e->compound, ender_renderer_get(child));
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_x_set(Enesim_Renderer *r, Ender *child, double x)
{
	Eon_Canvas *e;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	e = _eon_canvas_get(r);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (e->children, l, ech)
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
EAPI void eon_canvas_child_y_set(Enesim_Renderer *r, Ender *child, double y)
{
	Eon_Canvas *e;
	Eon_Canvas_Child *ech;
	Eina_List *l;

	e = _eon_canvas_get(r);
	/* get the bounding box, transform to destination coordinates
	 * check that is inside the pointer, trigger the event */
	EINA_LIST_FOREACH (e->children, l, ech)
	{
		if (ech->ender == child)
		{
			ech->y = y;
		}
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_fill_color_set(Enesim_Renderer *r, Enesim_Color color)
{

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_fill_ender_set(Enesim_Renderer *r, Ender *ender)
{

}
