/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2014 Jorge Luis Zapata
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
#include <Eon.h>
/*----------------------------------------------------------------------------*
 *                           Enesim Image Finder API                          *
 *----------------------------------------------------------------------------*/
static const char * _enesim_image_eon_data_from(Enesim_Stream *data)
{
	char buf[4096];
	char *ret = NULL;
	int count;
	int i;

	/* TODO we should find the first tag and skip all the comments */
	/* only try to find the <svg tag in the first 4096 bytes */
	count = enesim_stream_read(data, buf, 4096);
	for (i = 0; i < count; i++)
	{
		if (buf[i] == '<' && i + 4 < count)
		{
			/* check for the <svg tag */
			if (!strncmp(&buf[i], "<eon", 4))
			{
				ret = "application/eon+xml";
				break;
			}
		}
	}
	return ret;
}

static const char * _enesim_image_eon_extension_from(const char *ext)
{
	if (!strcmp(ext, "eon"))
		return "application/eon+xml";
	return NULL;
}

static Enesim_Image_Finder_Descriptor _finder = {
	/* .version 		= */ ENESIM_IMAGE_FINDER_DESCRIPTOR_VERSION,
	/* .data_from 		= */ _enesim_image_eon_data_from,
	/* .extension_from 	= */ _enesim_image_eon_extension_from,
};
/*----------------------------------------------------------------------------*
 *                             Module API                                     *
 *----------------------------------------------------------------------------*/
static Eina_Bool _enesim_image_eon_init(void)
{
	if (!enesim_image_finder_register(&_finder))
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

static void _enesim_image_eon_shutdown(void)
{
	enesim_image_finder_unregister(&_finder);
}

EINA_MODULE_INIT(_enesim_image_eon_init);
EINA_MODULE_SHUTDOWN(_enesim_image_eon_shutdown);

