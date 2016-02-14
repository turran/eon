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
typedef struct _Eon_Theme_Mars_Paned
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *border_color;
	Egueb_Dom_Node *area_border_color;
	/* private */
	Enesim_Renderer *area;
	Enesim_Renderer *compound;
	Enesim_Renderer *content1;
	Enesim_Renderer *content2;
	Enesim_Renderer *splitter;
} Eon_Theme_Mars_Paned;
/*----------------------------------------------------------------------------*
 *                              Paned interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_paned_version_get(void)
{
	return EON_THEME_ELEMENT_PANED_VERSION;
}

static void _eon_theme_mars_paned_dtor(void *data)
{
	Eon_Theme_Mars_Paned *thiz = data;

	enesim_renderer_unref(thiz->area);
	enesim_renderer_unref(thiz->compound);
	enesim_renderer_unref(thiz->content1);
	enesim_renderer_unref(thiz->content2);
	enesim_renderer_unref(thiz->splitter);
	free(thiz);
}

static const char * _eon_theme_mars_paned_tag_name_get(void)
{
	return "paned";
}

static Eina_Bool _eon_theme_mars_paned_process(void *data)
{
	Eon_Theme_Mars_Paned *thiz;
	Eina_Rectangle geom;
	Enesim_Argb argb;
	Enesim_Color border_color;
	Enesim_Color area_border_color;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->border_color, &argb);
	border_color = enesim_color_argb_from(argb);
	egueb_dom_attr_final_get(thiz->area_border_color, &argb);
	area_border_color = enesim_color_argb_from(argb);
	/* get the inherited members */
	eon_theme_renderable_geometry_get(thiz->n, &geom);

	/* setup the whole rectangle */
	enesim_renderer_rectangle_position_set(thiz->area, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->area, geom.w, geom.h);
	/* setup the splitter */
	enesim_renderer_shape_stroke_color_set(thiz->splitter, border_color);
	/* setup the areas */
	if (!area_border_color)
	{
		enesim_renderer_shape_draw_mode_set(thiz->content1,
				ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
		enesim_renderer_shape_draw_mode_set(thiz->content2,
				ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	}
	else
	{
		enesim_renderer_shape_draw_mode_set(thiz->content1,
				ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
		enesim_renderer_shape_stroke_color_set(thiz->content1, area_border_color);
		enesim_renderer_shape_draw_mode_set(thiz->content2,
				ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
		enesim_renderer_shape_stroke_color_set(thiz->content2, area_border_color);
	}

	return EINA_TRUE;
}

static Enesim_Renderer * _eon_theme_mars_paned_renderer_get(void *data)
{
	Eon_Theme_Mars_Paned *thiz = data;
	return enesim_renderer_ref(thiz->area);
}

static void _eon_theme_mars_paned_padding_get(void *data, Eon_Box *padding)
{
	padding->top = padding->bottom = padding->left = padding->right = 0;
}

static void _eon_theme_mars_paned_first_content_set(void *data, Enesim_Renderer *r, Eina_Rectangle *area)
{
	Eon_Theme_Mars_Paned *thiz = data;
	Enesim_Renderer_Compound_Layer *l;
	Enesim_Renderer *other;

	enesim_renderer_compound_layer_clear(thiz->compound);
	other = enesim_renderer_shape_fill_renderer_get(thiz->content2);

	if (r)
	{
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
		enesim_renderer_compound_layer_renderer_set(l,
				enesim_renderer_ref(thiz->content1));
		enesim_renderer_compound_layer_add(thiz->compound, l);

		enesim_renderer_rectangle_position_set(thiz->content1, area->x, area->y);
		enesim_renderer_rectangle_size_set(thiz->content1, area->w, area->h);
	}

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->splitter));
	enesim_renderer_compound_layer_add(thiz->compound, l);

	if (other)
	{
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
		enesim_renderer_compound_layer_renderer_set(l,
				enesim_renderer_ref(thiz->content2));
		enesim_renderer_compound_layer_add(thiz->compound, l);
		enesim_renderer_unref(other);
	}
	enesim_renderer_shape_fill_renderer_set(thiz->content1, r);
}

static void _eon_theme_mars_paned_second_content_set(void *data, Enesim_Renderer *r, Eina_Rectangle *area)
{
	Eon_Theme_Mars_Paned *thiz = data;
	Enesim_Renderer_Compound_Layer *l;
	Enesim_Renderer *other;

	enesim_renderer_compound_layer_clear(thiz->compound);
	other = enesim_renderer_shape_fill_renderer_get(thiz->content1);

	if (other)
	{
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
		enesim_renderer_compound_layer_renderer_set(l,
				enesim_renderer_ref(thiz->content1));
		enesim_renderer_compound_layer_add(thiz->compound, l);
		enesim_renderer_unref(other);
	}

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->splitter));
	enesim_renderer_compound_layer_add(thiz->compound, l);

	if (r)
	{
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
		enesim_renderer_compound_layer_renderer_set(l,
				enesim_renderer_ref(thiz->content2));
		enesim_renderer_compound_layer_add(thiz->compound, l);

		enesim_renderer_rectangle_position_set(thiz->content2, area->x, area->y);
		enesim_renderer_rectangle_size_set(thiz->content2, area->w, area->h);
	}
	enesim_renderer_shape_fill_renderer_set(thiz->content2, r);
}

