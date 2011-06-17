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
#include "eon_private.h"
/* TODO Whenever we decide how to add callbacks to eon_elements that must be called
 * at the beginning, like a mouse_in,out,down for a button, etc, every
 * laout should propagate the look_at, so the child is actually returned
 * not the layout
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LAYOUT_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_LAYOUT_MAGIC)
#define EON_LAYOUT_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_LAYOUT_MAGIC, ret)

typedef struct _Eon_Layout
{
	EINA_MAGIC;
	Eon_Layout_Child_Add child_add;
	Eon_Layout_Child_Remove child_remove;
	Eon_Layout_Child_Clear child_clear;
	Eon_Layout_Child_At child_at;
	Eon_Element_Initialize initialize;
	Enesim_Renderer_Delete free;
	Eina_Tiler *tiler;
	Eina_Array *obscure;
	Eina_Array *damage;
	unsigned int width;
	unsigned int height;
	void *data;
} Eon_Layout;

static inline Eon_Layout * _eon_layout_get(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = eon_element_data_get(r);
	EON_LAYOUT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_layout_initialize(Ender_Element *ender)
{
	Eon_Layout *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(ender);
	thiz = _eon_layout_get(r);
	if (thiz->initialize)
		thiz->initialize(ender);
}

static void _eon_layout_free(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

static void _child_changed(Ender_Element *child, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;

	thiz = data;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *descriptor,
		void *data)
{
	Eon_Layout *thiz;
	Eon_Element_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Layout));
	EINA_MAGIC_SET(thiz, EON_LAYOUT_MAGIC);
	thiz->data = data;
	thiz->initialize = descriptor->initialize;
	thiz->free = descriptor->free;
	thiz->child_add = descriptor->child_add;
	thiz->child_remove = descriptor->child_remove;
	thiz->child_clear = descriptor->child_clear;
	thiz->child_at = descriptor->child_at;

	pdescriptor.initialize = _eon_layout_initialize;
	pdescriptor.free = _eon_layout_free;
	pdescriptor.name = descriptor->name;

	r = eon_element_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_layout_data_get(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	return thiz->data;
}

void eon_layout_actual_size_get(Enesim_Renderer *r, double *width, double *height)
{
	/* whenever we are the topmost, the user must have set the
	 * the width and height of the object
	 */
	if (eon_layout_is_topmost(r))
	{
		eon_element_width_get(r, width);
		eon_element_height_get(r, height);
	}
	else
	{
		eon_element_actual_width_get(r, width);
		eon_element_actual_height_get(r, height);
	}
}

void eon_layout_actual_width_get(Enesim_Renderer *r, double *width)
{
	if (eon_layout_is_topmost(r))
	{
		eon_element_width_get(r, width);
	}
	else
	{
		eon_element_actual_width_get(r, width);
	}
}

void eon_layout_actual_height_get(Enesim_Renderer *r, double *height)
{
	if (eon_layout_is_topmost(r))
	{
		eon_element_height_get(r, height);
	}
	else
	{
		eon_element_actual_height_get(r, height);
	}
}

Eina_Bool eon_layout_state_setup(Enesim_Renderer *r, unsigned int width,
		unsigned int height)
{
#if 0
	Eon_Layout *thiz;
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
#endif
}

void eon_layout_stack_cleanup(Enesim_Renderer *r)
{
#if 0
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	/* remove every dirty rectangle? */
	if (thiz->tiler) eina_tiler_clear(thiz->tiler);
#endif
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_layout(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	if (!eon_is_element(r)) return EINA_FALSE;
	thiz = eon_element_data_get(r);
	/* FIXME looks like compiling with flag  > -O0 gives
	 * a bad behaviour
	 */
	//printf("ok %08x\n", thiz->__magic);
	if (!EINA_MAGIC_CHECK(thiz, EON_LAYOUT_MAGIC)) {
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void eon_layout_redraw_get(Enesim_Renderer *r, Eina_List **redraws)
{
	/* get every damage from the tiler */
	/* remove the obscures */
	/* return that list */
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void eon_layout_obscure_add(Enesim_Renderer *r, Eina_Rectangle *obscure)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void eon_layout_damage_add(Enesim_Renderer *r, Eina_Rectangle *damage)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	/* add the rectangle to the tiler */
}

/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool eon_layout_is_topmost(Enesim_Renderer *r)
{
	Ender_Element *ender, *parent;
	Eina_Bool ret = EINA_FALSE;

	if (!eon_is_layout(r)) goto end;
	ender = ender_element_renderer_from(r);
	if (!ender) goto end;
	parent = ender_element_parent_get(ender);
	if (parent) goto end;
	ret = EINA_TRUE;

end:
	//printf("layout is topmost = %d\n", ret);
	return ret;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->child_remove(r, child);
	ender_event_listener_remove(child, "Mutation", _child_changed);
	eon_element_property_remove(r, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Layout *thiz;
	Ender_Element *curr_parent;

	thiz = _eon_layout_get(r);
	curr_parent = ender_element_parent_get(child);
	if (curr_parent)
	{
		ender_element_value_remove(curr_parent, "child", child, NULL);
	}
	eon_element_property_add(r, "child", child, NULL);
	thiz->child_add(r, child);
	ender_event_listener_add(child, "Mutation", _child_changed, thiz);
	/* TODO whenever a child is appended to a layout
	 * call the init of it (on init you should register
	 * the callbacks, etc) just in case the ender
	 * is of type element (or widget?)
	 */
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_clear(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->child_clear(r);
	//ender_event_listener_remove(child, "Mutation", _child_changed);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_layout_child_get_at_coord(Enesim_Renderer *r, double x, double y)
{
	Eon_Layout *thiz;
	Ender_Element *child;

	thiz = _eon_layout_get(r);
	if (!thiz) return NULL;

	printf("%p\n", thiz);
	if (!thiz->child_at)
		return NULL;
	return thiz->child_at(r, x, y);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_layout_child_get_at_destination_coord(Enesim_Renderer *r,
		unsigned int x, unsigned int y)
{
	Eon_Layout *thiz;
	Ender_Element *child;
	Enesim_Matrix matrix;
	Enesim_Matrix_Type mtype;
	double rx, ry;

	thiz = _eon_layout_get(r);
	/* first from the destination coordinates get the origin coordinates */
	enesim_renderer_transformation_get(r, &matrix);
	mtype = enesim_matrix_type_get(&matrix);
	if (mtype != ENESIM_MATRIX_IDENTITY)
	{
		enesim_matrix_point_transform(&matrix, x, y, &rx, &ry);
	}
	else
	{
		double ox, oy;

		enesim_renderer_origin_get(r, &ox, &oy);
		rx = x - ox;
		ry = y - oy;
		printf("changing %p %d %d - %g %g\n", r, x, y, rx, ry);
	}
	return eon_layout_child_get_at_coord(r, rx, ry);
}
