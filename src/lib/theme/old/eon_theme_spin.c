/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
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
#define EON_THEME_SPIN_MAGIC 0xe0410002
#define EON_THEME_SPIN_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_SPIN_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_SPIN_MAGIC);\
	} while(0)

#define EON_THEME_SPIN_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_SPIN_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_SPIN_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Spin
{
	EINA_MAGIC;
	/* properties */
	Enesim_Renderer *content;
	/* private */
	void *data;
	Eon_Theme_Spin_Margin_Get margin_get;
	Eon_Theme_Spin_Entry_Set entry_set;
	Eon_Theme_Spin_Arrows_Is_Inside arrows_is_inside;
	Enesim_Renderer_Delete free;
} Eon_Theme_Spin;

static inline Eon_Theme_Spin * _eon_theme_spin_get(Enesim_Renderer *r)
{
	Eon_Theme_Spin *thiz;

	thiz = eon_theme_widget_data_get(r);
	EON_THEME_SPIN_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static const char * _eon_theme_spin_name(Enesim_Renderer *r)
{
	return "theme_spin";
}

static void _eon_theme_spin_free(Enesim_Renderer *r)
{
	Eon_Theme_Spin *thiz;

	thiz = _eon_theme_spin_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_spin_new(Eon_Theme_Spin_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Spin *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Spin));
	EINA_MAGIC_SET(thiz, EON_THEME_SPIN_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->margin_get = descriptor->margin_get;
	thiz->entry_set = descriptor->entry_set;
	thiz->arrows_is_inside = descriptor->arrows_is_inside;
	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.setup = descriptor->setup;
	pdescriptor.name = _eon_theme_spin_name;
	pdescriptor.free = _eon_theme_spin_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

Eina_Bool eon_is_theme_spin(Enesim_Renderer *r)
{
	Eon_Theme_Spin *thiz;

	if (!eon_is_theme_widget(r))
		return EINA_FALSE;

	thiz = eon_theme_widget_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_SPIN_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

void * eon_theme_spin_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Spin *thiz;

	thiz = _eon_theme_spin_get(r);
	return thiz->data;
}

void eon_theme_spin_margin_get(Enesim_Renderer *r, Eon_Margin *margin)
{
	Eon_Theme_Spin *thiz;

	thiz = _eon_theme_spin_get(r);
	if (thiz->margin_get)
		thiz->margin_get(r, margin);
	else
	{
		margin->left = 0;
		margin->right = 0;
		margin->top = 0;
		margin->bottom = 0;
	}
}

void eon_theme_spin_entry_set(Enesim_Renderer *r, Enesim_Renderer *entry, Enesim_Renderer *old_entry)
{
	Eon_Theme_Spin *thiz;

	thiz = _eon_theme_spin_get(r);
	if (thiz->entry_set)
		thiz->entry_set(r, entry, old_entry);
}

void eon_theme_spin_arrows_is_inside(Enesim_Renderer *r, Eon_Position *cursor, Eina_Bool *inc, Eina_Bool *dec)
{
	Eon_Theme_Spin *thiz;

	thiz = _eon_theme_spin_get(r);
	*inc = EINA_FALSE;
	*dec = EINA_FALSE;
	/* get the inc/dec arrow anc check if we are inside its bounds */
	if (thiz->arrows_is_inside)
		thiz->arrows_is_inside(r, cursor, inc, dec);
	/* FIXME add a default case tht we should get the renderers for each arrow
	 * and just intersect on them
	 */
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

