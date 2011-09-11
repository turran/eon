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
#include "Eon_Basic.h"
#include "eon_basic_private.h"
#include <math.h>
#include <float.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Progressbar
{
	/* properties */
	double progression;
	Enesim_Color light;
	Enesim_Color dark;
	/* private */
	struct {
		Enesim_Renderer *content; /* the compound */
		Enesim_Renderer *rectangle;
	} widget;
	struct {
		Enesim_Renderer *background;
		Enesim_Renderer *bar;
	} content;
	struct {
		Enesim_Renderer *content; /* the compound */
	} bar;
	struct {
		Enesim_Renderer *pattern;
		Enesim_Renderer *shade;
	} bar_content;
	Enesim_Renderer_Sw_Fill fill;
} Progressbar;

static inline Progressbar * _progressbar_get(Enesim_Renderer *r)
{
	Progressbar *thiz;

	thiz = eon_theme_progressbar_data_get(r);
	return thiz;
}

static void _draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Progressbar *thiz;

	thiz = _progressbar_get(r);
	thiz->fill(thiz->widget.rectangle, x, y, len, dst);
}

static void _progressbar_update_size(Enesim_Renderer *r)
{
	Progressbar *thiz;
	Enesim_Matrix t;
	double sa, ca;
	double width, height;

	thiz = _progressbar_get(r);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	enesim_renderer_rectangle_width_set(thiz->widget.rectangle, width);
	enesim_renderer_rectangle_height_set(thiz->widget.rectangle, height);

	sa = sin(-40 * M_PI / 180.0);
	ca = cos(-40 * M_PI / 180.0);
	t.xx = ca;               t.xy = sa;               t.xz = 0;
	t.yx = -sa / 2;    t.yy = ca / 2;     t.yz = 0;
	t.zx = 0;                t.zy = 0;                t.zz = 1;
	enesim_renderer_transformation_set(thiz->bar_content.pattern, &t);
	enesim_renderer_stripes_even_thickness_set(thiz->bar_content.shade, height / 4.0);
	enesim_renderer_stripes_odd_thickness_set(thiz->bar_content.shade, height / 4.0);
	t.xx = 1;               t.xy = 0;                t.xz = 0;
	t.yx = 0;               t.yy = 0.5;              t.yz = 0;
	t.zx = 0;               t.zy = 0;                t.zz = 1;
	enesim_renderer_transformation_set(thiz->bar_content.shade, &t);

	width = width * thiz->progression;
	enesim_renderer_rectangle_width_set(thiz->content.bar, width);
	enesim_renderer_rectangle_height_set(thiz->content.bar, height);
}
/*----------------------------------------------------------------------------*
 *                   The Eon's theme progressbar interface                    *
 *----------------------------------------------------------------------------*/
static double _progressbar_min_width_get(Enesim_Renderer *r)
{
	return 22;
}

static double _progressbar_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _progressbar_min_height_get(Enesim_Renderer *r)
{
	return 22;
}

