/* ENDER - Enesim's descriptor library
 * Copyright (C) 2010 Jorge Luis Zapata
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
#ifndef _ENDER_PRIVATE_H
#define _ENDER_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Egueb_Smil.h>

/* core */
#define ERR(...) EINA_LOG_DOM_ERR(eon_theme_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eon_theme_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(eon_theme_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eon_theme_log_dom, __VA_ARGS__)
extern int eon_theme_log_dom;

Egueb_Dom_Node * eon_theme_element_instance_new(void);
void eon_theme_element_instance_relative_set(Egueb_Dom_Node *n, Egueb_Dom_Node *rel);

#include "eon_theme_namespace_private.h"

#if 0
struct _Eon_Theme_Container
{
	Eon_Theme_Value_Type type;
	int ref;
	/* sub containers */
	Eina_List *elements;
	Eon_Theme_Constraint *constraint;
	void *serialize_data;
};

typedef struct _Eon_Theme_Value_Data
{
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	uint64_t u64;
	double d;
	void *ptr;
} Eon_Theme_Value_Data;

struct _Eon_Theme_Value
{
	Eon_Theme_Container *container;
	int ref;
	Eon_Theme_Value_Free free_cb;
	void *free_cb_data;
	Eina_Bool owned;
	Eon_Theme_Value_Data data;
};

/* constructor */
typedef struct _Eon_Theme_Constructor Eon_Theme_Constructor;
typedef void (*Eon_Theme_Constructor_Free)(void *data);

/* descriptor */
typedef Eina_Bool (*Eon_Theme_Descriptor_Validate)(const char *name, Eon_Theme_Namespace *ns,
		Eon_Theme_Creator creator,
		Eon_Theme_Destructor destructor,
		Eon_Theme_Descriptor *parent, Eon_Theme_Descriptor_Type type);
typedef void *(*Eon_Theme_Descriptor_Creator)(Eon_Theme_Descriptor *d);
typedef void (*Eon_Theme_Descriptor_Destructor)(Eon_Theme_Descriptor *d, void *data);
typedef Eon_Theme_Property * (*Eon_Theme_Descriptor_Property_Add)(Eon_Theme_Descriptor *d,
		const char *name,
		Eon_Theme_Container *ec, Eon_Theme_Getter get, Eon_Theme_Setter set,
		Eon_Theme_Add add, Eon_Theme_Remove remove, Eon_Theme_Clear clear,
		Eon_Theme_Is_Set is_set,
		Eina_Bool relative,
		int offset);
typedef size_t (*Eon_Theme_Descriptor_Size_Get)(Eon_Theme_Descriptor *d);

typedef struct _Eon_Theme_Descriptor_Backend
{
	Eon_Theme_Descriptor_Validate validate;
	Eon_Theme_Descriptor_Creator creator;
	Eon_Theme_Descriptor_Destructor destructor;
	Eon_Theme_Descriptor_Property_Add property_add;
	Eon_Theme_Descriptor_Size_Get size_get;
} Eon_Theme_Descriptor_Backend;

struct _Eon_Theme_Descriptor
{
	char *name;
	int size;
	Eon_Theme_Descriptor_Type type;
	Eon_Theme_Descriptor *parent;
	Eon_Theme_Creator create;
	Eon_Theme_Destructor destroy;
	Eina_Ordered_Hash *properties;
	Eina_Ordered_Hash *functions;
	Eon_Theme_Namespace *ns;
	void *data;
};

typedef struct _Eon_Theme_Descriptor_Property
{
	Eon_Theme_Getter get;
	Eon_Theme_Setter set;
	Eon_Theme_Add add;
	Eon_Theme_Remove remove;
	Eon_Theme_Clear clear;
	Eon_Theme_Is_Set is_set;
} Eon_Theme_Descriptor_Property;

void eon_theme_descriptor_object_property_set(Eon_Theme_Property *p,
		Eon_Theme_Element *e, Eon_Theme_Value *v, void *data);
void eon_theme_descriptor_object_property_add(Eon_Theme_Property *p,
		Eon_Theme_Element *e, Eon_Theme_Value *v, void *data);
void eon_theme_descriptor_object_property_remove(Eon_Theme_Property *p,
		Eon_Theme_Element *e, Eon_Theme_Value *v, void *data);
void eon_theme_descriptor_object_property_get(Eon_Theme_Property *p,
		Eon_Theme_Element *e, Eon_Theme_Value *v, void *data);
void eon_theme_descriptor_object_property_clear(Eon_Theme_Property *p,
		Eon_Theme_Element *e, void *data);
