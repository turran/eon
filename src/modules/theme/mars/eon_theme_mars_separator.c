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
typedef struct _Eon_Theme_Mars_Separator
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *color;
	/* private */
	Enesim_Renderer *line;
} Eon_Theme_Mars_Separator;

/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_separator_version_get(void)
{
	return EON_THEME_ELEMENT_BUTTON_VERSION;
}

static void _eon_theme_mars_separator_dtor(void *data)
{
	Eon_Theme_Mars_Separator *thiz = data;

	/* attributes */
	egueb_dom_node_unref(thiz->color);
	/* private */
	enesim_renderer_unref(thiz->line);
	free(thiz);
}

static const char * _eon_theme_mars_separator_tag_name_get(void)
{
	return "separator";
}

static Eina_Bool _eon_theme_mars_separator_process(void *data)
{
	Eon_Theme_Mars_Separator *thiz;
	Eina_Rectangle geom;
	Enesim_Argb argb;
	Enesim_Color color;
	Eon_Orientation orientation;
	int x0, y0, x1, y1;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->color, &argb);
	color = enesim_color_argb_from(argb);
	/* get the inherited members */
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	orientation = eon_theme_element_separator_orientation_get(thiz->n);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		x0 = geom.x + EON_THEME_MARS_MARGIN;
		x1 = geom.x + geom.w - (EON_THEME_MARS_MARGIN * 2);
		y0 = y1 = geom.y + (geom.h / 2);
	}
	else
	{
		y0 = geom.y + EON_THEME_MARS_MARGIN;
		y1 = geom.y + geom.h - (EON_THEME_MARS_MARGIN * 2);
		x0 = x1 = geom.x + (geom.w / 2);
	}
	enesim_renderer_line_coords_set(thiz->line, x0, y0, x1, y1);
	enesim_renderer_shape_stroke_color_set(thiz->line, color);

	return EINA_TRUE;
}

static int _eon_theme_mars_separator_thickness_get(void *data)
{
	return EON_THEME_MARS_MARGIN * 2;
}

static Enesim_Renderer * _eon_theme_mars_separator_renderer_get(void *data)
{
	Eon_Theme_Mars_Separator *thiz = data;
	return enesim_renderer_ref(thiz->line);
}

static Eon_Theme_Element_Separator_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_separator_version_get,
	/* .ctor 		= */ eon_theme_mars_separator_new,
	/* .dtor 		= */ _eon_theme_mars_separator_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_separator_tag_name_get,
	/* .process 		= */ _eon_theme_mars_separator_process,
	/* .renderer_get	= */ _eon_theme_mars_separator_renderer_get,
	/* .thickness_get	= */ _eon_theme_mars_separator_thickness_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_separator_new(void)
{
	Eon_Theme_Mars_Separator *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Separator));
	/* the separator button */
	thiz->line = enesim_renderer_line_new();
	enesim_renderer_shape_draw_mode_set(thiz->line, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
	enesim_renderer_shape_stroke_weight_set(thiz->line, EON_THEME_MARS_BORDER);

	n = eon_theme_element_separator_new(&_descriptor, thiz);
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


