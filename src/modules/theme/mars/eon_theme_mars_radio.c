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
typedef struct _Eon_Theme_Mars_Radio
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *border_color;
	Egueb_Dom_Node *button_color;
	/* private */
	Enesim_Renderer *area;
	Enesim_Renderer *border;
	Enesim_Renderer *button;
	Enesim_Renderer *radio;
	Enesim_Renderer *blur;
	Enesim_Renderer *proxy;
} Eon_Theme_Mars_Radio;

/*----------------------------------------------------------------------------*
 *                              Button interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_radio_version_get(void)
{
	return EON_THEME_ELEMENT_BUTTON_VERSION;
}

static void _eon_theme_mars_radio_dtor(void *data)
{
	Eon_Theme_Mars_Radio *thiz = data;

	/* attributes */
	egueb_dom_node_unref(thiz->border_color);
	egueb_dom_node_unref(thiz->button_color);
	/* private */
	enesim_renderer_unref(thiz->area);
	enesim_renderer_unref(thiz->border);
	enesim_renderer_unref(thiz->button);
	enesim_renderer_unref(thiz->radio);
	enesim_renderer_unref(thiz->blur);
	enesim_renderer_unref(thiz->proxy);
	free(thiz);
}

static const char * _eon_theme_mars_radio_tag_name_get(void)
{
	return "radio";
}

static Eina_Bool _eon_theme_mars_radio_process(void *data)
{
	Eon_Theme_Mars_Radio *thiz;
	Eina_Rectangle geom;
	Eina_Bool enabled;
	Enesim_Argb argb;
	Enesim_Color button_color;
	Enesim_Color border_color;
	int cx, cy;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->button_color, &argb);
	button_color = enesim_color_argb_from(argb);
	egueb_dom_attr_final_get(thiz->border_color, &argb);
	border_color = enesim_color_argb_from(argb);
	/* get the inherited members */
	enabled = eon_theme_widget_enabled_get(thiz->n);
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	/* rad + padding */
	cx = geom.x + 12;
	cy = geom.y + (geom.h / 2);
	/* set the area */
	enesim_renderer_rectangle_position_set(thiz->area, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->area, geom.w, geom.h);
	/* set the border */
	enesim_renderer_shape_stroke_color_set(thiz->border, border_color);
	enesim_renderer_circle_center_set(thiz->border, cx, cy);
	/* set the button */
	enesim_renderer_shape_fill_color_set(thiz->button, button_color);
	enesim_renderer_circle_center_set(thiz->button, cx, cy);
	/* apply the blur value */
	if (!enabled)
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur,
				enesim_renderer_ref(thiz->area));
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->blur));
	}
	else
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur, NULL);
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->area));
	}
	enesim_renderer_origin_set(thiz->blur, geom.x + 1, geom.y);

	return EINA_TRUE;
}

static void _eon_theme_mars_radio_padding_get(void *data, Eon_Box *padding)
{
	padding->top = 5;
	padding->bottom = 5;
	/* rad + padding + rad + padding */
	padding->left = 24;
	padding->right = 5;
}

static void _eon_theme_mars_radio_content_set(void *data, Enesim_Renderer *r)
{
	Eon_Theme_Mars_Radio *thiz = data;
	Enesim_Renderer_Compound_Layer *l;

	enesim_renderer_compound_layer_clear(thiz->radio);
	/* the radio button */
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->border));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->radio, l);
	/* the content */
	if (r)
	{
		l = enesim_renderer_compound_layer_new();
		enesim_renderer_compound_layer_renderer_set(l, r);
		enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
		enesim_renderer_compound_layer_add(thiz->radio, l);
	}
}

static Enesim_Renderer * _eon_theme_mars_radio_renderer_get(void *data)
{
	Eon_Theme_Mars_Radio *thiz = data;
	return enesim_renderer_ref(thiz->proxy);
}

static Eon_Theme_Element_Button_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_radio_version_get,
	/* .ctor 		= */ eon_theme_mars_radio_new,
	/* .dtor 		= */ _eon_theme_mars_radio_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_radio_tag_name_get,
	/* .process 		= */ _eon_theme_mars_radio_process,
	/* .renderer_get	= */ _eon_theme_mars_radio_renderer_get,
	/* .content_set		= */ _eon_theme_mars_radio_content_set,
	/* .padding_get		= */ _eon_theme_mars_radio_padding_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_radio_new(void)
{
	Eon_Theme_Mars_Radio *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Radio));
	/* the radio button */
	thiz->button = enesim_renderer_circle_new();
	enesim_renderer_circle_radius_set(thiz->button, 3);
	enesim_renderer_shape_draw_mode_set(thiz->button, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);

	thiz->border = enesim_renderer_circle_new();
	enesim_renderer_circle_radius_set(thiz->border, 7);
	enesim_renderer_shape_stroke_weight_set(thiz->border, 2);
	enesim_renderer_shape_draw_mode_set(thiz->border, ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_fill_renderer_set(thiz->border, enesim_renderer_ref(thiz->button));

	/* the button and the content */
	thiz->radio = enesim_renderer_compound_new();
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_renderer_set(l, enesim_renderer_ref(thiz->border));
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_add(thiz->radio, l);

	/* the widget area */
	thiz->area = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->area, ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_shape_fill_renderer_set(thiz->area, enesim_renderer_ref(thiz->radio));

	/* the blur effect for disabled buttons */
	thiz->blur = enesim_renderer_blur_new();
	enesim_renderer_blur_radius_x_set(thiz->blur, 3);
	enesim_renderer_blur_radius_y_set(thiz->blur, 3);

	thiz->proxy = enesim_renderer_proxy_new();
	enesim_renderer_proxy_proxied_set(thiz->proxy, enesim_renderer_ref(thiz->area));

	n = eon_theme_element_button_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_chars("border-color");
	thiz->border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	s = egueb_dom_string_new_with_static_chars("button-color");
	thiz->button_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->border_color), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->button_color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

