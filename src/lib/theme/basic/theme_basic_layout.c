/* EON - Canvas and Toolkit library
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
#include "Eon_Basic.h"
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Theme_Basic_Layout
{
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
static void _layout_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	/* check the type of the child, if it is an eon_theme_element
	 * we can move it by x, y
	 * if it is not, then we only support renderers that have the
	 * translation flag, if not then we just skip this child and
	 * give a warning
	 */
}

static void _layout_child_remove(Enesim_Renderer *r, Ender_Element *child)
{

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

	thiz = _layout_get(r);

	if (!enesim_renderer_sw_setup(thiz->compound))
	{
		printf("not available to setup yet\n");
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

	thiz = calloc(1, sizeof(Theme_Basic_Layout));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = eon_theme_layout_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}
