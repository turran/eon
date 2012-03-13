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
	Ender_Element *layout;
	unsigned int width;
	unsigned int height;
	Eon_Backend *backend;
	void *data;
};

static Ecore_Idle_Enterer * _global_idler = NULL;

static Eina_Bool _global_idler_cb(void *data)
{
	/* we must call this before the window idlers
	 * basically we process the emage events here
	 */
	emage_dispatch();
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Eon_Window * eon_window_new(Eon_Backend *backend, Ender_Element *layout,
		unsigned int width, unsigned int height)
{
	Eon_Window *ee;
	void *data;

	if (!backend) return NULL;
	if (!layout) return NULL;
	if (!eon_is_layout(layout)) return NULL;

	if (!eon_backend_window_new(backend, layout, width, height, &data))
		return NULL;

	if (!_global_idler)
		_global_idler = ecore_idle_enterer_add(_global_idler_cb, NULL);

	ee = calloc(1, sizeof(Eon_Window));
	ee->width = width;
	ee->height = height;
	ee->backend = backend;
	ee->layout = layout;
	ee->data = data;

	return ee;
}
