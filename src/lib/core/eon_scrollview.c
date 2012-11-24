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
/**
 * @todo
 * The scrollview should not behave exactly as a bin, the min width/height
 * should not be the contents plus this, but only this
 * Add the inset/outset theme property
 * Add the hbar, vbar position property (left, right, top, bottom)
 * The best way to do this renderer would be to create another thread for rendering
 * the content but only when we are scrolling, whenever we stop scrolling continue
 * using the content renderer
 */
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_container.h"
#include "eon_bin.h"
#include "eon_scrollbar.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
#include "eon_private_container.h"
#include "eon_private_bin.h"

#include "theme/eon_theme_widget.h"
#include "theme/eon_theme_container.h"
#include "theme/eon_theme_bin.h"
#include "theme/eon_theme_scrollview.h"
/* TODO add directions for the bars, hbar: top, bottom. vbar: left, right
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_scrollview_log

static int _eon_scrollview_log = -1;

static Ender_Property *EON_SCROLLVIEW_X_POSITION;
static Ender_Property *EON_SCROLLVIEW_Y_POSITION;

typedef struct _Eon_Scrollview_State
{
	Eon_Position offset;
} Eon_Scrollview_State;

typedef struct _Eon_Scrollview
{
	/* properties */
	Eon_Scrollview_State current;
	Eon_Scrollview_State past;
	/* private */

	struct {
		Ender_Element *e;
		Eon_Element *ee;
	} hbar, vbar;

	Eina_Bool has_vbar;
	Eina_Bool has_hbar;
	Eon_Geometry area;

	/* remove this */
	Eina_Bool changed : 1;
} Eon_Scrollview;

static inline Eon_Scrollview * _eon_scrollview_get(Eon_Element *ee)
{
	Eon_Scrollview *thiz;

	thiz = eon_bin_data_get(ee);
	return thiz;
}

static void _eon_scrollview_scrollbars_setup(Eon_Scrollview *thiz, Eon_Size *area, double cw, double ch)
{
	printf("setting up the scrollbars %g %g - %g %g\n", cw, ch, area->width, area->height);
	if (cw > area->width)
	{
		eon_scrollbar_page_size_set(thiz->hbar.e, area->width / cw);
	}

	if (ch > area->height)
	{
		eon_scrollbar_page_size_set(thiz->vbar.e, area->height / ch);
	}
}

static void _eon_scrollview_persistent_scrollbars_geometry_set(Eon_Scrollview *thiz, Eon_Geometry *g, Eon_Size *darea)
{
	Eon_Hints vbh;
	Eon_Hints hbh;
	Eon_Geometry bg;
	Eon_Size area;

	area.width = g->width;
	area.height = g->height;

	eon_element_hints_get(thiz->hbar.ee, &hbh);
	eon_element_hints_get(thiz->vbar.ee, &vbh);
	area.height -= hbh.min.height;
	area.width -= vbh.min.width;

	bg.x = g->x;
	bg.y = g->y + area.height;
	bg.width = area.width;
	bg.height = hbh.min.height;
	eon_element_geometry_set(thiz->hbar.ee, &bg);

	bg.x = g->x + area.width;
	bg.y = g->y;
	bg.width = vbh.min.width;
	bg.height = area.height;
	eon_element_geometry_set(thiz->vbar.ee, &bg);

	*darea = area;
	thiz->has_hbar = EINA_TRUE;
	thiz->has_vbar = EINA_TRUE;
}

static inline double _eon_scrollview_child_position_get(double c, double l, double cl, double v)
{
	return c - ((cl - l) * v);
}

static double _eon_scrollview_child_x_position_get(Eon_Scrollview *thiz, double width, double v)
{
	return _eon_scrollview_child_position_get(thiz->area.x, thiz->area.width, width, v);
}

static double _eon_scrollview_child_y_position_get(Eon_Scrollview *thiz, double height, double v)
{
	return _eon_scrollview_child_position_get(thiz->area.y, thiz->area.height, height, v);
}

static void _eon_scrollview_child_geometry_set(Eon_Scrollview *thiz, Eon_Element *child, double cw, double ch)
{
	Eon_Geometry g;
	double v;

	g.width = cw;
	g.height = ch;

	eon_scrollbar_value_get(thiz->hbar.e, &v);
	g.x = _eon_scrollview_child_x_position_get(thiz, g.width, v);
	eon_scrollbar_value_get(thiz->vbar.e, &v);
	g.y = _eon_scrollview_child_y_position_get(thiz, g.height, v);
	eon_element_geometry_set(child, &g);
}

