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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_container.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"
/* TODO Whenever we decide how to add callbacks to eon_elements that must be called
 * at the beginning, like a mouse_in,out,down for a button, etc, every
 * laout should propagate the look_at, so the child is actually returned
 * not the layout
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_CONTAINER_MAGIC 0xe0400002
#define EON_CONTAINER_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_CONTAINER_MAGIC)
#define EON_CONTAINER_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_CONTAINER_MAGIC, ret)

static Ender_Property *EON_CONTAINER_CHILD;

typedef struct _Eon_Container_Descriptor_Internal
{
	Eon_Container_Child_Add child_add;
	Eon_Container_Child_Remove child_remove;
	Eon_Container_Child_Foreach child_foreach;
	Eon_Container_Child_At child_at;
	Eon_Element_Initialize initialize;
	Eon_Element_Backend_Added backend_added;
	Eon_Element_Backend_Removed backend_removed;
	Eon_Element_Setup setup;
	Eon_Element_Free free;
} Eon_Container_Descriptor_Internal;

typedef struct _Eon_Container
{
	EINA_MAGIC;
	/* the interface */
	Eon_Container_Descriptor_Internal descriptor;
	/* internal */
	Ender_Element *e;
	unsigned int width;
	unsigned int height;
	void *data;
} Eon_Container;

static void _eon_container_child_remove(Eon_Element *ee, Ender_Element *child);

static inline Eon_Container * _eon_container_get(Eon_Element *ee)
{
	Eon_Container *thiz;

	thiz = eon_widget_data_get(ee);
	EON_CONTAINER_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
	
static Eina_Bool _child_clear_cb(Eon_Element *e, Ender_Element *child, void *data)
{
	_eon_container_child_remove(e, child);
	return EINA_TRUE;
}

static Eina_Bool _child_backend_set_cb(Eon_Element *e, Ender_Element *child, void *data)
{
	Eon_Backend *backend = data;
	Eon_Element *ee;

	ee = ender_element_object_get(child);
	eon_element_backend_set(ee, backend);
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                         The Eon Widget interface                           *
 *----------------------------------------------------------------------------*/
static void _eon_container_initialize(Ender_Element *e)
{
	Eon_Container *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	thiz->e = e;
	if (thiz->descriptor.initialize)
		thiz->descriptor.initialize(e);
}

static void _eon_container_backend_added(Eon_Element *e, Eon_Backend *b)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(e);
	/* iterate over the childs and set the backend too */	
	eon_container_internal_child_foreach(e, _child_backend_set_cb, b);
}

static void _eon_container_backend_removed(Eon_Element *e, Eon_Backend *b)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(e);
	/* iterate over the childs and remove the backend too */	
	eon_container_internal_child_foreach(e, _child_backend_set_cb, NULL);
}

/* FIXME We must delete this one */
static Eina_Bool _eon_container_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Container *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_container_get(ee);
	if (thiz->descriptor.setup)
		return thiz->descriptor.setup(e, state, s, err);
	return EINA_TRUE;
}

static void _eon_container_free(Eon_Element *ee)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	if (thiz->descriptor.free)
		thiz->descriptor.free(ee);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_container_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	if (thiz->descriptor.child_remove(ee, child))
	{
		Eon_Theme_Instance *theme;
		Enesim_Renderer *child_r;

		/* remove it from the theme */
		theme = eon_widget_theme_instance_get(ee);
		child_r = eon_element_renderer_get(child);
		eon_theme_instance_property_remove(theme, "child", child_r, NULL);
		eon_element_parent_set(child, NULL, NULL, NULL);

		eon_element_inform_change(ee);
	}
}

