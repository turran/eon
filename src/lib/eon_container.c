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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Container
{
	/* properties */
	Ender_Element *content;
	/* private */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	Eon_Container_Min_Width_Get min_width_get;
	Eon_Container_Max_Width_Get max_width_get;
	Eon_Container_Min_Height_Get min_height_get;
	Eon_Container_Max_Height_Get max_height_get;
	Eon_Container_Preferred_Width_Get preferred_width_get;
	Eon_Container_Preferred_Height_Get preferred_height_get;
	Eon_Container_Element_At element_at;
	Eina_Bool pass_events;
	void *data;
	Eina_Hash *input_states;
} Eon_Container;

static inline Eon_Container * _eon_container_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}

Ender_Element * _eon_container_element_get(Ender_Element *e, double x, double y)
{
	Eon_Container *thiz;
	Ender_Element *at = NULL;
	Enesim_Renderer *r;
	Enesim_Renderer *content_r;
	double ax, ay;
	Eon_Size size;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	content_r = ender_element_renderer_get(thiz->content);
	eon_element_actual_size_get(content_r, &size);
	eon_element_actual_position_get(content_r, &ax, &ay);

	if (thiz->element_at)
		at = thiz->element_at(e, x, y);
	if (!at)
	{
		if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
			at = thiz->content;
	}
	return at;
}

static Eon_Input_State * _eon_container_input_state_get(Eon_Container *thiz, Ender_Element *e, Eon_Input *input)
{
	Eon_Input_State *eis;

	if (!thiz->input_states)
		thiz->input_states = eina_hash_pointer_new(NULL);
	eis = eina_hash_find(thiz->input_states, (const void *)&input);
	if (!eis)
	{
		eis = eon_input_state_new(input, e, _eon_container_element_get);
		eina_hash_add(thiz->input_states, (const void *)&input, eis);
	}
	return eis;
}

static void _eon_container_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Container *thiz;
	Eon_Event_Mouse_Down *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	eis = _eon_container_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_down(eis);
}

static void _eon_container_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Container *thiz;
	Eon_Event_Mouse_Up *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	eis = _eon_container_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_up(eis);
}

static void _eon_container_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Container *thiz;
	Eon_Event_Mouse_In *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	eis = _eon_container_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_in(eis);
}

static void _eon_container_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Container *thiz;
	Eon_Event_Mouse_Out *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	eis = _eon_container_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_out(eis);
}

static void _eon_container_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Container *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;
	double ox, oy;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	eis = _eon_container_input_state_get(thiz, e, ev->input);
	eon_element_actual_position_get(r, &ox, &oy);
	eon_input_state_feed_mouse_move(eis, ev->x, ev->y, ev->offset_x + ox, ev->offset_y + oy);
}

static void _eon_container_mouse_wheel(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Container *thiz;
	Eon_Event_Mouse_Wheel *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);

	eis = _eon_container_input_state_get(thiz, e, ev->input);
	/* FIXME */
	eon_input_state_feed_mouse_wheel(eis, 0);
}

/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static void _eon_container_content_set(Enesim_Renderer *r, Ender_Element *content)
{
	Eon_Container *thiz;
	Enesim_Renderer *content_r;
	Enesim_Renderer *content_t;

	thiz = _eon_container_get(r);
	if (!thiz) return;

	content_r = ender_element_renderer_get(content);
	if (!eon_is_element(content_r))
		return;

	thiz->content = content;
	content_t = eon_element_renderer_get(content);
	eon_widget_property_set(r, "content", content_t, NULL);
}

static void _eon_container_content_get(Enesim_Renderer *r, const Ender_Element **content)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (!thiz) return;

	*content = thiz->content;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_container_initialize(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	/* we first register our own events */
	if (thiz->pass_events)
	{
		ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_container_mouse_move, NULL);
		ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN], _eon_container_mouse_in, NULL);
		ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT], _eon_container_mouse_out, NULL);
		ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DOWN], _eon_container_mouse_down, NULL);
		ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_UP], _eon_container_mouse_up, NULL);
		ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_WHEEL], _eon_container_mouse_wheel, NULL);
	}

	if (thiz->initialize)
		thiz->initialize(e);
}

static double _eon_container_min_width_get(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->content)
		eon_element_min_width_get(thiz->content, &v);
	if (thiz->max_width_get)
		v = thiz->min_width_get(e, v);

	return v;
}

static double _eon_container_max_width_get(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->content)
		eon_element_max_width_get(thiz->content, &v);
	else
		return DBL_MAX;
	if (thiz->max_width_get)
		v = thiz->max_width_get(e, v);

	return v;
}

static double _eon_container_min_height_get(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->content)
		eon_element_min_height_get(thiz->content, &v);
	if (thiz->max_height_get)
		v = thiz->min_height_get(e, v);

	return v;
}

static double _eon_container_max_height_get(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->content)
		eon_element_max_height_get(thiz->content, &v);
	else
		return DBL_MAX;
	if (thiz->max_height_get)
		v = thiz->max_height_get(e, v);

	return v;
}

static double _eon_container_preferred_width_get(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->content)
		eon_element_preferred_width_get(thiz->content, &v);
	if (thiz->preferred_width_get)
		v = thiz->preferred_width_get(e, v);

	return v;
}

static double _eon_container_preferred_height_get(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->content)
		eon_element_preferred_height_get(thiz->content, &v);
	if (thiz->preferred_height_get)
		v = thiz->preferred_height_get(e, v);

	printf("container preferred height %g\n", v);
	return v;
}

static void _eon_container_free(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

/* TODO */
/* register the initialize and free too
 * on initialize we should just check whenever the content has changed
 * (i.e mutation event) if so, inform that we have changed too
 */
#include "eon_generated_container.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_container_data_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (!thiz) return NULL;
	return thiz->data;
}

Enesim_Renderer * eon_container_new(Eon_Container_Descriptor *descriptor, void *data)
{
	Eon_Container *thiz;
	Eon_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Container));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;
	thiz->min_width_get = descriptor->min_width_get;
	thiz->max_width_get = descriptor->max_width_get;
	thiz->min_height_get = descriptor->min_height_get;
	thiz->max_height_get = descriptor->max_height_get;
	thiz->pass_events = descriptor->pass_events;
	thiz->preferred_width_get = descriptor->preferred_width_get;
	thiz->preferred_height_get = descriptor->preferred_height_get;
	thiz->element_at = descriptor->element_at;

	pdescriptor.initialize = _eon_container_initialize;
	pdescriptor.free = _eon_container_free;
	pdescriptor.setup = descriptor->setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.min_width_get = _eon_container_min_width_get;
	pdescriptor.max_width_get = _eon_container_max_width_get;
	pdescriptor.min_height_get = _eon_container_min_height_get;
	pdescriptor.max_height_get = _eon_container_max_height_get;
	pdescriptor.preferred_width_get = _eon_container_preferred_width_get;
	pdescriptor.preferred_height_get = _eon_container_preferred_height_get;

	r = eon_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_set(Ender_Element *e, Ender_Element *content)
{
	ender_element_value_set(e, "content", content, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_get(Ender_Element *e, Ender_Element **content)
{
	ender_element_value_get(e, "content", content, NULL);
}

