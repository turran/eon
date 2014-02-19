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
#ifndef _EON_EVENT_GEOMETRY_PRIVATE_H_
#define _EON_EVENT_GEOMETRY_PRIVATE_H_

Egueb_Dom_Event * eon_event_geometry_new(void);
void eon_event_geometry_set(Egueb_Dom_Event *e, Eina_Rectangle *geom);
Eina_Bool eon_event_geometry_get(Egueb_Dom_Event *e, Eina_Rectangle *geom);

#endif