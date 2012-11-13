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

#include "Esvg.h"

#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
/* TODO use the backend to get the surface pool */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_SVG_FILE;

static int _esvg_init = 0;

typedef struct _Eon_Svg
{
	/* properties */
	char *file;
	/* private */
	/* in order to place the svg in x,y we need to use an image */
	Enesim_Renderer *image;
	Enesim_Surface *s;
	int iw;
	int ih;
	int w;
	int h;

	Ender_Element *svg;
	/* in case the svg is relative or not */
	Eina_Bool relative : 1;
	Eina_Bool changed : 1;
	/* TODO add the event handler */
} Eon_Svg;

typedef struct _Eon_Svg_Damages
{
	Eon_Svg *thiz;
	Eina_List *areas;
} Eon_Svg_Damages;

static inline Eon_Svg * _eon_svg_get(Eon_Element *e)
{
	Eon_Svg *thiz;

	thiz = eon_element_data_get(e);
	return thiz;
}

static Eina_Bool _eon_svg_damages_cb(Ender_Element *e, Eina_Rectangle *damage, void *user_data)
{
	Eon_Svg_Damages *data = user_data;
	Eina_Rectangle *area;

	area = malloc(sizeof(Eina_Rectangle));
	*area = *damage;

	data->areas = eina_list_append(data->areas, area);
	enesim_renderer_image_damage_add(data->thiz->image, area);

	return EINA_TRUE;
}

static Eina_Bool _eon_svg_idler(void *user_data)
{
	Eon_Svg *thiz = user_data;
	Eon_Svg_Damages data;
	Eina_Rectangle *area;

	if (!thiz->svg) goto done;
	if (!thiz->s) goto done;
	/* get the damages, render them into the surface and inform enesim
	 * that the surface has damages
	 */
	data.areas = NULL;
	data.thiz = thiz;

	esvg_svg_setup(thiz->svg, NULL);
	esvg_svg_damages_get(thiz->svg, _eon_svg_damages_cb, &data);
	if (!data.areas) goto done;

	esvg_svg_draw_list(thiz->svg, thiz->s, data.areas, 0, 0, NULL);
	EINA_LIST_FREE (data.areas, area)
		free(area);

done:
	return EINA_TRUE;
}

static Eina_Bool _eon_svg_timer(void *user_data)
{
	Eon_Svg *thiz = user_data;

	if (!thiz->svg) goto done;

	esvg_svg_time_tick(thiz->svg);
done:
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                         The Esvg's parser interface                        *
 *----------------------------------------------------------------------------*/
#if 0
static void _eon_svg_href_set(void *data, Enesim_Renderer *r, const char *href)
{
	Eon_Svg *thiz = data;

	printf("eon svg href set %p %s\n", r, href);
}

static Esvg_Parser_Descriptor _svg_descriptor = {
	/* .href_set = */ _eon_svg_href_set,
};
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
/* FIXME add the click event here instead of the svg itself, as every
 * inherited eleement of this type must handle this
 */
static void _eon_svg_initialize(Ender_Element *e)
{
	Eon_Svg *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_svg_get(ee);
	if (!_esvg_init++)
		esvg_init();
#if 0
	ender_event_listener_add(e, "MouseIn", _svg_mouse_in, NULL);
	ender_event_listener_add(e, "MouseOut", _svg_mouse_out, NULL);
	ender_event_listener_add(e, "MouseDown", _svg_mouse_down, NULL);
	ender_event_listener_add(e, "MouseUp", _svg_mouse_up, NULL);
	if (thiz->initialize)
		thiz->initialize(e);
#endif
	/* add the idler */
}

static void _eon_svg_backend_added(Eon_Element *e, Eon_Backend *b)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(e);
	eon_backend_idler_add(b, _eon_svg_idler, thiz);
	eon_backend_timer_add(b, 1.0/30.0, _eon_svg_timer, thiz);
}

static void _eon_svg_backend_removed(Eon_Element *e, Eon_Backend *b)
{
	//eon_backend_idler_remove(b, _eon_svg_idler, e);
}

