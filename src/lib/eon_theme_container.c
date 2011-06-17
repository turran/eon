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
	Enesim_Renderer *content;
	/* private */
	Eon_Theme_Container_Decoration_Width_Get decoration_width_get;
	Eon_Theme_Container_Decoration_Height_Get decoration_height_get;
	void *data;
	Enesim_Renderer_Delete free;
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

static double _eon_theme_container_min_width_get(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;
	double v = 0;

	thiz = _eon_theme_container_get(r);
	if (thiz->decoration_width_get)
		v = thiz->decoration_width_get(r);
	if (!thiz->content)
		goto end;
	if (eon_is_element(thiz->content))
	{
		double cmin_width;

		eon_element_min_width_get(thiz->content, &cmin_width);
		v += cmin_width;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(thiz->content, &boundings);
		v += boundings.w;
	}
end:
	return v;
}

static double _eon_theme_container_max_width_get(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;
	double v = 0;

	thiz = _eon_theme_container_get(r);
	if (thiz->decoration_width_get)
		v = thiz->decoration_width_get(r);
	if (!thiz->content)
		goto end;
	if (eon_is_element(thiz->content))
	{
		double cmax_width;

		eon_element_max_width_get(thiz->content, &cmax_width);
		v += cmax_width;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(thiz->content, &boundings);
		v += boundings.w;
	}
end:
	return v;
}

static double _eon_theme_container_min_height_get(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;
	double v = 0;

	thiz = _eon_theme_container_get(r);
	if (thiz->decoration_height_get)
		v = thiz->decoration_height_get(r);
	if (!thiz->content)
		goto end;
	if (eon_is_element(thiz->content))
	{
		double cmin_height;

		eon_element_min_height_get(thiz->content, &cmin_height);
		v += cmin_height;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(thiz->content, &boundings);
		v += boundings.w;
	}
end:
	return v;
}

static double _eon_theme_container_max_height_get(Enesim_Renderer *r)
{
	Eon_Theme_Container *thiz;
	double v = 0;

	thiz = _eon_theme_container_get(r);
	if (thiz->decoration_height_get)
		v = thiz->decoration_height_get(r);
	if (!thiz->content)
		goto end;
	if (eon_is_element(thiz->content))
	{
		double cmax_height;

		eon_element_max_height_get(thiz->content, &cmax_height);
		v += cmax_height;
	}
	else
	{
		Enesim_Rectangle boundings;

		enesim_renderer_boundings(thiz->content, &boundings);
		v += boundings.w;
	}
end:
	return v;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Renderer * eon_theme_container_new(Eon_Theme_Container_Descriptor *descriptor,
		void *data)
{
	Eon_Theme_Container *thiz;
	Eon_Theme_Widget_Descriptor pdescriptor;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Theme_Container));
	EINA_MAGIC_SET(thiz, EON_THEME_CONTAINER_MAGIC);
	thiz->data = data;
	thiz->free = descriptor->free;
	thiz->decoration_width_get = descriptor->decoration_width_get;
	thiz->decoration_height_get = descriptor->decoration_height_get;
	pdescriptor.max_width_get = _eon_theme_container_max_width_get;
	pdescriptor.min_width_get = _eon_theme_container_min_width_get;
	pdescriptor.max_height_get = _eon_theme_container_max_height_get;
	pdescriptor.min_height_get = _eon_theme_container_min_height_get;
	pdescriptor.sw_setup = descriptor->sw_setup;
	pdescriptor.sw_cleanup = descriptor->sw_cleanup;
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
	thiz->content = content;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_theme_container_content_get(Enesim_Renderer *r, Enesim_Renderer **content)
{
	Eon_Theme_Container *thiz;

	thiz = _eon_theme_container_get(r);
	*content = thiz->content;
}

