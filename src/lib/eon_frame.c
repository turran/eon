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
	Ender_Element *description;
} Eon_Frame;

static inline Eon_Frame * _eon_frame_get(Enesim_Renderer *r)
{
	Eon_Frame *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
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

static Eina_Bool _eon_frame_setup(Ender_Element *e)
{
	Eon_Frame *thiz;
	Ender_Element *content;
	Enesim_Renderer *r;
	Enesim_Renderer *description_r;
	Eina_Bool ret = EINA_TRUE;
	double rw;
	double rh;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	/* setup the content */
	eon_container_content_get(e, &content);
	if (content)
	{
		if (!eon_element_setup(content))
		{
			printf("impossible to setup the content\n");
			return EINA_FALSE;
		}
	}
	/* set the size and position of the description */
	if (thiz->description)
	{
		description_r = ender_element_renderer_get(thiz->description);
		eon_element_real_width_get(thiz->description, &rw);
		eon_element_real_height_get(thiz->description, &rh);
		eon_element_actual_size_set(description_r, rw, rh);
		eon_element_actual_position_set(description_r, 5, 5);
		printf("setting size %g %g\n", rw, rh);

		ret = eon_element_setup(thiz->description);

	}

	return ret;
}

static double _eon_frame_min_width_get(Ender_Element *e, double cmv)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	if (thiz->description)
	{
		eon_element_real_width_get(thiz->description, &v);
	}
	
	return v + cmv;
}

static double _eon_frame_max_width_get(Ender_Element *e, double cmv)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	if (thiz->description)
	{
		eon_element_real_width_get(thiz->description, &v);
	}

	return v + cmv;
}

static double _eon_frame_min_height_get(Ender_Element *e, double cmv)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	if (thiz->description)
	{
		eon_element_real_height_get(thiz->description, &v);
	}

	return v + cmv;
}

static double _eon_frame_max_height_get(Ender_Element *e, double cmv)
{
	Eon_Frame *thiz;
	Enesim_Renderer *r;
	double v = 0;

	r = ender_element_renderer_get(e);
	thiz = _eon_frame_get(r);
	if (thiz->description)
	{
		eon_element_real_height_get(thiz->description, &v);
	}

	return v + cmv;
}

static Eon_Container_Descriptor _descriptor = {
	.free = _eon_frame_free,
	.setup = _eon_frame_setup,
	.min_width_get = _eon_frame_min_width_get,
	.min_height_get = _eon_frame_min_height_get,
	.max_width_get = _eon_frame_max_width_get,
	.max_height_get = _eon_frame_max_height_get,
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

static void _eon_frame_description_get(Enesim_Renderer *r, Ender_Element **e)
{
	Eon_Frame *thiz;

	thiz = _eon_frame_get(r);
	*e = thiz->description;
}

static void _eon_frame_description_set(Enesim_Renderer *r, Ender_Element *description)
{
	Eon_Frame *thiz;
	Enesim_Renderer *description_r;
	Enesim_Renderer *description_rr;

	thiz = _eon_frame_get(r);
	if (!thiz) return;

	description_r = ender_element_renderer_get(description);
	if (!eon_is_element(description_r))
		return;
	thiz->description = description;
	description_rr = eon_element_renderer_get(description);
	eon_widget_property_set(r, "description", description_rr, NULL);
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
EAPI void eon_frame_description_get(Ender_Element *e, Ender_Element **description)
{
	ender_element_value_get(e, "description", description, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_frame_description_set(Ender_Element *e, Ender_Element *description)
{
	ender_element_value_set(e, "description", description, NULL);
}
