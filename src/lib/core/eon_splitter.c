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
static Ender_Property *EON_SPLITTER_ORIENTATION;
static Ender_Property *EON_SPLITTER_SECOND_CONTENT;
static Ender_Property *EON_SPLITTER_POSITION;

typedef struct _Eon_Splitter_State {
	Eon_Orientation orientation;
	Ender_Element *second_content;
	double position;
} Eon_Splitter_State;

typedef struct _Eon_Splitter
{
	/* properties */
	Eon_Splitter_State current;
	Eon_Splitter_State past;
	/* private */
	Eina_Bool changed : 1;
	double minl, maxl;
	Eina_Bool dragging;
	double offset_dragging;
} Eon_Splitter;

static inline Eon_Splitter * _eon_splitter_get(Enesim_Renderer *r)
{
	Eon_Splitter *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static void _eon_splitter_mouse_move(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Splitter *thiz;
	Eon_Event_Mouse_Move *ev = event_data;
	Enesim_Renderer *r;
	double c;
	double v = 0;
	double length;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	if (!thiz->dragging) return;

	/* get the absolute position of the event */
	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		c = ev->x - thiz->offset_dragging;
	}
	else
	{
		c = ev->y - thiz->offset_dragging;
	}
	length = thiz->maxl - thiz->minl;
	if (c < thiz->minl)
		c = thiz->minl;
	if (c > thiz->maxl)
		c = thiz->maxl;
	c -= thiz->minl;
	if (length)
		v = c / length;
	eon_splitter_position_set(e, v);
}

static void _eon_splitter_mouse_drag_stop(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	thiz->dragging = EINA_FALSE;
}

static void _eon_splitter_mouse_drag_start(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Splitter *thiz;
	Eon_Event_Mouse_Drag_Start *ev = event_data;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Enesim_Rectangle g;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		eon_theme_splitter_position_get(theme_r, &g.x);
		g.y = 0;
		eon_theme_splitter_thickness_get(theme_r, &g.w);
		eon_element_actual_height_get(e, &g.h);
		thiz->offset_dragging = ev->x - g.x;
	}
	else
	{
		g.x = 0;
		eon_theme_splitter_position_get(theme_r, &g.y);
		eon_element_actual_width_get(e, &g.w);
		eon_theme_splitter_thickness_get(theme_r, &g.h);
		thiz->offset_dragging = ev->y - g.y;
	}
	if (enesim_rectangle_is_inside(&g, ev->rel_x, ev->rel_y))
	{
		thiz->dragging = EINA_TRUE;
	}
}
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static double _eon_splitter_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double bmv;
	double scmv = 0;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.second_content)
		eon_element_min_width_get(thiz->current.second_content, &scmv);

	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double thickness;
		bmv = scmv + cmv;

		eon_theme_splitter_thickness_get(theme_r, &thickness);
		bmv += thickness;
	}
	else
	{
		double length;

		bmv = MAX(scmv, cmv);
		eon_theme_splitter_min_length_get(theme_r, &length);
		bmv = MIN(bmv, length);
	}
	return bmv;
}

static double _eon_splitter_max_width_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double bmv;
	double scmv = 0;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.second_content)
		eon_element_max_width_get(thiz->current.second_content, &scmv);

	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double thickness;
		bmv = scmv + cmv;

		eon_theme_splitter_thickness_get(theme_r, &thickness);
		bmv += thickness;
	}
	else
	{
		double length;

		bmv = MIN(scmv, cmv);
		eon_theme_splitter_min_length_get(theme_r, &length);
		bmv = MAX(length, bmv);
	}
	return bmv;
}

static double _eon_splitter_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double bmv;
	double scmv = 0;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.second_content)
		eon_element_min_height_get(thiz->current.second_content, &scmv);
	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double length;

		bmv = scmv < cmv ? scmv : cmv;
		eon_theme_splitter_min_length_get(theme_r, &length);
		bmv = bmv < length ? length : bmv;
	}
	else
	{
		double thickness;
		bmv = scmv + cmv;

		eon_theme_splitter_thickness_get(theme_r, &thickness);
		bmv += thickness;
	}
	return bmv;
}

static double _eon_splitter_max_height_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double bmv;
	double scmv = 0;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.second_content)
		eon_element_max_width_get(thiz->current.second_content, &scmv);

	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double length;

		bmv = MIN(scmv, cmv);
		eon_theme_splitter_min_length_get(theme_r, &length);
		bmv = MAX(length, bmv);
	}
	else
	{
		double thickness;
		bmv = scmv + cmv;

		eon_theme_splitter_thickness_get(theme_r, &thickness);
		bmv += thickness;
	}
	return bmv;
}

static double _eon_splitter_preferred_width_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double bmv = cmv;
	double scmv = 0;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.second_content)
		eon_element_preferred_width_get(thiz->current.second_content, &scmv);
	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double thickness;

		eon_theme_splitter_thickness_get(theme_r, &thickness);
		bmv += scmv + thickness;
	}
	else
	{
		/* FIXME */
	}

	return bmv;
}

