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
#include "eon_private_main.h"

#include "eon_input.h"
#include "eon_window.h"
#include "eon_main.h"

#include "eon_private_element.h"
#include "eon_private_backend.h"
#include "eon_private_input.h"

#ifdef BUILD_BACKEND_REMOTE
#include "eon_ecore_remote.h"
#include <Ecore.h>
#include <Eix.h>
#endif
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
	Ender_Element *e;
	int id;
} Eon_Ecore_Remote_Element;

/* Looks like this is the way to define unions for eet */
struct {
	Eon_Ecore_Remote_Data_Type type;
	const char *name;
} eet_mappings[] = {
	{ EON_ECORE_REMOTE_DATA_UINT32, "Eon_Ecore_Remote_Data_Uint32" },
	{ EON_ECORE_REMOTE_DATA_DOUBLE, "Eon_Ecore_Remote_Data_Double" },
	{ EON_ECORE_REMOTE_DATA_STRING, "Eon_Ecore_Remote_Data_String" },
	{ EON_ECORE_REMOTE_DATA_LIST, "Eon_Ecore_Remote_Data_List" },
	{ EON_ECORE_REMOTE_DATA_TYPES, NULL },
};

#define INDEX(n) (n - EON_ECORE_REMOTE_CLIENT_NEW)
static Eet_Data_Descriptor *_descriptors[EON_ECORE_REMOTE_PROPERTY_CLEAR - EON_ECORE_REMOTE_CLIENT_NEW + 1];
static Eet_Data_Descriptor *_ddescriptors[EON_ECORE_REMOTE_DATA_TYPES];
static Eet_Data_Descriptor *_unified;
static Eet_Data_Descriptor *_data;

static const char * _renderer_key = "_eon_ecore_remote";

static Eon_Ecore_Remote_Element * _remote_element_ender_from(Ender_Element *e)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	return enesim_renderer_private_get(r, _renderer_key);
}

