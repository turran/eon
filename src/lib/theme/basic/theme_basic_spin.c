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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Basic_Spin
{
	Enesim_Renderer *entry;
	Enesim_Renderer *compound;
} Basic_Spin;

static inline Basic_Spin * _spin_get(Enesim_Renderer *r)
{
	Basic_Spin *thiz;

	thiz = eon_theme_spin_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Spin theme interface                         *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _spin_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Basic_Spin *thiz;
	double ox, oy;
	double width, height;

	thiz = _spin_get(r);

	eon_theme_widget_width_get(r, &width);
	eon_theme_widget_height_get(r, &height);

	/* setup common properties */
	enesim_renderer_origin_get(r, &ox, &oy);
	enesim_renderer_origin_set(thiz->compound, ox, oy);

	return thiz->compound;
}

static void _spin_entry_set(Enesim_Renderer *r, Enesim_Renderer *entry)
{
	Basic_Spin *thiz;

	thiz = _spin_get(r);
	/* FIXME this logic of checking if the entry is the same or not
	 * should be handled on the widget */
	if (thiz->entry == entry) return;

	if (thiz->entry)
	{
		enesim_renderer_compound_layer_remove(thiz->compound, thiz->entry);
	}

	if (entry)
	{
		enesim_renderer_compound_layer_add(thiz->compound, entry);
	}
	thiz->entry = entry;
}

static void _spin_free(Enesim_Renderer *r)
{
	Basic_Spin *thiz;

	thiz = _spin_get(r);
	free(thiz);
}

static Eon_Theme_Spin_Descriptor _descriptor = {
	.entry_set = _spin_entry_set,
	.setup = _spin_setup,
	.free = _spin_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_spin_new(void)
{
	Enesim_Renderer *r;
	Basic_Spin *thiz;

	thiz = calloc(1, sizeof(Basic_Spin));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	thiz->compound = r;

	r = eon_theme_spin_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	free(thiz);
	return NULL;
}
