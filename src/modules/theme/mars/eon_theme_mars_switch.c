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
#include "eon_theme_mars_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Mars_Switch
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *active_color;
	Egueb_Dom_Node *inactive_color;
	Egueb_Dom_Node *level;
	/* private */
	Enesim_Renderer *proxy;
	Enesim_Renderer *blur;
	Enesim_Renderer *box;
	Enesim_Renderer *box_content;
	Enesim_Renderer *active_txt;
	Enesim_Renderer *active_box;
	Enesim_Renderer *inactive_txt;
	Enesim_Renderer *inactive_box;
} Eon_Theme_Mars_Switch;
/*----------------------------------------------------------------------------*
 *                              Switch interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_switch_version_get(void)
{
	return EON_THEME_ELEMENT_SWITCH_VERSION;
}

static void _eon_theme_mars_switch_dtor(void *data)
{
	Eon_Theme_Mars_Switch *thiz = data;

	egueb_dom_node_unref(thiz->active_color);
	egueb_dom_node_unref(thiz->inactive_color);
	egueb_dom_node_unref(thiz->level);
	enesim_renderer_unref(thiz->proxy);
	enesim_renderer_unref(thiz->blur);
	enesim_renderer_unref(thiz->box);
	enesim_renderer_unref(thiz->box_content);
	enesim_renderer_unref(thiz->active_txt);
	enesim_renderer_unref(thiz->active_box);
	enesim_renderer_unref(thiz->inactive_txt);
	enesim_renderer_unref(thiz->inactive_box);
	free(thiz);
}

static const char * _eon_theme_mars_switch_tag_name_get(void)
{
	return "switch";
}

static Eina_Bool _eon_theme_mars_switch_process(void *data)
{
	Eon_Theme_Mars_Switch *thiz;
	Eina_Rectangle geom;
	Enesim_Rectangle bounds;
	Eina_Bool enabled;
	Enesim_Argb argb;
	Enesim_Color inactive_color;
	Enesim_Color active_color;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->active_color, &argb);
	active_color = enesim_color_argb_from(argb);
	egueb_dom_attr_final_get(thiz->inactive_color, &argb);
	inactive_color = enesim_color_argb_from(argb);
	/* get the inherited members */
	enabled = eon_theme_widget_enabled_get(thiz->n);
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	/* set the position of the elements */
	enesim_renderer_shape_stroke_color_set(thiz->box, active_color);
	enesim_renderer_rectangle_position_set(thiz->box, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->box, geom.w, geom.h);

	enesim_renderer_shape_geometry_get(thiz->active_txt, &bounds);
	enesim_renderer_origin_set(thiz->active_txt, geom.x, geom.y);
	enesim_renderer_color_set(thiz->active_txt, inactive_color);
	enesim_renderer_rectangle_position_set(thiz->active_box, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->active_box, bounds.w, bounds.h);
	enesim_renderer_shape_fill_color_set(thiz->active_box, active_color);

	enesim_renderer_origin_set(thiz->inactive_txt, geom.x + bounds.w, geom.y);
	enesim_renderer_color_set(thiz->inactive_txt, active_color);
	enesim_renderer_rectangle_position_set(thiz->inactive_box, geom.x + bounds.w, geom.y);
	enesim_renderer_rectangle_size_set(thiz->inactive_box, geom.w - bounds.w, bounds.h);
	enesim_renderer_shape_fill_color_set(thiz->inactive_box, inactive_color);

	/* apply the blur value */
	if (!enabled)
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur,
				enesim_renderer_ref(thiz->box));
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->blur));
	}
	else
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur, NULL);
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->box));
	}
	enesim_renderer_origin_set(thiz->blur, geom.x, geom.y);

	return EINA_TRUE;
}

static void _eon_theme_mars_switch_active_text_set(void *data, const char *txt)
{
	Eon_Theme_Mars_Switch *thiz = data;
	enesim_renderer_text_span_text_set(thiz->active_txt, txt);
}

static void _eon_theme_mars_switch_inactive_text_set(void *data, const char *txt)
{
	Eon_Theme_Mars_Switch *thiz = data;
	enesim_renderer_text_span_text_set(thiz->inactive_txt, txt);
}

