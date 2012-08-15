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
#include <Ecore.h>

#include "eon_private_main.h"

#include "eon_main.h"
#include "eon_window.h"

#include "eon_private_backend.h"
#include "eon_private_ecore_common.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Window_Instance_Timer
{
	int refs;
	Eina_List *instances;
	Ecore_Timer *timer;
} Eon_Window_Instance_Timer;

static int _init = 0;
static Eina_List *_instance_timers = NULL;

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

	/* TODO check that the instance inherits from theme widget, if not
	 * we should not process it
	 */
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

static void _eon_ecore_idler_add(void *backend_data, Eon_Backend_Idler cb, void *data)
{
	ecore_idle_enterer_add(cb, data);
}

static void _eon_ecore_run(void *backend_data)
{
	ecore_main_loop_begin();
}

static void _eon_ecore_quit(void *backend_data)
{
	ecore_main_loop_quit();
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Backend * eon_ecore_backend_new(Eon_Ecore_Backend_Descriptor *descriptor,
		void *data)
{
	Eon_Backend *backend;
	Eon_Backend_Descriptor pdescriptor;

	pdescriptor.idler_add = _eon_ecore_idler_add;
	pdescriptor.run = _eon_ecore_run;
	pdescriptor.quit = _eon_ecore_quit;

	pdescriptor.window_new = descriptor->window_new;
	pdescriptor.window_delete = descriptor->window_delete;

	backend = eon_backend_new(&pdescriptor, data);
	return backend;
}

void eon_ecore_common_init(void)
{
	if (!_init++)
	{
		ecore_init();
		escen_instance_new_callback_add(_instance_new_cb, NULL);
	}
}

void eon_ecore_common_shutdown(void)
{
	if (_init == 1)
	{
		ecore_shutdown();
	}
	_init--;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