Eina_Bool eon_theme_descriptor_object_property_is_set(Eon_Theme_Property *p,
		Eon_Theme_Element *e, void *data);
void eon_theme_descriptor_object_property_free(void *data);

void eon_theme_descriptor_init(void);
void eon_theme_descriptor_shutdown(void);
Eon_Theme_Descriptor * eon_theme_descriptor_new(const char *name, Eon_Theme_Namespace *ns,
		Eon_Theme_Creator creator,
		Eon_Theme_Destructor destructor,
		Eon_Theme_Descriptor *parent, Eon_Theme_Descriptor_Type type,
		int size);
void eon_theme_descriptor_free(Eon_Theme_Descriptor *thiz);
const char * eon_theme_descriptor_name_get(Eon_Theme_Descriptor *edesc);
Eon_Theme_Descriptor * eon_theme_descriptor_find(const char *name);

/* property */
typedef void (*Eon_Theme_Property_Accessor)(Eon_Theme_Property *ep, Eon_Theme_Element *e, Eon_Theme_Value *v, void *data);
typedef Eina_Bool (*Eon_Theme_Property_Is_Set)(Eon_Theme_Property *ep, Eon_Theme_Element *e, void *data);
typedef Eon_Theme_Property_Accessor Eon_Theme_Property_Getter;
typedef Eon_Theme_Property_Accessor Eon_Theme_Property_Setter;
typedef Eon_Theme_Property_Accessor Eon_Theme_Property_Add;
typedef Eon_Theme_Property_Accessor Eon_Theme_Property_Remove;
typedef void (*Eon_Theme_Property_Clear)(Eon_Theme_Property *ep, Eon_Theme_Element *e, void *data);
typedef void (*Eon_Theme_Property_Free)(void *data);
Eon_Theme_Property * eon_theme_property_new(const char *name,
		Eon_Theme_Container *ec,
		Eon_Theme_Property_Getter get,
		Eon_Theme_Property_Setter set,
		Eon_Theme_Property_Add add,
		Eon_Theme_Property_Remove remove,
		Eon_Theme_Property_Clear clear,
		Eon_Theme_Property_Is_Set is_set,
		Eina_Bool relative,
		Eon_Theme_Property_Free free,
		void *data);
void eon_theme_property_free(Eon_Theme_Property *thiz);
void * eon_theme_property_data_get(Eon_Theme_Property *thiz);

void eon_theme_property_element_value_set(Eon_Theme_Property *ep, Eon_Theme_Element *e,
		Eon_Theme_Value *v);
void eon_theme_property_element_value_get(Eon_Theme_Property *ep, Eon_Theme_Element *e,
		Eon_Theme_Value *v);
void eon_theme_property_element_value_add(Eon_Theme_Property *ep, Eon_Theme_Element *e,
		Eon_Theme_Value *v);
void eon_theme_property_element_value_remove(Eon_Theme_Property *ep, Eon_Theme_Element *e,
		Eon_Theme_Value *v);
void eon_theme_property_element_value_clear(Eon_Theme_Property *ep, Eon_Theme_Element *e);
Eina_Bool eon_theme_property_element_value_is_set(Eon_Theme_Property *ep, Eon_Theme_Element *e);

/* object */
extern Eon_Theme_Descriptor_Backend eon_theme_object_backend;
/* struct */
extern Eon_Theme_Descriptor_Backend eon_theme_struct_backend;
void eon_theme_struct_init(void);
/* union */
extern Eon_Theme_Descriptor_Backend eon_theme_union_backend;
void eon_theme_union_init(void);

/* marshaller */
void eon_theme_marshaller_init(void);
void eon_theme_marshaller_shutdown(void);

/* function */
Eon_Theme_Function * eon_theme_function_new(const char *name,
		Eon_Theme_Accessor f,
		Eon_Theme_Marshaller marshaller,
		Eon_Theme_Container *ret, Eina_List *args);
Eina_Bool eon_theme_function_call(Eon_Theme_Function *thiz, void *o,
		Eon_Theme_Value *ret, Eina_List *args);

/* container */
/* serializer */
typedef void * (*Eon_Theme_Serializer_Container_New)(Eon_Theme_Container *c);
typedef void * (*Eon_Theme_Serializer_Element_Marshal)(Eon_Theme_Descriptor *d,
		Eon_Theme_Element *e, unsigned int *len);
typedef Eon_Theme_Element * (*Eon_Theme_Serializer_Element_Unmarshal)(
		Eon_Theme_Descriptor *d, void *data, unsigned int len);
