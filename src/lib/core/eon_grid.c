/* EON - Grid and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
#include "eon_widget.h"
#include "eon_bin.h"
#include "eon_grid.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"

#include "eon_private_keyboard_proxy.h"
#include "eon_private_keyboard_proxy_navigation.h"

#include "eon_private_layout.h"
#include "eon_private_layout_grid.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_grid_log

static int _eon_grid_log = -1;

static Ender_Property *EON_GRID_NUM_COLUMNS;

typedef struct _Eon_Grid_Child
{
	Ender_Element *ender;
} Eon_Grid_Child;

typedef struct _Eon_Grid
{
	/* properties */
	int num_columns;
	/* private */
	Eon_Input_Proxy *proxy;
	/* container related data */
	Eina_List *children;
	/* layout related data */
	int rows;
	int columns;
	Eon_Size min_size;
	int child_count;
} Eon_Grid;

static inline Eon_Grid * _eon_grid_get(Eon_Element *ee)
{
	Eon_Grid *thiz;

	thiz = eon_container_data_get(ee);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                       The Eon Input State interface                        *
 *----------------------------------------------------------------------------*/
/* FIXME we need to decide how are we going to get find an element on the scene
 * so far we were intersecting the input coordinates with the object layout
 * geometry, for things like comboboxes we might need to intersect against
 * the displayed geometry, given that a combobox display somehting outside
 * the layout tree but the user should interact with it normally
 */
static Ender_Element * _eon_grid_state_element_get(Ender_Element *e, double x, double y)
{
	Eon_Grid *thiz;
	Eon_Grid_Child *ech;
	Ender_Element *child = NULL;
	Eon_Element *ee;
	Eina_List *l;

	ee = ender_element_object_get(e);
	thiz = _eon_grid_get(ee);

	EINA_LIST_FOREACH (thiz->children, l, ech)
	{
		Eon_Geometry g;
		Eon_Element *child_e;

		child_e = ender_element_object_get(ech->ender);
		eon_element_geometry_get(child_e, &g);
		if ((x < g.x + g.width) && (x >= g.x) && (y < g.y + g.height) && (y >= g.y))
			child = ech->ender;
	}
	printf("grid returning %p at %g %g\n", child, x, y);

	return child;
}

static Ender_Element * _eon_grid_state_element_next(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Ender_Element * _eon_grid_state_element_prev(Ender_Element *e,
		Ender_Element *curr)
{
	return NULL;
}

static Eon_Input_State_Descriptor _grid_proxy_descriptor = {
	/* .element_get 	= */ _eon_grid_state_element_get,
	/* .element_next 	= */ _eon_grid_state_element_next,
	/* .element_prev 	= */ _eon_grid_state_element_prev,
};
/*----------------------------------------------------------------------------*
 *                    The Keyboard Proxy Navigation interface                 *
 *----------------------------------------------------------------------------*/
static Ender_Element * _grid_navigation_tab(void *data,
		Ender_Element *current)
{
	Eon_Grid *thiz = data;
	Eon_Grid_Child *child;
	Eina_List *l;

	printf("grid tab %p\n", current);
	if (!current)
	{
		child = eina_list_data_get(thiz->children);
		if (!child) return NULL;

		return child->ender;
	}
	else
	{
		/* TODO We need to implement the siblings */
		EINA_LIST_FOREACH(thiz->children, l, child)
		{
			if (child->ender == current)
			{
				l = eina_list_next(l);
				if (!l) return NULL;
				child = eina_list_data_get(l);
				return child->ender;
			}
		}
		return NULL;
	}
}

static Ender_Element * _grid_navigation_reverse_tab(void *data,
		Ender_Element *current)
{
	Eon_Grid *thiz = data;
	Eon_Grid_Child *child;
	Eina_List *l;

	if (!current)
	{
		l = eina_list_last(thiz->children);
		if (!l) return NULL;

		child = eina_list_data_get(l);
		return child->ender;
	}
	else
	{
		/* TODO We need to implement the siblings */
		EINA_LIST_FOREACH(thiz->children, l, child)
		{
			if (child->ender == current)
			{
				l = eina_list_prev(l);
				if (!l) return NULL;

				child = eina_list_data_get(l);
				return child->ender;
			}
		}
		return NULL;
	}
}

static Eon_Keyboard_Proxy_Navigation_Descriptor _grid_navigation_descriptor = {
	/* .tab 	= */ _grid_navigation_tab,
	/* .reverse_tab = */ _grid_navigation_reverse_tab,
	/* .up 		= */ NULL,
	/* .down 	= */ NULL,
	/* .left 	= */ NULL,
	/* .right 	= */ NULL
};
/*----------------------------------------------------------------------------*
 *                        The main layout descriptors                         *
 *----------------------------------------------------------------------------*/
static void _grid_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Grid_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_geometry_set(e, g);
}

static void _grid_layout_rows_columns_set(void *ref, int rows, int columns)
{
	Eon_Grid *thiz = ref;
	thiz->rows = rows;
	thiz->columns = columns;
}

static void _grid_layout_rows_columns_get(void *ref, int *rows, int *columns)
{
	Eon_Grid *thiz = ref;
	*rows = thiz->rows;
	*columns = thiz->columns;
}

static int _grid_layout_num_columns_get(void *ref)
{
	Eon_Grid *thiz = ref;
	return thiz->num_columns;
}

static void _grid_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Grid_Child *child_thiz = child;
	Eon_Element *e;

	e = ender_element_object_get(child_thiz->ender);
	eon_element_hints_get(e, hints);
}

