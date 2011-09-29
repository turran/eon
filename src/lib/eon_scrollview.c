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
/**
 * @todo
 * The scrollview should not behave exactly as a container, the min width/height
 * should not be the contents plus this, but only this
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Scrollview
{
	/* properties */
	Eon_Position offset;
	/* private */
	Ender_Element *hbar;
	Ender_Element *vbar;
} Eon_Scrollview;

static inline Eon_Scrollview * _eon_scrollview_get(Enesim_Renderer *r)
{
	Eon_Scrollview *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static void _scrollview_mouse_drag_start(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	printf("mouse drag\n");
}

static void _scrollview_mouse_drag_stop(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	printf("mouse drag\n");
}

static void _scrollview_mouse_wheel(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	printf("mouse wheel\n");
	/* FIXME what will happen if the theres something inside the scrollview which also
	 * scrolls, i.e receives the mouse_wheel callback? from the current state, we will get
	 * two scrolls, one on the child object and another on the current object
	 */
}

static void _bar_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollview *thiz;
	Ender_Element *element = data;
	Ender_Event_Mutation *ev = event_data;

	
	if (!strcmp(ev->name, "value"))
	{
		Eon_Position offset;
		Enesim_Renderer *r;
		double x, y;

		r = ender_element_renderer_get(element);
		thiz = _eon_scrollview_get(r);

		/* check if the property changed is the value, if so, move the content */
		if (thiz->hbar == e)
		{
			double v;

			v = ender_value_double_get(ev->value);
			printf("hbar %g\n", v);
			eon_scrollview_x_position_set(element, -10);
		}
		else
		{
			double v;

			v = ender_value_double_get(ev->value);
			printf("vbar %g\n", v);
			eon_scrollview_y_position_set(element, -10);
		}
	}
	eon_element_changed_set(element, EINA_TRUE);
}
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static double _eon_scrollview_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;
	double bmv;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);

	/* FIXME scrollbar inside/outside? */
	eon_element_min_width_get(thiz->vbar, &bmv);

	return cmv + bmv;
}

static double _eon_scrollview_max_width_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static double _eon_scrollview_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Scrollview *thiz;
	Ender_Element *content;
	Enesim_Renderer *r;
	double bmv;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);

	/* FIXME scrollbar inside/outside? */
	eon_element_min_height_get(thiz->vbar, &bmv);

	return cmv + bmv;
}

static double _eon_scrollview_max_height_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static Ender_Element * _eon_scrollview_element_at(Ender_Element *e, double x, double y)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *bar_r;
	Eon_Size size;
	double ax, ay;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);

	bar_r = ender_element_renderer_get(thiz->hbar);
	eon_element_actual_size_get(bar_r, &size);
	eon_element_actual_position_get(bar_r, &ax, &ay);
	if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
		return thiz->hbar;

	bar_r = ender_element_renderer_get(thiz->vbar);
	eon_element_actual_size_get(bar_r, &size);
	eon_element_actual_position_get(bar_r, &ax, &ay);
	if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
		return thiz->vbar;

	return NULL;
}

static void _eon_scrollview_initialize(Ender_Element *e)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);
	ender_event_listener_add(e, "MouseDragStart", _scrollview_mouse_drag_start, NULL);
	ender_event_listener_add(e, "MouseDragStop", _scrollview_mouse_drag_stop, NULL);
	ender_event_listener_add(e, "MouseWheel", _scrollview_mouse_wheel, NULL);
	/* FIXME on finalize remove this callbacks */
	ender_event_listener_add(thiz->hbar, "Mutation", _bar_changed, e);
	ender_event_listener_add(thiz->vbar, "Mutation", _bar_changed, e);
}