static void _eon_scrollview_child_size_get(Eon_Element *child, double w, double h, double *dw, double *dh)
{
	Eon_Hints chints;
	double cw, ch;

	/* always use the preferred size if set, if not, make the child fit */
	eon_element_hints_get(child, &chints);
	printf("scrollview: child hints %g %g %g %g %g %g\n",
		chints.min.width, chints.min.height,
		chints.max.width, chints.max.height,
		chints.preferred.width, chints.preferred.height);

	/* first the hbar */
	cw = chints.preferred.width;
	/* in case the preferred is -1, just occupy the whole area */
	if (cw < 0)
	{
		cw = w;
		if (cw < chints.min.width)
			cw = chints.min.width;
		if (cw > chints.max.width)
			cw = chints.max.width;
	}

	/* now the vbar */
	ch = chints.preferred.height;
	/* in case the preferred is -1, just occupy the whole area */
	if (ch < 0)
	{
		ch = w;
		if (ch < chints.min.height)
			ch = chints.min.height;
		if (ch > chints.max.height)
			ch = chints.max.height;
	}

	*dw = cw;
	*dh = ch;
}
/*----------------------------------------------------------------------------*
 *                                Event handlers                              *
 *----------------------------------------------------------------------------*/
#if 0
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
#endif

static void _bar_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	Eon_Element *child_e;
	Eon_Geometry g;
	Eon_Geometry cg;
	Ender_Element *element = data;
	Ender_Element *child;
	Ender_Event_Mutation *ev = event_data;
	double v;


	if (strcmp(ev->name, "value"))
		return;

	v = ender_value_double_get(ev->value);
	printf("bar value!!! %g\n", v);

	ee = ender_element_object_get(element);
	eon_element_geometry_get(ee, &g);
	thiz = _eon_scrollview_get(ee);

	child = eon_bin_internal_child_get(ee);
	child_e = ender_element_object_get(child);
	eon_element_geometry_get(child_e, &cg);

	if (thiz->hbar.e == e)
	{
		cg.x = _eon_scrollview_child_x_position_get(thiz, cg.width, v);
		eon_element_geometry_set(child_e, &cg);
	}
	else
	{
		cg.y = _eon_scrollview_child_y_position_get(thiz, cg.height, v);
		eon_element_geometry_set(child_e, &cg);
	}
}
/*----------------------------------------------------------------------------*
 *                         The Eon's bin interface                            *
 *----------------------------------------------------------------------------*/
static Ender_Element * _eon_scrollview_element_at(Ender_Element *e, double x, double y)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	Ender_Element *child;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);

	if (thiz->has_hbar)
	{
		Eon_Geometry g;

		eon_element_geometry_get(thiz->hbar.ee, &g);
		if (eon_geometry_is_inside(&g, x, y))
			return thiz->hbar.e;
	}

	if (thiz->has_vbar)
	{
		Eon_Geometry g;

		eon_element_geometry_get(thiz->vbar.ee, &g);
		if (eon_geometry_is_inside(&g, x, y))
			return thiz->vbar.e;
	}

	child = eon_bin_internal_child_get(ee);
	if (child)
	{
		Eon_Geometry g;
		Eon_Element *child_e;

		child_e = ender_element_object_get(child);
		eon_element_geometry_get(child_e, &g);
		if (eon_geometry_is_inside(&g, x, y))
			return child;
	}

	return NULL;
}

static void _eon_scrollview_initialize(Ender_Element *e)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);
#if 0
	ender_event_listener_add(e, "MouseDragStart", _scrollview_mouse_drag_start, NULL);
	ender_event_listener_add(e, "MouseDragStop", _scrollview_mouse_drag_stop, NULL);
	ender_event_listener_add(e, "MouseWheel", _scrollview_mouse_wheel, NULL);
	/* FIXME on finalize remove this callbacks */
#endif
	ender_event_listener_add(thiz->hbar.e, "Mutation", _bar_changed, e);
	ender_event_listener_add(thiz->vbar.e, "Mutation", _bar_changed, e);
}

static void _eon_scrollview_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Scrollview *thiz;
	Ender_Element *child;

	thiz = _eon_scrollview_get(e);
	child = eon_bin_internal_child_get(e);
	if (child)
	{
		Eon_Element *child_e;
		Eon_Size area;
		Eon_Geometry cg;

		child_e = ender_element_object_get(child);
		/* for now we always display the two bars
		 * but we can make it configureable through
		 * a property on the scrollview
		 */
		_eon_scrollview_persistent_scrollbars_geometry_set(thiz, g, &area);
		thiz->area.x = g->x;
		thiz->area.y = g->y;
		thiz->area.width = area.width;
		thiz->area.height = area.height;

		/* get the size of the child */
		_eon_scrollview_child_size_get(child_e, area.width, area.height, &cg.width, &cg.height);
		/* set the size and position of the bars */
		_eon_scrollview_scrollbars_setup(thiz, &area, cg.width, cg.height);
		/* get the child position */
		_eon_scrollview_child_geometry_set(thiz, child_e, cg.width, cg.height);

		printf("scrollview child size %g %g\n", cg.width, cg.height);
	}
	else
	{
		/* TODO in case of persitant scrollbars, draw theme disabled */
	}
	/* now set the bars */
	printf("scrollview: setting the geometry %g %g %g %g\n", g->x, g->y, g->width, g->height);
}

