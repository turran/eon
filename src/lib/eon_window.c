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
struct _Eon_Window
{
	Ender_Element *layout;
	Eina_Bool changed;
	unsigned int width;
	unsigned int height;
	Eon_Backend *backend;
	Eon_Backend_Data data;

	Ecore_Idler *idler;
};

typedef struct _Eon_Window_Instance_Timer
{
	int refs;
	Eina_List *instances;
	Ecore_Timer *timer;
} Eon_Window_Instance_Timer;

static Eina_List *_instance_timers = NULL;

static Eina_Bool _idler_cb(void *data)
{
	Eon_Window *ee = data;
	Eina_List *redraws = NULL;
	Eina_List *l;
	Enesim_Renderer *r;
	Eina_Rectangle area;

	if (!ee->changed) return EINA_TRUE;
	r = ender_element_renderer_get(ee->layout);
	/* get the damage rectangles */
	eon_layout_redraw_get(r, &redraws);
	/* render only those rectangles */
	enesim_renderer_draw_list(r, ee->data.surface, redraws, 0, 0);
	/* call the flush on the backend of such rectangles */
	/* FIXME for now the layout always returns nothing, force a render anyway */
	//ee->flush(ee->data, redraws);
	//EINA_LIST_FOREACH
	{
		eina_rectangle_coords_from(&area, 0, 0, ee->width, ee->height);
		ee->backend->flush(&ee->data, &area);
	}
	//ee->changed = EINA_FALSE;
	return EINA_TRUE;
}

static Eina_Bool _instance_timer_cb(void *data)
{
	Eon_Window_Instance_Timer *timer = data;
	Escen_Instance *eei;
	Eina_List *l;

	EINA_LIST_FOREACH(timer->instances, l, eei)
	{
		escen_instance_process(eei);
	}

        return EINA_TRUE;
}

static void _instance_new_cb(Escen_Instance *eei, void *data)
{
	Eon_Window_Instance_Timer *timer;
	Escen *escen;
	Escen_Ender *ee;
	Eina_List *l;
	Eina_Bool found = EINA_FALSE;
	double spf;
	int fps;

	ee = escen_instance_escen_ender_get(eei);
	escen = escen_ender_escen_get(ee);
	fps = escen_fps_get(escen);
	spf = 1.0/fps;
	EINA_LIST_FOREACH(_instance_timers, l, timer)
	{
		double interval;

		interval = ecore_timer_interval_get(timer->timer);
		if (fabs(interval - spf) < DBL_EPSILON)
		{
			found = EINA_TRUE;
			break;
		}
	}
	if (!found)
	{
		timer = calloc(1, sizeof(Eon_Window_Instance_Timer));
		timer->timer = ecore_timer_add(spf, _instance_timer_cb, timer);
		_instance_timers = eina_list_append(_instance_timers, timer);
	}
	timer->refs++;
	timer->instances = eina_list_append(timer->instances, eei);
}

static void _property_change(Ender_Element *e, const char *event_name,
		void *event_data, void *data)
{
	Eon_Window *ee = data;

	printf("inside\n");
	ee->changed = EINA_TRUE;
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
	Eon_Backend_Data data;
	Enesim_Renderer *l;

	if (!backend || !backend->setup) return NULL;
	if (!layout) return NULL;
	/* FIXME we should just compare the type name */
	l = ender_element_renderer_get(layout);
	if (!l) return NULL;
	if (!eon_is_layout(l)) return NULL;

	if (backend->setup(layout, width, height, &data))
	{
		Eon_Window *ee;

		ee = calloc(1, sizeof(Eon_Window));
		ee->width = width;
		ee->height = height;
		ee->backend = backend;
		ee->layout = layout;
		ee->changed = EINA_TRUE;
		ee->data = data;
		ee->idler = ecore_idler_add(_idler_cb, ee);
		ender_event_listener_add(layout, "Mutation", _property_change, ee);
		ee->changed = EINA_TRUE;

		return ee;
	}
	return NULL;
}
