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
#include "Eon_Theme.h"
#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Theme_Compound
{
	EINA_MAGIC;
	/* properties */
	/* interface */
	//Enesim_Renderer_Name name_get;
	Enesim_Renderer_Delete free;
	/* private */
	Enesim_Renderer *relative;
	void *data;
} Eon_Theme_Compound;

static inline Eon_Theme_Compound * _eon_theme_compound_get(Enesim_Renderer *r)
{
	Eon_Theme_Compound *thiz;

	thiz = eon_theme_widget_data_get(r);

	return thiz;
}

static Enesim_Renderer * _eon_theme_compound_renderer_get(Enesim_Renderer *r)
{
	Eon_Theme_Compound *thiz;

	thiz = _eon_theme_compound_get(r);
	return eon_theme_widget_renderer_get(thiz->relative);
}

static Eina_Bool _eon_theme_compound_setup(Enesim_Renderer *r,
		const Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Eon_Theme_Compound *thiz;

	thiz = _eon_theme_compound_get(r);
	return eon_theme_widget_setup(thiz->relative, error);
}

static const char * _eon_theme_compound_name(Enesim_Renderer *r)
{
	return "theme_compound";
}

static void _eon_theme_compound_free(Enesim_Renderer *r)
{
	Eon_Theme_Compound *thiz;

	thiz = _eon_theme_compound_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_compound_new(
		Eon_Theme_Compound_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Compound *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Compound));
	thiz->data = data;
	thiz->free = descriptor->free;

	pdescriptor.renderer_get = _eon_theme_compound_renderer_get;
	pdescriptor.setup = _eon_theme_compound_setup;
	pdescriptor.name = _eon_theme_compound_name;
	pdescriptor.free = _eon_theme_compound_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_compound_renderer_set(Enesim_Renderer *r, Enesim_Renderer *relative)
{
	Eon_Theme_Compound *thiz;

	thiz = _eon_theme_compound_get(r);
	thiz->relative = relative;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_theme_compound_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Compound *thiz;

	thiz = _eon_theme_compound_get(r);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_theme_compound(Enesim_Renderer *r)
{
	return EINA_TRUE;
}

