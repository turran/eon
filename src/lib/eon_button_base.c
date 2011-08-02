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
typedef struct _Eon_Button_Base
{
	/* private */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	void *data;
} Eon_Button_Base;

static inline Eon_Button_Base * _eon_button_base_get(Enesim_Renderer *r)
{
	Eon_Button_Base *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_button_base_initialize(Ender_Element *e)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);
	if (thiz->initialize)
		thiz->initialize(e);
}

static double _eon_button_base_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v = 0;
	double min;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	v += cmv;

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_button_base_decoration_size_get(theme_r, &min, NULL);
	v += min;

	return v;
}

static double _eon_button_base_max_width_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	Ender_Element *content;
	double v = 0;
	double max;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	v += cmv;

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_button_base_decoration_size_get(theme_r, &max, NULL);
	v += max;

	return v;
}

static double _eon_button_base_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	Ender_Element *content;
	double v = 0;
	double min;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	v += cmv;

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_button_base_decoration_size_get(theme_r, NULL, &min);
	v += min;

	return v;
}

static double _eon_button_base_max_height_get(Ender_Element *e, double cmv)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	Ender_Element *content;
	double v = 0;
	double max;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);

	v += cmv;

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_button_base_decoration_size_get(theme_r, NULL, &max);
	v += max;

	return v;
}

static Eina_Bool _eon_button_base_setup(Ender_Element *e)
{
	Eon_Button_Base *thiz;
	Enesim_Renderer *r;
	Ender_Element *content;

	r = ender_element_renderer_get(e);
	thiz = _eon_button_base_get(r);
	eon_container_content_get(e, &content);
	if (content)
	{
		Enesim_Renderer *content_r;
		Enesim_Renderer *theme_r;
		double aw, ah;
		double ax, ay;
		double dw = 0;
		double dh = 0;
		double cx = 0;
		double cy = 0;

		content_r = ender_element_renderer_get(content);
		theme_r = eon_widget_theme_renderer_get(r);
		/* set the size and position */
		eon_element_actual_width_get(e, &aw);
		eon_element_actual_height_get(e, &ah);
		eon_element_actual_position_get(r, &ax, &ay);
		printf("current geometry %g %g %g %g\n", aw, ah, ax, ay);
		eon_theme_button_base_decoration_size_get(theme_r, &dw, &dh);
		printf("decoration %g %g\n", dw, dh);
		eon_element_actual_size_set(content_r, aw - dw, ah - dh);
		if (!eon_element_setup(content))
		{
			printf("impossible to setup the content\n");
			return EINA_FALSE;
		}
		eon_theme_button_base_content_position_get(theme_r, &cx, &cy);
		printf("setting size %g %g and position %g %g (%g %g)\n", aw - dw, ah - dh, ax + cx, ay + cy, cx, cy);
		//eon_element_actual_position_set(content_r, ax + cx, ay + cy);
		eon_element_actual_position_set(content_r, cx, cy);
	}
	if (thiz->setup)
		return thiz->setup(e);

	return EINA_TRUE;
}

static void _eon_button_base_free(Enesim_Renderer *r)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

#include "eon_generated_button_base.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_button_base_data_get(Enesim_Renderer *r)
{
	Eon_Button_Base *thiz;

	thiz = _eon_button_base_get(r);
	if (!thiz) return NULL;
	return thiz->data;
}

Enesim_Renderer * eon_button_base_new(Eon_Button_Base_Descriptor *descriptor, void *data)
{
	Eon_Button_Base *thiz;
	Eon_Container_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Button_Base));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;

	pdescriptor.initialize = _eon_button_base_initialize;
	pdescriptor.free = _eon_button_base_free;
	pdescriptor.setup = _eon_button_base_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.min_width_get = _eon_button_base_min_width_get;
	pdescriptor.max_width_get = _eon_button_base_max_width_get;
	pdescriptor.min_height_get = _eon_button_base_min_height_get;
	pdescriptor.max_height_get = _eon_button_base_max_height_get;

	r = eon_container_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
