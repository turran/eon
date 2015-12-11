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
#include "eon_private_main.h"

#include "eon_input.h"
#include "eon_main.h"

#include "eon_private_input.h"
#include "eon_private_mouse_proxy.h"
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

void eon_mouse_proxy_feed_move(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		double x, double y, double offset_x, double offset_y)
{
	if (!thiz->descriptor->move)
		return;
	thiz->descriptor->move(thiz->data, e, input, x, y, offset_x, offset_y);
}

void eon_mouse_proxy_feed_button_down(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		int button)
{
	if (!thiz->descriptor->button_down)
		return;

	thiz->descriptor->button_down(thiz->data, e, input, button);
}

void eon_mouse_proxy_feed_button_up(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		int button)
{
	if (!thiz->descriptor->button_up)
		return;
	thiz->descriptor->button_up(thiz->data, e, input, button);
}

void eon_mouse_proxy_feed_wheel(Eon_Mouse_Proxy *thiz,
		Ender_Element *e,
		Eon_Input *input,
		int orientation)
{
	if (!thiz->descriptor->wheel)
		return;

	thiz->descriptor->wheel(thiz->data, e, input, orientation);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

