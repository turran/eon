#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Canvas_Child
{
	double x;
	double y;
} Eon_Canvas_Child;

typedef struct _Eon_Canvas_State
{
	unsigned int width;
	unsigned int height;
} Eon_Canvas_State;

typedef struct _Eon_Canvas
{
	Eina_Tiler *tiler;
	Eon_Canvas_State old, curr;
} Eon_Canvas;

static inline Eon_Canvas * _eon_canvas_get(Enesim_Renderer *r)
{
	Eon_Canvas *e;

	e = enesim_renderer_data_get(r);
	return e;
}


static void _canvas_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	/* just iterate over the list of dirty rectangles and intersect against the span */
	/* if it intersects render the child that is on that span from bottom to top */
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_canvas_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	if (!e) return EINA_FALSE;
	if (e->curr.width == 0 || e->curr.height == 0) return EINA_FALSE;

	return EINA_TRUE;
}

static void _eon_canvas_cleanup(Enesim_Renderer *r)
{
	Eon_Canvas *e;

	e = _eon_canvas_get(r);
	if (!e) return;
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

	/* we should calculate the current width/height */
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

	e = _eon_canvas_get(r);
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

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_x_set(Enesim_Renderer *r, Ender *child, double x)
{

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_canvas_child_y_set(Enesim_Renderer *r, Ender *child, double y)
{

}