static double _progressbar_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eina_Bool _setup(Enesim_Renderer *r, Enesim_Surface *s,
		Enesim_Renderer_Sw_Fill *fill, Enesim_Error **error)
{
	Progressbar *thiz;
	Enesim_Matrix matrix;
	double ox, oy;

	thiz = _progressbar_get(r);
	/* the matrix */
	enesim_renderer_transformation_get(r, &matrix);
	enesim_renderer_transformation_set(thiz->widget.rectangle, &matrix);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->widget.rectangle, ox, oy);

	/* if we are at zero progression level just draw the background and not the bar */
	enesim_renderer_compound_layer_clear(thiz->widget.content);
	enesim_renderer_compound_layer_add(thiz->widget.content, thiz->content.background);
	if (thiz->progression > 0.0)
	{
		enesim_renderer_compound_layer_add(thiz->widget.content, thiz->content.bar);
	}
	_progressbar_update_size(r);

	if (!enesim_renderer_sw_setup(thiz->widget.rectangle, s, error))
	{
		printf("failed 1\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->widget.rectangle);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _draw;

	return EINA_TRUE;
}

static void _cleanup(Enesim_Renderer *r)
{
	Progressbar *thiz;

	thiz = _progressbar_get(r);
	enesim_renderer_sw_cleanup(thiz->widget.rectangle);
}

static void _free(Enesim_Renderer *r)
{
	Progressbar *thiz;

	thiz = _progressbar_get(r);

	thiz = enesim_renderer_data_get(r);

	if (thiz->widget.rectangle)
		enesim_renderer_unref(thiz->widget.rectangle);
	if (thiz->widget.content)
		enesim_renderer_unref(thiz->widget.content);
	if (thiz->content.background)
		enesim_renderer_unref(thiz->content.background);
	if (thiz->content.bar)
		enesim_renderer_unref(thiz->content.bar);
	if (thiz->bar.content)
		enesim_renderer_unref(thiz->bar.content);
	if (thiz->bar_content.shade)
		enesim_renderer_unref(thiz->bar_content.shade);
	if (thiz->bar_content.pattern)
		enesim_renderer_unref(thiz->bar_content.pattern);
}

static Eon_Theme_Progressbar_Descriptor _descriptor = {
	.max_width_get = _progressbar_max_width_get,
	.max_height_get = _progressbar_max_height_get,
	.min_width_get = _progressbar_min_width_get,
	.min_height_get = _progressbar_min_height_get,
	.sw_setup = _setup,
	.sw_cleanup = _cleanup,
	.free = _free,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
Enesim_Renderer * eon_basic_progressbar_new(void)
{
	Progressbar *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Progressbar));
	if (!thiz) return NULL;

	/* first the bar content */
	r = enesim_renderer_stripes_new();
	if (!r) goto bar_content_pattern_err;
	thiz->bar_content.pattern = r;
	enesim_renderer_stripes_even_color_set(r, 0xffa1ff38);
	enesim_renderer_stripes_odd_color_set(r, 0xffb3ff00);
	enesim_renderer_stripes_even_thickness_set(r, 3);
	enesim_renderer_stripes_odd_thickness_set(r, 5);

	r = enesim_renderer_stripes_new();
	if (!r) goto bar_content_shade_err;
	thiz->bar_content.shade = r;
	enesim_renderer_stripes_even_color_set(r, 0);
	enesim_renderer_stripes_odd_color_set(r, 0x33000000);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);

	/* now the bar itself */
	r = enesim_renderer_compound_new();
	if (!r) goto bar_content_err;
	thiz->bar.content = r;
	enesim_renderer_compound_layer_add(r, thiz->bar_content.pattern);
	enesim_renderer_rop_set(thiz->bar_content.pattern, ENESIM_FILL);
	enesim_renderer_compound_layer_add(r, thiz->bar_content.shade);
	enesim_renderer_rop_set(thiz->bar_content.shade, ENESIM_BLEND);

	/* now the content */
	r = enesim_renderer_rectangle_new();
	if (!r) goto content_bar_err;
	thiz->content.bar = r;
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_rectangle_corner_radius_set(r, 5);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_stroke_color_set(r, 0xffa1ff38);
	enesim_renderer_shape_fill_renderer_set(r, thiz->bar.content);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = enesim_renderer_background_new();
	if (!r) goto content_background_err;
	thiz->content.background = r;
	enesim_renderer_background_color_set(r, 0xffffffff);

	/* now the widget */
	r = enesim_renderer_compound_new();
	if (!r) goto widget_content_err;
	thiz->widget.content = r;
	enesim_renderer_compound_layer_add(r, thiz->content.background);
	enesim_renderer_rop_set(thiz->content.background, ENESIM_FILL);
	enesim_renderer_compound_layer_add(r, thiz->content.bar);
	enesim_renderer_rop_set(thiz->content.bar, ENESIM_BLEND);

	r = enesim_renderer_rectangle_new();
	if (!r) goto widget_rectangle_err;
	thiz->widget.rectangle = r;
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_rectangle_corner_radius_set(r, 5);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_renderer_set(r, thiz->widget.content);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_progressbar_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_unref(thiz->widget.rectangle);
widget_rectangle_err:
	enesim_renderer_unref(thiz->widget.content);
widget_content_err:
	enesim_renderer_unref(thiz->content.background);
content_background_err:
	enesim_renderer_unref(thiz->content.bar);
content_bar_err:
	enesim_renderer_unref(thiz->bar.content);
bar_content_err:
	enesim_renderer_unref(thiz->bar_content.shade);
bar_content_shade_err:
	enesim_renderer_unref(thiz->bar_content.pattern);
bar_content_pattern_err:
	free(thiz);
	return NULL;
}

/**
 *
 */
void eon_basic_progressbar_light_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Progressbar *thiz;
	Enesim_Renderer *r1;

	thiz = _progressbar_get(r);
	r1 = thiz->bar_content.pattern;
	enesim_renderer_stripes_even_color_set(r1, color);

	r1 = thiz->content.bar;
	enesim_renderer_shape_stroke_color_set(r1, color);
}

/**
 *
 */
void eon_basic_progressbar_dark_color_set(Enesim_Renderer *r, Enesim_Color color)
{
	Progressbar *thiz;
	Enesim_Renderer *r1;

	thiz = _progressbar_get(r);
	r1 = thiz->bar_content.pattern;
	enesim_renderer_stripes_odd_color_set(r1, color);
}

/**
 *
 */
void eon_basic_progressbar_progression_set(Enesim_Renderer *r, double progression)
{
	Progressbar *thiz;

	thiz = _progressbar_get(r);
	if (progression > 1) progression = 1;
	else if (progression < 0) progression = 0;

	thiz->progression = progression;
}
