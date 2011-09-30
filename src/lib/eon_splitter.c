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
typedef struct _Eon_Splitter
{
	/* properties */
	Eon_Orientation orientation;
	Ender_Element *second_content;
	double position;
	/* private */
	Eina_Bool dragging;
	double offset_dragging;
} Eon_Splitter;

static inline Eon_Splitter * _eon_splitter_get(Enesim_Renderer *r)
{
	Eon_Splitter *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                       The Eon's container interface                        *
 *----------------------------------------------------------------------------*/
static double _eon_splitter_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	double bmv;

	r = ender_element_renderer_get(e);
	thiz = _eon_splitter_get(r);

#if 0
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
#endif
	return cmv;
}

static double _eon_splitter_max_width_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static double _eon_splitter_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Splitter *thiz;
	Ender_Element *content;
	Enesim_Renderer *r;
	double bmv;

	r = ender_element_renderer_get(e);
	thiz = _eon_splitter_get(r);

	return cmv;
}

static double _eon_splitter_max_height_get(Ender_Element *e, double cmv)
{
	return DBL_MAX;
}

static Ender_Element * _eon_splitter_element_at(Ender_Element *e, double x, double y)
{
#if 0
	Eon_Splitter *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *bar_r;
	Eon_Size size;
	double ax, ay;

	r = ender_element_renderer_get(e);
	thiz = _eon_splitter_get(r);

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
#endif
	return NULL;
}

static void _eon_splitter_initialize(Ender_Element *e)
{
	Eon_Splitter *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_splitter_get(r);
}

static Eina_Bool _eon_splitter_setup(Ender_Element *e)
{
	Eon_Splitter *thiz;
	Eon_Size size;
	Ender_Element *content;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *content_r;
	double percent;
	double max, min;
	double thumb_size;
	double length;

	r = ender_element_renderer_get(e);
	thiz = _eon_splitter_get(r);

	eon_container_content_get(e, &content);
	if (!content) return EINA_FALSE;
#if 0
	if (!eon_element_setup(content))
	{
		printf("impossible to setup the content\n");
		return EINA_FALSE;
	}
	theme_r = eon_widget_theme_renderer_get(r);
#endif

	eon_element_actual_size_get(r, &size);
	if (thiz->orientation == EON_ORIENTATION_HORIZONTAL)
		length = size.width;
	else
		length = size.height;

	return EINA_TRUE;
}

static void _eon_splitter_free(Enesim_Renderer *r)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	free(thiz);
}

static Eon_Container_Descriptor _eon_splitter_container_descriptor = {
	.name = "splitter",
	.initialize = _eon_splitter_initialize,
	.free = _eon_splitter_free,
	.setup = _eon_splitter_setup,
	.min_width_get = _eon_splitter_min_width_get,
	.max_width_get = _eon_splitter_max_width_get,
	.min_height_get = _eon_splitter_min_height_get,
	.max_height_get = _eon_splitter_max_height_get,
	//.preferred_width_get = _eon_splitter_preferred_width_get,
	//.preferred_height_get = _eon_splitter_preferred_height_get,
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
	thiz->position = 0.5;
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
	thiz->orientation = orientation;
	eon_widget_property_set(r, "orientation", orientation, NULL);
}

static void _eon_splitter_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	*orientation = thiz->orientation;
}

static void _eon_splitter_second_content_set(Enesim_Renderer *r, Ender_Element *second_content)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	thiz->second_content = second_content;
	eon_widget_property_set(r, "second_content", second_content, NULL);
}

static void _eon_splitter_second_content_get(Enesim_Renderer *r, Ender_Element **second_content)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	*second_content = thiz->second_content;
}

static void _eon_splitter_position_set(Enesim_Renderer *r, double position)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	thiz->position = position;
	eon_widget_property_set(r, "position", position, NULL);
}

static void _eon_splitter_position_get(Enesim_Renderer *r, double *position)
{
	Eon_Splitter *thiz;

	thiz = _eon_splitter_get(r);
	if (!thiz) return;
	*position = thiz->position;
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

