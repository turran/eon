/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
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
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Button
{
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill;
} Button;

static inline Button * _button_get(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = enesim_renderer_data_get(r);
	return thiz;
}

static void _button_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->fill(thiz->compound, x, y, len, dst);
}

static Eina_Bool _button_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Button *thiz;

	thiz = _button_get(r);
	thiz->fill = enesim_renderer_sw_fill_get(thiz->compound);
	if (!thiz->fill) return EINA_FALSE;

	*fill = _button_draw;

	return EINA_TRUE;
}

static void _button_cleanup(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = _button_get(r);
	enesim_renderer_sw_cleanup(thiz->compound);
}

static void _button_free(Enesim_Renderer *r)
{
	Button *thiz;

	thiz = _button_get(r);
	if (thiz->compound)
		enesim_renderer_delete(thiz->compound);
	free(thiz);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _button_setup,
	.sw_cleanup = _button_cleanup,
	.free = _button_free,
};

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Enesim_Renderer * eon_basic_button_new(void)
{
	Enesim_Renderer *r;
	Enesim_Renderer_Flag flags;
	Button *thiz;

	thiz = calloc(1, sizeof(Button));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	thiz->compound = r;

	r = enesim_renderer_new(&_descriptor, flags, thiz);
	if (!r) goto renderer_err;

	return r;
renderer_err:
	enesim_renderer_delete(thiz->compound);
compound_err:
	free(thiz);
	return NULL;
}
