/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
#define EON_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_WIDGET_MAGIC)
#define EON_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_WIDGET_MAGIC, ret)

typedef struct _Eon_Widget
{
	EINA_MAGIC
	Eon_Widget_Descriptor *descriptor;
	void *data; /* the data provided by the widget types */
	Escen_Ender *escen_ender; /* the theme ender */
	Escen_Ender_Instance *eei;
	/* FIXME add a way to setup and cleanup an ender whenever
	 * a widget ender is associated with a layout
	 */
	Enesim_Renderer_Sw_Fill fill;
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	if (!r) return NULL;
	thiz = eon_element_data_get(r);
	EON_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _widget_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_instance_ender_get(ew->eei));
	ew->fill(er, x, y, len, dst);
}

static void _widget_mouse_in(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Enesim_Renderer *r;
	Eon_Widget *thiz;
	Escen_State *new_state;

	r = ender_element_renderer_get(e);
	thiz = _eon_widget_get(r);
	new_state = escen_ender_state_get(thiz->escen_ender, "mouse_in");
	if (!new_state) return;

	escen_instance_state_set(thiz->eei, new_state);
}

static void _widget_mouse_out(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Enesim_Renderer *r;
	Eon_Widget *thiz;
	Escen_State *new_state;

	r = ender_element_renderer_get(e);
	thiz = _eon_widget_get(r);
	new_state = escen_ender_state_get(thiz->escen_ender, "mouse_out");
	if (!new_state) return;

	escen_instance_state_set(thiz->eei, new_state);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_widget_initialize(Ender_Element *ender)
{
	Eon_Widget *thiz;
	Enesim_Renderer *r;


	r = ender_element_renderer_get(ender);
	thiz = _eon_widget_get(r);
	/* register every needed callback */
	ender_event_listener_add(ender, "MouseIn", _widget_mouse_in, NULL);
	ender_event_listener_add(ender, "MouseOut", _widget_mouse_out, NULL);
	if (thiz->descriptor->initialize)
		thiz->descriptor->initialize(ender);
}

static void _eon_widget_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Widget *thiz;
	Ender_Element *ender;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	ender_element_value_set(ender, "width", width, NULL);
}

static void _eon_widget_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Widget *thiz;
	Ender_Element *ender;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	ender_element_value_set(ender, "height", height, NULL);
}

static double _eon_widget_max_width_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender_Element *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "max_width", &v, NULL);
	return v;
}

static double _eon_widget_min_width_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender_Element *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "min_width", &v, NULL);
	return v;
}

static double _eon_widget_max_height_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender_Element *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "max_height", &v, NULL);
	return v;
}

static double _eon_widget_min_height_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;
	Ender_Element *ender;
	double v;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	ender_element_value_get(ender, "min_height", &v, NULL);
	return v;
}

static Eon_Element_Descriptor _eon_widget_element_descriptor = {
	.initialize = _eon_widget_initialize,
	.actual_width_set = _eon_widget_actual_width_set,
	.actual_height_set = _eon_widget_actual_height_set,
	.max_width_get = _eon_widget_max_width_get,
	.min_width_get = _eon_widget_min_width_get,
	.max_height_get = _eon_widget_max_height_get,
	.min_height_get = _eon_widget_min_height_get,
};
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_widget_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;
	Enesim_Color color;
	double ox, oy;
	double width, height;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_instance_ender_get(ew->eei));
	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(er, ox, oy);
	/* FIXME if the user changes the color of the widget, the theme should
	 * reflect that value
	 */
	enesim_renderer_color_get(r, &color);
	enesim_renderer_color_set(er, color);
#if 0
	/* set the width and height to the widget */
	/* FIXME if this is working, maybe we should just remove the callbacks
	 * from eon_element?
	 */
	eon_element_actual_size_get(r, &width, &height);
	printf("actual size get %g %g\n", width, height);
	eon_theme_widget_width_set(er, width);
	eon_theme_widget_height_set(er, height);
#endif
	if (!enesim_renderer_sw_setup(er))
	{
		printf("not available to setup yet\n");
		return EINA_FALSE;
	}
	/* get the ender from the escen ender and get the fill function */
	ew->fill = enesim_renderer_sw_fill_get(er);
	if (!ew->fill) return EINA_FALSE;

	*fill = _widget_draw;
	return EINA_TRUE;
}

