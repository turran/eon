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
typedef struct _Eon_Theme_Mars_Scale
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *acolor;
	Egueb_Dom_Node *icolor;
	/* private */
	Enesim_Renderer *compound;
	Enesim_Renderer *aline;
	Enesim_Renderer *iline;
	Enesim_Renderer *knob;
} Eon_Theme_Mars_Scale;

/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_scale_version_get(void)
{
	return EON_THEME_ELEMENT_BUTTON_VERSION;
}

static void _eon_theme_mars_scale_dtor(void *data)
{
	Eon_Theme_Mars_Scale *thiz = data;

	/* attributes */
	egueb_dom_node_unref(thiz->acolor);
	egueb_dom_node_unref(thiz->icolor);
	/* private */
	enesim_renderer_unref(thiz->compound);
	enesim_renderer_unref(thiz->aline);
	enesim_renderer_unref(thiz->iline);
	enesim_renderer_unref(thiz->knob);
	free(thiz);
}

static const char * _eon_theme_mars_scale_tag_name_get(void)
{
	return "scale";
}

static Eina_Bool _eon_theme_mars_scale_process(void *data)
{
	Eon_Theme_Mars_Scale *thiz;
	Eon_Orientation orientation;
	Enesim_Argb argb;
	Enesim_Color acolor, icolor;
	Eina_Rectangle geom;
	Eina_Bool aactive = EINA_TRUE, iactive = EINA_TRUE;
	double value;
	int ax0, ay0, ax1, ay1;
	int ix0, iy0, ix1, iy1;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->acolor, &argb);
	acolor = enesim_color_argb_from(argb);
	egueb_dom_attr_final_get(thiz->icolor, &argb);
	icolor = enesim_color_argb_from(argb);
	/* get the inherited members */
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	orientation = eon_theme_element_scale_orientation_get(thiz->n);
	value = eon_theme_element_scale_value_get(thiz->n);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		int length = geom.w - (EON_THEME_MARS_MARGIN * 2);
		ax0 = geom.x + EON_THEME_MARS_MARGIN;
		ax1 = ax0 + (length * value);
		ix0 = ax1;
		ix1 = geom.x + geom.w - EON_THEME_MARS_MARGIN;
		ay0 = ay1 = iy0 = iy1 = geom.y + (geom.h / 2);
	}
	else
	{
		int length = geom.h - (EON_THEME_MARS_MARGIN * 2);
		ay0 = geom.y + EON_THEME_MARS_MARGIN;
		ay1 = ay0 + (length * value);
		iy0 = ay1;
		iy1 = geom.y + geom.h - EON_THEME_MARS_MARGIN;
		ax0 = ax1 = ix0 = ix1 = geom.x + (geom.w / 2);
	}
	enesim_renderer_line_coords_set(thiz->aline, ax0, ay0, ax1, ay1);
	enesim_renderer_shape_stroke_color_set(thiz->aline, acolor);
	enesim_renderer_visibility_set(thiz->aline, aactive);
		
	enesim_renderer_line_coords_set(thiz->iline, ix0, iy0, ix1, iy1);
	enesim_renderer_shape_stroke_color_set(thiz->iline, icolor);
	enesim_renderer_visibility_set(thiz->iline, iactive);

	enesim_renderer_shape_fill_color_set(thiz->knob, acolor);
	enesim_renderer_circle_center_set(thiz->knob, ax1, ay1);

	return EINA_TRUE;
}

static int _eon_theme_mars_scale_thickness_get(void *data)
{
	return EON_THEME_MARS_MARGIN * 2;
}

static int _eon_theme_mars_scale_min_length_get(void *data)
{
	Eon_Theme_Mars_Scale *thiz = data;
	return EON_THEME_MARS_MARGIN * 2;
}

static Enesim_Renderer * _eon_theme_mars_scale_renderer_get(void *data)
{
	Eon_Theme_Mars_Scale *thiz = data;
	return enesim_renderer_ref(thiz->compound);
}

static Eon_Theme_Element_Scale_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_scale_version_get,
	/* .ctor 		= */ eon_theme_mars_scale_new,
	/* .dtor 		= */ _eon_theme_mars_scale_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_scale_tag_name_get,
	/* .process 		= */ _eon_theme_mars_scale_process,
	/* .renderer_get	= */ _eon_theme_mars_scale_renderer_get,
	/* .min_length_get	= */ _eon_theme_mars_scale_min_length_get,
	/* .thickness_get	= */ _eon_theme_mars_scale_thickness_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_scale_new(void)
{
	Eon_Theme_Mars_Scale *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Scale));

	/* the renderers */
	thiz->aline = enesim_renderer_line_new();
	enesim_renderer_shape_draw_mode_set(thiz->aline, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
	enesim_renderer_shape_stroke_weight_set(thiz->aline, EON_THEME_MARS_BORDER);

	thiz->iline = enesim_renderer_line_new();
	enesim_renderer_shape_draw_mode_set(thiz->iline, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
	enesim_renderer_shape_stroke_weight_set(thiz->iline, EON_THEME_MARS_BORDER);

	thiz->knob = enesim_renderer_circle_new();
	enesim_renderer_shape_draw_mode_set(thiz->knob, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_circle_radius_set(thiz->knob, EON_THEME_MARS_MARGIN);

	thiz->compound = enesim_renderer_compound_new();
	enesim_renderer_compound_background_enable_set(thiz->compound, EINA_TRUE);
	enesim_renderer_compound_background_color_set(thiz->compound, 0x000000);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->aline));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->compound, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->iline));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->compound, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->knob));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->compound, l);

	n = eon_theme_element_scale_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_chars("inactive-color");
	thiz->icolor = egueb_css_attr_color_new(s, NULL, EINA_TRUE, EINA_TRUE,
			EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->icolor), NULL);
	s = egueb_dom_string_new_with_static_chars("active-color");
	thiz->acolor = egueb_css_attr_color_new(s, NULL, EINA_TRUE, EINA_TRUE,
			EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->acolor), NULL);

	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

