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
typedef struct _Eon_Image
{
	/* properties */
} Eon_Image;

static void _eon_image_file_get(Enesim_Renderer *r, const char **file);

static inline Eon_Image * _eon_image_get(Enesim_Renderer *r)
{
	Eon_Image *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                         The Eon's element interface                        *
 *----------------------------------------------------------------------------*/
static void _eon_image_free(Enesim_Renderer *r)
{
	Eon_Image *thiz;

	thiz = _eon_image_get(r);
	free(thiz);
}

static Eon_Element_Descriptor _descriptor = {
	.name = "image",
	.free = _eon_image_free,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_image_new(void)
{
	Eon_Image *thiz;
	Enesim_Renderer *r;

	printf("creating new image\n");
	thiz = calloc(1, sizeof(Eon_Image));
	if (!thiz) return NULL;

	r = eon_element_new(&_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_image_file_set(Enesim_Renderer *r, const char *file)
{
	eon_element_property_set(r, "file", file, NULL);
}

static void _eon_image_file_get(Enesim_Renderer *r, const char **file)
{
	eon_element_property_get(r, "file", file, NULL);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
	return ender_element_new_with_namespace("image", "eon");
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