static double _eon_splitter_preferred_height_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	double bmv = cmv;
	double scmv = 0;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	if (thiz->current.second_content)
		eon_element_preferred_height_get(thiz->current.second_content, &scmv);
	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		/* FIXME */
	}
	else
	{
		double thickness;

		eon_theme_splitter_thickness_get(theme_r, &thickness);
		bmv += scmv + thickness;
	}

	return bmv;
}

static Ender_Element * _eon_splitter_element_at(Ender_Element *e, double x, double y)
{
	Eon_Splitter *thiz;
	Eon_Size size;
	Enesim_Renderer *r;
	double ax, ay;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);

	if (thiz->current.second_content)
	{
		Enesim_Renderer *content_r;

		content_r = ender_element_object_get(thiz->current.second_content);
		eon_element_actual_size_get(content_r, &size);
		eon_element_actual_position_get(content_r, &ax, &ay);
		if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
		{
			return thiz->current.second_content;
		}
	}

	return NULL;
}

static void _eon_splitter_initialize(Ender_Element *e)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_START], _eon_splitter_mouse_drag_start, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_STOP], _eon_splitter_mouse_drag_stop, NULL);
	ender_event_listener_add(e, eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], _eon_splitter_mouse_move, NULL);
}

static Eina_Bool _eon_splitter_setup(Ender_Element *e,
		const Eon_Element_State *state,
		const Eon_Container_State *cstate,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *content_r;
	Enesim_Renderer *theme_r;
	double aw, ah;
	double cw, ch, cx, cy;
	double scw, sch, scx, scy;
	double position;
	double thickness;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);

	theme_r = eon_widget_theme_renderer_get(r);

	/* calculate the real position */
	position = thiz->current.position;
	if (!cstate->content)
	{
		printf("no content\n");
		return EINA_FALSE;
	}
	content_r = ender_element_object_get(cstate->content);

	if (!thiz->current.second_content)
	{
		printf("no second content\n");
		return EINA_FALSE;
	}
	/* set the content position and size */
	eon_theme_splitter_thickness_get(theme_r, &thickness);
	eon_element_actual_width_get(e, &aw);
	eon_element_actual_height_get(e, &ah);
	if (thiz->current.orientation == EON_ORIENTATION_HORIZONTAL)
	{
		double len;
		double cmin, cmax;
		double scmin, scmax;

		eon_element_min_width_get(cstate->content, &cmin);
		eon_element_min_width_get(thiz->current.second_content, &scmin);
		eon_element_max_width_get(cstate->content, &cmax);
		eon_element_max_width_get(thiz->current.second_content, &scmax);

		thiz->minl = MAX(cmin, aw - thickness - scmax);
		thiz->maxl = MIN(cmax, aw - thickness - scmin);

		len = thiz->maxl - thiz->minl;
		len = thiz->minl + (len * thiz->current.position);

		cx = 0;
		cy = 0;
		cw = len;
		ch = ah;
		scx = len + thickness;
		scy = 0;
		scw = aw - len - thickness;
		sch = ah;

		eon_theme_splitter_position_set(theme_r, len);
	}
	else
	{
		double len;
		double cmin, cmax;
		double scmin, scmax;

		eon_element_min_height_get(cstate->content, &cmin);
		eon_element_min_height_get(thiz->current.second_content, &scmin);
		eon_element_max_height_get(cstate->content, &cmax);
		eon_element_max_height_get(thiz->current.second_content, &scmax);

		thiz->minl = MAX(cmin, ah - thickness - scmax);
		thiz->maxl = MIN(cmax, ah - thickness - scmin);

		len = thiz->maxl - thiz->minl;
		len = thiz->minl + (len * thiz->current.position);

		cx = 0;
		cy = 0;
		cw = aw;
		ch = len;
		scx = 0;
		scy = len + thickness;
		scw = aw;
		sch = ah - len - thickness;

		eon_theme_splitter_position_set(theme_r, len);
	}

	eon_element_actual_width_set(content_r, cw);
	eon_element_actual_height_set(content_r, ch);
	eon_element_actual_position_set(content_r, cx, cy);

	content_r = ender_element_object_get(thiz->current.second_content);
	eon_element_actual_width_set(content_r, scw);
	eon_element_actual_height_set(content_r, sch);
	eon_element_actual_position_set(content_r, scx, scy);

	if (!eon_element_setup(cstate->content, s, err))
	{
		printf("impossible to setup the content\n");
		return EINA_FALSE;
	}
	if (!eon_element_setup(thiz->current.second_content, s, err))
	{
		printf("impossible to setup the content\n");
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static void _eon_splitter_free(Enesim_Renderer *r)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	free(thiz);
}