static void _eon_widget_cleanup(Enesim_Renderer *r)
{
	Eon_Widget *ew;
	Enesim_Renderer *er;

	ew = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_instance_ender_get(ew->eei));
	enesim_renderer_sw_cleanup(er);
}

static void _eon_widget_free(Enesim_Renderer *r)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);
}

static void _eon_widget_boundings(Enesim_Renderer *r, Enesim_Rectangle *rect)
{
	Eon_Widget *thiz;
	Enesim_Renderer *er;
	double aw, ah;

	thiz = _eon_widget_get(r);
	eon_element_actual_size_get(r, &aw, &ah);
	/* There's no layout, or the layout didnt set an active width/height */
	if (aw < 0 || ah < 0)
	{
		eon_element_real_width_get(r, &aw);
		eon_element_real_height_get(r, &ah);
	}
	rect->x = 0;
	rect->y = 0;
	rect->w = aw;
	rect->h = ah;
}

static void _eon_widget_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Eon_Widget *thiz;
	Enesim_Renderer *er;

	thiz = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_instance_ender_get(thiz->eei));
	enesim_renderer_flags(er, flags);
}

static Enesim_Renderer_Descriptor _eon_widget_descriptor = {
	.sw_setup = _eon_widget_setup,
	.sw_cleanup = _eon_widget_cleanup,
	.boundings = _eon_widget_boundings,
	.free = _eon_widget_free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Escen_Ender_Instance * eon_widget_theme_instance_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);

	return thiz->eei;
}

Escen_Ender * eon_widget_theme_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);

	return thiz->escen_ender;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_widget_new(Eon_Widget_Descriptor *descriptor, void *data)
{
	Eon_Widget *thiz;
	Escen *escen;
	Escen_Ender *escen_ender;
	Ender_Element *ender;
	Enesim_Renderer *r;
	Enesim_Renderer *escen_renderer;
	Enesim_Color color;
	char theme[PATH_MAX];

	escen = eon_theme_get();
	if (!escen) {
		printf("no theme\n");
		return NULL;
	}

	thiz = calloc(1, sizeof(Eon_Widget));
	EINA_MAGIC_SET(thiz, EON_WIDGET_MAGIC);
	thiz->data = data;
	thiz->descriptor = descriptor;

	escen_ender = escen_ender_get(escen, descriptor->name);
	if (!escen_ender) goto ender_err;
	thiz->escen_ender = escen_ender;
	thiz->eei = escen_instance_new(thiz->escen_ender);

	ender = escen_instance_ender_get(thiz->eei);
	escen_renderer = ender_element_renderer_get(ender);
	if (!escen_renderer) goto escen_renderer_err;

	r = eon_element_new(&_eon_widget_element_descriptor,
			&_eon_widget_descriptor, thiz);
	if (!r) goto renderer_err;

	/* Set the default properties from the state */
	enesim_renderer_color_get(escen_renderer, &color);
	enesim_renderer_color_set(r, color);

	/* Add default event handlers: mouse move, mouse in, mouse out, etc */
	/* Whenever the state changes, we must set the common properties again */
	//printf("creating new widget %p %s with theme %p\n", r, name, escen_renderer);

	return r;

renderer_err:
	/* free the escen_ender */
escen_renderer_err:

ender_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_widget(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = eon_element_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_WIDGET_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_theme_set(Enesim_Renderer *r, const char *file)
{
	/* remove the theme already associated with the widget
	 * and set this, get the correct escen_ender and set
	 * the current state
	 */
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_widget_data_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = _eon_widget_get(r);
	if (!e) return NULL;

	return e->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_property_set(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	Ender_Element *ender;
	va_list va_args;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	va_start(va_args, name);
	ender_element_value_set_valist(ender, name, va_args);
	va_end(va_args);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_property_get(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Widget *thiz;
	Ender_Element *ender;
	va_list va_args;

	thiz = _eon_widget_get(r);
	ender = escen_instance_ender_get(thiz->eei);
	va_start(va_args, name);
	ender_element_value_get_valist(ender, name, va_args);
	va_end(va_args);
}
