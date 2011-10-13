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

typedef struct _Eon_Ecore_SDL
{
	Uint32 flags;
} Eon_Ecore_SDL;

typedef struct _Eon_Ecore_SDL_Window
{
	Eon_Ecore_SDL *backend;
	SDL_Surface *native_surface;
	Enesim_Surface *surface;
	Eon_Input *input;
	Eon_Input_State *input_state;
	Ender_Element *layout;
	Enesim_Buffer *buffer;
	unsigned int width;
	unsigned int height;
	Ecore_Idle_Enterer *idler;
	Eina_Bool needs_resize;
} Eon_Ecore_SDL_Window;

static Eina_Bool _initialized = EINA_FALSE;

static void _sdl_setup_buffers(Eon_Ecore_SDL_Window *thiz)
{
	Eon_Ecore_SDL *sdl;
	Enesim_Buffer_Sw_Data buffer_data;

	sdl = thiz->backend;
	if (thiz->buffer)
	{
		enesim_buffer_unref(thiz->buffer);
		thiz->buffer = NULL;
	}
	if (thiz->surface)
	{
		enesim_surface_unref(thiz->surface);
		thiz->surface = NULL;
	}
	thiz->native_surface = SDL_SetVideoMode(thiz->width, thiz->height, 24, sdl->flags);
	/* TODO create a buffer based on the real format */
	buffer_data.rgb888.plane0_stride = thiz->native_surface->pitch;
	buffer_data.rgb888.plane0 = thiz->native_surface->pixels;
	thiz->buffer = enesim_buffer_new_data_from(ENESIM_CONVERTER_BGR888,
			thiz->width, thiz->height, EINA_FALSE, &buffer_data);
	thiz->surface = enesim_surface_new(ENESIM_FORMAT_ARGB8888,
			thiz->width, thiz->height);
}

