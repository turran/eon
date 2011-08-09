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
	Eina_Hash *input_states;
} Eon_Layout;

#define _eon_layout_get(r) \
	eon_widget_data_get(r); \
	if (!EINA_MAGIC_CHECK((Eon_Layout *)eon_widget_data_get(r), EON_LAYOUT_MAGIC)) \
		EINA_MAGIC_FAIL((Eon_Layout *)eon_widget_data_get(r), EON_LAYOUT_MAGIC)

#if 0
static inline Eon_Layout * _eon_layout_get(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = eon_element_data_get(r);
	EON_LAYOUT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
#endif

static Eon_Input_State * _eon_layout_input_state_get(Eon_Layout *thiz, Ender_Element *e, Eon_Input *input)
{
	Eon_Input_State *eis;

	if (!thiz->input_states)
		thiz->input_states = eina_hash_pointer_new(NULL);
	eis = eina_hash_find(thiz->input_states, (const void *)&input);
	if (!eis)
	{
		eis = eon_layout_input_state_new(e, input);
		eina_hash_add(thiz->input_states, (const void *)&input, eis);
	}
	return eis;
}

static void _eon_layout_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;
	Eon_Event_Mouse_Down *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);

	eis = _eon_layout_input_state_get(thiz, e, ev->input);
	printf("passing mouse down\n");
	eon_input_state_feed_mouse_down(eis);
}

static void _eon_layout_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;
	Eon_Event_Mouse_Up *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);

	eis = _eon_layout_input_state_get(thiz, e, ev->input);
	printf("passing mouse up\n");
	eon_input_state_feed_mouse_up(eis);
}

static void _eon_layout_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;
	Eon_Event_Mouse_In *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);

	eis = _eon_layout_input_state_get(thiz, e, ev->input);
	printf("passing mouse in\n");
	eon_input_state_feed_mouse_in(eis);
}

static void _eon_layout_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;
	Eon_Event_Mouse_Out *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);

	eis = _eon_layout_input_state_get(thiz, e, ev->input);
	printf("passing mouse out\n");
	eon_input_state_feed_mouse_out(eis);
}

static void _eon_layout_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Layout *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);

	eis = _eon_layout_input_state_get(thiz, e, ev->input);
	printf("passing mouse move\n");
	eon_input_state_feed_mouse_move(eis, 0, 0);
}

static void _eon_layout_initialize(Ender_Element *e)
{
	Eon_Layout *thiz;
	Enesim_Renderer *r;
	int i;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_layout_mouse_move, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN], _eon_layout_mouse_in, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT], _eon_layout_mouse_out, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DOWN], _eon_layout_mouse_down, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_UP], _eon_layout_mouse_up, NULL);
	/* we should register all the callbacks so whenever
	 * something happens on a child layout we propagate
	 */
	if (thiz->initialize)
		thiz->initialize(e);
}

static void _eon_layout_free(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_layout_child_remove(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->child_remove(r, child);
	eon_widget_property_remove(r, "child", child, NULL);
}

static void _eon_layout_child_add(Enesim_Renderer *r, Ender_Element *child)
{
	Eon_Layout *thiz;
	Ender_Element *curr_parent;
	Ender_Element *theme;
	Enesim_Renderer *child_r;

	thiz = _eon_layout_get(r);
	child_r = ender_element_renderer_get(child);
	if (!eon_is_element(child_r))
		return;
	curr_parent = ender_element_parent_get(child);
	if (curr_parent)
	{
		ender_element_value_remove(curr_parent, "child", child, NULL);
	}
	//theme = eon_widget_theme_element_get(child_r);
	//eon_widget_property_add(r, "child", theme, NULL);
	eon_widget_property_add(r, "child", child_r, NULL);
	thiz->child_add(r, child);
}

static void _eon_layout_child_clear(Enesim_Renderer *r)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	thiz->child_clear(r);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Input_State * eon_layout_input_state_new(Ender_Element *e, Eon_Input *input)
{
	Eon_Layout *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_layout_get(r);
	if (thiz->child_at)
		return eon_input_state_new(input, e, thiz->child_at);
	return NULL;
}

Enesim_Renderer * eon_layout_new(Eon_Layout_Descriptor *descriptor,
		void *data)
{
	Eon_Layout *thiz;
	Eon_Widget_Descriptor pdescriptor = { 0 };
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
	pdescriptor.min_width_get = descriptor->min_width_get;
	pdescriptor.min_height_get = descriptor->min_height_get;
	pdescriptor.setup = descriptor->setup;

	r = eon_widget_new(&pdescriptor, thiz);
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

void eon_layout_actual_size_get(Ender_Element *e, double *width, double *height)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	/* whenever we are the topmost, the user must have set the
	 * the width and height of the object
	 */
	if (eon_layout_is_topmost(r))
	{
		eon_element_width_get(e, width);
		eon_element_height_get(e, height);
	}
	else
	{
		eon_element_actual_width_get(e, width);
		eon_element_actual_height_get(e, height);
	}
}

void eon_layout_actual_width_get(Ender_Element *e, double *width)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (eon_layout_is_topmost(r))
	{
		eon_element_width_get(e, width);
	}
	else
	{
		eon_element_actual_width_get(e, width);
	}
}

void eon_layout_actual_height_get(Ender_Element *e, double *height)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (eon_layout_is_topmost(r))
	{
		eon_element_height_get(e, height);
	}
	else
	{
		eon_element_actual_height_get(e, height);
	}
}

void eon_layout_child_real_width_get(Ender_Element *e, Ender_Element *child, double *width)
{
	double set;

	eon_element_width_get(child, &set);
	/* fill the layout size */
	if (set < 0)
	{
		eon_layout_actual_width_get(e, width);
	}
	else
	{
		eon_element_real_width_get(child, width);
	}
}

void eon_layout_child_real_height_get(Ender_Element *e, Ender_Element *child, double *height)
{
	double set;

	eon_element_height_get(child, &set);
	/* fill the layout size */
	if (set < 0)
	{
		eon_layout_actual_height_get(e, height);
	}
	else
	{
		eon_element_real_height_get(child, height);
	}
}

#define _eon_layout_child_get NULL
#define _eon_layout_child_set NULL
#include "eon_generated_layout.c"
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

	if (!eon_is_widget(r)) return EINA_FALSE;
	thiz = eon_widget_data_get(r);
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

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_layout_child_get_at_coord(Enesim_Renderer *r, double x, double y)
{
	Eon_Layout *thiz;

	thiz = _eon_layout_get(r);
	if (!thiz) return NULL;

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
#endif
