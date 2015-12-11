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
#include <unistd.h>
#include "eon_private_main.h"

#include "eon_main.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
char * eon_misc_path_to_absolute(const char *file)
{
	char path[PATH_MAX];
	char *ret;
	size_t len1;
	size_t len2;

	ret = getcwd(path, PATH_MAX);
	if (!ret)
	{
		printf("not enough space\n");
		return strdup(file);
	}
	len1 = strlen(path);
	len2 = len1 + strlen(file) + 2;
	ret = malloc(len2);
	strcpy(ret, path);
	/* FIXME windows case? */
	ret[len1] = '/';
	ret[len1 + 1] = '\0';
	strcat(ret, file);
	ret[len2] = '\0';

	return ret;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