static void _grid_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Grid *thiz = ref;
	Eon_Grid_Child *thiz_child;
	Eina_List *l;

	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ref, thiz_child, data);
	}
}

static void _grid_layout_min_size_set(void *ref, Eon_Size *min)
{
	Eon_Grid *thiz = ref;

	thiz->min_size = *min;
}

static void _grid_layout_min_size_get(void *ref, Eon_Size *min)
{
	Eon_Grid *thiz = ref;

	*min = thiz->min_size;
}

static void _grid_layout_child_count_set(void *ref, int count)
{
	Eon_Grid *thiz = ref;

	thiz->child_count = count;
}

static int _grid_layout_child_count_get(void *ref)
{
	Eon_Grid *thiz = ref;

	return thiz->child_count;
}

static Eon_Layout_Grid_Descriptor _grid_layout = {
	/* .min_size_get 	= */ _grid_layout_min_size_get,
	/* .min_size_set 	= */ _grid_layout_min_size_set,
	/* .child_count_get 	= */ _grid_layout_child_count_get,
	/* .child_count_set 	= */ _grid_layout_child_count_set,
	/* .rows_columns_get 	= */ _grid_layout_rows_columns_get,
	/* .rows_columns_set 	= */ _grid_layout_rows_columns_set,
	/* .num_columns_get 	= */ _grid_layout_num_columns_get,
	/* .child_foreach 	= */ _grid_layout_child_foreach,
	/* .child_hints_get 	= */ _grid_layout_child_hints_get,
	/* .child_geometry_set 	= */ _grid_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_grid_free(Eon_Element *ee)
{
	Eon_Grid *thiz;

	thiz = _eon_grid_get(ee);
	free(thiz);
}

static void _eon_grid_initialize(Ender_Element *e)
{
	Eon_Grid *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_grid_get(ee);
	thiz->proxy = eon_input_proxy_new(e, &_grid_proxy_descriptor);
}

static Eina_Bool _eon_grid_child_add(Eon_Element *ee, Ender_Element *child)
{
	Eon_Grid *thiz;
	Eon_Grid_Child *thiz_child;

	thiz = _eon_grid_get(ee);
	thiz_child = calloc(1, sizeof(Eon_Grid_Child));
	thiz_child->ender = child;
	thiz->children = eina_list_append(thiz->children, thiz_child);

	return EINA_TRUE;
}

static Eina_Bool _eon_grid_child_remove(Eon_Element *ee, Ender_Element *child)
{
	Eon_Grid *thiz;
	Eon_Grid_Child *thiz_child;
	Eina_List *l, *l_next;
	Eina_Bool found = EINA_FALSE;

	thiz = _eon_grid_get(ee);
	EINA_LIST_FOREACH_SAFE(thiz->children, l, l_next, thiz_child)
	{
		if (thiz_child->ender == child)
		{
			thiz->children = eina_list_remove_list(thiz->children, l);
			found = EINA_TRUE;
			break;
		}
	}
	if (found)
	{
		return EINA_TRUE;
	}
	return EINA_FALSE;
}

static void _eon_grid_child_foreach(Eon_Element *ee, Eon_Container_Child_Foreach_Cb cb, void *user_data)
{
	Eon_Grid *thiz;
	Eon_Grid_Child *thiz_child;
	Eina_List *l;

	thiz = _eon_grid_get(ee);
	EINA_LIST_FOREACH(thiz->children, l, thiz_child)
	{
		cb(ee, thiz_child->ender, user_data);
	}
}

static void _eon_grid_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Grid *thiz;

	thiz = _eon_grid_get(e);
	eon_layout_geometry_set(&eon_layout_grid, &_grid_layout, thiz, g);
}