static void _sdl_flush(Eon_Ecore_SDL_Window *thiz, Eina_Rectangle *rect)
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
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Event_Mouse_IO *ev = event;

	eon_input_state_feed_mouse_in(thiz->input_state);
	eon_input_state_feed_mouse_move(thiz->input_state, ev->x, ev->y, 0, 0);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_out(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Event_Mouse_IO *ev = event;

	eon_input_state_feed_mouse_move(thiz->input_state, ev->x, ev->y, 0, 0);
	eon_input_state_feed_mouse_out(thiz->input_state);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_wheel(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Event_Mouse_Wheel *ev = event;

	eon_input_state_feed_mouse_wheel(thiz->input_state, ev->direction);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_move(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Event_Mouse_Move *ev = event;

	eon_input_state_feed_mouse_move(thiz->input_state, ev->x, ev->y, 0, 0);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_button_down(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;

	eon_input_state_feed_mouse_down(thiz->input_state);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _mouse_button_up(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;

	eon_input_state_feed_mouse_up(thiz->input_state);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _key_down(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Event_Key *ev = event;

	printf("DOWN %s %s %s %s\n", ev->keyname, ev->key, ev->string, ev->compose);
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _key_up(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Event_Key *ev = event;

	printf("UP %s %s %s %s\n", ev->keyname, ev->key, ev->string, ev->compose);
	return ECORE_CALLBACK_RENEW;
}

static void _calculate_layout_size(Eon_Ecore_SDL_Window *thiz, int width, int height)
{
	Enesim_Renderer *r;
	Eon_Size window_size;
	Eon_Size size;

	window_size.width = width;
	window_size.height = height;
	eon_element_real_relative_size_get(thiz->layout, &window_size, &size);
	r = ender_element_renderer_get(thiz->layout);
	eon_element_actual_width_set(r, size.width);
	eon_element_actual_height_set(r, size.height);
	//printf("actual size %g %g\n", size.width, size.height);
}

static Eina_Bool _resize(void *data, int type, void *event)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Ecore_Sdl_Event_Video_Resize *ev = event;


	_calculate_layout_size(thiz, ev->w, ev->h);
	if (ev->w != thiz->width || ev->h != thiz->height)
		thiz->needs_resize = EINA_TRUE;
	thiz->width = ev->w;
	thiz->height = ev->h;
	//printf("resize to %d %d\n", thiz->width, thiz->height);
	/* FIXME we should limit the size of the window, looks like we cannot
	 * do it with SDL itself, but using some X11 hints
	 */
	//printf("%d %d: %g %g - %g %g\n", ev->w, ev->h, min_w, min_h, max_w, max_h);

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _feed_events(void *data)
{
	ecore_sdl_feed_events();
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _idler_cb(void *data)
{
	Eon_Ecore_SDL_Window *thiz = data;
	Enesim_Renderer *r;
	Enesim_Error *error = NULL;
	Eina_List *redraws = NULL;
	//Eina_List *l;
	Eina_Rectangle area;

	if (!eon_element_has_changed(thiz->layout))
	{
		return EINA_TRUE;
	}
	if (thiz->needs_resize)
	{
		_sdl_setup_buffers(thiz);
		thiz->needs_resize = EINA_FALSE;
	}
	r = ender_element_renderer_get(thiz->layout);
	/* get the damage rectangles */
	eon_layout_redraw_get(r, &redraws);
	{
		Eina_Rectangle final;
		int w, h;

		enesim_renderer_destination_boundings(r, &final, 0, 0);
		enesim_surface_size_get(thiz->surface, &w, &h);
		printf("rendering to %d %d from %d %d %d %d\n", w, h, final.x, final.y, final.w, final.h);
	}
	/* render only those rectangles */
	if (!enesim_renderer_draw_list(r, thiz->surface, redraws, 0, 0, &error))
	{
		enesim_error_dump(error);
		return EINA_TRUE;
	}
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

static Eina_Bool _sdl_window_new(void *data, Ender_Element *layout, unsigned int width, unsigned int height, void **window_data)
{
	Eon_Ecore_SDL *backend;
	Eon_Ecore_SDL_Window *thiz;

	/* Eon_Ecore_SDL_Window only supports one window, impossible to create
	 * more than one eon_ecore
	 */
	if (_initialized) return EINA_FALSE;

	eon_ecore_common_init();
	ecore_sdl_init(NULL);
	SDL_Init(SDL_INIT_VIDEO);

	backend = data;

	thiz = calloc(1, sizeof(Eon_Ecore_SDL_Window));
	thiz->layout = layout;
	thiz->width = width;
	thiz->height = height;
	thiz->input = eon_input_new();
	thiz->backend = backend;
	thiz->input_state = eon_layout_input_state_new(layout, thiz->input);
	_sdl_setup_buffers(thiz);

	_calculate_layout_size(thiz, width, height);

	thiz->idler = ecore_idle_enterer_add(_idler_cb, thiz);
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

	ecore_event_handler_add(ECORE_SDL_EVENT_RESIZE, _resize, thiz);

	_initialized = EINA_TRUE;
	*window_data = thiz;

	return EINA_TRUE;
}

static void _sdl_window_delete(void *data, void *window_data)
{
	Eon_Ecore_SDL_Window *thiz = data;

	if (_initialized)
	{
		ecore_sdl_shutdown();
		if (thiz->surface)
			enesim_surface_unref(thiz->surface);
		if (thiz->buffer)
			enesim_buffer_unref(thiz->buffer);
		free(data);

		_initialized = EINA_FALSE;
	}
}

static Eon_Backend_Descriptor _backend = {
	/* .window_new =    */ _sdl_window_new,
	/* .window_delete = */ _sdl_window_delete,
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
	Eon_Ecore_SDL *thiz;
	Eon_Backend *backend;

	thiz = calloc(1, sizeof(Eon_Ecore_SDL));
	thiz->flags = SDL_RESIZABLE;
	backend = eon_backend_new(&_backend, thiz);
	return backend;
#else
	return NULL;
#endif
}

EAPI Eon_Backend * eon_ecore_sdl_new_with_options(Eina_Bool fullscreen, Eina_Bool resizable)
{
#ifdef BUILD_BACKEND_SDL
	Eon_Ecore_SDL *thiz;
	Eon_Backend *backend;
	Uint32 flags = 0;

	thiz = calloc(1, sizeof(Eon_Ecore_SDL));
	if (fullscreen)
		flags |= SDL_FULLSCREEN;
	if (resizable)
		flags |= SDL_RESIZABLE;
	thiz->flags = flags;

	backend = eon_backend_new(&_backend, thiz);
	return backend;
#else
	return NULL;
#endif
}