static Eina_Bool _eon_ecore_remote_data_from_value(Eon_Ecore_Remote_Data *data, Ender_Value *v)
{
	Ender_Container *container;
	Ender_Value_Type type;

	printf("setting value!!!\n");
	container = ender_value_container_get(v);
	type = ender_container_type_get(container);

	if (ender_container_is_compound(container))
	{
		printf("compound!!\n");
		return EINA_FALSE;
	}
	printf("type = %d\n", type);
	data->type = EON_ECORE_REMOTE_DATA_UINT32;
	data->value.duint32.u32 = 1;
	switch (type)
	{
		case ENDER_BOOL:
		data->type = EON_ECORE_REMOTE_DATA_UINT32;
		data->value.duint32.u32 = ender_value_bool_get(v);
		break;

		case ENDER_UINT32:
		data->type = EON_ECORE_REMOTE_DATA_UINT32;
		data->value.duint32.u32 = ender_value_uint32_get(v);
		break;

		case ENDER_INT32:
		data->type = EON_ECORE_REMOTE_DATA_UINT32;
		data->value.duint32.u32 = ender_value_int32_get(v);
		break;

		case ENDER_DOUBLE:
		data->type = EON_ECORE_REMOTE_DATA_DOUBLE;
		data->value.ddouble.d = ender_value_double_get(v);
		printf(">>> setting double %g\n", data->value.ddouble.d);
		break;

		case ENDER_COLOR:
		data->type = EON_ECORE_REMOTE_DATA_UINT32;
		data->value.duint32.u32 = ender_value_color_get(v);
		break;

		case ENDER_ARGB:
		data->type = EON_ECORE_REMOTE_DATA_UINT32;
		data->value.duint32.u32 = ender_value_argb_get(v);
		break;

		case ENDER_STRING:
		data->type = EON_ECORE_REMOTE_DATA_STRING;
		data->value.dstring.s = ender_value_string_get(v);
		break;

		case ENDER_ENDER:
		{
			Ender_Element *rel;
			Eon_Ecore_Remote_Element *re;

			data->type = EON_ECORE_REMOTE_DATA_UINT32;
			rel = ender_value_ender_get(v);
			if (!rel) return EINA_FALSE;
			re = _remote_element_ender_from(rel);
			if (!re) return EINA_FALSE;
			data->value.duint32.u32 = re->id;
		}
		break;

		case ENDER_MATRIX:
		case ENDER_OBJECT:
		case ENDER_SURFACE:
		case ENDER_POINTER:
		default:
		break;
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Encode/Decode                                 *
 *----------------------------------------------------------------------------*/
static const char * _remote_data_type_get(const void *data, Eina_Bool *unknown)
{
	const Eon_Ecore_Remote_Data_Type * u = data;
	int i;

	if (unknown) *unknown = EINA_FALSE;

	for (i = 0; eet_mappings[i].name != NULL; ++i)
	{
		if (*u == eet_mappings[i].type)
			return eet_mappings[i].name;
	}

	if (unknown) *unknown = EINA_TRUE;

	return NULL;

}

static Eina_Bool _remote_data_type_set(const char *type, void *data, Eina_Bool *unknown)
{
	Eon_Ecore_Remote_Data_Type * u = data;
	int i;

	if (unknown) return EINA_FALSE;

	for (i = 0; eet_mappings[i].name != NULL; ++i)
	{
		if (strcmp(eet_mappings[i].name, type) == 0)
		{
			   *u = eet_mappings[i].type;
		   return EINA_TRUE;
		}
	}

	return EINA_FALSE;
}
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
	Eon_Ecore_Remote *thiz = data;
	Eon_Ecore_Remote_Element *re;
	Ender_Event_Mutation *ev = event_data;

	re = _remote_element_ender_from(e);
	printf("changing property %s\n", ev->name);
	switch (ev->type)
	{
		case ENDER_EVENT_MUTATION_SET:
		{
			Eon_Ecore_Remote_Property_Set evs;
			Eon_Ecore_Remote_Data rdata;

			evs.id = re->id;
			evs.name = ev->name;
			evs.value = &rdata;
			_eon_ecore_remote_data_from_value(&rdata, ev->value);
			eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_PROPERTY_SET, &evs, 0, 0);
		}
		break;

		case ENDER_EVENT_MUTATION_ADD:
		{
			Eon_Ecore_Remote_Property_Add evs;
			Eon_Ecore_Remote_Data rdata;

			evs.id = re->id;
			evs.name = ev->name;
			evs.value = &rdata;
			_eon_ecore_remote_data_from_value(&rdata, ev->value);
			eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_PROPERTY_ADD, &evs, 0, 0);
		}
		break;

		case ENDER_EVENT_MUTATION_REMOVE:
		{
			Eon_Ecore_Remote_Property_Remove evs;
			Eon_Ecore_Remote_Data rdata;

			evs.id = re->id;
			evs.name = ev->name;
			evs.value = &rdata;
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
static void _global_constructor_callback(Ender_Element *e, void *data)
{
	Eon_Ecore_Remote_Element *re;
	Enesim_Renderer *r;
	static int _id = 0;

	r = ender_element_object_get(e);
	if (!eon_is_element(r))
	{
		printf("is not an element\n");
		return;
	}

	re = calloc(1, sizeof(Eon_Ecore_Remote_Element));
	re->id = ++_id;
	re->e = e;
	enesim_renderer_private_set(r, _renderer_key, re);
}

static void _client_constructor_callback(Ender_Element *e, void *data)
{
	Eon_Ecore_Remote *thiz;
	Eon_Ecore_Remote_Element *re;
	Eon_Ecore_Remote_Element_New new;
	Eix_Error err;
	const char *name;

	thiz = data;

	re = _remote_element_ender_from(e);
	if (!re)
	{
		printf("no remote ender found\n");
		return;
	}

	name = ender_element_name_get(e);
	/* send the 'new' event */
	new.id = re->id;
	new.name = name;
	err = eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_ELEMENT_NEW, &new, 0, 0);
	ender_event_listener_add(e, "Mutation", _element_changed, thiz);
}
/*----------------------------------------------------------------------------*
 *                           Eon backend interface                            *
 *----------------------------------------------------------------------------*/
static Eina_Bool _remote_window_new(void *data, Ender_Element *layout, unsigned int width, unsigned int height, void **window_data)
{
	Eix_Server *srv;
	Eon_Ecore_Remote *thiz;
	Eon_Ecore_Remote_Element *re;
	Eon_Ecore_Remote_Client_New client_new;

	ecore_init();
	eix_init();

	/* handle the common initializer */
	if (!_initialized++)
	{
		/* register the messages */
		eon_ecore_remote_init();
		/* add our ender creator callback */
		ender_element_new_listener_add(_global_constructor_callback, NULL);
	}

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
	ender_element_new_listener_add(_client_constructor_callback, thiz);
	thiz->srv = srv;

	eon_ecore_remote_server_setup(thiz->srv);
	/* FIXME once the Eon_Window is refactored to be an Ender itself
	 * or at least be able to set the layout *after* the creation
	 * we should remove the following lines
	 */
	_global_constructor_callback(layout, NULL);
	_client_constructor_callback(layout, thiz);
	/* send the size of the client */
	client_new.width = width;
	client_new.height = height;
	re = _remote_element_ender_from(layout);
	client_new.layout_id = re->id;
	eix_server_message_send(thiz->srv, EON_ECORE_REMOTE_CLIENT_NEW, &client_new, 0, 0);

	*window_data = thiz;

	return EINA_TRUE;
}

static void _remote_window_delete(void *data, void *window_data)
{
	Eon_Ecore_Remote *thiz = window_data;

	if (_initialized == 1)
		ender_element_new_listener_remove(_global_constructor_callback, NULL);
	ender_element_new_listener_remove(_client_constructor_callback, thiz);
	_initialized--;

	eix_shutdown();
	ecore_shutdown();
}

static Eon_Backend_Descriptor _backend = {
	/* .window_new =    */ _remote_window_new,
	/* .window_delete = */ _remote_window_delete,
};

#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Backend * eon_ecore_remote_new(void)
{
#ifdef BUILD_BACKEND_REMOTE
	Eon_Backend *backend;

	eon_ecore_common_init();
	backend = eon_backend_new(&_backend, NULL);
	return backend;
#else
	return NULL;
#endif
}

EAPI void eon_ecore_remote_server_setup(void *srv)
{
#ifdef BUILD_BACKEND_REMOTE
	int i;
	int limit;

	limit = EON_ECORE_REMOTE_PROPERTY_CLEAR - EON_ECORE_REMOTE_CLIENT_NEW + 1;

	for (i = 0; i < limit; i++)
	{
		printf("adding messages %d %p\n", EON_ECORE_REMOTE_CLIENT_NEW + i, srv);
		eix_server_message_add(srv, EON_ECORE_REMOTE_CLIENT_NEW + i, _descriptors[i], 0);
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
	/* the unified */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Data_Value", sizeof(Eon_Ecore_Remote_Data_Value));
	eddc.func.type_get = _remote_data_type_get;
	eddc.func.type_set = _remote_data_type_set;
	_unified = eet_data_descriptor_stream_new(&eddc);

	/* the remote data */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Data", sizeof(Eon_Ecore_Remote_Data));
	edd = eet_data_descriptor_stream_new(&eddc);
	_data = edd;
	EET_DATA_DESCRIPTOR_ADD_UNION(edd, Eon_Ecore_Remote_Data, "value", value, type, _unified);

	/* u32 */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Data_Uint32", sizeof(Eon_Ecore_Remote_Data_Uint32));
	edd = eet_data_descriptor_stream_new(&eddc);
	_ddescriptors[EON_ECORE_REMOTE_DATA_UINT32] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Data_Uint32, "u32", u32, EET_T_UINT);
	/* double */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Data_Double", sizeof(Eon_Ecore_Remote_Data_Double));
	edd = eet_data_descriptor_stream_new(&eddc);
	_ddescriptors[EON_ECORE_REMOTE_DATA_DOUBLE] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Data_Double, "d", d, EET_T_DOUBLE);
	/* string */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Data_String", sizeof(Eon_Ecore_Remote_Data_String));
	edd = eet_data_descriptor_stream_new(&eddc);
	_ddescriptors[EON_ECORE_REMOTE_DATA_STRING] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Data_String, "s", s, EET_T_INLINED_STRING);
	/* list of datas */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Data_List", sizeof(Eon_Ecore_Remote_Data_List));
	edd = eet_data_descriptor_stream_new(&eddc);
	_ddescriptors[EON_ECORE_REMOTE_DATA_DOUBLE] = edd;
	EET_DATA_DESCRIPTOR_ADD_LIST(edd, Eon_Ecore_Remote_Data_List, "list", list, _data);

	EET_DATA_DESCRIPTOR_ADD_MAPPING(_unified, "Eon_Ecore_Remote_Data_Uint32", _ddescriptors[EON_ECORE_REMOTE_DATA_UINT32]);
	EET_DATA_DESCRIPTOR_ADD_MAPPING(_unified, "Eon_Ecore_Remote_Data_String", _ddescriptors[EON_ECORE_REMOTE_DATA_STRING]);
	EET_DATA_DESCRIPTOR_ADD_MAPPING(_unified, "Eon_Ecore_Remote_Data_Double", _ddescriptors[EON_ECORE_REMOTE_DATA_DOUBLE]);
	EET_DATA_DESCRIPTOR_ADD_MAPPING(_unified, "Eon_Ecore_Remote_Data_List", _ddescriptors[EON_ECORE_REMOTE_DATA_LIST]);

	/* now the messages */
	/* client size */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Client_New", sizeof(Eon_Ecore_Remote_Client_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_CLIENT_NEW)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Client_New, "width", width, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Client_New, "height", height, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Client_New, "layout_id", layout_id, EET_T_UINT);
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
	EET_DATA_DESCRIPTOR_ADD_SUB(edd, Eon_Ecore_Remote_Property_Set, "value", value, _data);
	/* property add */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Property_Add", sizeof(Eon_Ecore_Remote_Property_Add));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_PROPERTY_ADD)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Add, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Add, "name", name, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_SUB(edd, Eon_Ecore_Remote_Property_Add, "value", value, _data);
	/* property remove */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Eon_Ecore_Remote_Property_Remove", sizeof(Eon_Ecore_Remote_Property_Remove));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(EON_ECORE_REMOTE_PROPERTY_REMOVE)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Remove, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eon_Ecore_Remote_Property_Remove, "name", name, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_SUB(edd, Eon_Ecore_Remote_Property_Remove, "value", value, _data);
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



