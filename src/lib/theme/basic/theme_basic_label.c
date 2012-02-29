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
#include "Eon_Theme.h"
#include "Eon_Basic.h"
#include "eon_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Label
{
} Label;

static inline Label * _label_get(Enesim_Renderer *r)
{
	Label *thiz;

	thiz = eon_theme_label_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                      The Eon's theme label interface                       *
 *----------------------------------------------------------------------------*/
static void _label_padding_get(Enesim_Renderer *r, Eon_Margin *padding)
{
	padding->top = 0;
	padding->left = 0;
	padding->right = 0;
	padding->bottom = 0;
}

static void _label_free(Enesim_Renderer *r)
{
	Label *thiz;

	thiz = _label_get(r);
	free(thiz);
}

static Eon_Theme_Label_Descriptor _descriptor = {
	.padding_get = _label_padding_get,
	.free = _label_free,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_label_new(void)
{
	Enesim_Renderer *r;
	Label *thiz;

	thiz = calloc(1, sizeof(Label));
	if (!thiz) return NULL;

	r = eon_theme_label_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}
