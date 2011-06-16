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
#define EON_ELEMENT_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_ELEMENT_MAGIC)
#define EON_ELEMENT_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_ELEMENT_MAGIC, ret)

typedef struct _Eon_Element
{
	EINA_MAGIC
	/* properties */
	double width;
	double height;
	double max_width;
	double max_height;
	double min_width;
	double min_height;
	double actual_width;
	double actual_height;
	/* private */
	Escen_Ender *theme_ender;
	Escen_Instance *theme_instance;
	Ender_Element *theme_element;
	Enesim_Renderer *theme_renderer;
	Enesim_Renderer_Descriptor descriptor;
	Enesim_Renderer_Sw_Fill fill;
	Eon_Element_Initialize initialize;
	Enesim_Renderer_Delete free;
	void *data;
} Eon_Element;

static inline Eon_Element * _eon_element_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_ELEMENT_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _element_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Eon_Element *thiz;
	Enesim_Renderer *er;

	thiz = _eon_element_get(r);
	thiz->fill(thiz->theme_renderer, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static void _eon_element_boundings(Enesim_Renderer *r, Enesim_Rectangle *rect)
{
	Eon_Element *thiz;
	double aw, ah;

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

static void _eon_element_free(Enesim_Renderer *r)
{
	Eon_Element *thiz;

}
#if 0
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

static void _eon_widget_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Eon_Widget *thiz;
	Enesim_Renderer *er;

	thiz = _eon_widget_get(r);
	er = ender_element_renderer_get(escen_instance_ender_get(thiz->eei));
	enesim_renderer_flags(er, flags);
}
#endif
#if 0
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

Escen_Ender * eon_widget_theme_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);

	return thiz->escen_ender;
}

Escen_Instance * eon_widget_theme_instance_get(Enesim_Renderer *r)
{
	Eon_Widget *thiz;

	thiz = _eon_widget_get(r);

	return thiz->eei;
}



#endif
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_element_initialize(Ender_Element *ender)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(ender);
	thiz = _eon_element_get(r);
	if (thiz->initialize)
		thiz->initialize(ender);
}

Enesim_Renderer * eon_element_new(Eon_Element_Descriptor *descriptor,
		void *data)
{
	Eon_Element *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Element));
	EINA_MAGIC_SET(thiz, EON_ELEMENT_MAGIC);
	thiz->data = data;
	thiz->actual_width = -1;
	thiz->actual_height = -1;
	thiz->max_width = thiz->max_height = DBL_MAX;
	thiz->min_width = thiz->min_height = 0;
	thiz->width = -1;
	thiz->height = -1;
#if 0
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

renderer_err:
	/* free the escen_ender */
escen_renderer_err:

ender_err:
	free(thiz);
	return NULL;
#endif


	r = enesim_renderer_new(descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

void * eon_element_data_get(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	return thiz->data;
}

void eon_element_actual_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	ender_element_value_set(thiz->theme_element, "width", width, NULL);
}

void eon_element_actual_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_height = height;
	ender_element_value_set(thiz->theme_element, "height", height, NULL);
}

void eon_element_actual_size_set(Enesim_Renderer *r, double width, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->actual_width = width;
	thiz->actual_height = height;
	ender_element_value_set(thiz->theme_element, "width", width, NULL);
	ender_element_value_set(thiz->theme_element, "height", height, NULL);
}

void eon_element_actual_size_get(Enesim_Renderer *r, double *width, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->actual_width;
	*height = thiz->actual_height;
}

void eon_element_real_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double rw;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;

	set = thiz->width;
	eon_element_min_width_get(r, &min);
	eon_element_max_width_get(r, &max);
	rw = set > max ? max : set;
	rw = rw < min ? min : rw;

	//printf("real width = %g (%g %g %g)\n", rw, min, set, max);
	*width = rw;
}

void eon_element_real_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double rh;
	double min, set, max;

	thiz = _eon_element_get(r);
	if (!thiz) return;


	thiz = _eon_element_get(r);
	set = thiz->height;
	eon_element_min_width_get(r, &min);
	eon_element_max_width_get(r, &max);
	rh = set > max ? max : set;
	rh = rh < min ? min : rh;

	//printf("real height = %g (%g %g %g)\n", rh, min, set, max);
	*height = rh;
}

void eon_element_property_set(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Element *thiz;
	Ender_Element *ender;
	va_list va_args;

	thiz = _eon_element_get(r);
	va_start(va_args, name);
	ender_element_value_set_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}

void eon_element_property_get(Enesim_Renderer *r, const char *name, ...)
{
	Eon_Element *thiz;
	Ender_Element *ender;
	va_list va_args;

	thiz = _eon_element_get(r);
	va_start(va_args, name);
	ender_element_value_get_valist(thiz->theme_element, name, va_args);
	va_end(va_args);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_element(Enesim_Renderer *r)
{
	Eon_Element *thiz;

	thiz = enesim_renderer_data_get(r);
	if (!thiz) return EINA_FALSE;
	if (!EINA_MAGIC_CHECK(thiz, EON_ELEMENT_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->actual_height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_actual_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->actual_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*height = thiz->height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	*width = thiz->width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double v = 0;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "min_width", &v, NULL);
	*width = v > thiz->min_width ? v : thiz->min_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->min_height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_min_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double v = 0;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "min_height", &v, NULL);
	*height = v > thiz->min_height ? v : thiz->min_height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "max_width", &v, NULL);
	*width = v < thiz->max_width ? v : thiz->max_width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_width_set(Enesim_Renderer *r, double width)
{
	Eon_Element *thiz;

	if (!width) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_set(Enesim_Renderer *r, double height)
{
	Eon_Element *thiz;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	thiz->max_height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_max_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Element *thiz;
	double v = DBL_MAX;

	if (!height) return;
	thiz = _eon_element_get(r);
	if (!thiz) return;
	ender_element_value_get(thiz->theme_element, "max_height", &v, NULL);
	*height = v < thiz->max_height ? v : thiz->max_height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_element_theme_set(Enesim_Renderer *r, const char *file)
{
	/* remove the theme already associated with the element
	 * and set this, get the correct escen_ender and set
	 * the current state
	 */
}
