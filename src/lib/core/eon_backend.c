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

#include "eon_window.h"
#include "eon_private_backend.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Backend
{
	Eon_Backend_Descriptor *descriptor;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Backend * eon_backend_new(Eon_Backend_Descriptor *descriptor, void *data)
{
	Eon_Backend *backend;

	backend = calloc(1, sizeof(Eon_Backend));
	backend->descriptor = descriptor;
	backend->data = data;

	return backend;
}

Eina_Bool eon_backend_window_new(Eon_Backend *backend, Ender_Element *layout,
		unsigned int width, unsigned int height, void *window_data)
{
	if (!backend->descriptor->window_new) return EINA_FALSE;

	return backend->descriptor->window_new(backend->data, layout, width, height, &window_data);
}

void eon_backend_window_delete(Eon_Backend *backend, void *window_data)
{
	if (!backend->descriptor->window_delete) return;
	backend->descriptor->window_delete(backend->data, window_data);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
