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
static Ender_Property *EON_PROGRESSBAR_PROGRESSION;

typedef struct _Eon_Progressbar
{
	/* properties */
	/* private */
} Eon_Progressbar;

static inline Eon_Progressbar * _eon_progressbar_get(Eon_Element *ee)
{
	Eon_Progressbar *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                        *
 *----------------------------------------------------------------------------*/
static double _eon_progressbar_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_progressbar_min_width_get(theme_r, &v);

	return v;
}

static double _eon_progressbar_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_progressbar_max_width_get(theme_r, &v);

	return v;
}

static double _eon_progressbar_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_progressbar_min_height_get(theme_r, &v);

	return v;
}

static double _eon_progressbar_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_progressbar_max_height_get(theme_r, &v);

	return v;
}

static double _eon_progressbar_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_progressbar_preferred_width_get(theme_r, &v);

	return v;
}

static double _eon_progressbar_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_progressbar_preferred_height_get(theme_r, &v);

	return v;
}

static void _eon_progressbar_initialize(Ender_Element *ender)
{
	/* register every needed callback */
}

static Eon_Widget_Descriptor _eon_progressbar_widget_descriptor = {
	.initialize = _eon_progressbar_initialize,
	.name = "progressbar",
	.min_width_get = _eon_progressbar_min_width_get,
	.max_width_get = _eon_progressbar_max_width_get,
	.min_height_get = _eon_progressbar_min_height_get,
	.max_height_get = _eon_progressbar_max_height_get,
	.preferred_width_get = _eon_progressbar_preferred_width_get,
	.preferred_height_get = _eon_progressbar_preferred_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_progressbar_new(void)
{
	Eon_Progressbar *thiz;
	Eon_Element *ee;

	thiz = calloc(1, sizeof(Eon_Progressbar));
	if (!thiz) return NULL;

	ee = eon_widget_new(&_eon_progressbar_widget_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_progressbar_progression_get(Eon_Element *ee, double *progression)
{
	Eon_Progressbar *thiz;

	thiz = _eon_progressbar_get(ee);
	if (!thiz) return;

}

static void _eon_progressbar_progression_set(Eon_Element *ee, double progression)
{
	Eon_Progressbar *thiz;

	thiz = _eon_progressbar_get(ee);
	if (!thiz) return;

	eon_widget_property_set(ee, "progression", progression, NULL);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_progressbar_delete NULL
#include "eon_generated_progressbar.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_progressbar_new(void)
{
	return ender_element_new_with_namespace("progressbar", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_progressbar_progression_get(Ender_Element *e, double *progression)
{
	ender_element_value_get(e, "progression", progression, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_progressbar_progression_set(Ender_Element *e, double progression)
{
	ender_element_value_set(e, "progression", progression, NULL);
}
