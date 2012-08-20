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
	Enesim_Surface *s;
	Enesim_Renderer *image;

	struct {
		Ender_Element *e;
		Eon_Element *ee;
	} hbar, vbar;

	Eina_Bool has_vbar;
	Eina_Bool has_hbar;

	/* needed for the layouts */
	int main_min_length;
	int pre_min_length;

	/* remove this */
	Eina_Bool changed : 1;
} Eon_Scrollview;

static inline Eon_Scrollview * _eon_scrollview_get(Eon_Element *ee)
{
	Eon_Scrollview *thiz;

	thiz = eon_bin_data_get(ee);
	return thiz;
}

#if 0
static void _eon_scrollview_child_geometry_get(Eon_Element *child, Eon_Geometry *g, Eon_Geometry *dst)
{
	Eon_Hints chints;
	Eon_Size cs;
	Eina_Bool has_hbar = EINA_FALSE;
	Eina_Bool has_vbar = EINA_FALSE;

	eon_element_hints_get(child, &chints);
	cs.width = chints.preferred.width;
	/* first the hbar */
	/* in case the preferred is -1, just occupy the whole area */
	if (cs.width < g->width)
		cs.width = g->width;

	if (cs.width < chints.min.width)
		cs.width = chints.min.width;
	if (cs.width > chints.max.width)
		cs.width = chints.max.width;

	if (cs.width > g->width)
	{
		has_hbar = EINA_TRUE;
	}

	/* now the vbar */
	if (cs.height < g->height)
		cs.height = g->height;

	if (cs.height < chints.min.height)
		cs.height = chints.min.height;
	if (cs.height > chints.max.height)
		cs.height = chints.max.height;

	if (cs.height > g->height)
	{
		has_vbar = EINA_TRUE;
	}
}
#endif

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

static void _bar_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Scrollview *thiz;
	Ender_Element *element = data;
	Ender_Event_Mutation *ev = event_data;

	if (!strcmp(ev->name, "value"))
	{
		Ender_Element *content;
		Eon_Size size;
		Eon_Element *ee;

		ee = ender_element_object_get(element);
		thiz = _eon_scrollview_get(ee);
		eon_bin_content_get(element, &content);
		ee = ender_element_object_get(content);
		eon_element_actual_size_get(ee, &size);
		/* check if the property changed is the value, if so, move the content */
		if (thiz->hbar == e)
		{
			double v;

			v = (ender_value_double_get(ev->value) / 100) * size.width;
			eon_scrollview_x_position_set(element, -v);
		}
		else
		{
			double v;

			v = ender_value_double_get(ev->value) / 100 * size.height;
			eon_scrollview_y_position_set(element, -v);
		}
	}
}
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's bin interface                            *
 *----------------------------------------------------------------------------*/
static Ender_Element * _eon_scrollview_element_at(Ender_Element *e, double x, double y)
{
#if 0
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	Eon_Element *bar_e;
	Eon_Size size;
	double ax, ay;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);

	if (thiz->has_hbar)
	{
		bar_e = ender_element_object_get(thiz->hbar);
		eon_element_actual_size_get(bar_e, &size);
		eon_element_actual_position_get(bar_e, &ax, &ay);
		if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
			return thiz->hbar;
	}

	if (thiz->has_vbar)
	{
		bar_e = ender_element_object_get(thiz->vbar);
		eon_element_actual_size_get(bar_e, &size);
		eon_element_actual_position_get(bar_e, &ax, &ay);
		if ((x >= ax && x < ax + size.width) && (y >= ay && y < ay + size.height))
			return thiz->vbar;
	}
#endif
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
	ender_event_listener_add(thiz->hbar, "Mutation", _bar_changed, e);
	ender_event_listener_add(thiz->vbar, "Mutation", _bar_changed, e);
#endif
}