static void _eon_svg_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Svg *thiz;
	int iw, ih;

	thiz = _eon_svg_get(e);

	if (!thiz->svg) return;

	iw = round(g->width);
	ih = round(g->height);

	if (thiz->iw != iw || thiz->ih != ih)
	{
		esvg_svg_container_width_set(thiz->svg, g->width);
		esvg_svg_container_height_set(thiz->svg, g->height);

		/* TODO create the surface using the eon way */
		if (thiz->relative)
		{
			/* in case of relative svg, create the svg of size of the geometry */
			if (thiz->s)
				enesim_surface_unref(thiz->s);
			thiz->s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, iw, ih);
			enesim_renderer_image_width_set(thiz->image, iw);
			enesim_renderer_image_height_set(thiz->image, ih);
		}
		else
		{
			/* in case the svg does not use relative coordinates, just create the image
			 * of size of the svg
			 */
			if (!thiz->s)
			{
				thiz->s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, thiz->w, thiz->h);
				enesim_renderer_image_width_set(thiz->image, thiz->w);
				enesim_renderer_image_height_set(thiz->image, thiz->h);
			}
		}
		enesim_renderer_image_src_set(thiz->image, thiz->s);
	}
	enesim_renderer_image_x_set(thiz->image, g->x);
	enesim_renderer_image_y_set(thiz->image, g->y);
}

static void _eon_svg_hints_get(Eon_Element *e, Eon_Hints *hints)
{
	Eon_Svg *thiz;
	Esvg_Length length;
	Eina_Bool xrel = EINA_TRUE;
	Eina_Bool yrel = EINA_TRUE;

	thiz = _eon_svg_get(e);
	/* load again the file */
	if (thiz->changed)
	{
		if (thiz->svg)
		{
			ender_element_unref(thiz->svg);
			thiz->svg = NULL;
		}
		if (thiz->file)
		{
			thiz->svg = esvg_parser_load(thiz->file);
		}
		thiz->changed = EINA_FALSE;
	}
	if (!thiz->svg) return;

	esvg_svg_width_get(thiz->svg, &length);
	if (length.unit != ESVG_UNIT_LENGTH_PERCENT)
	{
		double w;

		esvg_svg_actual_width_get(thiz->svg, &w);
		hints->min.width = w;
		hints->max.width = w;
		hints->preferred.width = w;
		xrel = EINA_FALSE;

		thiz->w = round(w);
	}
	esvg_svg_height_get(thiz->svg, &length);
	if (length.unit != ESVG_UNIT_LENGTH_PERCENT)
	{
		double h;

		esvg_svg_actual_height_get(thiz->svg, &h);
		hints->min.height = h;
		hints->max.height = h;
		hints->preferred.height = h;
		yrel = EINA_FALSE;

		thiz->h = round(h);
	}
	thiz->relative = yrel || xrel;
}

static Enesim_Renderer * _eon_svg_renderer_get(Ender_Element *e)
{
	Eon_Svg *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_svg_get(ee);
	return thiz->image;
}

static void _eon_svg_free(Eon_Element *e)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(e);
	if (thiz->file)
		free(thiz->file);
	if (thiz->svg)
		ender_element_unref(thiz->svg);
	free(thiz);
}

static Eon_Element_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_svg_initialize,
	/* .backend_added 	= */ _eon_svg_backend_added,
	/* .backend_removed 	= */ _eon_svg_backend_removed,
	/* .renderer_get	= */ _eon_svg_renderer_get,
	/* .hints_get 		= */ _eon_svg_hints_get,
	/* .geometry_set 	= */ _eon_svg_geometry_set,
	/* .is_focusable	= */ NULL,
	/* .free		= */ _eon_svg_free,
	/* .name 		= */ "svg",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_svg_new(void)
{
	Eon_Svg *thiz;
	Eon_Element *e;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Svg));
	if (!thiz) return NULL;

	r = enesim_renderer_image_new();
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->image = r;

	e = eon_element_new(&_descriptor, thiz);
	if (!e) goto base_err;

	return e;

base_err:
	free(thiz);
	return NULL;
}

static void _eon_svg_file_set(Eon_Element *e, const char *file)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(e);
	thiz->file = strdup(file);
	thiz->changed = EINA_TRUE;
}

static void _eon_svg_file_get(Eon_Element *e, const char **file)
{
	Eon_Svg *thiz;

	if (!file) return;
	thiz = _eon_svg_get(e);
	*file = thiz->file;
}

#define _eon_svg_delete NULL
#include "eon_generated_svg.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_svg_init(void)
{
	_eon_svg_init();
}

void eon_svg_shutdown(void)
{
	_eon_svg_shutdown();
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_svg_new(void)
{
	return EON_ELEMENT_NEW("svg");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_svg_file_set(Ender_Element *e, const char *file)
{
	ender_element_property_value_set(e, EON_SVG_FILE, file, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_svg_file_get(Ender_Element *e, const char **file)
{
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	_eon_svg_file_get(ee, file);
}

