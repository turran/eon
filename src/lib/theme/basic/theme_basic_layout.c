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
#include "Eon_Basic.h"
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Theme_Basic_Layout
{
	Enesim_Renderer *background;
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill;
} Theme_Basic_Layout;

static inline Theme_Basic_Layout * _layout_get(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = eon_theme_layout_data_get(r);
	return thiz;
}

static void _layout_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's layout theme interface                      *
 *----------------------------------------------------------------------------*/
#if 0
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

	//printf("setting up the stack %p\n", r);
	if (thiz->relayout)
	{
		//if (!eon_layout_state_setup(r, thiz->curr.width, thiz->curr.height))
		//	return EINA_FALSE;
		/* set the coordinates on every child */
		/* the way to setting the actual size is based on min-size, max-size
		 * and the boundings for an eon element or only the boundings
		 * for an enesim renderer
		 */
		_stack_relayout(r, thiz);
	}
	else
	{
		Eina_List *l;
		Eon_Stack_Child *ech;

		/* just set the new coordinates on every child */
		EINA_LIST_FOREACH (thiz->children, l, ech)
		{
			Enesim_Renderer *r_child;

			r_child = ender_element_renderer_get(ech->ender);
			enesim_renderer_origin_set(r_child, ech->curr_x, ech->curr_y);
		}
	}

	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		DBG("Cannot setup the compound renderer");
		_eon_stack_cleanup(r);
		return EINA_FALSE;
	}

	thiz->fill_func = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill_func) return EINA_FALSE;
	*fill = _stack_draw;

	//printf("end setting up the stack %p\n", r);
	return EINA_TRUE;
}

/* TODO this code might be the same among every layout */
static void _eon_stack_boundings(Enesim_Renderer *r, Enesim_Rectangle *rect)
{
	Eon_Stack *thiz;
	double w, h;

	thiz = _eon_stack_get(r);
	if (!thiz) return;

	eon_layout_actual_size_get(r, &w, &h);
	rect->x = 0;
	rect->y = 0;
	rect->w = w;
	rect->h = h;
	//printf("stack %p boundings %g %g\n", r, w, h);
}
#endif

static void _layout_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	Theme_Basic_Layout *thiz;
	Enesim_Renderer *rchild;
	/* check the type of the child, if it is an eon_theme_element
	 * we can move it by x, y
	 * if it is not, then we only support renderers that have the
	 * translation flag, if not then we just skip this child and
	 * give a warning
	 */
	thiz = _layout_get(r);
	rchild = ender_element_renderer_get(child);
	enesim_renderer_compound_layer_add(thiz->compound, rchild);
	//printf("child %p added to stack %p\n", rchild, r);
}

static void _layout_child_remove(Enesim_Renderer *r, Ender_Element *child)
{
	Theme_Basic_Layout *thiz;
	Enesim_Renderer *rchild;

	thiz = _layout_get(r);
	rchild = ender_element_renderer_get(child);
	enesim_renderer_compound_layer_remove(thiz->compound, rchild);
	//printf("child %p removed from stack %p\n", rchild, r);
}

static double _layout_min_width_get(Enesim_Renderer *r)
{
	return 0;
}

static double _layout_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _layout_min_height_get(Enesim_Renderer *r)
{
	return 0;
}

static double _layout_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eina_Bool _layout_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Theme_Basic_Layout *thiz;
	double ox;
	double oy;

	thiz = _layout_get(r);

	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);
	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("compound cannot setup yet\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _layout_draw;

	return EINA_TRUE;
}

static void _layout_cleanup(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	/* cleanup common properties */
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _layout_free(Enesim_Renderer *r)
{
	Theme_Basic_Layout *thiz;

	thiz = _layout_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static Eon_Theme_Layout_Descriptor _descriptor = {
	.child_add = _layout_child_add,
	.child_remove = _layout_child_remove,
	.max_width_get = _layout_max_width_get,
	.max_height_get = _layout_max_height_get,
	.min_width_get = _layout_min_width_get,
	.min_height_get = _layout_min_height_get,
	.sw_setup = _layout_setup,
	.sw_cleanup = _layout_cleanup,
	.free = _layout_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_layout_new(void)
{
	Enesim_Renderer *r;
	Theme_Basic_Layout *thiz;
#if 0
	const int color[] = { 0xffffffff, 0xff00ff00, 0x88008888 };
	static int i = 0;
#endif

	thiz = calloc(1, sizeof(Theme_Basic_Layout));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_background_new();
	if (!r) goto background_err;
	enesim_renderer_compound_layer_add(thiz->compound, r);
	enesim_renderer_rop_set(r, ENESIM_FILL);
#if 0
	enesim_renderer_background_color_set(r, color[i++ % (sizeof(color) / sizeof(int))]);
#else
	enesim_renderer_background_color_set(r, 0xffffffff);
#endif
	thiz->background = r;

	r = eon_theme_layout_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->background);
background_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}
