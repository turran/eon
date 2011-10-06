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
typedef struct _Basic_Entry
{
	/* properties */
	/* private */
	Enesim_Renderer_Sw_Fill fill;
} Basic_Entry;

/*----------------------------------------------------------------------------*
 *                          The Entry theme interface                         *
 *----------------------------------------------------------------------------*/
static Eon_Theme_Entry_Descriptor _descriptor = {
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Renderer * eon_basic_entry_new(void)
{
	Enesim_Renderer *r;
	Basic_Entry *thiz;

	thiz = calloc(1, sizeof(Basic_Entry));
	if (!thiz) return NULL;

	r = eon_theme_entry_new(&_descriptor, thiz);
	if (!r) goto renderer_err;
	return r;

renderer_err:
	free(thiz);
	return NULL;
}