#if 0
static Eina_Bool _eon_scrollview_setup(Ender_Element *e,
		const Eon_Element_State *state,
		const Eon_Container_State *cstate,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	Eina_Bool ret = EINA_TRUE;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);
	/* setup the content */
	if (cstate->content)
	{
		Eon_Size content_size;
		Eon_Size size;
		Eon_Size hbar_size;
		Eon_Size vbar_size;
		Enesim_Renderer *theme_r;
		Eon_Element *content_e;
		Eina_Bool has_vbar = EINA_FALSE;
		Eina_Bool has_hbar = EINA_FALSE;
		double aw, ah;
		double ax, ay;

		theme_r = eon_widget_theme_renderer_get(ee);
		content_e = ender_element_object_get(cstate->content);

		aw = state->actual_size.width;
		ah = state->actual_size.height;
		ax = state->actual_position.x;
		ay = state->actual_position.y;

		size.width = aw;
		size.height = ah;
		eon_element_real_size_get(cstate->content, &content_size);

		/* hbar */
		eon_element_real_size_get(thiz->hbar, &hbar_size);
		eon_element_real_size_get(thiz->vbar, &vbar_size);
		if (content_size.width > aw)
		{
			has_hbar = EINA_TRUE;
			if (content_size.height > ah - hbar_size.height)
				has_vbar = EINA_TRUE;
		}
		if (content_size.height > ah)
		{
			has_vbar = EINA_TRUE;
			if (content_size.width > aw - vbar_size.width)
				has_hbar = EINA_TRUE;
		}

		if (has_hbar)
		{
			Eon_Element *bar_e;
			Enesim_Renderer *bar_rr;
			double scale;
			double length;

			bar_e = ender_element_object_get(thiz->hbar);
			bar_rr = eon_element_renderer_get(thiz->hbar);
			eon_theme_scrollview_hbar_set(theme_r, bar_rr);
			length = aw - (has_vbar ? vbar_size.width : 0);
			eon_element_actual_size_set(bar_e, length, hbar_size.height);
			eon_element_actual_position_set(bar_e, ax, ay + ah - hbar_size.height);

			scale = length / content_size.width;
			eon_scrollbar_page_size_set(thiz->hbar, scale * 100);
			eon_element_setup(thiz->hbar, s, err);
		}
		else
		{
			eon_theme_scrollview_hbar_set(theme_r, NULL);
			eon_scrollbar_value_set(thiz->hbar, 0);
		}
		/* vbar */
		if (has_vbar)
		{
			Eon_Element *bar_e;
			Enesim_Renderer *bar_rr;
			double scale;
			double length;

			bar_e = ender_element_object_get(thiz->vbar);
			bar_rr = eon_element_renderer_get(thiz->vbar);
			eon_theme_scrollview_vbar_set(theme_r, bar_rr);
			length = ah - (has_hbar ? hbar_size.height : 0);
			eon_element_actual_size_set(bar_e, vbar_size.width, length);
			eon_element_actual_position_set(bar_e, ax + aw - vbar_size.width, ay);

			scale = length / content_size.height;
			eon_scrollbar_page_size_set(thiz->vbar, scale * 100);
			eon_element_setup(thiz->vbar, s, err);
		}
		else
		{
			eon_theme_scrollview_vbar_set(theme_r, NULL);
			eon_scrollbar_value_set(thiz->vbar, 0);
		}

		thiz->has_vbar = has_vbar;
		thiz->has_hbar = has_hbar;
		/* set the logic size and position, so all the events continue working
		 * the gfx position is handled on the theme with theme_scrollview_offset_set
		 */
		eon_element_actual_size_set(content_e, content_size.width, content_size.height);
		eon_element_actual_position_set(content_e, ax + thiz->current.offset.x, ay + thiz->current.offset.y);
		if (!eon_element_setup(cstate->content, s, err))
		{
			printf("impossible to setup the content\n");
			return EINA_FALSE;
		}
	}
	thiz->changed = EINA_FALSE;
	thiz->past = thiz->current;

	return ret;
}

static Eina_Bool _eon_scrollview_needs_setup(Ender_Element *e)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);

	if (eon_element_needs_setup(thiz->hbar))
	{
		return EINA_TRUE;
	}

	if (eon_element_needs_setup(thiz->vbar))
	{
		return EINA_TRUE;
	}

	if (!thiz->changed) return EINA_FALSE;
	if (thiz->current.offset.x != thiz->past.offset.x)
	{
		return EINA_TRUE;
	}

	if (thiz->current.offset.y != thiz->past.offset.y)
	{
		return EINA_TRUE;
	}

	return EINA_FALSE;
}

