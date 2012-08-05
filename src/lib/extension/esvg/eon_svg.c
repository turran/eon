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
	Enesim_Renderer *generated_r;
	Eina_Bool needs_setup : 1;
	/* TODO add the event handler */
} Eon_Svg;

static inline Eon_Svg * _eon_svg_get(Eon_Element *e)
{
	Eon_Svg *thiz;

	thiz = eon_element_data_get(e);
	return thiz;
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

}

static void _eon_svg_hints_get(Eon_Element *e, Eon_Size *min, Eon_Size *max, Eon_Size *preferred)
{

}

static Eina_Bool _eon_svg_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Svg *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_svg_get(ee);
	if (thiz->file && !thiz->generated_r)
	{

		thiz->generated_r = esvg_parser_load(thiz->file, NULL, NULL); //&_svg_descriptor, thiz);
	}
	thiz->needs_setup = EINA_FALSE;

	return EINA_TRUE;
}

static Enesim_Renderer * _eon_svg_renderer_get(Ender_Element *e)
{
	Eon_Svg *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_svg_get(ee);
	return thiz->generated_r;
}

static Eina_Bool _eon_svg_needs_setup(Ender_Element *e)
{
	Eon_Svg *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_svg_get(ee);
	return thiz->needs_setup;
}

static void _eon_svg_free(Eon_Element *e)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(e);
	free(thiz);
}

static Eon_Element_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_svg_initialize,
	/* .setup 		= */ _eon_svg_setup,
	/* .renderer_get	= */ _eon_svg_renderer_get,
	/* .needs_setup 	= */ _eon_svg_needs_setup,
	/* .hints_get 		= */ _eon_svg_hints_get,
	/* .geometry_set 	= */ _eon_svg_geometry_set,
	/* .is_focusable	= */ EINA_TRUE,
	/* .free		= */ _eon_svg_free,
	/* .name 		= */ "svg",
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_svg_new(void)
{
	Eon_Svg *thiz;
	Eon_Element *e;

	thiz = calloc(1, sizeof(Eon_Svg));
	if (!thiz) return NULL;

	e = eon_element_new(&_descriptor, thiz);
	if (!e) goto renderer_err;

	return e;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_svg_file_set(Eon_Element *e, const char *file)
{
	Eon_Svg *thiz;

	thiz = _eon_svg_get(e);
	thiz->file = strdup(file);
	thiz->needs_setup = EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_svg_delete NULL
#define _eon_svg_file_get NULL
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
	ender_element_value_set(e, "file", file, NULL);
}