typedef void * (*Eon_Theme_Serializer_Value_Marshal)(void *sd,
		const Eon_Theme_Value *v, unsigned int *len);
typedef Eon_Theme_Value * (*Eon_Theme_Serializer_Value_Unmarshal)(
		Eon_Theme_Container *c, void *sd, void *data, unsigned int len);
typedef void * (*Eon_Theme_Serializer_Native_Marshal)(Eon_Theme_Descriptor *d,
		void *native, unsigned int *len);
typedef void (*Eon_Theme_Serializer_Native_Unmarshal)(
		Eon_Theme_Descriptor *d, void *native, const void *data,
		unsigned int len);

typedef struct _Eon_Theme_Serializer {
	Eon_Theme_Serializer_Container_New container_new;
	Eon_Theme_Serializer_Element_Marshal element_marshal;
	Eon_Theme_Serializer_Element_Unmarshal element_unmarshal;
	Eon_Theme_Serializer_Value_Marshal value_marshal;
	Eon_Theme_Serializer_Value_Unmarshal value_unmarshal;
	Eon_Theme_Serializer_Native_Marshal native_marshal;
	Eon_Theme_Serializer_Native_Unmarshal native_unmarshal;
} Eon_Theme_Serializer;

void * eon_theme_serializer_container_new(Eon_Theme_Container *ec);
void * eon_theme_serializer_element_marshal(Eon_Theme_Element *e, unsigned int *len);
Eon_Theme_Element * eon_theme_serializer_element_unmarshal(Eon_Theme_Descriptor *d,
		void *data, unsigned int len);
Eon_Theme_Value * eon_theme_serializer_value_unmarshal(Eon_Theme_Container *ec,
		void *data, unsigned int len);
void * eon_theme_serializer_value_marshal(const Eon_Theme_Value *v, unsigned int *len);
void * eon_theme_serializer_native_marshal(Eon_Theme_Descriptor *d, void *native, unsigned int *len);
void eon_theme_serializer_native_unmarshal(Eon_Theme_Descriptor *d,
		void *native, const void *data, unsigned int len);
Eon_Theme_Serializer * eon_theme_serializer_eet_get(void);

/* namespace */
typedef void (*Eon_Theme_Namespace_Initialize)(Eon_Theme_Namespace *thiz, void *data);
void eon_theme_namespace_initialize_cb_set(Eon_Theme_Namespace *thiz, Eon_Theme_Namespace_Initialize cb, void *data);
void eon_theme_namespace_init(void);
void eon_theme_namespace_shutdown(void);
void eon_theme_namespace_dump(Eon_Theme_Namespace *ns);
Eon_Theme_Element * eon_theme_namespace_element_new_from_descriptor(Eon_Theme_Namespace *thiz, Eon_Theme_Descriptor *desc);
Eon_Theme_Element * eon_theme_namespace_element_new_from_descriptor_and_data(
		Eon_Theme_Namespace *thiz, Eon_Theme_Descriptor *desc, void *data);

/* element */
Eon_Theme_Element * eon_theme_element_new(Eon_Theme_Descriptor *d);
Eon_Theme_Element * eon_theme_element_new_from_data(Eon_Theme_Descriptor *desc, void *data);

/* the loader */
void eon_theme_loader_init(void);
void eon_theme_loader_shutdown(void);
void eon_theme_loader_load_all(void);

/* the grammar */
typedef struct _Eon_Theme_Grammar_Type
{
	char *name;
	char *alias;
	Eon_Theme_Parser_Container *container;
} Eon_Theme_Grammar_Type;

/* the parser */
typedef struct _Eon_Theme_Parser Eon_Theme_Parser;
void eon_theme_parser_error(Eon_Theme_Parser *thiz, const char *str, int line,
	int col);
void eon_theme_parser_add_using(Eon_Theme_Parser *thiz, Eina_List *using);
void eon_theme_parser_add_namespace(Eon_Theme_Parser *thiz, const char *name,
		int version);
void eon_theme_parser_add_native(Eon_Theme_Parser *thiz, const char *name,
		const char *alias, Eon_Theme_Descriptor_Type type,
		const char *parent);
void eon_theme_parser_add_property(Eon_Theme_Parser *thiz, Eon_Theme_Parser_Property *p);
void eon_theme_parser_add_function(Eon_Theme_Parser *thiz, Eon_Theme_Parser_Function *f);
void eon_theme_parser_container_free(Eon_Theme_Parser_Container *c);
#endif

#endif
