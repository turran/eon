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

#include <Eix.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#ifdef BUILD_BACKEND_REMOTE
static int _initialized = 0;

typedef struct _Eon_Ecore_Remote
{
	Eix_Server *srv;
} Eon_Ecore_Remote;

typedef struct _Eon_Ecore_Remote_Element
{
	Eon_Ecore_Remote *remote;
	Ender_Element *e;
	int id;
} Eon_Ecore_Remote_Element;

#define INDEX(n) (n - EON_ECORE_REMOTE_CLIENT_SIZE)
static Eet_Data_Descriptor *_descriptors[EON_ECORE_REMOTE_PROPERTY_CLEAR - EON_ECORE_REMOTE_CLIENT_SIZE + 1];
static Eet_Data_Descriptor *_ddescriptors[2];
/*----------------------------------------------------------------------------*
 *                              Encode/Decode                                 *
 *----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*
 *                               Eon callbacks                                *
 *----------------------------------------------------------------------------*/
static void _element_changed(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Ecore_Remote_Element *re = data;
	Eon_Ecore_Remote *thiz = re->remote;
	Ender_Event_Mutation *ev = event_data;

	printf("element changed\n");
	switch (ev->type)
	{
		case ENDER_EVENT_MUTATION_SET:
		{
			Eon_Ecore_Remote_Property_Set evs;
			evs.id = re->id;
			evs.name = ev->name;
			eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_PROPERTY_SET, &evs, 0, 0);
		}
		break;

		case ENDER_EVENT_MUTATION_ADD:
		{
			Eon_Ecore_Remote_Property_Add evs;
			evs.id = re->id;
			evs.name = ev->name;
			eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_PROPERTY_ADD, &evs, 0, 0);
		}
		break;

		case ENDER_EVENT_MUTATION_REMOVE:
		{
			Eon_Ecore_Remote_Property_Remove evs;
			evs.id = re->id;
			evs.name = ev->name;
			eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_PROPERTY_REMOVE, &evs, 0, 0);
		}
		break;

		case ENDER_EVENT_MUTATION_CLEAR:
		{
			Eon_Ecore_Remote_Property_Clear evs;

			evs.id = re->id;
			evs.name = ev->name;
			eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_PROPERTY_CLEAR, &evs, 0, 0);
		}
		break;
	}
}
/*----------------------------------------------------------------------------*
 *                               Ender calbacks                               *
 *----------------------------------------------------------------------------*/
static void _constructor_callback(Ender_Element *e, void *data)
{
	Eon_Ecore_Remote *thiz;
	Eon_Ecore_Remote_Element *re;
	Eon_Ecore_Remote_Element_New new;
	Enesim_Renderer *r;
	Eix_Error err;
	const char *name;
	static int _id = 0;

	thiz = data;
	r = ender_element_renderer_get(e);
	if (!eon_is_element(r))
		return;
	name = ender_element_name_get(e);

	re = calloc(1, sizeof(Eon_Ecore_Remote_Element));
	re->id = ++_id;
	re->e = e;
	re->remote = thiz;
	/* send the 'new' event */
	new.id = re->id;
	new.name = name;
	err = eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_ELEMENT_NEW, &new, 0, 0);
	ender_event_listener_add(e, "Mutation", _element_changed, re);
}
/*----------------------------------------------------------------------------*
 *                           Eon backend interface                            *
 *----------------------------------------------------------------------------*/
static Eina_Bool _remote_setup(Ender_Element *layout, unsigned int width, unsigned int height, Eon_Backend_Data *data)
{
	Eix_Server *srv;
	Eon_Ecore_Remote *thiz;
	Eon_Ecore_Remote_Client_Size client_size;

	ecore_init();
	eix_init();

	/* handle the ipc mechanism */
	srv = eix_connect("eon-remote", 0);
	if (!srv)
	{
		return EINA_FALSE;
	}
	thiz = calloc(1, sizeof(Eon_Ecore_Remote));

	ecore_event_handler_add(EIX_EVENT_SERVER_ADD,
		handler_server_add, NULL);
	ecore_event_handler_add(EIX_EVENT_SERVER_DEL,
		handler_server_del, NULL);
	/* FIXME something is wrong here as every element created with eon
	 * will be created on the server too ...
	 */
	if (!_initialized++)
	{
		/* add our ender creator callback */
		ender_element_new_listener_add(_constructor_callback, thiz);
		/* register the messages */
		eon_ecore_remote_init();
	}
	thiz->srv = srv;
	data->prv = thiz;

	eon_ecore_remote_server_setup(thiz->srv);
	/* send the size of the client */ 
	client_size.width = width;
	client_size.height = height;
	eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_CLIENT_SIZE, &client_size, 0, 0);

	return EINA_TRUE;
}

static void _remote_cleanup(Eon_Backend_Data *data)
{
	eix_shutdown();
	ecore_shutdown();
	if (_initialized == 1)
		ender_element_new_listener_remove(_constructor_callback, NULL);
	_initialized--;
}

static Eon_Backend _backend = {
	.setup = _remote_setup,
	.cleanup = _remote_cleanup,
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

EAPI void eon_ecore_remote_server_setup(void *srv)
{
#ifdef BUILD_BACKEND_REMOTE
	int i;
	int limit;

	limit = EON_ECORE_REMOTE_PROPERTY_CLEAR - EON_ECORE_REMOTE_CLIENT_SIZE + 1;

	for (i = 0; i < limit; i++)
	{
		printf("adding messages %d %p\n", EON_ECORE_REMOTE_CLIENT_SIZE + i, srv);
		eix_server_message_add(srv, EON_ECORE_REMOTE_CLIENT_SIZE + i, _descriptors[i], 0);
	}
#else
	return;
#endif
}

EAPI void eon_ecore_remote_init(void)
{
#ifdef BUILD_BACKEND_REMOTE
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor_Class eddc;

	/* frst the datas */
#if 0
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Element_New", sizeof(Eon_Ecore_Remote_Element_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_ddescriptors[0] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Element_New, "id", id, EET_T_UINT);
#endif
	
	/* now the messages */
	/* client size */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Client_Size", sizeof(Eon_Ecore_Remote_Client_Size));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_CLIENT_SIZE)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Client_Size, "width", width, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Client_Size, "height", height, EET_T_UINT);
	/* remote new */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Element_New", sizeof(Eon_Ecore_Remote_Element_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_ELEMENT_NEW)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Element_New, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Element_New, "name", name, EET_T_INLINED_STRING);
	/* property set */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Property_Set", sizeof(Eon_Ecore_Remote_Property_Set));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_PROPERTY_SET)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Set, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Set, "name", name, EET_T_INLINED_STRING);
	/* property add */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Property_Add", sizeof(Eon_Ecore_Remote_Property_Add));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_PROPERTY_ADD)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Add, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Add, "name", name, EET_T_INLINED_STRING);
	/* property remove */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Property_Remove", sizeof(Eon_Ecore_Remote_Property_Remove));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_PROPERTY_REMOVE)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Remove, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Remove, "name", name, EET_T_INLINED_STRING);
	/* property clear */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Property_Clear", sizeof(Eon_Ecore_Remote_Property_Clear));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_PROPERTY_CLEAR)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Clear, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Clear, "name", name, EET_T_INLINED_STRING);
#else
	return;
#endif
}
