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

#include "eon_backend.h"
#include "eon_private_backend.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Backend
{
	Eon_Backend_Descriptor descriptor;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Backend * eon_backend_new(Eon_Backend_Descriptor *descriptor, void *data)
{
	Eon_Backend *backend;

	backend = calloc(1, sizeof(Eon_Backend));
	backend->descriptor = *descriptor;
	backend->data = data;

	return backend;
}

void eon_backend_window_delete(Eon_Backend *backend, void *window_data)
{
	if (!backend->descriptor.window_delete) return;
	backend->descriptor.window_delete(backend->data, window_data);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Eon_Window * eon_backend_window_new(Eon_Backend *backend, Ender_Element *container,
		unsigned int width, unsigned int height)
{
	void *data;

	if (!backend) return NULL;
	if (!container) return NULL;
	if (!eon_is_container(container)) return NULL;

	if (!backend->descriptor.window_new) return NULL;
	if (!backend->descriptor.window_new(backend->data, container, width, height, &data))
		return NULL;

	return eon_window_new(backend, width, height, data);
}

EAPI void eon_backend_idler_add(Eon_Backend *backend, Eon_Backend_Idler cb, void *data)
{
	if (!backend->descriptor.idler_add) return;
	backend->descriptor.idler_add(backend->data, cb, data);
}

EAPI void eon_backend_run(Eon_Backend *backend)
{
	if (!backend->descriptor.run) return;
	backend->descriptor.run(backend->data);
}

EAPI void eon_backend_delete(Eon_Backend *backend)
{
	if (!backend->descriptor.free) return;
	backend->descriptor.free(backend->data);
}

EAPI void eon_backend_quit(Eon_Backend *backend)
{
	if (!backend->descriptor.quit) return;
	backend->descriptor.quit(backend->data);
}

