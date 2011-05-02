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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LAYOUT_MAGIC 0xe0400002
#define EON_LAYOUT_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_WIDGET_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_WIDGET_MAGIC);\
	} while(0)

#define EON_LAYOUT_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_LAYOUT_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_LAYOUT_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Layout
{
	EINA_MAGIC;
	Eon_Layout_Descriptor *descriptor;
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

static void _child_changed(Ender *child, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;

	thiz = data;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *ldescriptor,
		Eon_Element_Descriptor *edescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Layout *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Layout));
	EINA_MAGIC_SET(thiz, EON_LAYOUT_MAGIC);
	thiz->data = data;
	thiz->descriptor = ldescriptor;

	r = eon_element_new(edescriptor, descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
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

	thiz = eon_element_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_LAYOUT_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void * eon_layout_data_get(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	return thiz->data;
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
	Ender *ender, *parent;
	Eina_Bool ret = EINA_FALSE;

	if (!eon_is_layout(r)) goto end;
	ender = ender_element_renderer_from(r);
	if (!ender) goto end;
	parent = ender_element_parent_get(ender);
	if (parent) goto end;
	ret = EINA_TRUE;

end:
	printf("layout is topmost = %d\n", ret);
	return ret;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender *child)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->descriptor->child_remove(r, child);
	ender_event_listener_remove(child, "Mutation", _child_changed);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender *child)
{
	Eon_Layout *thiz;
	Ender *curr_parent;

	thiz = _eon_layout_get(r);
	curr_parent = ender_element_parent_get(child);
	if (curr_parent)
	{
		ender_element_value_remove(curr_parent, "child", child, NULL);
	}
	thiz->descriptor->child_add(r, child);
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
EAPI Ender * eon_layout_child_get_at_coord(Enesim_Renderer *r, unsigned int x, unsigned int y)
{
	Eon_Layout *thiz;
	Enesim_Matrix matrix;
	Enesim_Matrix_Type mtype;
	double rx, ry;

	thiz = _eon_layout_get(r);
	enesim_renderer_transformation_get(r, &matrix);
	enesim_matrix_type_get(&matrix);
	if (mtype != ENESIM_MATRIX_IDENTITY)
	{
		enesim_matrix_point_transform(&matrix, x, y, &rx, &ry);
	}
	else
	{
		rx = x;
		ry = y;
	}
	if (!thiz->descriptor->child_at)
		return NULL;
	return thiz->descriptor->child_at(r, rx, ry);
}

