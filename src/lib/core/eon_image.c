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
#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_image.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_IMAGE_FILE;

typedef enum _Eon_Image_State
{
	EON_IMAGE_STATE_FILE_CHANGED,
	EON_IMAGE_STATE_LOADING,
	EON_IMAGE_STATE_LOADED,
	EON_IMAGE_STATE_ERROR,
} Eon_Image_State;

typedef struct _Eon_Image
{
	/* properties */
	char *file;
	Eina_Bool scalable;
	/* private */
	Eon_Image_State state;
	int original_width;
	int original_height;
	Enesim_Surface *surface;
} Eon_Image;

static void _eon_image_file_get(Eon_Element *ee, const char **file);

static inline Eon_Image * _eon_image_get(Eon_Element *ee)
{
	Eon_Image *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}

#if 0
static void _emage_async_cb(Enesim_Surface *s, void *data, int error)
{
	Eon_Image *thiz;
	Eon_Element *ee = data;

	thiz = _eon_image_get(ee);
	if (error)
	{
		eon_theme_instance_state_set(ee, "failed", EINA_FALSE);
		thiz->original_width = -1;
		thiz->original_height = -1;
		return;
	}
	enesim_surface_size_get(s, &thiz->original_width, &thiz->original_height);
	/* set the new new surface to the theme associated */
	eon_widget_property_set(ee, "source", s, NULL);
	eon_theme_instance_state_set(ee, "loaded", EINA_FALSE);
}
#endif
/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
#if 0
static Eina_Bool _eon_image_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Image *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_image_get(ee);
	if (!thiz->file_changed) return EINA_TRUE;

	eon_theme_instance_state_set(ee, "loading", EINA_FALSE);
	emage_load_async(thiz->file, NULL, ENESIM_FORMAT_ARGB8888, NULL, _emage_async_cb,
			ee, NULL);
	thiz->file_changed = EINA_FALSE;

	return EINA_TRUE;
}

static double _eon_image_max_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size image_size, out;

	image_size.width = 2048;
	image_size.height = 2048;
	eon_theme_image_max_size_get(theme_r, &image_size, &out);

	return out.width;
}

static double _eon_image_max_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size image_size, out;

	image_size.width = 2048;
	image_size.height = 2048;
	eon_theme_image_max_size_get(theme_r, &image_size, &out);

	return out.height;
}

static double _eon_image_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size image_size, out;

	image_size.width = 1;
	image_size.height = 1;
	eon_theme_image_min_size_get(theme_r, &image_size, &out);

	return out.width;
}

static double _eon_image_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Size image_size, out;

	image_size.width = 1;
	image_size.height = 1;
	eon_theme_image_min_size_get(theme_r, &image_size, &out);

	return out.height;
}

static double _eon_image_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Image *thiz;
	Eon_Size image_size, out;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_image_get(ee);

	if (thiz->original_width < 0 || thiz->original_height < 0)
		return -1;

	image_size.width = thiz->original_width;
	image_size.height = thiz->original_height;
	eon_theme_image_preferred_size_get(theme_r, &image_size, &out);

	return out.width;
}

static double _eon_image_preferred_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	Eon_Image *thiz;
	Eon_Size image_size, out;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_image_get(ee);

	if (thiz->original_width < 0 || thiz->original_height < 0)
		return -1;

	image_size.width = thiz->original_width;
	image_size.height = thiz->original_height;
	eon_theme_image_preferred_size_get(theme_r, &image_size, &out);

	return out.height;
}
#endif

static void _eon_image_free(Eon_Element *e)
{
	Eon_Image *thiz;

	thiz = _eon_image_get(e);
	if (thiz->surface)
		enesim_surface_unref(thiz->surface);
	free(thiz);
}

static void _eon_image_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	Eon_Image *thiz;

	thiz = _eon_image_get(e);
	printf("image hints get\n");
	if (!thiz->file) return;

	if (thiz->state == EON_IMAGE_STATE_FILE_CHANGED)
	{
		emage_info_load(thiz->file, &thiz->original_width,
				&thiz->original_height, NULL);
		if (thiz->surface)
		{
			enesim_surface_unref(thiz->surface);
			thiz->surface = NULL;
		}
		thiz->surface = enesim_surface_new(ENESIM_FORMAT_ARGB8888,
				thiz->original_width, thiz->original_height);
		printf("going to set the surface\n");
		eon_theme_instance_property_set(theme, "surface", thiz->surface, NULL);
	}

	eon_size_values_set(&hints->preferred, thiz->original_width, thiz->original_height);
	if (thiz->scalable)
	{
		eon_size_values_set(&hints->min, 0, 0);
		eon_size_values_set(&hints->max, 2048, 2048);
	}
	else
	{
		hints->min = hints->preferred;
		hints->max = hints->preferred;
	}
}

static Eon_Widget_Descriptor _eon_image_descriptor = {
	/* .initialize 		= */ NULL,
	/* .setup 		= */ NULL,
	/* .needs_setup 	= */ NULL,
	/* .geometry_set 	= */ NULL,
	/* .free		= */ _eon_image_free,
	/* .name 		= */ "image",
	/* .hints_get 		= */ _eon_image_hints_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_image_new(void)
{
	Eon_Image *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;

	theme = eon_theme_instance_new("label", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Image));

	ee = eon_widget_new(theme, &_eon_image_descriptor, thiz);
	if (!ee) goto base_err;

	return ee;

base_err:
	free(thiz);
	return NULL;
}

static void _eon_image_file_set(Eon_Element *ee, const char *file)
{
	Eon_Image *thiz;

	thiz = _eon_image_get(ee);
	if (thiz->file)
	{
		free(thiz->file);
		thiz->file = NULL;
	}
	if (file)
	{
		thiz->file = strdup(file);
	}
	thiz->state = EON_IMAGE_STATE_FILE_CHANGED;
	/* TODO inform that we need to recaclulate the size */
}

static void _eon_image_file_get(Eon_Element *ee, const char **file)
{
	Eon_Image *thiz;

	thiz = _eon_image_get(ee);
	*file = thiz->file;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#define _eon_image_delete NULL
#include "eon_generated_image.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_image_new(void)
{
	return EON_ELEMENT_NEW("image");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_image_file_set(Ender_Element *e, const char *file)
{
	ender_element_value_set(e, "file", file, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_image_file_get(Ender_Element *e, const char **file)
{
	ender_element_value_get(e, "file", file, NULL);
}
