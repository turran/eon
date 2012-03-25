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
struct _Eon_Mouse_Proxy
{
	Eon_Mouse_Proxy_Descriptor *descriptor;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Mouse_Proxy * eon_mouse_proxy_new(
		Eon_Mouse_Proxy_Descriptor *descriptor,
		void *data)
{
	Eon_Mouse_Proxy *thiz;

	thiz = calloc(1, sizeof(Eon_Mouse_Proxy));
	thiz->descriptor = descriptor;
	thiz->data = data;

	return thiz;
}

void eon_mouse_proxy_delete(Eon_Mouse_Proxy *thiz)
{
	Eina_Bool ret = EINA_TRUE;

	if (thiz->descriptor->destroy)
		ret = thiz->descriptor->destroy(thiz->data);

	if (ret)
		free(thiz);
}

void eon_mouse_proxy_feed_mouse_move(Eon_Mouse_Proxy *thiz,
		double x, double y, double offset_x, double offset_y)
{

}

void eon_mouse_proxy_feed_mouse_down(Eon_Mouse_Proxy *thiz)
{
}

void eon_mouse_proxy_feed_mouse_up(Eon_Mouse_Proxy *thiz)
{

}

void eon_mouse_proxy_feed_mouse_wheel(Eon_Mouse_Proxy *thiz, int direction)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