static void _eon_grid_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_Grid *thiz;

	thiz = _eon_grid_get(e);
	eon_layout_hints_get(&eon_layout_grid, &_grid_layout, thiz, hints);
}

static void _eon_grid_num_columns_set(Eon_Element *e, int num_columns)
{
	Eon_Grid *thiz;

	thiz = _eon_grid_get(e);
	thiz->num_columns = num_columns;
	eon_element_inform_change(e);
}

static void _eon_grid_num_columns_get(Eon_Element *e, int *num_columns)
{
	Eon_Grid *thiz;

	thiz = _eon_grid_get(e);
	if (num_columns) *num_columns = thiz->num_columns;
}


static Eon_Container_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_grid_initialize,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .geometry_set 	= */ _eon_grid_geometry_set,
	/* .free	 	= */ _eon_grid_free,
	/* .name 		= */ "grid",
	/* .hints_get	 	= */ _eon_grid_hints_get,
	/* .child_add 		= */ _eon_grid_child_add,
	/* .child_remove 	= */ _eon_grid_child_remove,
	/* .child_foreach 	= */ _eon_grid_child_foreach,
	/* .child_at 		= */ NULL,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_grid_new(void)
{
	Eon_Grid *thiz;
	Eon_Element *ee;
	Eon_Keyboard_Proxy *kp;
	Eon_Theme_Instance *theme;

	theme = eon_theme_instance_new("grid", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Grid));
	if (!thiz) return NULL;

	ee = eon_container_new(theme, &_descriptor, thiz);
	if (!ee) goto base_err;

	kp = eon_keyboard_proxy_navigation_new(&_grid_navigation_descriptor, thiz);
	eon_element_keyboard_proxy_set(ee, kp);

	return ee;

base_err:
	free(thiz);
	return NULL;
}

#define _eon_grid_delete NULL
#include "eon_generated_grid.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_grid_init(void)
{
	_eon_grid_log = eina_log_domain_register("eon_grid", EON_LOG_COLOR_DEFAULT);
	if (_eon_grid_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_grid_init();
}

void eon_grid_shutdown(void)
{
	if (_eon_grid_log < 0)
		return;
	_eon_grid_shutdown();
	eina_log_domain_unregister(_eon_grid_log);
	_eon_grid_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_grid_new(void)
{
	return EON_ELEMENT_NEW("grid");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_grid_num_columns_set(Ender_Element *e, int num_columns)
{
	ender_element_property_value_set(e, EON_GRID_NUM_COLUMNS, num_columns, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_grid_num_columns_get(Ender_Element *e, int *num_columns)
{
	Eon_Grid *thiz;
	Eon_Element *ee;

	if (!num_columns) return;
	ee = ender_element_object_get(e);
	thiz = _eon_grid_get(ee);
	*num_columns = thiz->num_columns;
}


