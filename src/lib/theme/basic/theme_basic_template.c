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
#include "Eon_Basic.h"
#include "eon_theme_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Template
{
	Enesim_Renderer *compound;
} Template;

static inline Template * _template_get(Enesim_Renderer *r)
{
	Template *thiz;

	thiz = enesim_renderer_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _template_setup(Enesim_Renderer *r, Enesim_Error **error)
{
	Template *thiz;

	thiz = _template_get(r);

	return thiz->compound;
}

static void _template_cleanup(Enesim_Renderer *r)
{
	Template *thiz;

	thiz = _template_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _template_free(Enesim_Renderer *r)
{
	Template *thiz;

	thiz = _template_get(r);
	if (thiz->compound)
		enesim_renderer_unref(thiz->compound);
	free(thiz);
}

static void _template_boundings(Enesim_Renderer *r, Eina_Rectangle *boundings)
{
	Template *thiz;

	thiz = _template_get(r);
	enesim_renderer_boundings(thiz->compound, boundings);
}

static void _template_flags(Enesim_Renderer *r, Enesim_Renderer_Flag *flags)
{
	Template *thiz;

	thiz = _template_get(r);
	enesim_renderer_flags(thiz->compound, flags);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _template_setup,
	.sw_cleanup = _template_cleanup,
	.boundings = _template_boundings,
	.flags = _template_flags,
	.free = _template_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_theme_basic_template_new(void)
{
	Enesim_Renderer *r;
	Template *thiz;

	thiz = calloc(1, sizeof(Template));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_unref(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}
