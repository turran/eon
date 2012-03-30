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
#include "Eon_Theme.h"
#include "eon_theme_private.h"
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

static Ender_Property *EON_LAYOUT_CHILD;

typedef struct _Eon_Layout
{
	EINA_MAGIC;
	/* the interface */
	Eon_Layout_Child_Add child_add;
	Eon_Layout_Child_Remove child_remove;
	Eon_Layout_Child_Clear child_clear;
	Eon_Layout_Child_At child_at;
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Eon_Element_Free free;
	/* internal */
	Eon_Input_Proxy *proxy;
	Ender_Element *e;
	unsigned int width;
	unsigned int height;
	void *data;
} Eon_Layout;

static inline Eon_Layout * _eon_layout_get(Eon_Element *ee)
{
	Eon_Layout *thiz;

	thiz = eon_widget_data_get(ee);
	EON_LAYOUT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       The Eon Input State interface                        *
 *----------------------------------------------------------------------------*/
static Ender_Element * _eon_layout_state_element_get(Ender_Element *e,
		double x, double y)
{
	Eon_Layout *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_layout_get(ee);
	if (thiz->child_at)
		return thiz->child_at(e, x, y);
	return NULL;
}

static Ender_Element * _eon_layout_state_element_next(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Ender_Element * _eon_layout_state_element_prev(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Eon_Input_State_Descriptor _layout_proxy_descriptor = {
	/* .element_get 	= */ _eon_layout_state_element_get,
	/* .element_next 	= */ _eon_layout_state_element_next,
	/* .element_prev 	= */ _eon_layout_state_element_prev,
};
/*----------------------------------------------------------------------------*
 *                         The Eon Widget interface                           *
 *----------------------------------------------------------------------------*/

static void _eon_layout_initialize(Ender_Element *e)
{
	Eon_Layout *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_layout_get(ee);
	thiz->proxy = eon_input_proxy_new(e, &_layout_proxy_descriptor);
	thiz->e = e;
	if (thiz->initialize)
		thiz->initialize(e);
}

/* FIXME We must delete this one */
static Eina_Bool _eon_layout_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Layout *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_layout_get(ee);
	if (thiz->setup)
		return thiz->setup(e, state, s, err);
	return EINA_TRUE;
}

static void _eon_layout_free(Eon_Element *ee)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(ee);
	if (thiz->free)
		thiz->free(ee);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_layout_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(ee);
	if (!thiz->child_remove(ee, child))
	{
		eon_element_parent_set(child, NULL, NULL, NULL);
	}
}

static void _eon_layout_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Layout *thiz;
	Ender_Element *curr_parent;

	thiz = _eon_layout_get(ee);
	if (!eon_is_element(child))
		return;

	curr_parent = ender_element_parent_get(child);
	/* TODO check if the parent is the same */
	if (curr_parent)
	{
		/* FIXME this is wrong, we should remove from the theme the child_rr */
		printf("warning, first remove from its old parent\n");
	}
	if (thiz->child_add(ee, child))
	{
		eon_element_parent_set(child, thiz->e, NULL, NULL);
	}
}

static void _eon_layout_child_set(Eon_Element *ee, Eina_List *childs)
{
	Eina_List *l;
	Ender_Element *child;

	EINA_LIST_FOREACH(childs, l, child)
	{
		_eon_layout_child_add(ee, child);
	}
}


static void _eon_layout_child_clear(Eon_Element *ee)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(ee);

	thiz->child_clear(ee);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Element * eon_layout_new(Eon_Layout_Descriptor *descriptor,
		void *data)
{
	Eon_Layout *thiz;
	Eon_Widget_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Layout));
	EINA_MAGIC_SET(thiz, EON_LAYOUT_MAGIC);
	thiz->data = data;
	thiz->initialize = descriptor->initialize;
	thiz->free = descriptor->free;
	thiz->child_add = descriptor->child_add;
	thiz->child_remove = descriptor->child_remove;
	thiz->child_clear = descriptor->child_clear;
	thiz->child_at = descriptor->child_at;
	thiz->setup = descriptor->setup;

	pdescriptor.initialize = _eon_layout_initialize;
	pdescriptor.free = _eon_layout_free;
	pdescriptor.name = descriptor->name;
	pdescriptor.max_width_get = descriptor->max_width_get;
	pdescriptor.max_height_get = descriptor->max_height_get;
	pdescriptor.min_width_get = descriptor->min_width_get;
	pdescriptor.min_height_get = descriptor->min_height_get;
	pdescriptor.preferred_width_get = descriptor->preferred_width_get;
	pdescriptor.preferred_height_get = descriptor->preferred_height_get;
	pdescriptor.setup = _eon_layout_setup;
	pdescriptor.needs_setup = descriptor->needs_setup;

	ee = eon_widget_new(&pdescriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_layout_data_get(Eon_Element *ee)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(ee);
	return thiz->data;
}

#define _eon_layout_child_get NULL
#include "eon_generated_layout.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_layout(Ender_Element *e)
{
	Eon_Layout *thiz;
	Eon_Element *ee;

	if (!eon_is_widget(e)) return EINA_FALSE;
	ee = ender_element_object_get(e);
	thiz = eon_widget_data_get(ee);
	if (!EINA_MAGIC_CHECK(thiz, EON_LAYOUT_MAGIC)) {
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool eon_layout_is_topmost(Ender_Element *e)
{
	Eina_Bool ret = EINA_FALSE;
#if 0
	Ender_Element *ender, *parent;
	if (!eon_is_layout(ee)) goto end;
	ender = ender_element_renderer_from(ee);
	if (!ender) goto end;
	parent = ender_element_parent_get(ender);
	if (parent) goto end;
	ret = EINA_TRUE;

end:
	//printf("layout is topmost = %d\n", ret);
#endif
	return ret;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_remove(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_remove(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_add(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_add(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_layout_child_clear(Ender_Element *e)
{
	ender_element_value_clear(e, "child");
}
