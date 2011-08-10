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
	double x_position;
	double y_position;
	/* private */
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
}
/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static double _eon_scrollview_min_max_width_get(Ender_Element *e, double cmv)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Eon_Margin margin;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollview_margin_get(theme_r, &margin);

	return cmv + margin.left + margin.right;
}

static double _eon_scrollview_min_max_height_get(Ender_Element *e, double cmv)
{
	Eon_Scrollview *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Eon_Margin margin;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_scrollview_margin_get(theme_r, &margin);

	return cmv + margin.top + margin.bottom;
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
}

static Eina_Bool _eon_scrollview_setup(Ender_Element *e)
{
	Eon_Scrollview *thiz;
	Ender_Element *content;
	Enesim_Renderer *r;
	Eina_Bool ret = EINA_TRUE;
	double rw;
	double rh;

	r = ender_element_renderer_get(e);
	thiz = _eon_scrollview_get(r);
	/* setup the content */
	eon_container_content_get(e, &content);
	if (content)
	{
		Eon_Margin margin;
		Enesim_Renderer *theme_r;
		Enesim_Renderer *content_r;
		double aw, ah;
		double ax, ay;
		double cw, ch;

		theme_r = eon_widget_theme_renderer_get(r);
		content_r = ender_element_renderer_get(content);

		eon_element_actual_width_get(e, &aw);
		eon_element_actual_height_get(e, &ah);
		eon_element_actual_position_get(r, &ax, &ay);

		/* so far the margin was used only for the min/max/pref thing
		 * the actual size should be the real one or the scrollview one minus
		 * the margins
		 */
		eon_element_real_width_get(content, &cw);
		eon_element_real_height_get(content, &ch);

		eon_theme_scrollview_margin_get(theme_r, &margin);
		eon_element_actual_size_set(content_r, cw, ch);
		eon_element_actual_position_set(content_r, margin.left, margin.top);
		printf("setting scrollview position!\n");
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
	.min_width_get = _eon_scrollview_min_max_width_get,
	.min_height_get = _eon_scrollview_min_max_height_get,
	.max_width_get = _eon_scrollview_min_max_width_get,
	.max_height_get = _eon_scrollview_min_max_height_get,
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

	thiz = calloc(1, sizeof(Eon_Scrollview));
	if (!thiz) return NULL;

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

	thiz = _eon_scrollview_get(r);
	thiz->x_position = x;
}

static void _eon_scrollview_x_position_get(Enesim_Renderer *r, double *x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*x = thiz->x_position;
}

static void _eon_scrollview_y_position_set(Enesim_Renderer *r, double y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	thiz->y_position = y;
}

static void _eon_scrollview_y_position_get(Enesim_Renderer *r, double *y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(r);
	*y = thiz->y_position;
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
	return ender_element_new("scrollview");
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