static double _eon_scrollview_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	double vbmv;
	double hbmv;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);

	/* FIXME scrollbar inside/outside? */
	eon_element_min_width_get(thiz->vbar, &vbmv);
	eon_element_min_width_get(thiz->hbar, &hbmv);

	return vbmv + hbmv;
}

static double _eon_scrollview_max_width_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static double _eon_scrollview_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Scrollview *thiz;
	Eon_Element *ee;
	double vbmv;
	double hbmv;

	ee = ender_element_object_get(e);
	thiz = _eon_scrollview_get(ee);

	/* FIXME scrollbar inside/outside? */
	eon_element_min_height_get(thiz->vbar, &vbmv);
	eon_element_min_height_get(thiz->hbar, &hbmv);

	return vbmv + hbmv;
}

static double _eon_scrollview_max_height_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static double _eon_scrollview_preferred_width_get(Ender_Element *e, double cmv)
{
	return cmv;
}

static double _eon_scrollview_preferred_height_get(Ender_Element *e, double cmv)
{
	return cmv;
}
#endif

static void _eon_scrollview_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Scrollview *thiz;
	Ender_Element *child;
	Eon_Hints hints;
	Eon_Geometry cg;
	Eon_Theme_Instance *theme;

	thiz = _eon_scrollview_get(e);
	child = eon_bin_internal_child_get(e);
	theme = eon_widget_theme_instance_get(e);

	if (child)
	{
		Eon_Element *child_e;
		Eon_Size area;
		Eon_Hints vbh;
		Eon_Hints hbh;
		Eon_Geometry bg;
		Enesim_Renderer *child_r;

		/* for now we always display the two bars
		 * but we can make it configureable through
		 * a property on the scrollview
		 */
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


		if (thiz->s)
			enesim_surface_unref(thiz->s);
		thiz->s = enesim_surface_new(ENESIM_FORMAT_ARGB8888,
				ceil(area.width), ceil(area.height));
		
		/* TODO now draw into the scrollable area */
		child_e = ender_element_object_get(child);
		eon_element_hints_get(child_e, &hints);
		printf("scrollview: child hints %g %g %g %g %g %g\n",
			hints.min.width, hints.min.height,
			hints.max.width, hints.max.height,
			hints.preferred.width, hints.preferred.height);

		/* TODO set the size of the child */
#if 0
		cg.x = g->x;
		cg.y = g->y;
		cg.width = area.width;
		cg.height = area.height;
		eon_hints_geometry_align(hints, &cg,
				EON_HORIZONTAL_ALIGNMENT_CENTER,
				EON_VERTICAL_ALIGNMENT_CENTER);
		if (cg.width > area.width)
		{

		}

		if (cg.height > area.height)
		{

		}
#endif
	}
	else
	{

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
	/* .setup 		= */ NULL,
	/* .needs_setup 	= */ NULL,
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
	eon_scrollbar_max_set(e, 100);
	r = eon_element_renderer_get(thiz->hbar.e);
	eon_theme_scrollview_hbar_set(theme->object, r);

	e = eon_scrollbar_new();
	thiz->vbar.e = e;
	thiz->vbar.ee = ender_element_object_get(e);
	eon_scrollbar_orientation_set(e, EON_ORIENTATION_VERTICAL);
	eon_scrollbar_min_set(e, 0);
	eon_scrollbar_max_set(e, 100);
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
	Enesim_Renderer *theme_r;

	thiz = _eon_scrollview_get(ee);
	thiz->current.offset.x = x;
	thiz->changed = EINA_TRUE;
#if 0
	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_scrollview_offset_set(theme_r, &thiz->current.offset);
#endif
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
	Enesim_Renderer *theme_r;

	thiz = _eon_scrollview_get(ee);
	thiz->current.offset.y = y;
	thiz->changed = EINA_TRUE;

#if 0
	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_scrollview_offset_set(theme_r, &thiz->current.offset);
#endif
}

static void _eon_scrollview_y_position_get(Eon_Element *ee, double *y)
{
	Eon_Scrollview *thiz;

	thiz = _eon_scrollview_get(ee);
	*y = thiz->current.offset.y;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_scrollview_delete NULL
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

