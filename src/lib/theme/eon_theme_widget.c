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
#include "eon_private.h"
/**
 * @todo
 * We must need to simplify the theme creation, all of them need a renderer
 * inside (being a compound or whatever), abstract the whole thing to simplify
 * the callbacks needed for each theme_widget child
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_THEME_WIDGET_MAGIC_CHECK(d) EON_MAGIC_CHECK(d, EON_THEME_WIDGET_MAGIC)
#define EON_THEME_WIDGET_MAGIC_CHECK_RETURN(d, ret) EON_MAGIC_CHECK_RETURN(d, EON_THEME_WIDGET_MAGIC, ret)

typedef struct _Eon_Theme_Widget
{
	EINA_MAGIC
	/* properties */
	double width;
	double height;
	double x;
	double y;
	/* private */
	Enesim_Renderer *real_r;
	Enesim_Renderer_Sw_Fill fill;
	Eon_Theme_Widget_Renderer_Get renderer_get;
	Eon_Theme_Widget_Setup setup;
	Eon_Theme_Widget_Cleanup cleanup;
	Eon_Theme_Widget_Has_Changed has_changed;
	Enesim_Renderer_Name name;
	Enesim_Renderer_Delete free;
	void *data;
} Eon_Theme_Widget;

#define _eon_theme_widget_get(r) \
	enesim_renderer_data_get(r); \
	if (!EINA_MAGIC_CHECK((Eon_Theme_Widget *)enesim_renderer_data_get(r), EON_THEME_WIDGET_MAGIC)) \
		EINA_MAGIC_FAIL((Eon_Theme_Widget *)enesim_renderer_data_get(r), EON_THEME_WIDGET_MAGIC)

#if 0
static inline Eon_Theme_Widget * _eon_theme_widget_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = enesim_renderer_data_get(r);
	EON_THEME_WIDGET_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}
#endif

static Enesim_Renderer * _eon_theme_widget_renderer_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz->real_r)
		thiz->real_r = thiz->renderer_get(r);
	return thiz->real_r;
}

static void _eon_theme_widget_draw(Enesim_Renderer *r, int x, int y, unsigned int len, void *dst)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	thiz->fill(thiz->real_r, x, y, len, dst);
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static void _eon_theme_widget_free(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}

static void _eon_theme_widget_boundings(Enesim_Renderer *r, Enesim_Rectangle *rectangle)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	rectangle->x = 0;
	rectangle->y = 0;
	rectangle->w = thiz->width;
	rectangle->h = thiz->height;
}

static Eina_Bool _eon_theme_widget_sw_setup(Enesim_Renderer *r,
		const Enesim_Renderer_State *state,
		Enesim_Surface *s,
		Enesim_Renderer_Sw_Fill *fill, Enesim_Error **error)
{
	Eon_Theme_Widget *thiz;
	Enesim_Renderer *real_r;
	Enesim_Rop rop;

	thiz = _eon_theme_widget_get(r);
	real_r = _eon_theme_widget_renderer_get(r);
	/* common properties */
	enesim_renderer_rop_get(r, &rop);
	enesim_renderer_rop_set(real_r, rop);

	if (!thiz->setup)
		return EINA_FALSE;
	if (!thiz->setup(r, error))
		return EINA_FALSE;
	enesim_renderer_setup(real_r, s, error);
	thiz->fill = enesim_renderer_sw_fill_get(real_r);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _eon_theme_widget_draw;

	return EINA_TRUE;
}

static void _eon_theme_widget_sw_cleanup(Enesim_Renderer *r, Enesim_Surface *s)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (thiz->cleanup)
		thiz->cleanup(r);
	/* FIXME */
	if (thiz->real_r)
		enesim_renderer_cleanup(thiz->real_r, s);
}

static const char * _eon_theme_widget_name(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (thiz->name)
		return thiz->name(r);
	else
		return "theme_widget";
}

