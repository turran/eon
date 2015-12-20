/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
#include "eon_theme_mars_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Mars_Progress_Bar
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *color;
	/* private */
	Enesim_Renderer *bar;
	Enesim_Renderer *progress;
} Eon_Theme_Mars_Progress_Bar;

/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_progress_bar_version_get(void)
{
	return EON_THEME_ELEMENT_PROGRESS_BAR_VERSION;
}

static void _eon_theme_mars_progress_bar_dtor(void *data)
{
	Eon_Theme_Mars_Progress_Bar *thiz = data;

	/* attributes */
	egueb_dom_node_unref(thiz->color);
	/* private */
	enesim_renderer_unref(thiz->bar);
	enesim_renderer_unref(thiz->progress);
	free(thiz);
}

static const char * _eon_theme_mars_progress_bar_tag_name_get(void)
{
	return "progress_bar";
}

static Eina_Bool _eon_theme_mars_progress_bar_process(void *data)
{
	Eon_Theme_Mars_Progress_Bar *thiz;
	Eon_Orientation orientation;
	Eina_Rectangle geom;
	Enesim_Argb argb;
	Enesim_Color color;
	Eina_Bool show_progress = EINA_TRUE;
	double progression;
	double w, h;
	double pw, ph;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->color, &argb);
	color = enesim_color_argb_from(argb);
	/* get the inherited members */
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	progression = eon_theme_element_progress_bar_progression_get(thiz->n);
	orientation = eon_theme_element_progress_bar_orientation_get(thiz->n);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		pw = geom.w * progression;
		if ((int)pw <= 0)
			show_progress = EINA_FALSE;
		ph = h = EON_THEME_MARS_MARGIN * 2;
		w = geom.w;
	}
	else
	{
		pw = w = EON_THEME_MARS_MARGIN * 2;
		ph = geom.h * progression;
		if ((int)ph <= 0.0)
			show_progress = EINA_FALSE;
		h = geom.h;
	}
	enesim_renderer_rectangle_position_set(thiz->bar, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->bar, w, h);

	if (!show_progress)
	{
		enesim_renderer_shape_draw_mode_set(thiz->bar, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
		enesim_renderer_shape_fill_renderer_set(thiz->bar, NULL);
	}
	else
	{
		enesim_renderer_shape_draw_mode_set(thiz->bar, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
		enesim_renderer_shape_fill_renderer_set(thiz->bar, enesim_renderer_ref(thiz->progress));
	}
	enesim_renderer_shape_fill_color_set(thiz->progress, color);
	enesim_renderer_rectangle_position_set(thiz->progress, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->progress, pw, ph);

	return EINA_TRUE;
}

static int _eon_theme_mars_progress_bar_thickness_get(void *data)
{
	return EON_THEME_MARS_MARGIN * 2;
}

static Enesim_Renderer * _eon_theme_mars_progress_bar_renderer_get(void *data)
{
	Eon_Theme_Mars_Progress_Bar *thiz = data;
	return enesim_renderer_ref(thiz->bar);
}

static Eon_Theme_Element_Progress_Bar_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_progress_bar_version_get,
	/* .ctor 		= */ eon_theme_mars_progress_bar_new,
	/* .dtor 		= */ _eon_theme_mars_progress_bar_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_progress_bar_tag_name_get,
	/* .process 		= */ _eon_theme_mars_progress_bar_process,
	/* .renderer_get	= */ _eon_theme_mars_progress_bar_renderer_get,
	/* .thickness_get	= */ _eon_theme_mars_progress_bar_thickness_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_progress_bar_new(void)
{
	Eon_Theme_Mars_Progress_Bar *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Progress_Bar));
	/* the progress */
	thiz->progress = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->progress, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	/* the bar */
	thiz->bar = enesim_renderer_rectangle_new();
	enesim_renderer_shape_stroke_weight_set(thiz->bar, 1.0);

	n = eon_theme_element_progress_bar_new(&_descriptor, thiz);
	/* the attributes */
	thiz->color = egueb_css_attr_color_new(
			egueb_dom_string_ref(EON_NAME_COLOR), NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
