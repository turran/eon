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

/*
 * This engine is a just a prototype of what a remote canvas should be
 * Basically we need to send some basic stuff, like when an object is created,
 * deleted, when a property changes, child appended, child removed, etc.
 * Even so it has some problems, mainly:
 *
 * 1 object creation:
 * As the engine callbacks are called only whenever an object is actually
 * attached to a "renderable" canvas, we might change properties *before*
 * the object is attached and lost here
 * Also, we dont want to send the object's class name as it might be too large
 * we better send a unique id, but that id has to be shared between the client
 * and the server, what happens then if a user creates subtypes? or plugins
 * that on one side we get an id X and on the other (because we have another
 * plugin, or whatever) we have X + 1
 * Also we need to send animations, keys, etc, i.e every object attached to the
 * canvas, not only "enginable" objects
 *
 * 2 property change:
 * We need a way to send events whenever a property changed, On ekeko we have
 * some defined properties so it is very easy to serialize them, but what about
 * user defined property values? like matrix, color, whatever? we need and id
 * for them and also should be shared between the server and the client
 *
 * How to solve?
 * Do some kind of "factory" for the creation of every object? instead of the
 * "classic" foo_type_get()
 */
#include "Eon.h"
#include "eon_private.h"

#include <Ecore_Ipc.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#ifdef BUILD_BACKEND_REMOTE
static int _ids = 0;
static int _initialized = 0;

typedef struct _Eon_Ecore_Remote
{
	Ecore_Ipc_Server *srv;
} Eon_Ecore_Remote;
/*----------------------------------------------------------------------------*
 *                           Ecore IPC callbacks                              *
 *----------------------------------------------------------------------------*/
Eina_Bool handler_server_add(void *data, int ev_type, void *ev)
{
	printf("SERVER ADDDDDDDDDDDDDD\n");
	return EINA_TRUE;
}

Eina_Bool handler_server_del(void *data, int ev_type, void *ev)
{
	printf("SERVER DELLLL\n");
	return EINA_TRUE;
}

Eina_Bool handler_server_data(void *data, int ev_type, void *ev)
{
	printf("SERVER DATA!!!\n");
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                               Eon callbacks                                *
 *----------------------------------------------------------------------------*/
#if 0
static void _mutation_cb(const Ender_Element *o, Ekeko_Event *ev, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)ev;
	Object *ob = (Object *)data;

	printf("[REMOTE] changing property %s %s\n", em->prop, ekeko_object_type_name_get(o));
	/* TODO only do this on the curr state, not post */
	ecore_ipc_server_send(rdoc->srv, 2, em->prop_id, ob->id, 0, 0, NULL, 0);
}

static void _object_new_cb(const Ender_Element *o, Ekeko_Event *ev, void *data)
{
	Eon_Document_Object_New *e = (Eon_Document_Object_New *)ev;

	printf("[REMOTE] Creating a new object %s %p\n", e->name, ev->target);
}

static void * document_create(Eon_Document *d, const char *options)
{

	printf("[REMOTE] Initializing engine\n");

}

static void document_delete(void *d)
{
	Engine_Remote_Document *rdoc = d;

}

static void * object_create(Ender_Element *o)
{
	Object *ob;
	char *name;
	printf("[REMOTE] Object created %s\n", ekeko_object_type_name_get(o));

	ob = malloc(sizeof(Object));
	ob->o = o;
	ob->id = _ids++;

	name = ekeko_object_type_name_get(o);
	ecore_ipc_server_send(rdoc->srv, 0, 0, 0, 0, 0, name, strlen(name) + 1);
	ekeko_event_listener_add(o, EKEKO_EVENT_PROP_MODIFY, _mutation_cb, EINA_FALSE, ob);
	/* TODO register a callback on every property change */
	/* TODO register a callback on every tree change */
	return ob;
}

static void * canvas_create(Eon_Canvas *c, void *dd, Eina_Bool root, int w, int h)
{
	printf("CANVAS CREATE\n");
	return object_create((Ender_Element *)c);
}

static Eina_Bool canvas_flush(void *src, Eina_Rectangle *srect)
{
	printf("CANVAS FLUSH\n");
}


static void object_delete(void *data)
{
	Object *ob = (Object *)data;

	ekeko_event_listener_remove(ob->o, EKEKO_EVENT_PROP_MODIFY, _mutation_cb, EINA_FALSE, ob);
	/* remove every callback */
	/* delete the object */
}
#endif

/*----------------------------------------------------------------------------*
 *                           Ender calbacks                            *
 *----------------------------------------------------------------------------*/
static void _constructor_callback(Ender_Element *e, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (!eon_is_element(r))
		return;
	/* send the 'new' event */
}
/*----------------------------------------------------------------------------*
 *                           Eon backend interface                            *
 *----------------------------------------------------------------------------*/
static Eina_Bool _remote_setup(Ender_Element *layout, unsigned int width, unsigned int height, Eon_Backend_Data *data)
{
	Ecore_Ipc_Server *srv;
	Eon_Ecore_Remote *thiz;

	ecore_init();
	ecore_ipc_init();

	if (!_initialized++)
	{
		/* add our ender creator callback */
		ender_element_new_listener_remove(_constructor_callback, NULL);
	}
	/* handle the ipc mechanism */
	srv = ecore_ipc_server_connect(ECORE_IPC_LOCAL_SYSTEM, "eon-remote", 0, NULL);
	if (!srv)
	{
		return EINA_FALSE;
	}
	thiz = calloc(1, sizeof(Eon_Ecore_Remote));

	ecore_event_handler_add(ECORE_IPC_EVENT_SERVER_ADD,
		handler_server_add, NULL);
	ecore_event_handler_add(ECORE_IPC_EVENT_SERVER_DEL,
		handler_server_del, NULL);
	ecore_event_handler_add(ECORE_IPC_EVENT_SERVER_DATA,
		handler_server_data, NULL);

	thiz->srv = srv;
	data->prv = thiz;

	return EINA_TRUE;
}

static void _remote_cleanup(Eon_Backend_Data *data)
{
	ecore_ipc_shutdown();
	ecore_shutdown();
	if (_initialized == 1)
		ender_element_new_listener_remove(_constructor_callback, NULL);
	_initialized--;
}

static void _remote_flush(Eon_Backend_Data *data, Eina_Rectangle *rect)
{
}

static Eon_Backend _backend = {
	.setup = _remote_setup,
	.cleanup = _remote_cleanup,
	.flush = _remote_flush,
};

#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Backend * eon_ecore_remote_new(void)
{
#ifdef BUILD_BACKEND_REMOTE
	eon_ecore_common_init();
	return &_backend;
#else
	return NULL;
#endif
}