static void _eon_container_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Container *thiz;
	Ender_Element *curr_parent;

	thiz = _eon_container_get(ee);
	if (!eon_is_element(child))
		return;

	curr_parent = ender_element_parent_get(child);
	/* TODO check if the parent is the same */
	if (curr_parent)
	{
		/* FIXME this is wrong, we should remove from the theme the child_rr */
		printf("warning, first remove from its old parent\n");
	}
	if (thiz->descriptor.child_add(ee, child))
	{
		Eon_Theme_Instance *theme;
		Enesim_Renderer *child_r;

		/* add it to the theme */
		theme = eon_widget_theme_instance_get(ee);
		child_r = eon_element_renderer_get(child);
		eon_theme_instance_property_add(theme, "child", child_r, NULL);
		eon_element_parent_set(child, thiz->e, NULL, NULL);

		eon_element_inform_change(ee);
	}
}

static void _eon_container_child_set(Eon_Element *ee, Eina_List *childs)
{
	Eina_List *l;
	Ender_Element *child;

	EINA_LIST_FOREACH(childs, l, child)
	{
		_eon_container_child_add(ee, child);
	}
}


static void _eon_container_child_clear(Eon_Element *e)
{
	eon_container_internal_child_foreach(e, _child_clear_cb, NULL);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Element * eon_container_new(Eon_Theme_Instance *theme,
		Eon_Container_Descriptor *descriptor, void *data)
{
	Eon_Container *thiz;
	Eon_Widget_Descriptor pdescriptor = { 0 };
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Container));
	EINA_MAGIC_SET(thiz, EON_CONTAINER_MAGIC);
	thiz->data = data;
	thiz->descriptor.initialize = descriptor->initialize;
	thiz->descriptor.backend_added = descriptor->backend_added;
	thiz->descriptor.backend_removed = descriptor->backend_removed;
	thiz->descriptor.free = descriptor->free;
	thiz->descriptor.child_add = descriptor->child_add;
	thiz->descriptor.child_remove = descriptor->child_remove;
	thiz->descriptor.child_foreach = descriptor->child_foreach;
	thiz->descriptor.child_at = descriptor->child_at;
	thiz->descriptor.setup = descriptor->setup;

	/* element interface */
	pdescriptor.initialize = _eon_container_initialize;
	pdescriptor.backend_added = _eon_container_backend_added;
	pdescriptor.backend_removed = _eon_container_backend_removed;
	pdescriptor.free = _eon_container_free;
	pdescriptor.name = descriptor->name;
	pdescriptor.setup = _eon_container_setup;
	pdescriptor.needs_setup = descriptor->needs_setup;
	pdescriptor.geometry_set = descriptor->geometry_set;
	/* widget interface */
	pdescriptor.hints_get = descriptor->hints_get;

	ee = eon_widget_new(theme, &pdescriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_container_data_get(Eon_Element *ee)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(ee);
	return thiz->data;
}

void eon_container_internal_child_foreach(Eon_Element *e, Eon_Container_Child_Foreach_Cb cb, void *user_data)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(e);
	if (thiz->descriptor.child_foreach)
		thiz->descriptor.child_foreach(e, cb, user_data);
}

#define _eon_container_delete NULL
#define _eon_container_child_get NULL
#include "eon_generated_container.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_container(Ender_Element *e)
{
	Eon_Container *thiz;
	Eon_Element *ee;

	if (!eon_is_widget(e)) return EINA_FALSE;
	ee = ender_element_object_get(e);
	thiz = eon_widget_data_get(ee);
	if (!EINA_MAGIC_CHECK(thiz, EON_CONTAINER_MAGIC)) {
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool eon_container_is_topmost(Ender_Element *e)
{
	Eina_Bool ret = EINA_FALSE;
#if 0
	Ender_Element *ender, *parent;
	if (!eon_is_container(ee)) goto end;
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
EAPI void eon_container_child_remove(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_remove(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_child_add(Ender_Element *e, Ender_Element *child)
{
	ender_element_value_add(e, "child", child, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_child_clear(Ender_Element *e)
{
	ender_element_value_clear(e, "child");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_child_foreach(Ender_Element *e, Eon_Container_Child_Foreach_Cb cb, void *user_data)
{
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	eon_container_internal_child_foreach(ee, cb, user_data);
}
