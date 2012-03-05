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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_SPIN_MIN_RANGE;
static Ender_Property *EON_SPIN_MAX_RANGE;
static Ender_Property *EON_SPIN_STEP_INCREMENT;
static Ender_Property *EON_SPIN_VALUE;

typedef struct _Eon_Spin
{
	/* properties */
	double min_range;
	double max_range;
	double step_increment;
	double value;
	/* private */
	Ender_Element *entry;
	Enesim_Renderer *entry_rr;
	Eina_Hash *input_states;
} Eon_Spin;

static inline Eon_Spin * _eon_spin_get(Enesim_Renderer *r)
{
	Eon_Spin *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}

static void _entry_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	/* a change on the entry should be a change on the graphic component right?
	 * no need to call the setup again ... */
}
/*----------------------------------------------------------------------------*
 *                              The input wrapper                             *
 *----------------------------------------------------------------------------*/
Ender_Element * _eon_spin_element_get(Ender_Element *e, double x, double y)
{
	Eon_Spin *thiz;
	Ender_Element *at = NULL;
	Enesim_Renderer *r;
	Enesim_Renderer *entry_r;
	double ax, ay;
	Eon_Size size;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	entry_r = ender_element_object_get(thiz->entry);
	eon_element_actual_size_get(entry_r, &size);
	eon_element_actual_position_get(entry_r, &ax, &ay);

	if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
		at = thiz->entry;
	/* we still need the arrows */
	return at;
}

static Eon_Input_State * _eon_spin_input_state_get(Eon_Spin *thiz, Ender_Element *e, Eon_Input *input)
{
	Eon_Input_State *eis;

	if (!thiz->input_states)
		thiz->input_states = eina_hash_pointer_new(NULL);
	eis = eina_hash_find(thiz->input_states, (const void *)&input);
	if (!eis)
	{
		eis = eon_input_state_new(input, e, _eon_spin_element_get);
		eina_hash_add(thiz->input_states, (const void *)&input, eis);
	}
	return eis;
}

static void _eon_spin_mouse_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_Down *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eis = _eon_spin_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_down(eis);
}

static void _eon_spin_mouse_up(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_Up *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eis = _eon_spin_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_up(eis);
}

static void _eon_spin_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_In *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eis = _eon_spin_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_in(eis);
}

static void _eon_spin_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_Out *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eis = _eon_spin_input_state_get(thiz, e, ev->input);
	eon_input_state_feed_mouse_out(eis);
}

static void _eon_spin_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;
	double ox, oy;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eis = _eon_spin_input_state_get(thiz, e, ev->input);
	eon_element_actual_position_get(r, &ox, &oy);
	eon_input_state_feed_mouse_move(eis, ev->x, ev->y, ev->offset_x + ox, ev->offset_y + oy);
}

static void _eon_spin_mouse_wheel(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_Wheel *ev = event_data;
	Eon_Input_State *eis;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eis = _eon_spin_input_state_get(thiz, e, ev->input);
	/* FIXME */
	eon_input_state_feed_mouse_wheel(eis, 0);
}

static void _eon_spin_mouse_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Spin *thiz;
	Eon_Event_Mouse_Click *ev = event_data;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Eina_Bool inc = EINA_FALSE;
	Eina_Bool dec = EINA_FALSE;
	Eon_Position cursor;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);
	/* check inside what arrow are we */

	cursor.x = ev->rel_x;
	cursor.y = ev->rel_y;
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_spin_arrows_is_inside(theme_r, &cursor, &inc, &dec);

	//printf("inc = %d, dec = %d\n", inc, dec);
	if (inc || dec)
	{
		double v;

		if (inc)
		{
			v = thiz->value + thiz->step_increment;
			if (v >= thiz->max_range)
				v = thiz->max_range;
		}
		else
		{
			v = thiz->value - thiz->step_increment;
			if (v <= thiz->min_range)
				v = thiz->min_range;
		}
		eon_spin_value_set(e, v);
	}
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_spin_initialize(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	/* register every needed callback */
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_spin_mouse_move, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_IN], _eon_spin_mouse_in, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_OUT], _eon_spin_mouse_out, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DOWN], _eon_spin_mouse_down, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_UP], _eon_spin_mouse_up, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_WHEEL], _eon_spin_mouse_wheel, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK], _eon_spin_mouse_click, NULL);
	ender_event_listener_add(thiz->entry, "Mutation", _entry_changed, e);
}

static double _eon_spin_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Spin *thiz;
	Eon_Margin margin;
	Enesim_Renderer *r;
	double v;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eon_theme_spin_margin_get(theme_r, &margin);
	eon_element_min_width_get(thiz->entry, &v);

	return v + margin.left + margin.right;
}

static double _eon_spin_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Spin *thiz;
	Eon_Margin margin;
	Enesim_Renderer *r;
	double v;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eon_theme_spin_margin_get(theme_r, &margin);
	eon_element_max_width_get(thiz->entry, &v);

	return v + margin.left + margin.right;
}

static double _eon_spin_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Spin *thiz;
	Eon_Margin margin;
	Enesim_Renderer *r;
	double v;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eon_theme_spin_margin_get(theme_r, &margin);
	eon_element_min_height_get(thiz->entry, &v);

	return v + margin.top + margin.bottom;
}

static double _eon_spin_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Spin *thiz;
	Eon_Margin margin;
	Enesim_Renderer *r;
	double v;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eon_theme_spin_margin_get(theme_r, &margin);
	eon_element_max_height_get(thiz->entry, &v);

	return v + margin.top + margin.bottom;
}

