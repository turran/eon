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

#ifdef BUILD_BACKEND_SDL
#include "Ecore_Sdl.h"
#include "Ecore_Input.h"
#include "SDL.h"
#endif
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#ifdef BUILD_BACKEND_SDL

typedef struct _SDL
{
	SDL_Surface *surface;
	Eon_Input *input;
	Ender_Element *layout;
	Enesim_Buffer *buffer;
} SDL;

static Eina_Bool _initialized = EINA_FALSE;

static Eina_Bool _mouse_in(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_IO *ev = event;

	eon_input_feed_mouse_in(thiz->input, thiz->layout);
	eon_input_feed_mouse_move(thiz->input, thiz->layout, ev->x, ev->y);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_out(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_IO *ev = event;

	eon_input_feed_mouse_move(thiz->input, thiz->layout, ev->x, ev->y);
	eon_input_feed_mouse_out(thiz->input, thiz->layout);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_wheel(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_Wheel *ev = event;

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_move(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_Move *ev = event;

	eon_input_feed_mouse_move(thiz->input, thiz->layout, ev->x, ev->y);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_button_down(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_Button *ev = event;

	eon_input_feed_mouse_down(thiz->input, thiz->layout);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_button_up(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_Button *ev = event;

	eon_input_feed_mouse_up(thiz->input);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _key_down(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Key *ev = event;

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _key_up(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Key *ev = event;

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _feed_events(void *data)
{
	ecore_sdl_feed_events();
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _sdl_setup(Ender_Element *layout, unsigned int width, unsigned int height, Eon_Backend_Data *data)
{
	SDL *thiz;
	Enesim_Buffer_Data buffer_data;

	/* SDL only supports one window, impossible to create
	 * more than one eon_ecore
	 */
	if (_initialized) return EINA_FALSE;

	ecore_sdl_init(NULL);
	SDL_Init(SDL_INIT_VIDEO);

	thiz = calloc(1, sizeof(SDL));
	thiz->surface = SDL_SetVideoMode(width, height, 24, 0);
	thiz->layout = layout;
	thiz->input = eon_input_new();
	/* TODO create a buffer based on the real format */
	buffer_data.rgb888.plane0_stride = thiz->surface->pitch;
	buffer_data.rgb888.plane0 = thiz->surface->pixels;
	thiz->buffer = enesim_buffer_new_data_from(ENESIM_BACKEND_SOFTWARE, ENESIM_CONVERTER_BGR888, width, height, &buffer_data);
	/* fill the required data */
	data->surface = enesim_surface_new(ENESIM_BACKEND_SOFTWARE, ENESIM_FORMAT_ARGB8888, width, height);
	data->prv = thiz;

	/* FIXME for now use ecore_sdl, we shouldnt */
	ecore_poller_add(ECORE_POLLER_CORE, 1, _feed_events, NULL);
	ecore_poller_poll_interval_set(ECORE_POLLER_CORE, 0.006);
	/* FIXME for now use ecore_input, we shouldnt */
	ecore_event_handler_add(ECORE_EVENT_MOUSE_IN, _mouse_in, thiz);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_OUT, _mouse_out, thiz);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_MOVE, _mouse_move, thiz);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_WHEEL, _mouse_wheel, thiz);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_DOWN, _mouse_button_down, thiz);
	ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_UP, _mouse_button_up, thiz);
	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, _key_down, thiz);
	ecore_event_handler_add(ECORE_EVENT_KEY_UP, _key_up, thiz);

	_initialized = EINA_TRUE;
	return EINA_TRUE;
}

static void _sdl_cleanup(Eon_Backend_Data *data)
{
	if (_initialized)
	{
		ecore_sdl_shutdown();
		enesim_surface_delete(data->surface);
		free(data->prv);

		_initialized = EINA_FALSE;
	}
}

static void _sdl_flush(Eon_Backend_Data *data, Eina_Rectangle *rect)
{
	SDL *thiz;
	Sint32 x, y;
	Uint32 w, h;

	thiz = data->prv;
	if (!enesim_converter_surface(data->surface, thiz->buffer, ENESIM_ANGLE_0, rect, 0, 0))
	{
		printf("looks like we had an error here\n");
	}
	x = rect->x;
	y = rect->y;
	w = rect->w;
	h = rect->h;
	SDL_UpdateRect(thiz->surface, x, y, w, h);
}

static Eon_Backend _backend = {
	.setup = _sdl_setup,
	.cleanup = _sdl_cleanup,
	.flush = _sdl_flush,
};
#endif
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Backend * eon_ecore_sdl_new(void)
{
#ifdef BUILD_BACKEND_SDL
	eon_ecore_common_init();
	return &_backend;
#else
	return NULL;
#endif
}