static void _eon_theme_mars_paned_splitter_area_set(void *data, Eina_Rectangle *area)
{
	Eon_Theme_Mars_Paned *thiz = data;
	Eon_Orientation orientation;
	int x0, y0, x1, y1;

	orientation = eon_theme_element_paned_orientation_get(thiz->n);
	if (orientation == EON_ORIENTATION_HORIZONTAL)
	{
		x0 = x1 = area->x + (area->w / 2);
		y0 = (area->y + (area->h / 2)) - (EON_THEME_MARS_MARGIN * 4);
		y1 = (area->y + (area->h / 2)) + (EON_THEME_MARS_MARGIN * 4);
	}
	else
	{
		y0 = y1 = area->y + (area->h / 2);
		x0 = (area->x + (area->w / 2)) - (EON_THEME_MARS_MARGIN * 4);
		x1 = (area->x + (area->w / 2)) + (EON_THEME_MARS_MARGIN * 4);
	}
	enesim_renderer_line_coords_set(thiz->splitter, x0, y0, x1, y1);

}

static int _eon_theme_mars_paned_min_length_get(void *data)
{
	Eon_Theme_Mars_Paned *thiz = data;
	return EON_THEME_MARS_MARGIN * 4;
}

static int _eon_theme_mars_paned_thickness_get(void *data)
{
	Eon_Theme_Mars_Paned *thiz = data;
	return EON_THEME_MARS_MARGIN * 2;
}

static Eon_Theme_Element_Paned_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_paned_version_get,
	/* .ctor 		= */ eon_theme_mars_paned_new,
	/* .dtor 		= */ _eon_theme_mars_paned_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_paned_tag_name_get,
	/* .process 		= */ _eon_theme_mars_paned_process,
	/* .renderer_get	= */ _eon_theme_mars_paned_renderer_get,
	/* .first_content_set	= */ _eon_theme_mars_paned_first_content_set,
	/* .second_content_set	= */ _eon_theme_mars_paned_second_content_set,
	/* .splitter_area_set	= */ _eon_theme_mars_paned_splitter_area_set,
	/* .min_length_get	= */ _eon_theme_mars_paned_min_length_get,
	/* .thickness_get	= */ _eon_theme_mars_paned_thickness_get,
	/* .padding_get		= */ _eon_theme_mars_paned_padding_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_paned_new(void)
{
	Eon_Theme_Mars_Paned *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Paned));

	thiz->compound = enesim_renderer_compound_new();
	enesim_renderer_compound_background_enable_set(thiz->compound, EINA_TRUE);
	enesim_renderer_compound_background_color_set(thiz->compound, 0x000000);

	thiz->area = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->area,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_shape_fill_renderer_set(thiz->area,
			enesim_renderer_ref(thiz->compound));
	enesim_renderer_shape_stroke_weight_set(thiz->area,
			EON_THEME_MARS_BORDER);

	thiz->content1 = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->content1,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(thiz->content1,
			EON_THEME_MARS_BORDER);

	thiz->content2 = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->content2,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(thiz->content2,
			EON_THEME_MARS_BORDER);

	thiz->splitter = enesim_renderer_line_new();
	enesim_renderer_shape_draw_mode_set(thiz->splitter,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
	enesim_renderer_shape_stroke_weight_set(thiz->splitter,
			EON_THEME_MARS_BORDER);

	n = eon_theme_element_paned_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_chars("border-color");
	thiz->border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->border_color), NULL);
	s = egueb_dom_string_new_with_static_chars("area-border-color");
	thiz->area_border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->area_border_color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
