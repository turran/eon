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
#include "eon_input.h"
#include "eon_element.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
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
	int iw;
	int ih;

	Ender_Element *svg;
	Eina_Bool changed : 1;
	/* TODO add the event handler */
} Eon_Svg;

static inline Eon_Svg * _eon_svg_get(Eon_Element *e)
{
	Eon_Svg *thiz;

	thiz = eon_element_data_get(e);
	return thiz;
}

static void _eon_svg_idler(Eon_Svg *thiz)
{
	/* get the damages, render them into the surface and inform enesim
	 * that the surface has damages
	 */
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
}

static void _eon_svg_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	Eon_Svg *thiz;
	int iw, ih;

	thiz = _eon_svg_get(e);

	iw = round(g->width);
	ih = round(g->height);

	if (thiz->iw != iw || thiz->ih != ih)
	{
		/* TODO create the surface using the eon way */
		esvg_renderable_container_width_set(thiz->svg, g->width);
		esvg_renderable_container_height_set(thiz->svg, g->height);
	}
}

static void _eon_svg_hints_get(Eon_Element *e, Eon_Hints *hints)
{
	Eon_Svg *thiz;
	Esvg_Length length;

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
			thiz->svg = esvg_parser_load(thiz->file, NULL, NULL);
		thiz->changed = EINA_FALSE;
	}
	if (thiz->svg) return;
	esvg_svg_width_get(thiz->svg, &length);
	if (length.unit != ESVG_UNIT_LENGTH_PERCENT)
	{
		/* add a function on svg to get the pixel size */
	}
	esvg_svg_height_get(thiz->svg, &length);
	if (length.unit != ESVG_UNIT_LENGTH_PERCENT)
	{
		/* add a function on svg to get the pixel size */
	}
}

static Enesim_Renderer * _eon_svg_renderer_get(Ender_Element *e)
{
	Eon_Svg *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_svg_get(ee);
	return esvg_renderable_renderer_get(thiz->svg);
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
	/* .setup 		= */ NULL,
	/* .renderer_get	= */ _eon_svg_renderer_get,
	/* .needs_setup 	= */ NULL,
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
	thiz->image = r;

	e = eon_element_new(&_descriptor, thiz);
	if (!e) goto base_err;

	return e;

base_err:
	free(thiz);
	return NULL;
}

static void _eon_svg_file_set(Eon_Element *e, const char **file)
{
	Eon_Svg *thiz;

	if (!file) return;
	thiz = _eon_svg_get(e);
	*file = thiz->file;
}

static void _eon_svg_file_get(Eon_Element *e, const char *file)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(e);
	thiz->file = strdup(file);
	thiz->changed = EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_svg_delete NULL
#include "eon_generated_svg.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_svg_new(void)
{
	return esvg_svg_new();
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
EAPI void eon_svg_file_get(Ender_Element *e, const char *file)
{
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	_eon_svg_file_get(ee, file);
}

