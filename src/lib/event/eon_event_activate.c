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

#include "eon_event_activate_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                             Event interface                                *
 *----------------------------------------------------------------------------*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Event * eon_event_activate_new(void)
{
	return egueb_dom_event_external_new(
		egueb_dom_string_ref(EON_NAME_EVENT_ACTIVATE),
		EINA_TRUE, EINA_TRUE, EINA_TRUE, EGUEB_DOM_EVENT_DIRECTION_CAPTURE_BUBBLE,
		NULL, NULL);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