static void _eon_scrollview_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_Scrollview *thiz;
	Ender_Element *child;

	thiz = _eon_scrollview_get(e);
	/* the preferred size is the child size */
	child = eon_bin_internal_child_get(e);
	if (child)
	{
		Eon_Element *child_e;
		Eon_Hints child_hints;
		Eon_Hints hg, vg;
		//Eina_Bool vbar;
		//Eina_Bool hbar;

		child_e = ender_element_object_get(child);
		eon_element_hints_get(child_e, &child_hints);
		hints->preferred = child_hints.preferred;

		/* the min size is the scrollbars size */
		eon_element_hints_get(thiz->hbar.ee, &hg);
		eon_element_hints_get(thiz->vbar.ee, &vg);

		hints->min.width = hg.min.width;
		if (hints->min.width > child_hints.min.width)
			hints->min.width = child_hints.min.width;

		hints->min.height = vg.min.height;
		if (hints->min.height > child_hints.min.height)
			hints->min.height = child_hints.min.height;
	}
	else
	{
		hints->preferred.width = hints->preferred.height = -1;
	}
	printf("scrollview hints %g %g %g %g %g %g\n",
			hints->min.width, hints->min.height,
			hints->max.width, hints->max.height,
			hints->preferred.width, hints->preferred.height);
}


static void _eon_scrollview_free(Eon_Element *ee)
{
	/* TODO destroy the two bars */
}

static Eon_Bin_Descriptor _eon_scrollbar_descriptor = {
	/* .initialize 		= */ _eon_scrollview_initialize,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .geometry_set 	= */ _eon_scrollview_geometry_set,
	/* .free		= */ _eon_scrollview_free,
	/* .name 		= */ "scrollview",
	/* .hints_get 		= */ _eon_scrollview_hints_get,
	/* .child_at 		= */ _eon_scrollview_element_at,
	/* .pass_events 	= */ EINA_TRUE,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_scrollview_new(void)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;
	Ender_Element *e;
	Enesim_Renderer *r;

	theme = eon_theme_instance_new("scrollview", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Scrollview));
	if (!thiz) return NULL;

	e = eon_scrollbar_new();
	thiz->hbar.e = e;
	thiz->hbar.ee = ender_element_object_get(e);
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	eon_scrollbar_min_set(e, 0);
	eon_scrollbar_max_set(e, 1);
	eon_scrollbar_page_increment_set(e, 0.3);
	eon_scrollbar_step_increment_set(e, 0.1);
	r = eon_element_renderer_get(thiz->hbar.e);
	eon_theme_scrollview_hbar_set(theme->object, r);

	e = eon_scrollbar_new();
	thiz->vbar.e = e;
	thiz->vbar.ee = ender_element_object_get(e);
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_VERTICAL);
	eon_scrollbar_min_set(e, 0);
	eon_scrollbar_max_set(e, 1);
	eon_scrollbar_page_increment_set(e, 0.3);
	eon_scrollbar_step_increment_set(e, 0.1);
	r = eon_element_renderer_get(thiz->vbar.e);
	eon_theme_scrollview_vbar_set(theme->object, r);

	/* create the two scrollbars */
	ee = eon_bin_new(theme, &_eon_scrollbar_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_scrollview_x_position_set(Eon_Element *ee, double x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(ee);
	if (x > 1)
		x = 1;
	if (x < 0)
		x = 0;
	thiz->current.offset.x = x;
	eon_scrollbar_value_set(thiz->hbar.e, x);
}

static void _eon_scrollview_x_position_get(Eon_Element *ee, double *x)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(ee);
	*x = thiz->current.offset.x;
}

static void _eon_scrollview_y_position_set(Eon_Element *ee, double y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(ee);
	if (y > 1)
		y = 1;
	if (y < 0)
		y = 0;
	thiz->current.offset.y = y;
	eon_scrollbar_value_set(thiz->vbar.e, y);
}

static void _eon_scrollview_y_position_get(Eon_Element *ee, double *y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(ee);
	*y = thiz->current.offset.y;
}

#define _eon_scrollview_delete NULL
#include "eon_generated_scrollview.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_scrollview_init(void)
{
	_eon_scrollview_log = eina_log_domain_register("eon_scrollview", EON_LOG_COLOR_DEFAULT);
	if (_eon_scrollview_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_scrollview_init();
}

void eon_scrollview_shutdown(void)
{
	if (_eon_scrollview_log < 0)
		return;
	_eon_scrollview_shutdown();
	eina_log_domain_unregister(_eon_scrollview_log);
	_eon_scrollview_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_scrollview_new(void)
{
	return EON_ELEMENT_NEW("scrollview");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_set(Ender_Element *e, double x)
{
	ender_element_property_value_set(e, EON_SCROLLVIEW_X_POSITION, x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_x_position_get(Ender_Element *e, double *x)
{
	ender_element_property_value_get(e, EON_SCROLLVIEW_X_POSITION, x, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_set(Ender_Element *e, double y)
{
	ender_element_property_value_set(e, EON_SCROLLVIEW_Y_POSITION, y, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollview_y_position_get(Ender_Element *e, double *y)
{
	ender_element_property_value_get(e, EON_SCROLLVIEW_Y_POSITION, y, NULL);
}

