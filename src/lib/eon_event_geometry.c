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
#include "eon_private.h"
#include "eon_main.h"

#include "eon_event_geometry.h"
#include "eon_event_geometry_private.h"

/* TODO
 * - Move the event name here
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Event_Geometry
{
	Eina_Rectangle geometry;
	Eina_Bool set;
} Eon_Event_Geometry;

Egueb_Dom_Event * _eon_event_geometry_new(Egueb_Dom_String *name)
{
	Eon_Event_Geometry *thiz;
	Egueb_Dom_Event *e;

	thiz = calloc(1, sizeof(Eon_Event_Geometry));
	e = egueb_dom_event_external_new(name, EINA_TRUE, EINA_TRUE, EINA_TRUE,
			EGUEB_DOM_EVENT_DIRECTION_BUBBLE_CAPTURE, thiz, free);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Event * eon_event_geometry_invalidate_new(void)
{
	return _eon_event_geometry_new(egueb_dom_string_ref(EON_EVENT_GEOMETRY_INVALIDATE));
}

Egueb_Dom_Event * eon_event_geometry_request_new(void)
{
	return _eon_event_geometry_new(egueb_dom_string_ref(EON_EVENT_GEOMETRY_REQUEST));
}

#if 0
Egueb_Dom_Event * eon_event_geometry_updated(Eina_Rectangle *geometry)
{
	Eon_Event_Geometry *thiz;
	Egueb_Dom_Event *e;

	e = _eon_event_geometry_new(
			egueb_dom_string_ref(EON_EVENT_GEOMETRY_UPDATED));
	thiz = egueb_dom_event_external_data_get(e);
	thiz->geometry = *geometry;
	return e;
}
#endif

Eina_Bool eon_event_geometry_request_geometry_get(Egueb_Dom_Event *e, Eina_Rectangle *geom)
{
	Eon_Event_Geometry *thiz;

	if (!e) return EINA_FALSE;
	thiz = egueb_dom_event_external_data_get(e);
	if (!thiz->set) return EINA_FALSE;

	*geom = thiz->geometry;
	/* reset it */
	thiz->set = EINA_FALSE;
	return EINA_TRUE;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_event_geometry_request_geometry_set(Egueb_Dom_Event *e,
		Eina_Rectangle *geom)
{
	Eon_Event_Geometry *thiz;

	if (!e) return;
	thiz = egueb_dom_event_external_data_get(e);
	thiz->geometry = *geom;
	thiz->set = EINA_TRUE;
}