static int _eon_theme_mars_switch_size_hints_get(void *data, Eon_Renderable_Size *size)
{
	Eon_Theme_Mars_Switch *thiz = data;
	Egueb_Dom_Node *font_attr;
	Enesim_Text_Font *font;
	Eina_Rectangle bounds;
	int ret = 0;
	int w = 0;
	int h = 0;

	font_attr = egueb_dom_element_attribute_node_get(
			thiz->n, EGUEB_CSS_NAME_FONT);
	if (font_attr)
	{
		font = egueb_css_attr_font_resolve(font_attr,
				0, 0);
		if (!font)
		{
			egueb_dom_node_unref(font_attr);
			return 0;
		}
	}
	egueb_dom_node_unref(font_attr);
	enesim_renderer_text_span_font_set(thiz->active_txt, enesim_text_font_ref(font));
	enesim_renderer_text_span_font_set(thiz->inactive_txt, enesim_text_font_ref(font));
	enesim_text_font_unref(font);

	if (enesim_renderer_destination_bounds_get(thiz->active_txt, &bounds, 0, 0, NULL))
	{
		w = bounds.w;
		h = bounds.h;
	}
	if (enesim_renderer_destination_bounds_get(thiz->inactive_txt, &bounds, 0, 0, NULL))
	{
		w = bounds.w > w ? bounds.w * 2 : w * 2;
		h = bounds.h > h ? bounds.h : h;
	}

	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	size->min_width = size->max_width = w;
	size->min_height = size->max_height = h;

	return ret;
}

static Enesim_Renderer * _eon_theme_mars_switch_renderer_get(void *data)
{
	Eon_Theme_Mars_Switch *thiz = data;
	return enesim_renderer_ref(thiz->proxy);
}

static Eon_Theme_Element_Switch_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_switch_version_get,
	/* .ctor 		= */ eon_theme_mars_switch_new,
	/* .dtor 		= */ _eon_theme_mars_switch_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_switch_tag_name_get,
	/* .process 		= */ _eon_theme_mars_switch_process,
	/* .renderer_get	= */ _eon_theme_mars_switch_renderer_get,
	/* .active_text_set	= */ _eon_theme_mars_switch_active_text_set,
	/* .inactive_text_set	= */ _eon_theme_mars_switch_inactive_text_set,
	/* .size_hints_get	= */ _eon_theme_mars_switch_size_hints_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_switch_new(void)
{
	Eon_Theme_Mars_Switch *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Switch));
	/* the active part */
	thiz->active_txt = enesim_renderer_text_span_new();
	thiz->active_box = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->active_box,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	/* the inactive part */
	thiz->inactive_txt = enesim_renderer_text_span_new();
	thiz->inactive_box = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->inactive_box,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	/* the box content, where the active/inactive texts/boxes lay */
	thiz->box_content = enesim_renderer_compound_new();
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->active_box));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(thiz->box_content, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->active_txt));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->box_content, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->inactive_box));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(thiz->box_content, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->inactive_txt));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->box_content, l);
	/* the real switch */
	thiz->box = enesim_renderer_rectangle_new();
	enesim_renderer_shape_stroke_weight_set(thiz->box, 1);
	enesim_renderer_shape_fill_renderer_set(thiz->box,
			enesim_renderer_ref(thiz->box_content));
	enesim_renderer_shape_draw_mode_set(thiz->box,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);

	/* the blur effect for disabled switchs */
	thiz->blur = enesim_renderer_blur_new();
	enesim_renderer_blur_radius_x_set(thiz->blur, 3);
	enesim_renderer_blur_radius_y_set(thiz->blur, 3);

	thiz->proxy = enesim_renderer_proxy_new();
	enesim_renderer_proxy_proxied_set(thiz->proxy,
		enesim_renderer_ref(thiz->box));

	n = eon_theme_element_switch_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_string("level");
	thiz->level = egueb_dom_attr_int_new(s, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	s = egueb_dom_string_new_with_static_string("active-color");
	thiz->active_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	s = egueb_dom_string_new_with_static_string("inactive-color");
	thiz->inactive_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	/* FIXME the level can be on the theme element switch itself? */
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->level), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->active_color), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->inactive_color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

