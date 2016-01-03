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
typedef struct _Eon_Theme_Mars_Expander
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
} Eon_Theme_Mars_Expander;
/*----------------------------------------------------------------------------*
 *                              Expander interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_expander_version_get(void)
{
	return EON_THEME_ELEMENT_EXPANDER_VERSION;
}

static void _eon_theme_mars_expander_dtor(void *data)
{
	Eon_Theme_Mars_Expander *thiz = data;

	enesim_renderer_unref(thiz->area);
	enesim_renderer_unref(thiz->compound);
	enesim_renderer_unref(thiz->content1);
	enesim_renderer_unref(thiz->content2);
	free(thiz);
}

static const char * _eon_theme_mars_expander_tag_name_get(void)
{
	return "expander";
}

static Eina_Bool _eon_theme_mars_expander_process(void *data)
{
	Eon_Theme_Mars_Expander *thiz;
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

static Enesim_Renderer * _eon_theme_mars_expander_renderer_get(void *data)
{
	Eon_Theme_Mars_Expander *thiz = data;
	return enesim_renderer_ref(thiz->area);
}

static void _eon_theme_mars_expander_first_padding_get(void *data, Eon_Box *padding)
{
	padding->top = EON_THEME_MARS_MARGIN;
	padding->bottom = EON_THEME_MARS_MARGIN;
	padding->left = EON_THEME_MARS_MARGIN;
	padding->right = EON_THEME_MARS_MARGIN;
}

static void _eon_theme_mars_expander_second_padding_get(void *data, Eon_Box *padding)
{
	padding->top = EON_THEME_MARS_MARGIN;
	padding->bottom = EON_THEME_MARS_MARGIN;
	padding->left = EON_THEME_MARS_MARGIN;
	padding->right = EON_THEME_MARS_MARGIN;
}

static void _eon_theme_mars_expander_first_content_set(void *data, Enesim_Renderer *r, Eina_Rectangle *area)
{
	Eon_Theme_Mars_Expander *thiz = data;
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

static void _eon_theme_mars_expander_second_content_set(void *data, Enesim_Renderer *r, Eina_Rectangle *area)
{
	Eon_Theme_Mars_Expander *thiz = data;
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

static Eon_Theme_Element_Expander_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_expander_version_get,
	/* .ctor 		= */ eon_theme_mars_expander_new,
	/* .dtor 		= */ _eon_theme_mars_expander_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_expander_tag_name_get,
	/* .process 		= */ _eon_theme_mars_expander_process,
	/* .renderer_get	= */ _eon_theme_mars_expander_renderer_get,
	/* .first_content_set	= */ _eon_theme_mars_expander_first_content_set,
	/* .second_content_set	= */ _eon_theme_mars_expander_second_content_set,
	/* .first_padding_get	= */ _eon_theme_mars_expander_first_padding_get,
	/* .second_padding_get	= */ _eon_theme_mars_expander_second_padding_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_expander_new(void)
{
	Eon_Theme_Mars_Expander *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Expander));

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

	n = eon_theme_element_expander_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_string("border-color");
	thiz->border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->border_color), NULL);
	s = egueb_dom_string_new_with_static_string("area-border-color");
	thiz->area_border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->area_border_color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

