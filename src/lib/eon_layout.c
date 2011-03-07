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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *ldescriptor,
		Enesim_Renderer_Descriptor *descriptor,
		void *data)
{
	Eon_Layout *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Layout));
	EINA_MAGIC_SET(thiz, EON_LAYOUT_MAGIC);
	thiz->data = data;
	thiz->descriptor = ldescriptor;

	r = eon_element_new(descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
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
EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender *child)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->descriptor->child_remove(r, child);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender *child)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->descriptor->child_add(r, child);
}
