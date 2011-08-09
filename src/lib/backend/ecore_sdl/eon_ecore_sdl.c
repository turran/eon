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
	SDL_Surface *native_surface;
	Enesim_Surface *surface;
	Eon_Input *input;
	Eon_Input_State *input_state;
	Ender_Element *layout;
	Enesim_Buffer *buffer;
	unsigned int width;
	unsigned int height;
	Ecore_Idle_Enterer *idler;
} SDL;

static Eina_Bool _initialized = EINA_FALSE;

static void _sdl_flush(SDL *thiz, Eina_Rectangle *rect)
{
	Sint32 x, y;
	Uint32 w, h;

	if (!enesim_converter_surface(thiz->surface, thiz->buffer, ENESIM_ANGLE_0, rect, 0, 0))
	{
		printf("looks like we had an error here\n");
	}
	x = rect->x;
	y = rect->y;
	w = rect->w;
	h = rect->h;
	SDL_UpdateRect(thiz->native_surface, x, y, w, h);
}


static Eina_Bool _mouse_in(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_IO *ev = event;

	eon_input_state_feed_mouse_in(thiz->input_state);
	eon_input_state_feed_mouse_move(thiz->input_state, ev->x, ev->y);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_out(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_IO *ev = event;

	eon_input_state_feed_mouse_move(thiz->input_state, ev->x, ev->y);
	eon_input_state_feed_mouse_out(thiz->input_state);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_wheel(void *data, int type, void *event)
{
/*
	SDL *thiz = data;
	Ecore_Event_Mouse_Wheel *ev = event;
*/

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_move(void *data, int type, void *event)
{
	SDL *thiz = data;
	Ecore_Event_Mouse_Move *ev = event;

	eon_input_state_feed_mouse_move(thiz->input_state, ev->x, ev->y);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_button_down(void *data, int type, void *event)
{
	SDL *thiz = data;

	eon_input_state_feed_mouse_down(thiz->input_state);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_button_up(void *data, int type, void *event)
{
	SDL *thiz = data;

	eon_input_state_feed_mouse_up(thiz->input_state);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _key_down(void *data, int type, void *event)
{
/*
	SDL *thiz = data;
	Ecore_Event_Key *ev = event;
*/

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _key_up(void *data, int type, void *event)
{
/*
	SDL *thiz = data;
	Ecore_Event_Key *ev = event;
*/

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _feed_events(void *data)
{
	ecore_sdl_feed_events();
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _idler_cb(void *data)
{
	SDL *thiz = data;
	Eina_List *redraws = NULL;
	//Eina_List *l;
	Enesim_Renderer *r;
	Eina_Rectangle area;

	if (!eon_element_has_changed(thiz->layout))
	{
		return EINA_TRUE;
	}
	r = ender_element_renderer_get(thiz->layout);
	/* get the damage rectangles */
	eon_layout_redraw_get(r, &redraws);
	/* render only those rectangles */
	enesim_renderer_draw_list(r, thiz->surface, redraws, 0, 0);
	/* call the flush on the backend of such rectangles */
	/* FIXME for now the layout always returns nothing, force a render anyway */
	//ee->flush(ee->data, redraws);
	//EINA_LIST_FOREACH
	{
		eina_rectangle_coords_from(&area, 0, 0, thiz->width, thiz->height);

		_sdl_flush(thiz, &area);
	}
	return EINA_TRUE;
}


static Eina_Bool _sdl_setup(Ender_Element *layout, unsigned int width, unsigned int height, Eon_Backend_Data *data)
{
	SDL *thiz;
	Enesim_Buffer_Sw_Data buffer_data;

	/* SDL only supports one window, impossible to create
	 * more than one eon_ecore
	 */
	if (_initialized) return EINA_FALSE;

	ecore_sdl_init(NULL);
	SDL_Init(SDL_INIT_VIDEO);

	thiz = calloc(1, sizeof(SDL));
	thiz->native_surface = SDL_SetVideoMode(width, height, 24, 0);
	thiz->layout = layout;
	thiz->width = width;
	thiz->height = height;
	thiz->input = eon_input_new();

	thiz->input_state = eon_layout_input_state_new(layout, thiz->input);

	/* TODO create a buffer based on the real format */
	buffer_data.rgb888.plane0_stride = thiz->native_surface->pitch;
	buffer_data.rgb888.plane0 = thiz->native_surface->pixels;
	thiz->buffer = enesim_buffer_new_data_from(ENESIM_CONVERTER_BGR888, width, height, EINA_FALSE, &buffer_data);
	thiz->idler = ecore_idle_enterer_add(_idler_cb, thiz);
	thiz->surface = enesim_surface_new(ENESIM_FORMAT_ARGB8888, width, height);
	/* fill the required data */
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
	//SDL *thiz = data->prv;

	if (_initialized)
	{
		ecore_sdl_shutdown();
		enesim_surface_delete(data->surface);
		free(data->prv);

		_initialized = EINA_FALSE;
	}
}

static Eon_Backend _backend = {
	.setup = _sdl_setup,
	.cleanup = _sdl_cleanup,
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
