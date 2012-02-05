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
/* TODO
 * Add a simple
 * eon_frame_new_with_label(char *label);
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Frame
{
} Eon_Frame;

static inline Eon_Frame * _eon_frame_get(Enesim_Renderer *r)
{
	Eon_Frame *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static Eina_Bool _eon_frame_content_relayout(Ender_Element *content,
		Enesim_Renderer *theme_r,
		double ax,
		double ay,
		double aw,
		double ah,
		Enesim_Surface *s,
		Enesim_Error **err)
{
	Eon_Margin margin;
	Enesim_Renderer *content_r;

	content_r = ender_element_renderer_get(content);

	eon_theme_frame_margin_get(theme_r, &margin);
	eon_element_actual_size_set(content_r, aw - margin.left - margin.right,
			ah - margin.bottom - margin.top);
	eon_element_actual_position_set(content_r, margin.left, margin.top);
	if (!eon_element_setup(content, s, err))
	{
		printf("impossible to setup the content\n");
		return EINA_FALSE;
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                        The Eon's widget interface                          *
 *----------------------------------------------------------------------------*/
static void _eon_frame_free(Enesim_Renderer *r)
{
	Eon_Frame *thiz;

	thiz = _eon_frame_get(r);
	free(thiz);
}

static Eina_Bool _eon_frame_setup(Ender_Element *e,
		const Eon_Element_State *state,
		const Eon_Container_State *cstate,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	Eina_Bool ret = EINA_TRUE;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	/* setup the content */
	if (cstate->content)
	{
		Enesim_Renderer *theme_r;

		theme_r = eon_widget_theme_renderer_get(r);
		ret =_eon_frame_content_relayout(cstate->content, theme_r,
				state->actual_position.x,
				state->actual_position.y,
				state->actual_size.width,
				state->actual_size.height,
				s,
				err);
	}

	return ret;
}

static double _eon_frame_min_max_width_get(Ender_Element *e, double cmv)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Eon_Margin margin;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_frame_margin_get(theme_r, &margin);

	return cmv + margin.left + margin.right;
}

static double _eon_frame_min_max_height_get(Ender_Element *e, double cmv)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;
	Eon_Margin margin;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_frame_margin_get(theme_r, &margin);

	return cmv + margin.top + margin.bottom;
}

static Eon_Container_Descriptor _descriptor = {
	.free = _eon_frame_free,
	.setup = _eon_frame_setup,
	.min_width_get = _eon_frame_min_max_width_get,
	.min_height_get = _eon_frame_min_max_height_get,
	.max_width_get = _eon_frame_min_max_width_get,
	.max_height_get = _eon_frame_min_max_height_get,
	.name = "frame",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_frame_new(void)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Frame));
	if (!thiz) return NULL;

	r = eon_container_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_frame_description_get(Enesim_Renderer *r, const char **description)
{
	eon_widget_property_get(r, "description", description, NULL);
}

static void _eon_frame_description_set(Enesim_Renderer *r, const char *description)
{
	eon_widget_property_set(r, "description", description, NULL);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#include "eon_generated_frame.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_frame_new(void)
{
	return ender_element_new_with_namespace("frame", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_frame_description_get(Ender_Element *e, const char **description)
{
	ender_element_value_get(e, "description", description, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_frame_description_set(Ender_Element *e, const char *description)
{
	ender_element_value_set(e, "description", description, NULL);
}