static double _eon_spin_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Spin *thiz;
	Eon_Margin margin;
	Enesim_Renderer *r;
	double v;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eon_theme_spin_margin_get(theme_r, &margin);
	eon_element_preferred_width_get(thiz->entry, &v);

	return v + margin.left + margin.right;
}

static double _eon_spin_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Spin *thiz;
	Eon_Margin margin;
	Enesim_Renderer *r;
	double v;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	eon_theme_spin_margin_get(theme_r, &margin);
	eon_element_preferred_height_get(thiz->entry, &v);

	return v + margin.top + margin.bottom;
}

static Eina_Bool _eon_spin_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Spin *thiz;
	Eon_Size size;
	Eon_Size area;
	Eon_Size entry_size;
	Eon_Margin margin;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *entry_r;
	Enesim_Renderer *entry_rr;

	r = ender_element_object_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);

	size = state->actual_size;
	eon_theme_spin_margin_get(theme_r, &margin);
	area.width = size.width - margin.left - margin.right;
	area.height = size.height - margin.top - margin.bottom;

	entry_rr = eon_element_renderer_get(thiz->entry);
	entry_r = ender_element_object_get(thiz->entry);
	eon_element_real_relative_size_get(thiz->entry, &area, &entry_size);
	eon_element_actual_size_set(entry_r, entry_size.width, entry_size.height);
	/* check the real size and put the input where it should be aligned */
	eon_element_actual_position_set(entry_r, margin.left, margin.top);
	eon_element_setup(thiz->entry, s, err);

	/* pass it to the theme */
	if (thiz->entry_rr != entry_rr)
	{
		eon_theme_spin_entry_set(theme_r, entry_rr, thiz->entry_rr);
		thiz->entry_rr = entry_rr;
	}

	return EINA_TRUE;
}

static void _eon_spin_cleanup(Ender_Element *e, Enesim_Surface *s)
{

}

static Eon_Widget_Descriptor _eon_spin_widget_descriptor = {
	.initialize = _eon_spin_initialize,
	.setup = _eon_spin_setup,
	.cleanup = _eon_spin_cleanup,
	.name = "spin",
	.min_width_get = _eon_spin_min_width_get,
	.max_width_get = _eon_spin_max_width_get,
	.min_height_get = _eon_spin_min_height_get,
	.max_height_get = _eon_spin_max_height_get,
	.preferred_width_get = _eon_spin_preferred_width_get,
	.preferred_height_get = _eon_spin_preferred_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_spin_new(void)
{
	Eon_Spin *thiz;
	Ender_Element *e;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Spin));
	if (!thiz) return NULL;

	e = eon_entry_new();
	if (!e) goto entry_err;
	eon_entry_alignment_set(e, EON_HORIZONTAL_ALIGNMENT_RIGHT);
	thiz->entry = e;

	r = eon_widget_new(&_eon_spin_widget_descriptor, thiz);
	if (!r) goto renderer_err;

	/* default values */
	eon_entry_text_set(thiz->entry, "0");
	thiz->max_range = 100;
	thiz->min_range = 0;
	thiz->step_increment = 1;

	return r;

renderer_err:
	//ender_element_unref(e);
entry_err:
	free(thiz);
	return NULL;
}

static void _eon_spin_max_range_set(Enesim_Renderer *r, double max_range)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	thiz->max_range = max_range;
}

static void _eon_spin_max_range_get(Enesim_Renderer *r, double *max_range)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	*max_range = thiz->max_range;
}

static void _eon_spin_min_range_set(Enesim_Renderer *r, double min_range)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	thiz->min_range = min_range;
}

static void _eon_spin_min_range_get(Enesim_Renderer *r, double *min_range)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	*min_range = thiz->min_range;
}

static void _eon_spin_step_increment_set(Enesim_Renderer *r, double step_increment)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	thiz->step_increment = step_increment;
}

static void _eon_spin_step_increment_get(Enesim_Renderer *r, double *step_increment)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	*step_increment = thiz->step_increment;
}

static void _eon_spin_value_set(Enesim_Renderer *r, double value)
{
	Eon_Spin *thiz;
	char text[PATH_MAX];

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	/* FIXME check that the number is valid */
	thiz->value = value;
	snprintf(text, PATH_MAX, "%g", value);
	eon_entry_text_set(thiz->entry, text);
}

static void _eon_spin_value_get(Enesim_Renderer *r, double *value)
{
	Eon_Spin *thiz;

	thiz = _eon_spin_get(r);
	if (!thiz) return;

	*value = thiz->value;
}

#include "eon_generated_spin.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_spin_new(void)
{
	return ender_element_new_with_namespace("spin", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_max_range_set(Ender_Element *e, double max_range)
{
	ender_element_value_set(e, "max_range", max_range, NULL);

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_max_range_get(Ender_Element *e, double *max_range)
{
	ender_element_value_get(e, "max_range", max_range, NULL);

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_min_range_set(Ender_Element *e, double min_range)
{
	ender_element_value_set(e, "min_range", min_range, NULL);

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_min_range_get(Ender_Element *e, double *min_range)
{
	ender_element_value_get(e, "min_range", min_range, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_value_set(Ender_Element *e, double value)
{
	ender_element_value_set(e, "value", value, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_value_get(Ender_Element *e, double *value)
{
	ender_element_value_get(e, "value", value, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_step_increment_set(Ender_Element *e, double step_increment)
{
	ender_element_value_set(e, "step_increment", step_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_spin_step_increment_get(Ender_Element *e, double *step_increment)
{
	ender_element_value_get(e, "step_increment", step_increment, NULL);
}