static void _eon_theme_widget_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Enesim_Renderer *real_r;

	real_r = _eon_theme_widget_renderer_get(r);
	enesim_renderer_flags(real_r, flags);
}

static void _eon_theme_widget_damage(Enesim_Renderer *r, Enesim_Renderer_Damage_Cb cb, void *data)
{
	Enesim_Renderer *real_r;

	real_r = _eon_theme_widget_renderer_get(r);
	enesim_renderer_damages_get(real_r, cb, data);
}

static Eina_Bool _eon_theme_widget_has_changed(Enesim_Renderer *r)
{
	Enesim_Renderer *real_r;
	Eina_Bool ret;

	ret = eon_theme_widget_has_changed(r);
	if (ret) return ret;

	real_r = _eon_theme_widget_renderer_get(r);
	ret = enesim_renderer_has_changed(real_r);

	return ret;
}

static Eina_Bool _eon_theme_widget_is_inside(Enesim_Renderer *r, double x, double y)
{
	Enesim_Renderer *real_r;
	Eina_Bool ret;

	ret = 
	real_r = _eon_theme_widget_renderer_get(r);
	return enesim_renderer_is_inside(real_r, x, y);
}

static Enesim_Renderer_Descriptor _descriptor = {
	/* .version =     */ ENESIM_RENDERER_API,
	/* .name =        */ _eon_theme_widget_name,
	/* .free =        */ _eon_theme_widget_free,
	/* .boundings =   */ _eon_theme_widget_boundings,
	/* .flags =       */ _eon_theme_widget_flags,
	/* .is_inside =   */ _eon_theme_widget_is_inside,
	/* .damage =      */ _eon_theme_widget_damage,
	/* .has_changed = */ _eon_theme_widget_has_changed,
	/* .sw_setup =    */ _eon_theme_widget_sw_setup,
	/* .sw_cleanup =  */ _eon_theme_widget_sw_cleanup
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_widget_new(Eon_Theme_Widget_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Widget *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Widget));
	EINA_MAGIC_SET(thiz, EON_THEME_WIDGET_MAGIC);
	if (!descriptor->renderer_get)
	{
		printf("no renderer get\n");
		goto renderer_err;
	}
	thiz->renderer_get = descriptor->renderer_get;
	thiz->setup = descriptor->setup;
	thiz->cleanup = descriptor->cleanup;
	thiz->has_changed = descriptor->has_changed;
	thiz->name = descriptor->name;
	thiz->free = descriptor->free;
	thiz->data = data;

	r = enesim_renderer_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

Eina_Bool eon_is_theme_widget(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = enesim_renderer_data_get(r);
	if (!thiz) return EINA_FALSE;
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_WIDGET_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

void * eon_theme_widget_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	return thiz->data;
}

Eina_Bool eon_theme_widget_has_changed(Enesim_Renderer *r)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (thiz->has_changed)
		return thiz->has_changed(r);
	return EINA_FALSE;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_width_set(Enesim_Renderer *r, double width)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->width = width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_width_get(Enesim_Renderer *r, double *width)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*width = thiz->width;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_height_set(Enesim_Renderer *r, double height)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->height = height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_height_get(Enesim_Renderer *r, double *height)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*height = thiz->height;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_x_set(Enesim_Renderer *r, double x)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->x = x;
	/* FIXME for now */
	enesim_renderer_x_origin_set(r, x);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_x_get(Enesim_Renderer *r, double *x)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*x = thiz->x;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_y_set(Enesim_Renderer *r, double y)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	thiz->y = y;
	/* FIXME for now */
	enesim_renderer_y_origin_set(r, y);
	//printf("setting y @ %g\n", y);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_widget_y_get(Enesim_Renderer *r, double *y)
{
	Eon_Theme_Widget *thiz;

	thiz = _eon_theme_widget_get(r);
	if (!thiz) return;
	*y = thiz->y;
}
