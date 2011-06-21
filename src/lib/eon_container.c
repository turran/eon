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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Container
{
	/* properties */
	Ender_Element *content;
	/* private */
	Eon_Element_Initialize initialize;
	Eon_Element_Setup setup;
	Enesim_Renderer_Delete free;
	void *data;
} Eon_Container;

static inline Eon_Container * _eon_container_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_container_initialize(Ender_Element *e)
{
	Eon_Container *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_container_get(r);
	if (thiz->initialize)
		thiz->initialize(e);
}

static void _eon_container_setup(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (thiz->content)
	{
		Enesim_Renderer *content_r;

		content_r = ender_element_renderer_get(thiz->content);
		if (!eon_element_setup(content_r))
			return EINA_FALSE;
	}
	if (thiz->setup)
		return thiz->setup(r);

	return EINA_TRUE;
}

static void _eon_container_free(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (thiz->free)
		thiz->free(r);
	free(thiz);
}

/* TODO */
/* register the intiialize and free too
 * on initialize we should just check whenever the content has changed
 * (i.e mutation event) if so, inform that we have changed too
 */
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_container_data_get(Enesim_Renderer *r)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (!thiz) return NULL;
	return thiz->data;
}

Enesim_Renderer * eon_container_new(Eon_Container_Descriptor *descriptor, void *data)
{
	Eon_Container *thiz;
	Eon_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Container));
	if (!thiz) return NULL;
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->initialize = descriptor->initialize;

	pdescriptor.initialize = _eon_container_initialize;
	pdescriptor.free = _eon_container_free;
	pdescriptor.setup = _eon_container_setup;
	pdescriptor.name = descriptor->name;

	r = eon_widget_new(&pdescriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_set(Enesim_Renderer *r, Ender_Element *content)
{
	Eon_Container *thiz;
	Enesim_Renderer *rcontent;

	thiz = _eon_container_get(r);
	if (!thiz) return;

	thiz->content = content;
	rcontent = ender_element_renderer_get(content);
	eon_element_property_set(r, "content", rcontent, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_container_content_get(Enesim_Renderer *r, const Ender_Element **content)
{
	Eon_Container *thiz;

	thiz = _eon_container_get(r);
	if (!thiz) return;

	*content = thiz->content;
}

