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
typedef struct _Eon_Theme_Mars_Frame
{
	Egueb_Dom_Node *n;
	/* attributes */
	Egueb_Dom_Node *border_color;
	/* private */
	Enesim_Renderer *txt;
	Enesim_Renderer *txt_bkg;
	Enesim_Renderer *rectangle;
	Enesim_Renderer *compound;
} Eon_Theme_Mars_Frame;
/*----------------------------------------------------------------------------*
 *                              Frame interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_frame_version_get(void)
{
	return EON_THEME_ELEMENT_FRAME_VERSION;
}

static void _eon_theme_mars_frame_dtor(void *data)
{
	Eon_Theme_Mars_Frame *thiz = data;

	free(thiz);
}

static const char * _eon_theme_mars_frame_tag_name_get(void)
{
	return "frame";
}

static Eina_Bool _eon_theme_mars_frame_process(void *data)
{
	Eon_Theme_Mars_Frame *thiz;
	Eina_Rectangle geom;
	Enesim_Rectangle bounds;
	Enesim_Argb argb;
	Enesim_Color border_color;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->border_color, &argb);
	border_color = enesim_color_argb_from(argb);
	/* get the inherited members */
	eon_theme_renderable_geometry_get(thiz->n, &geom);

	/* setup the whole rectangle */
	enesim_renderer_shape_stroke_color_set(thiz->rectangle, border_color);
	enesim_renderer_rectangle_position_set(thiz->rectangle, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->rectangle, geom.w, geom.h);

	/* setup the text */
	enesim_renderer_origin_set(thiz->txt, geom.x, geom.y);

	/* setup the text background */
	if (enesim_renderer_destination_bounds_get(thiz->txt, &geom, 0, 0, NULL))
	{
		enesim_renderer_rectangle_position_set(thiz->txt_bkg, geom.x - 5, geom.y - 5);
		enesim_renderer_rectangle_size_set(thiz->txt_bkg, geom.w + 10, geom.h + 10);
	}
	else
	{
		enesim_renderer_rectangle_size_set(thiz->txt_bkg, 0, 0);
	}

	return EINA_TRUE;
}

static Enesim_Renderer * _eon_theme_mars_frame_renderer_get(void *data)
{
	Eon_Theme_Mars_Frame *thiz = data;
	return enesim_renderer_ref(thiz->compound);
}

static void _eon_theme_mars_frame_padding_get(void *data, Eon_Box *padding)
{
	padding->top = 15;
	padding->bottom = 15;
	padding->left = 15;
	padding->right = 15;
}

static void _eon_theme_mars_frame_content_set(void *data, Enesim_Renderer *r)
{
	Eon_Theme_Mars_Frame *thiz = data;
	enesim_renderer_shape_fill_renderer_set(thiz->rectangle, r);
}

static void _eon_theme_mars_frame_title_set(void *data, const char *txt)
{
	Eon_Theme_Mars_Frame *thiz = data;
	enesim_renderer_text_span_text_set(thiz->txt, txt);
}

static void _eon_theme_mars_frame_min_size_get(void *data, int *minw, int *minh)
{
	Eon_Theme_Mars_Frame *thiz = data;
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
			goto done;
		}
	}
	egueb_dom_node_unref(font_attr);
	enesim_renderer_text_span_font_set(thiz->txt, enesim_text_font_ref(font));
	enesim_text_font_unref(font);

	if (enesim_renderer_destination_bounds_get(thiz->txt, &bounds, 0, 0, NULL))
	{
		w = bounds.w;
		h = bounds.h;
	}
done:
	/* add the text padding */
	w += 20;
	h += 10;

	*minw = w;
	*minh = h;
}

static Eon_Theme_Element_Frame_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_frame_version_get,
	/* .ctor 		= */ eon_theme_mars_frame_new,
	/* .dtor 		= */ _eon_theme_mars_frame_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_frame_tag_name_get,
	/* .process 		= */ _eon_theme_mars_frame_process,
	/* .renderer_get	= */ _eon_theme_mars_frame_renderer_get,
	/* .content_set		= */ _eon_theme_mars_frame_content_set,
	/* .padding_get		= */ _eon_theme_mars_frame_padding_get,
	/* .title_set		= */ _eon_theme_mars_frame_title_set,
	/* .min_size_get	= */ _eon_theme_mars_frame_min_size_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_frame_new(void)
{
	Eon_Theme_Mars_Frame *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Frame));
	thiz->txt = enesim_renderer_text_span_new();

	thiz->txt_bkg = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->txt_bkg,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_shape_fill_color_set(thiz->txt_bkg, 0xff000000);

	thiz->rectangle = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->rectangle,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE_FILL);
	enesim_renderer_shape_stroke_weight_set(thiz->rectangle, 2);

	thiz->compound = enesim_renderer_compound_new();
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->rectangle));
	enesim_renderer_compound_layer_add(thiz->compound, l);

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->txt_bkg));
	enesim_renderer_compound_layer_add(thiz->compound, l);

	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->txt));
	enesim_renderer_compound_layer_add(thiz->compound, l);

	n = eon_theme_element_frame_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_string("border-color");
	thiz->border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->border_color), NULL);
	thiz->n = n;

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


