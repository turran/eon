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
typedef struct _Eon_Theme_Mars_Button
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *border_color;
	Egueb_Dom_Node *color;
	/* private */
	Enesim_Renderer *inner_button;
	Enesim_Renderer *button;
	Enesim_Renderer *button_content;
	Enesim_Renderer *blur;
	Enesim_Renderer *proxy;
} Eon_Theme_Mars_Button;

/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_button_version_get(void)
{
	return EON_THEME_ELEMENT_BUTTON_VERSION;
}

static void _eon_theme_mars_button_dtor(void *data)
{
	Eon_Theme_Mars_Button *thiz = data;

	/* attributes */
	egueb_dom_node_unref(thiz->border_color);
	egueb_dom_node_unref(thiz->color);
	/* private */
	enesim_renderer_unref(thiz->inner_button);
	enesim_renderer_unref(thiz->button);
	enesim_renderer_unref(thiz->button_content);
	enesim_renderer_unref(thiz->blur);
	enesim_renderer_unref(thiz->proxy);
	free(thiz);
}

static const char * _eon_theme_mars_button_tag_name_get(void)
{
	return "button";
}

static Eina_Bool _eon_theme_mars_button_process(void *data)
{
	Eon_Theme_Mars_Button *thiz;
	Eina_Rectangle geom;
	Eina_Bool enabled;
	Enesim_Argb argb;
	Enesim_Color color;
	Enesim_Color border_color;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->color, &argb);
	color = enesim_color_argb_from(argb);
	egueb_dom_attr_final_get(thiz->border_color, &argb);
	border_color = enesim_color_argb_from(argb);
	/* get the inherited members */
	enabled = eon_theme_widget_enabled_get(thiz->n);
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	/* set the rectangle area */
	enesim_renderer_rectangle_position_set(thiz->inner_button, geom.x + 6, geom.y + 6);
	enesim_renderer_rectangle_size_set(thiz->inner_button, geom.w - 12, geom.h - 12);
	enesim_renderer_shape_fill_color_set(thiz->inner_button, color);

	enesim_renderer_rectangle_position_set(thiz->button, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->button, geom.w, geom.h);
	/* set the border color */
	enesim_renderer_shape_stroke_color_set(thiz->button, border_color);
	enesim_renderer_shape_stroke_weight_set(thiz->button, 2);
	/* apply the blur value */
	if (!enabled)
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur,
				enesim_renderer_ref(thiz->button));
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->blur));
	}
	else
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur, NULL);
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->button));
	}
	enesim_renderer_origin_set(thiz->blur, geom.x, geom.y);

	return EINA_TRUE;
}

static void _eon_theme_mars_button_padding_get(void *data, Eon_Box *padding)
{
	padding->top = 15;
	padding->bottom = 15;
	padding->left = 15;
	padding->right = 15;
}

static void _eon_theme_mars_button_content_set(void *data, Enesim_Renderer *r)
{
	Eon_Theme_Mars_Button *thiz = data;
	Enesim_Renderer_Compound_Layer *l;

	enesim_renderer_compound_layer_clear(thiz->button_content);
	/* the background */
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->inner_button));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(thiz->button_content, l);
	/* the content */
	if (r)
	{
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_renderer_set(l, r);
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
		enesim_renderer_compound_layer_add(thiz->button_content, l);
	}
}

static Enesim_Renderer * _eon_theme_mars_button_renderer_get(void *data)
{
	Eon_Theme_Mars_Button *thiz = data;
	return enesim_renderer_ref(thiz->proxy);
}

static Eon_Theme_Element_Button_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_button_version_get,
	/* .ctor 		= */ eon_theme_mars_button_new,
	/* .dtor 		= */ _eon_theme_mars_button_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_button_tag_name_get,
	/* .process 		= */ _eon_theme_mars_button_process,
	/* .content_set		= */ _eon_theme_mars_button_content_set,
	/* .padding_get		= */ _eon_theme_mars_button_padding_get,
	/* .renderer_get	= */ _eon_theme_mars_button_renderer_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_button_new(void)
{
	Eon_Theme_Mars_Button *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Button));
	thiz->inner_button = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->inner_button, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);

	thiz->button_content = enesim_renderer_compound_new();
	/* the background */
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->inner_button));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_add(thiz->button_content, l);

	/* the real button */
	thiz->button = enesim_renderer_rectangle_new();
	enesim_renderer_shape_fill_renderer_set(thiz->button, enesim_renderer_ref(thiz->button_content));
	enesim_renderer_shape_draw_mode_set(thiz->button, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);

	/* the blur effect for disabled buttons */
	thiz->blur = enesim_renderer_blur_new();
	enesim_renderer_blur_radius_x_set(thiz->blur, 3);
	enesim_renderer_blur_radius_y_set(thiz->blur, 3);

	thiz->proxy = enesim_renderer_proxy_new();
	enesim_renderer_proxy_proxied_set(thiz->proxy, enesim_renderer_ref(thiz->button));

	n = eon_theme_element_button_new(&_descriptor, thiz);
	/* the attributes */
	thiz->color = egueb_css_attr_color_new(
			egueb_dom_string_ref(EON_NAME_COLOR), NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	s = egueb_dom_string_new_with_static_string("border-color");
	thiz->border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->color), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->border_color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