static void _eon_splitter_damage(Ender_Element *e, Enesim_Renderer_Damage_Cb cb, void *data)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);

	printf("entering the damages\n");
	if (thiz->changed)
	{
		Eina_Rectangle area;
		double x, y, w, h;

		eon_element_actual_position_get(r, &x, &y);
		eon_element_actual_width_get(e, &w);
		eon_element_actual_height_get(e, &h);
		area.x = (int)x;
		area.y = (int)y;
		area.w = (int)w;
		area.h = (int)h;

		cb(r, &area, EINA_FALSE, data);
		return;
	}
	else
	{
		Ender_Element *content;

		eon_container_content_get(e, &content);
		eon_element_damages_get(content, cb, data);

		if (thiz->current.second_content)
		{
			eon_element_damages_get(thiz->current.second_content, cb, data);
		}
	}
}

static void _eon_splitter_cleanup(Ender_Element *e, Enesim_Surface *s)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);

	if (thiz->current.second_content)
		eon_element_cleanup(thiz->current.second_content, s);

	thiz->past = thiz->current;
	thiz->changed = EINA_FALSE;
}

static Eina_Bool _eon_splitter_needs_setup(Ender_Element *e)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Eina_Bool ret;

	r = ender_element_object_get(e);
	thiz = _eon_splitter_get(r);

	/* check if we have changed */
	ret = thiz->changed;
	if (ret)
	{
		return ret;
	}

	/* check if the second content has changed */
	if (thiz->current.second_content)
		ret = eon_element_needs_setup(thiz->current.second_content);

	return ret;
}

static Eon_Container_Descriptor _eon_splitter_container_descriptor = {
	.name = "splitter",
	.initialize = _eon_splitter_initialize,
	.free = _eon_splitter_free,
	.setup = _eon_splitter_setup,
	.cleanup = _eon_splitter_cleanup,
	.damage = _eon_splitter_damage,
	.needs_setup = _eon_splitter_needs_setup,
	.min_width_get = _eon_splitter_min_width_get,
	.max_width_get = _eon_splitter_max_width_get,
	.min_height_get = _eon_splitter_min_height_get,
	.max_height_get = _eon_splitter_max_height_get,
	.preferred_width_get = _eon_splitter_preferred_width_get,
	.preferred_height_get = _eon_splitter_preferred_height_get,
	.element_at = _eon_splitter_element_at,
	.pass_events = EINA_TRUE,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_splitter_new(void)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Splitter));
	/* default values */
	thiz->current.position = 0.5;
	if (!thiz) return NULL;

	r = eon_container_new(&_eon_splitter_container_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_splitter_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	thiz->current.orientation = orientation;
	thiz->changed = EINA_TRUE;

	eon_widget_property_set(r, "orientation", orientation, NULL);
}

static void _eon_splitter_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	*orientation = thiz->current.orientation;
}

static void _eon_splitter_second_content_set(Enesim_Renderer *r, Ender_Element *second_content)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *second_content_rr;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	thiz->current.second_content = second_content;
	thiz->changed = EINA_TRUE;

	theme_r = eon_widget_theme_renderer_get(r);
	second_content_rr = eon_element_renderer_get(thiz->current.second_content);
	eon_theme_splitter_second_content_set(theme_r, second_content_rr);
}

static void _eon_splitter_second_content_get(Enesim_Renderer *r, Ender_Element **second_content)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	*second_content = thiz->current.second_content;
}

static void _eon_splitter_position_set(Enesim_Renderer *r, double position)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	if (position > 1.0) position = 1.0;
	if (position < 0) position = 0.0;

	thiz->current.position = position;
	thiz->changed = EINA_TRUE;
}

static void _eon_splitter_position_get(Enesim_Renderer *r, double *position)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	*position = thiz->current.position;
}

#include "eon_generated_splitter.c"
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
EAPI Ender_Element * eon_splitter_new(void)
{
	return ender_element_new_with_namespace("splitter", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_hsplitter_new(void)
{
	Ender_Element *e;

	e = eon_splitter_new();
	eon_splitter_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_vsplitter_new(void)
{
	Ender_Element *e;

	e = eon_splitter_new();
	eon_splitter_orientation_set(e, EON_ORIENTATION_VERTICAL);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_splitter_orientation_set(Ender_Element *e, Eon_Orientation orientation)
{
	ender_element_value_set(e, "orientation", orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_splitter_orientation_get(Ender_Element *e, Eon_Orientation *orientation)
{
	ender_element_value_get(e, "orientation", orientation, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_splitter_second_content_set(Ender_Element *e, Ender_Element *content)
{
	ender_element_value_set(e, "second_content", content, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_splitter_second_content_get(Ender_Element *e, Ender_Element **content)
{
	ender_element_value_get(e, "second_content", content, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_splitter_position_set(Ender_Element *e, double position)
{
	ender_element_value_set(e, "position", position, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_splitter_position_get(Ender_Element *e, double *position)
{
	ender_element_value_get(e, "position", position, NULL);
}

