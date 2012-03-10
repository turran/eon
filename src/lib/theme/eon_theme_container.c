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
#define EON_THEME_CONTAINER_MAGIC 0xe0410002
#define EON_THEME_CONTAINER_MAGIC_CHECK(d)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_CONTAINER_MAGIC))\
			EINA_MAGIC_FAIL(d, EON_THEME_CONTAINER_MAGIC);\
	} while(0)

#define EON_THEME_CONTAINER_MAGIC_CHECK_RETURN(d, ret)\
	do {\
		if (!EINA_MAGIC_CHECK(d, EON_THEME_CONTAINER_MAGIC)) {\
			EINA_MAGIC_FAIL(d, EON_THEME_CONTAINER_MAGIC);\
			return ret;\
		}\
	} while(0)

typedef struct _Eon_Theme_Container
{
	EINA_MAGIC;
	/* properties */
	Eon_Theme_Container_State current;
	Eon_Theme_Container_State past;
	/* interface */
	Eon_Theme_Widget_Needs_Setup needs_setup;
	Eon_Theme_Container_Setup setup;
	Enesim_Renderer_Delete free;
	/* private */
	Eina_Bool do_needs_setup : 1;
	void *data;
} Eon_Theme_Container;

static inline Eon_Theme_Container * _eon_theme_container_get(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;

	thiz = eon_theme_widget_data_get(r);
	EON_THEME_CONTAINER_MAGIC_CHECK_RETURN(thiz, NULL);

	return thiz;
}

static void _eon_theme_container_free(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	if (thiz->free) thiz->free(r);
	free(thiz);
}

static Eina_Bool _eon_theme_container_needs_setup(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	if (thiz->do_needs_setup)
		return EINA_TRUE;
	if (thiz->needs_setup)
		return thiz->needs_setup(r);
	return EINA_FALSE;
}

static Eina_Bool _eon_theme_container_setup(Enesim_Renderer *r,
		Eon_Theme_Widget_State *states[ENESIM_RENDERER_STATES],
		Enesim_Error **error)
{
	Eon_Theme_Container *thiz;
	const Eon_Theme_Container_State *cstates[ENESIM_RENDERER_STATES];

	thiz = _eon_theme_container_get(r);
	cstates[ENESIM_STATE_CURRENT] = &thiz->current;
	cstates[ENESIM_STATE_PAST] = &thiz->past;
	if (thiz->setup)
		return thiz->setup(r, states, cstates, error);
	thiz->do_needs_setup = EINA_FALSE;
	thiz->past = thiz->current;
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_container_new(Eon_Theme_Container_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Container *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor = { 0 };
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Container));
	EINA_MAGIC_SET(thiz, EON_THEME_CONTAINER_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->needs_setup = descriptor->needs_setup;
	thiz->setup = descriptor->setup;

	pdescriptor.renderer_get = descriptor->renderer_get;
	pdescriptor.setup = _eon_theme_container_setup;
	pdescriptor.cleanup = descriptor->cleanup;
	pdescriptor.needs_setup = _eon_theme_container_needs_setup;
	pdescriptor.name = descriptor->name;
	pdescriptor.free = _eon_theme_container_free;

	r = eon_theme_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

Eina_Bool eon_is_theme_container(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;

	if (!eon_is_theme_widget(r))
		return EINA_FALSE;

	thiz = eon_theme_widget_data_get(r);
	if (!EINA_MAGIC_CHECK(thiz, EON_THEME_CONTAINER_MAGIC))
		return EINA_FALSE;
	return EINA_TRUE;
}

void * eon_theme_container_data_get(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	return thiz->data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_container_content_set(Enesim_Renderer *r, Enesim_Renderer *content)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	if (thiz->current.content == content)
		return;
	thiz->current.content = content;
	thiz->do_needs_setup = EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_container_content_get(Enesim_Renderer *r, Enesim_Renderer **content)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	*content = thiz->current.content;
}