static Eina_Bool _eon_scrollview_setup(Ender_Element *e)
{
	Eon_Scrollview *thiz;
	Ender_Element *content;
	Enesim_Renderer *r;
	Eina_Bool ret = EINA_TRUE;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);
	/* setup the content */
	eon_container_content_get(e, &content);
	if (content)
	{
		Eon_Size size;
		Eon_Size content_size;
		Enesim_Renderer *theme_r;
		Enesim_Renderer *content_r;
		double aw, ah;
		double ax, ay;

		theme_r = eon_widget_theme_renderer_get(r);
		content_r = ender_element_renderer_get(content);

		eon_element_actual_width_get(e, &aw);
		eon_element_actual_height_get(e, &ah);
		eon_element_actual_position_get(r, &ax, &ay);

		eon_element_real_size_get(content, &content_size);

		/* hbar */
		if (content_size.width > aw)
		{
			Enesim_Renderer *bar_r;
			Enesim_Renderer *bar_rr;
			double scale;

			bar_r = ender_element_renderer_get(thiz->hbar);
			bar_rr = eon_element_renderer_get(thiz->hbar);
			eon_theme_scrollview_hbar_set(theme_r, bar_rr);
			eon_element_real_size_get(thiz->vbar, &size);
			eon_element_actual_size_set(bar_r, aw, size.height);
			eon_element_actual_position_set(bar_r, 0, ah - size.height);

			scale = aw / content_size.width;
			eon_scrollbar_page_size_set(thiz->hbar, scale * 100);
			eon_element_setup(thiz->hbar);
		}
		else
		{
			eon_theme_scrollview_hbar_set(theme_r, NULL);
		}
		/* vbar */
		if (content_size.height > ah)
		{
			Enesim_Renderer *bar_r;
			Enesim_Renderer *bar_rr;
			double scale;

			bar_r = ender_element_renderer_get(thiz->vbar);
			bar_rr = eon_element_renderer_get(thiz->vbar);
			eon_theme_scrollview_vbar_set(theme_r, bar_rr);
			eon_element_real_size_get(thiz->vbar, &size);
			eon_element_actual_size_set(bar_r, size.width, ah);
			eon_element_actual_position_set(bar_r, aw - size.width, 0);

			scale = ah / content_size.height;
			eon_scrollbar_page_size_set(thiz->vbar, scale * 100);
			eon_element_setup(thiz->vbar);
		}
		else
		{
			eon_theme_scrollview_vbar_set(theme_r, NULL);
		}

		eon_element_real_relative_size_get(content, &content_size, &size);

		/* set the logic size and position, so all the events continue working
		 * the gfx position is handled on the theme with theme_scrollview_offset_set
		 */
		eon_element_actual_size_set(content_r, size.width, size.height);
		eon_element_actual_position_set(content_r, thiz->offset.x, thiz->offset.y);
		if (!eon_element_setup(content))
		{
			printf("impossible to setup the content\n");
			return EINA_FALSE;
		}
	}

	return ret;
}

static Eon_Container_Descriptor _descriptor = {
	.initialize = _eon_scrollview_initialize,
	.setup = _eon_scrollview_setup,
	.min_width_get = _eon_scrollview_min_width_get,
	.min_height_get = _eon_scrollview_min_height_get,
	.max_width_get = _eon_scrollview_max_width_get,
	.max_height_get = _eon_scrollview_max_height_get,
	.element_at = _eon_scrollview_element_at,
	.pass_events = EINA_TRUE,
	.name = "scrollview",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_scrollview_new(void)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;
	Ender_Element *e;

	thiz = calloc(1, sizeof(Eon_Scrollview));
	if (!thiz) return NULL;

	e = eon_scrollbar_new();
	thiz->hbar = e;
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	eon_scrollbar_min_set(e, 0);
	eon_scrollbar_max_set(e, 100);

	e = eon_scrollbar_new();
	thiz->vbar = e;
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_VERTICAL);
	eon_scrollbar_min_set(e, 0);
	eon_scrollbar_max_set(e, 100);

	/* create the two scrollbars */
	r = eon_container_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_scrollview_x_position_set(Enesim_Renderer *r, double x)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_scrollview_get(r);
	thiz->offset.x = x;
	theme_r = eon_widget_theme_renderer_get(r);
	printf("setting x offset %g\n", x);
	eon_theme_scrollview_offset_set(theme_r, &thiz->offset);
}

static void _eon_scrollview_x_position_get(Enesim_Renderer *r, double *x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*x = thiz->offset.x;
}

static void _eon_scrollview_y_position_set(Enesim_Renderer *r, double y)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_scrollview_get(r);
	thiz->offset.y = y;
	theme_r = eon_widget_theme_renderer_get(r);
	printf("setting y offset %g\n", y);
	eon_theme_scrollview_offset_set(theme_r, &thiz->offset);
}

static void _eon_scrollview_y_position_get(Enesim_Renderer *r, double *y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*y = thiz->offset.y;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_scrollview.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_scrollview_new(void)
{
	return ender_element_new_with_namespace("scrollview", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_set(Ender_Element *e, double x)
{
	ender_element_value_set(e, "x_position", x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_get(Ender_Element *e, double *x)
{
	ender_element_value_get(e, "x_position", x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_set(Ender_Element *e, double y)
{
	ender_element_value_set(e, "y_position", y, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_get(Ender_Element *e, double *y)
{
	ender_element_value_get(e, "y_position", y, NULL);
}

