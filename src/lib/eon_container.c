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
	void *data;
} Eon_Container;

static inline Eon_Container * _eon_container_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
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
	if (thiz->initialize)
		thiz->initialize(e);
}

static double _eon_container_min_width_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;
	Enesim_Renderer *content_r;
	Enesim_Renderer *theme_r;
	double v = 0;
	double min;

	thiz = _eon_container_get(r);
	if (!thiz->content) return v;

	content_r = ender_element_renderer_get(thiz->content);
	eon_element_min_width_get(content_r, &min);
	v += min;

	theme_r = eon_element_theme_renderer_get(r);
	eon_theme_container_decoration_size_get(theme_r, &min, NULL);
	v += min;

	return v;
}

static double _eon_container_max_width_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;
	Enesim_Renderer *content_r;
	Enesim_Renderer *theme_r;
	double v = 0;
	double max;

	thiz = _eon_container_get(r);
	if (!thiz->content) return v;

	content_r = ender_element_renderer_get(thiz->content);
	eon_element_max_width_get(content_r, &max);
	v += max;

	theme_r = eon_element_theme_renderer_get(r);
	eon_theme_container_decoration_size_get(theme_r, &max, NULL);
	v += max;

	return v;
}

static double _eon_container_min_height_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;
	Enesim_Renderer *content_r;
	Enesim_Renderer *theme_r;
	double v = 0;
	double min;

	thiz = _eon_container_get(r);
	if (!thiz->content) return v;

	content_r = ender_element_renderer_get(thiz->content);
	eon_element_min_height_get(content_r, &min);
	v += min;
	printf("1 min height = %g\n", v);

	theme_r = eon_element_theme_renderer_get(r);
	eon_theme_container_decoration_size_get(theme_r, NULL, &min);
	v += min;

	printf("2 min height = %g\n", v);
	return v;
}

static double _eon_container_max_height_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;
	Enesim_Renderer *content_r;
	Enesim_Renderer *theme_r;
	double v = 0;
	double max;

	thiz = _eon_container_get(r);
	if (!thiz->content) return v;

	content_r = ender_element_renderer_get(thiz->content);
	eon_element_max_height_get(content_r, &max);
	v += max;

	theme_r = eon_element_theme_renderer_get(r);
	eon_theme_container_decoration_size_get(theme_r, NULL, &max);
	v += max;

	return v;
}

static Eina_Bool _eon_container_setup(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (thiz->content)
	{
		Enesim_Renderer *content_r;
		Enesim_Renderer *theme_r;
		double aw, ah;
		double dw = 0;
		double dh = 0;
		double cx = 0;
		double cy = 0;

		content_r = ender_element_renderer_get(thiz->content);
		theme_r = eon_element_theme_renderer_get(r);
		/* set the size and position */
		eon_element_actual_width_get(r, &aw);
		eon_element_actual_height_get(r, &ah);
		eon_theme_container_decoration_size_get(theme_r, &dw, &dh);
		printf("decoration %g %g\n", dw, dh);
		eon_element_actual_size_set(content_r, aw - dw, ah - dh);
		if (!eon_element_setup(content_r))
			return EINA_FALSE;
		printf("setting size %g %g\n", aw - dw, ah - dh);
		eon_theme_container_content_position_get(theme_r, &cx, &cy);
		eon_element_actual_position_set(content_r, cx, cy);
	}
	if (thiz->setup)
		return thiz->setup(r);

	return EINA_TRUE;
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
/* register the intiialize and free too
 * on initialize we should just check whenever the content has changed
 * (i.e mutation event) if so, inform that we have changed too
 */
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

	pdescriptor.initialize = _eon_container_initialize;
	pdescriptor.free = _eon_container_free;
	pdescriptor.setup = _eon_container_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.min_width_get = _eon_container_min_width_get;
	pdescriptor.max_width_get = _eon_container_max_width_get;
	pdescriptor.min_height_get = _eon_container_min_height_get;
	pdescriptor.max_height_get = _eon_container_max_height_get;

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
EAPI void eon_container_content_set(Enesim_Renderer *r, Ender_Element *content)
{
	Eon_Container *thiz;
	Enesim_Renderer *content_r;
	Enesim_Renderer *content_t;

	thiz = _eon_container_get(r);
	if (!thiz) return;

	thiz->content = content;
	content_r = ender_element_renderer_get(content);
	if (!eon_is_element(content_r))
		return;
	content_t = eon_element_theme_renderer_get(content_r);
	eon_element_property_set(r, "content", content_t, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_get(Enesim_Renderer *r, const Ender_Element **content)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (!thiz) return;

	*content = thiz->content;
}

