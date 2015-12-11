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
/**
 * @todo
 * - correctly abstract the window "class" to not use any ecore functionality
 * - we need to make the window itself have "properties" so the backend
 *   (specially the remote) can manage correctly the element's hierarchy
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Window
{
	Ender_Element *container;
	unsigned int width;
	unsigned int height;
	Eon_Backend *backend;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Window * eon_window_new(Eon_Backend *backend, Ender_Element *container,
		unsigned int width, unsigned int height, void *data)
{
	Eon_Window *ee;

	ee = calloc(1, sizeof(Eon_Window));
	ee->width = width;
	ee->height = height;
	ee->backend = backend;
	ee->container = container;
	ee->data = data;

	return ee;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

