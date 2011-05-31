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
		Enesim_Renderer *background;
		Enesim_Renderer *bar;
		Enesim_Renderer *rectangle;
	} shape;
	struct {
		Enesim_Renderer *pattern;
		Enesim_Renderer *shade;
	} background;
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
	thiz->fill(thiz->shape.rectangle, x, y, len, dst);
}

static void _progressbar_update_size(Enesim_Renderer *r)
{
	Progressbar *thiz;
	Enesim_Matrix t;
	float sa, ca;
	double width, height;

	thiz = _progressbar_get(r);
	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	printf("updating size %g %g\n", width, height);
	enesim_renderer_rectangle_width_set(thiz->shape.rectangle, width);
	enesim_renderer_rectangle_height_set(thiz->shape.rectangle, height);

	sa = sin(-40 * M_PI / 180.0);
	ca = cos(-40 * M_PI / 180.0);
	t.xx = ca;               t.xy = sa;               t.xz = 0;
	t.yx = -sa / 2;    t.yy = ca / 2;     t.yz = 0;
	t.zx = 0;                t.zy = 0;                t.zz = 1;
	enesim_renderer_transformation_set(thiz->background.pattern, &t);

	enesim_renderer_stripes_even_thickness_set(thiz->background.shade, height / 4.0);
	enesim_renderer_stripes_odd_thickness_set(thiz->background.shade, height / 4.0);
	t.xx = 1;               t.xy = 0;                t.xz = 0;
	t.yx = 0;               t.yy = 0.5;              t.yz = 0;
	t.zx = 0;               t.zy = 0;                t.zz = 1;
	enesim_renderer_transformation_set(thiz->background.shade, &t);

	width = width * thiz->progression;
	enesim_renderer_rectangle_width_set(thiz->shape.bar, width);
	enesim_renderer_rectangle_height_set(thiz->shape.bar, height);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme widget interface                      *
 *----------------------------------------------------------------------------*/
static double _progressbar_min_width_get(Enesim_Renderer *r)
{
	Progressbar *thiz;
	int min_width;

	thiz = _progressbar_get(r);
	enesim_renderer_rectangle_width_get(thiz->shape.rectangle, &min_width);

	return min_width;
}

static double _progressbar_max_width_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static double _progressbar_min_height_get(Enesim_Renderer *r)
{
	Progressbar *thiz;
	int min_height;

	thiz = _progressbar_get(r);
	enesim_renderer_rectangle_height_get(thiz->shape.rectangle, &min_height);

	return min_height;
}

static double _progressbar_max_height_get(Enesim_Renderer *r)
{
	return DBL_MAX;
}

static Eon_Theme_Widget_Descriptor _twdescriptor = {
	.max_width_get = _progressbar_max_width_get,
	.max_height_get = _progressbar_max_height_get,
	.min_width_get = _progressbar_min_width_get,
	.min_height_get = _progressbar_min_height_get,
};
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Progressbar *thiz;
	Enesim_Matrix matrix;
	double ox, oy;

	thiz = _progressbar_get(r);
	/* the matrix */
	enesim_renderer_transformation_get(r, &matrix);
	enesim_renderer_transformation_set(thiz->shape.rectangle, &matrix);
	/* the origin */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->shape.rectangle, ox, oy);

	 _progressbar_update_size(r);

	if (!enesim_renderer_sw_setup(thiz->shape.rectangle))
	{
		printf("rectangle cannot setup\n");
		return EINA_FALSE;
	}
	thiz->fill = enesim_renderer_sw_fill_get(thiz->shape.rectangle);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _draw;

	printf("setup correctly\n");
	return EINA_TRUE;
}

static void _cleanup(Enesim_Renderer *r)
{
	Progressbar *thiz;

	thiz = _progressbar_get(r);
	enesim_renderer_sw_cleanup(thiz->shape.bar);
}

static void _free(Enesim_Renderer *r)
{
	Progressbar *thiz;

	thiz = _progressbar_get(r);

	thiz = enesim_renderer_data_get(r);
	if (thiz->shape.background)
		enesim_renderer_delete(thiz->shape.background);
	if (thiz->shape.bar)
		enesim_renderer_delete(thiz->shape.bar);
}

static Enesim_Renderer_Descriptor _descriptor = {
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

	r = enesim_renderer_stripes_new();
	if (!r) goto background_pattern_err;
	thiz->background.pattern = r;
	enesim_renderer_stripes_even_color_set(r, 0xffa1ff38);
	enesim_renderer_stripes_odd_color_set(r, 0xffb3ff00);
	enesim_renderer_stripes_even_thickness_set(r, 3);
	enesim_renderer_stripes_odd_thickness_set(r, 5);

	r = enesim_renderer_stripes_new();
	if (!r) goto background_shade_err;
	thiz->background.shade = r;
	enesim_renderer_stripes_even_color_set(r, 0);
	enesim_renderer_stripes_odd_color_set(r, 0x33000000);
	enesim_renderer_stripes_even_thickness_set(r, 1);
	enesim_renderer_stripes_odd_thickness_set(r, 1);

	r = enesim_renderer_compound_new();
	if (!r) goto shape_background_err;
	thiz->shape.background = r;
	enesim_renderer_compound_layer_add(r, thiz->background.pattern);
	enesim_renderer_rop_set(thiz->background.pattern, ENESIM_FILL);
	enesim_renderer_compound_layer_add(r, thiz->background.shade);
	enesim_renderer_rop_set(thiz->background.shade, ENESIM_BLEND);

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_bar_err;
	thiz->shape.bar = r;
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_rectangle_corner_radius_set(r, 5);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_stroke_color_set(r, 0xffa1ff38);
	enesim_renderer_shape_fill_renderer_set(r, thiz->shape.background);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = enesim_renderer_rectangle_new();
	if (!r) goto shape_rectangle_err;
	thiz->shape.rectangle = r;
	enesim_renderer_rectangle_corners_set(r, EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);
	enesim_renderer_rectangle_corner_radius_set(r, 5);
	enesim_renderer_shape_stroke_weight_set(r, 2);
	enesim_renderer_shape_stroke_color_set(r, 0xff000000);
	enesim_renderer_shape_fill_renderer_set(r, thiz->shape.bar);
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_STROKE_FILL);

	r = eon_theme_progressbar_new(&_twdescriptor, &_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	enesim_renderer_delete(thiz->shape.rectangle);
shape_rectangle_err:
	enesim_renderer_delete(thiz->shape.bar);
shape_bar_err:
	enesim_renderer_delete(thiz->shape.background);
shape_background_err:
	enesim_renderer_delete(thiz->background.shade);
background_shade_err:
	enesim_renderer_delete(thiz->background.pattern);
background_pattern_err:
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
	r1 = thiz->background.pattern;
	enesim_renderer_stripes_even_color_set(r1, color);

	r1 = thiz->shape.bar;
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
	r1 = thiz->background.pattern;
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
