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
typedef struct _Eon_Spin
{
	/* properties */
	double min;
	double max;
	double step_increment;
	/* private */
} Eon_Spin;

static inline Eon_Spin * _eon_spin_get(Enesim_Renderer *r)
{
	Eon_Spin *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static double _eon_spin_min_width_get(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_spin_min_width_get(theme_r, &v);

	return v;
}

static double _eon_spin_max_width_get(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_spin_max_width_get(theme_r, &v);

	return v;
}

static double _eon_spin_min_height_get(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_spin_min_height_get(theme_r, &v);

	return v;
}

static double _eon_spin_max_height_get(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_spin_max_height_get(theme_r, &v);

	return v;
}

static double _eon_spin_preferred_width_get(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_spin_preferred_width_get(theme_r, &v);

	return v;
}

static double _eon_spin_preferred_height_get(Ender_Element *e)
{
	Eon_Spin *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_spin_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	//eon_theme_spin_preferred_height_get(theme_r, &v);

	return v;
}

static void _eon_spin_initialize(Ender_Element *ender)
{
	/* register every needed callback */
}

static Eon_Widget_Descriptor _eon_spin_widget_descriptor = {
	.initialize = _eon_spin_initialize,
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
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Spin));
	if (!thiz) return NULL;

	r = eon_widget_new(&_eon_spin_widget_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_spin_max_range_set(Enesim_Renderer *r, double max_range)
{

}

static void _eon_spin_max_range_get(Enesim_Renderer *r, double *max_range)
{

}

static void _eon_spin_min_range_set(Enesim_Renderer *r, double min_range)
{

}

static void _eon_spin_min_range_get(Enesim_Renderer *r, double *min_range)
{

}

static void _eon_spin_step_increment_set(Enesim_Renderer *r, double step_increment)
{

}

static void _eon_spin_step_increment_get(Enesim_Renderer *r, double *step_increment)
{

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
