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
#ifndef _ENDER_H
#define _ENDER_H

#include <Eina.h>
#include <Egueb_Css.h>
#include <Egueb_Dom.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef ENDER_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

EAPI void eon_theme_init(void);
EAPI void eon_theme_shutdown(void);

EAPI Egueb_Dom_Node * eon_theme_document_new(void);
EAPI Egueb_Dom_Node * eon_theme_document_instance_new(Egueb_Dom_Node *n,
		const char *id, Eina_Error *err);

EAPI Egueb_Dom_Node * eon_theme_element_eon_theme_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_state_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_scene_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_states_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_object_new(void);

EAPI void * eon_theme_element_instance_object_get(Egueb_Dom_Node *n);
EAPI Eina_Bool eon_theme_element_instance_state_set(Egueb_Dom_Node *n,
		const char *state, Eina_Error *err);

/* Shared strings */
EAPI extern Egueb_Dom_String *ENDER_ELEMENT_ENDER;
EAPI extern Egueb_Dom_String *ENDER_ELEMENT_INSTANCE;
EAPI extern Egueb_Dom_String *ENDER_ELEMENT_OBJECT;
EAPI extern Egueb_Dom_String *ENDER_ELEMENT_SCENE;
EAPI extern Egueb_Dom_String *ENDER_ELEMENT_STATES;
EAPI extern Egueb_Dom_String *ENDER_ELEMENT_STATE;

#include "eon_theme_attr_int.h"
#include "eon_theme_attr_double.h"
#include "eon_theme_attr_enesim_color.h"
#include "eon_theme_attr_font.h"
#include "eon_theme_attr_primitive.h"
#include "eon_theme_instance.h"
#include "eon_theme_namespace.h"

#if 0
/**
 * @mainpage Ender
 * @section intro Introduction
 * Ender is a library that loads descriptions of objects
 * them into a collection of properties that are accesible through a common API.
 *
 * @section syntax Syntax
 * The syntax of a description file (.ender) is:
 * @code
 * namespace "[PREFIX.]NAME" {
 *   (abstract | class) "NAME" {
 *     PROPERTY "NAME";
 *   }
 * }
 * Where PROPERTY can be one of the following:
 * uint | int | double | argb | color | surface | matrix | object | ender | LIST

 * Where LIST is defined as:
 * (PROPERTY [, PROPERTY])
 *
 * @endcode
 *
 * @section examples Examples
 *
 * @section dependencies Dependencies
 * - Eina
 * - Enesim
 *
 * @todo
 */

typedef struct _Eon_Theme_Element Eon_Theme_Element;
typedef struct _Eon_Theme_Property Eon_Theme_Property;
typedef struct _Eon_Theme_Function Eon_Theme_Function;
typedef struct _Eon_Theme_Container Eon_Theme_Container;
typedef struct _Eon_Theme_Descriptor Eon_Theme_Descriptor;
typedef struct _Eon_Theme_Namespace Eon_Theme_Namespace;
typedef struct _Eon_Theme_Constraint Eon_Theme_Constraint;
typedef struct _Eon_Theme_Value Eon_Theme_Value;

typedef void (*Eon_Theme_Event_Callback)(Eon_Theme_Element *e, const char *event_name, void *event_data, void *data);
typedef void (*Eon_Theme_New_Callback)(Eon_Theme_Element *e, void *data);

typedef enum _Eon_Theme_Descriptor_Type
{
	ENDER_TYPE_ABSTRACT,
	ENDER_TYPE_CLASS,
	ENDER_TYPE_STRUCT,
	ENDER_TYPE_UNION,
	ENDER_TYPES,
} Eon_Theme_Descriptor_Type;

/* TODO add an UNKNOWN */
typedef enum _Eon_Theme_Value_Type
{
	/* basic types */
	ENDER_BOOL,
	ENDER_UINT32,
	ENDER_INT32,
	ENDER_UINT64,
	ENDER_INT64,
	ENDER_DOUBLE,
	ENDER_COLOR,
	ENDER_ARGB,
	ENDER_STRING,
	ENDER_OBJECT,
	ENDER_ENDER,
	ENDER_POINTER,
	ENDER_VALUE,
	/* compound types */
	ENDER_LIST,
	/* descriptor based types? */
	ENDER_STRUCT,
	ENDER_UNION,
	ENDER_PROPERTY_TYPES,
} Eon_Theme_Value_Type;

typedef enum _Eon_Theme_Property_Flag
{
	ENDER_GET = (1 << 0),
	ENDER_SET = (1 << 1),
	ENDER_ADD = (1 << 2),
	ENDER_REMOVE = (1 << 3),
	ENDER_CLEAR = (1 << 4),
	ENDER_IS_SET = (1 << 5),
} Eon_Theme_Property_Flag;

/*
 * @defgroup Eon_Theme_Main_Group Main
 * @{
 */
EAPI void eon_theme_init(void);
EAPI void eon_theme_shutdown(void);
EAPI void eon_theme_version(unsigned int *major, unsigned int *minor, unsigned int *micro);

/**
 * @}
 * @defgroup Eon_Theme_Container_Group Container
 * @{
 */
EAPI Eon_Theme_Container * eon_theme_container_new(Eon_Theme_Value_Type t);
EAPI Eon_Theme_Container * eon_theme_container_new_descriptor_from(Eon_Theme_Descriptor *descriptor);
EAPI Eon_Theme_Container * eon_theme_container_list_new(Eon_Theme_Container *child);
EAPI Eon_Theme_Container * eon_theme_container_struct_new(Eon_Theme_Descriptor *descriptor);
EAPI Eon_Theme_Container * eon_theme_container_union_new(Eon_Theme_Descriptor *descriptor);

EAPI Eon_Theme_Container * eon_theme_container_ref(Eon_Theme_Container *thiz);
EAPI Eon_Theme_Container * eon_theme_container_unref(Eon_Theme_Container *thiz);

EAPI Eina_Bool eon_theme_container_is_compound(Eon_Theme_Container *ec);
EAPI Eon_Theme_Container * eon_theme_container_compound_get(Eon_Theme_Container *ec, unsigned int idx, const char **name);
EAPI Eon_Theme_Container * eon_theme_container_compound_get_by_name(Eon_Theme_Container *ec, const char *name, unsigned int *idx);

EAPI size_t eon_theme_container_size_get(Eon_Theme_Container *ec);

EAPI unsigned int eon_theme_container_compound_count(Eon_Theme_Container *ec);
EAPI Eon_Theme_Value_Type eon_theme_container_type_get(Eon_Theme_Container *c);

EAPI void eon_theme_container_constraint_set(Eon_Theme_Container *thiz, Eon_Theme_Constraint *c);
EAPI const Eon_Theme_Constraint * eon_theme_container_constraint_get(Eon_Theme_Container *thiz);

EAPI Eon_Theme_Value * eon_theme_container_value_unmarshal(Eon_Theme_Container *c, void *data, unsigned int len);

/**
 * @}
 * @defgroup Eon_Theme_Value_Group Value
 * @{
 */

typedef void (*Eon_Theme_Value_Free)(Eon_Theme_Value *value, void *data);

EAPI Eon_Theme_Value * eon_theme_value_basic_new(Eon_Theme_Value_Type type);
EAPI Eon_Theme_Value * eon_theme_value_list_new(Eon_Theme_Container *child);
EAPI Eon_Theme_Value * eon_theme_value_new_container_from(Eon_Theme_Container *container);
EAPI Eon_Theme_Value * eon_theme_value_new_container_static_from(Eon_Theme_Container *ec);
EAPI Eon_Theme_Container * eon_theme_value_container_get(const Eon_Theme_Value *value);
EAPI Eon_Theme_Value_Type eon_theme_value_type_get(const Eon_Theme_Value *value);

EAPI void * eon_theme_value_marshal(Eon_Theme_Value *v, unsigned int *len);

EAPI void eon_theme_value_bool_set(Eon_Theme_Value *value, Eina_Bool boolean);
EAPI Eina_Bool eon_theme_value_bool_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_int32_set(Eon_Theme_Value *value, int32_t i32);
EAPI int32_t eon_theme_value_int32_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_uint32_set(Eon_Theme_Value *value, uint32_t u32);
EAPI uint32_t eon_theme_value_uint32_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_int64_set(Eon_Theme_Value *value, int64_t i64);
EAPI int64_t eon_theme_value_int64_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_uint64_set(Eon_Theme_Value *value, uint64_t u64);
EAPI uint64_t eon_theme_value_uint64_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_double_set(Eon_Theme_Value *value, double d);
EAPI double eon_theme_value_double_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_argb_set(Eon_Theme_Value *value, uint32_t argb);
EAPI uint32_t eon_theme_value_argb_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_color_set(Eon_Theme_Value *value, uint32_t argb);
EAPI uint32_t eon_theme_value_color_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_string_set(Eon_Theme_Value *value, char * string);
EAPI void eon_theme_value_static_string_set(Eon_Theme_Value *value, const char * string);
EAPI const char * eon_theme_value_string_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_struct_set(Eon_Theme_Value *value, void * structure);
EAPI void * eon_theme_value_struct_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_union_set(Eon_Theme_Value *value, int type, void *un);
EAPI void * eon_theme_value_union_get(const Eon_Theme_Value *value, int *type);

EAPI void eon_theme_value_object_set(Eon_Theme_Value *value, void *object);
EAPI void * eon_theme_value_object_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_eon_theme_set(Eon_Theme_Value *value, Eon_Theme_Element *ender);
EAPI Eon_Theme_Element * eon_theme_value_eon_theme_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_pointer_set(Eon_Theme_Value *value, void *ptr, Eon_Theme_Value_Free free_cb, void *user_data);
EAPI void * eon_theme_value_pointer_get(const Eon_Theme_Value *value);

EAPI void eon_theme_value_list_add(Eon_Theme_Value *value, Eon_Theme_Value *child);
EAPI void eon_theme_value_list_set(Eon_Theme_Value *value, Eina_List *list);
EAPI const Eina_List * eon_theme_value_list_get(const Eon_Theme_Value *value);

EAPI Eon_Theme_Value * eon_theme_value_ref(Eon_Theme_Value *thiz);
EAPI void eon_theme_value_unref(Eon_Theme_Value *thiz);

typedef void (*Eon_Theme_Accessor)(void *o, ...);
typedef Eina_Bool (*Eon_Theme_Is_Set)(void *o);
typedef Eon_Theme_Accessor Eon_Theme_Getter;
typedef Eon_Theme_Accessor Eon_Theme_Setter;
typedef Eon_Theme_Accessor Eon_Theme_Add;
typedef Eon_Theme_Accessor Eon_Theme_Remove;
typedef void (*Eon_Theme_Clear)(void *o);

#define ENDER_IS_SET(f) ((Eon_Theme_Is_Set)(f))
#define ENDER_GETTER(f) ((Eon_Theme_Getter)(f))
#define ENDER_SETTER(f) ((Eon_Theme_Setter)(f))
#define ENDER_ADD(f) ((Eon_Theme_Add)(f))
#define ENDER_REMOVE(f) ((Eon_Theme_Remove)(f))
#define ENDER_CLEAR(f) ((Eon_Theme_Clear)(f))

/**
 * @}
 * @defgroup Eon_Theme_Namespace_Group Namespace
 * @{
 */
typedef Eina_Bool (*Eon_Theme_Namespace_List_Callback)(Eon_Theme_Namespace *thiz, void *data);
typedef Eina_Bool (*Eon_Theme_Descriptor_List_Callback)(Eon_Theme_Descriptor *thiz, void *data);
typedef void * (*Eon_Theme_Creator)(void);
typedef void (*Eon_Theme_Destructor)(void *);

#define ENDER_CREATOR(f) ((Eon_Theme_Creator)(f))
#define ENDER_DESTRUCTOR(f) ((Eon_Theme_Destructor)(f))

EAPI void eon_theme_namespace_list(Eon_Theme_Namespace_List_Callback cb, void *data);
EAPI Eina_Bool eon_theme_namespace_list_with_name(const char *name, Eon_Theme_Namespace_List_Callback cb, void *data);
EAPI Eon_Theme_Namespace * eon_theme_namespace_new(const char *name, int version);
EAPI Eon_Theme_Namespace * eon_theme_namespace_find(const char *name, int version);
EAPI void eon_theme_namespace_descriptor_list(Eon_Theme_Namespace *thiz,
		Eon_Theme_Descriptor_List_Callback cb, void *data);
EAPI Eon_Theme_Descriptor * eon_theme_namespace_descriptor_find(Eon_Theme_Namespace *thiz,
		const char *name);
EAPI Eon_Theme_Descriptor * eon_theme_namespace_descriptor_add(Eon_Theme_Namespace *ens,
		const char *name, Eon_Theme_Creator creator,
		Eon_Theme_Destructor destructor, Eon_Theme_Descriptor *parent,
		Eon_Theme_Descriptor_Type type, int size);
EAPI const char * eon_theme_namespace_name_get(Eon_Theme_Namespace *thiz);
EAPI int eon_theme_namespace_version_get(Eon_Theme_Namespace *thiz);
EAPI Eon_Theme_Element * eon_theme_namespace_element_new(Eon_Theme_Namespace *thiz, const char *name);

EAPI void eon_theme_namespace_element_new_listener_add(Eon_Theme_Namespace *thiz, Eon_Theme_New_Callback cb, void *data);
EAPI void eon_theme_namespace_element_new_listener_remove(Eon_Theme_Namespace *thiz, Eon_Theme_New_Callback cb, void *data);

/**
 * @}
 * @defgroup Eon_Theme_Marshaller_Group Marshaller
 * @{
 */

typedef Eina_Bool (*Eon_Theme_Marshaller)(void *data, Eon_Theme_Accessor f, Eon_Theme_Value *ret,
		Eina_List *args);

EAPI Eon_Theme_Marshaller eon_theme_marshaller_find(Eon_Theme_Container *ret,
		...);
EAPI Eon_Theme_Marshaller eon_theme_marshaller_find_list(Eon_Theme_Container *ret,
		Eina_List *args);

EAPI Eina_Bool eon_theme_marshaller_void__void(void *data, Eon_Theme_Accessor f,
		Eon_Theme_Value *ret, Eina_List *args);
EAPI Eina_Bool eon_theme_marshaller_void__string_string(void *data, Eon_Theme_Accessor f,
		Eon_Theme_Value *ret, Eina_List *args);
EAPI Eina_Bool eon_theme_marshaller_string__string(void *data, Eon_Theme_Accessor f,
		Eon_Theme_Value *ret, Eina_List *args);
EAPI Eina_Bool eon_theme_marshaller_eon_theme__string(void *data, Eon_Theme_Accessor f,
		Eon_Theme_Value *ret, Eina_List *args);
EAPI Eina_Bool eon_theme_marshaller_eon_theme__void(void *data, Eon_Theme_Accessor f,
		Eon_Theme_Value *ret, Eina_List *args);

/**
 * @}
 * @defgroup Eon_Theme_Function_Group Function
 * @{
 */
#define ENDER_FUNCTION(f) ((Eon_Theme_Accessor)(f))

EAPI int eon_theme_function_args_count(Eon_Theme_Function *thiz);
EAPI const Eina_List * eon_theme_function_args_get(Eon_Theme_Function *thiz);
EAPI Eon_Theme_Container * eon_theme_function_ret_get(Eon_Theme_Function *thiz);
EAPI const char * eon_theme_function_name_get(Eon_Theme_Function *thiz);

/**
 * @}
 * @defgroup Eon_Theme_Descriptor_Group Descriptor
 * @{
 */

#define ENDER_DESCRIPTOR_PROPERTY_ADD_SIMPLE(d, name, vtype, prefix)		\
	eon_theme_descriptor_property_add(d, #name, eon_theme_container_new(vtype),	\
			prefix##_##name##_get, prefix##_##name##_set, 		\
			NULL, NULL, NULL,					\
			prefix##_##_name##_is_set,				\
			EINA_FALSE, -1)
#define ENDER_DESCRIPTOR_PROPERTY_ADD_DESCRIPTOR(d, name, pd, prefix)		\
	eon_theme_descriptor_property_add(d, #name, 				\
			eon_theme_container_new_descriptor_from(pd), 		\
			prefix##_##name##_get, prefix##_##name##_set, 		\
			NULL, NULL, NULL,					\
			prefix##_##_name##_is_set,				\
			EINA_FALSE, -1)

typedef void (*Eon_Theme_Property_List_Callback)(Eon_Theme_Property *prop, void *data);
typedef void (*Eon_Theme_Function_List_Callback)(Eon_Theme_Function *func, void *data);

EAPI Eon_Theme_Descriptor * eon_theme_descriptor_find(const char *name);
EAPI Eon_Theme_Descriptor * eon_theme_descriptor_find_with_namespace(const char *name, const char *ns, int version);
EAPI Eon_Theme_Property * eon_theme_descriptor_property_add(Eon_Theme_Descriptor *edesc,
		const char *name,
		Eon_Theme_Container *ec,
		Eon_Theme_Getter get,
		Eon_Theme_Setter set,
		Eon_Theme_Add add,
		Eon_Theme_Remove remove,
		Eon_Theme_Clear clear,
		Eon_Theme_Is_Set is_set,
		Eina_Bool relative,
		int offset);
EAPI Eon_Theme_Property * eon_theme_descriptor_struct_property_add(
		Eon_Theme_Descriptor *thiz,
		const char *name, Eon_Theme_Container *ec, int offset);
EAPI void eon_theme_descriptor_property_list(Eon_Theme_Descriptor *thiz, Eon_Theme_Property_List_Callback cb, void *data);
EAPI void eon_theme_descriptor_property_list_recursive(Eon_Theme_Descriptor *thiz, Eon_Theme_Property_List_Callback cb, void *data);
EAPI Eon_Theme_Property * eon_theme_descriptor_property_get(Eon_Theme_Descriptor *thiz, const char *name);
EAPI Eon_Theme_Property * eon_theme_descriptor_property_get_at(Eon_Theme_Descriptor *thiz, int idx);

EAPI Eon_Theme_Function * eon_theme_descriptor_function_add(Eon_Theme_Descriptor *edesc, const char *name,
		Eon_Theme_Accessor f, Eon_Theme_Marshaller marshaller, Eon_Theme_Container *ret, ...);
EAPI Eon_Theme_Function * eon_theme_descriptor_function_add_list(Eon_Theme_Descriptor *edesc, const char *name,
		Eon_Theme_Accessor f, Eon_Theme_Marshaller marshaller, Eon_Theme_Container *ret,
		Eina_List *args);
EAPI Eon_Theme_Function * eon_theme_descriptor_function_get(Eon_Theme_Descriptor *thiz, const char *name);
EAPI void eon_theme_descriptor_function_list(Eon_Theme_Descriptor *thiz, Eon_Theme_Function_List_Callback cb, void *data);

EAPI void eon_theme_descriptor_list(Eon_Theme_Descriptor_List_Callback cb, void *data);
EAPI Eina_Bool eon_theme_descriptor_exists(const char *name);
EAPI Eon_Theme_Descriptor_Type eon_theme_descriptor_type(Eon_Theme_Descriptor *thiz);
EAPI const char * eon_theme_descriptor_name_get(Eon_Theme_Descriptor *thiz);
EAPI size_t eon_theme_descriptor_size_get(Eon_Theme_Descriptor *thiz);
EAPI Eon_Theme_Descriptor * eon_theme_descriptor_parent(Eon_Theme_Descriptor *thiz);
EAPI Eon_Theme_Namespace * eon_theme_descriptor_namespace_get(Eon_Theme_Descriptor *thiz);
EAPI Eon_Theme_Element * eon_theme_descriptor_element_new(Eon_Theme_Descriptor *thiz);
EAPI Eon_Theme_Element * eon_theme_descriptor_element_new_from_data(
		Eon_Theme_Descriptor *thiz, void *data);

EAPI Eon_Theme_Element * eon_theme_descriptor_element_unmarshal(Eon_Theme_Descriptor *thiz,
		void *data, unsigned int len);
EAPI void * eon_theme_descriptor_native_marshal(Eon_Theme_Descriptor *thiz,
		void *data, unsigned int *len);
EAPI void * eon_theme_descriptor_native_unmarshal(Eon_Theme_Descriptor *thiz,
		const void *data, unsigned int len);
EAPI void eon_theme_descriptor_native_unmarshal_inplace(Eon_Theme_Descriptor *thiz,
		void *native, const void *data, unsigned int len);
EAPI void * eon_theme_descriptor_native_new(Eon_Theme_Descriptor *thiz);
EAPI void eon_theme_descriptor_native_free(Eon_Theme_Descriptor *thiz, void *object);
EAPI void eon_theme_descriptor_data_set(Eon_Theme_Descriptor *thiz, void *data);
EAPI void * eon_theme_descriptor_data_get(Eon_Theme_Descriptor *thiz);

/**
 * @}
 * @defgroup Eon_Theme_Element_Group Element
 * @{
 */
typedef void (*Eon_Theme_Element_Accessor)(Eon_Theme_Element *e, Eon_Theme_Property *ep, Eon_Theme_Value *v, void *data);
typedef Eina_Bool (*Eon_Theme_Element_Is_Set)(Eon_Theme_Element *e, Eon_Theme_Property *ep, void *data);
typedef Eon_Theme_Element_Accessor Eon_Theme_Element_Getter;
typedef Eon_Theme_Element_Accessor Eon_Theme_Element_Setter;
typedef Eon_Theme_Element_Accessor Eon_Theme_Element_Add;
typedef Eon_Theme_Element_Accessor Eon_Theme_Element_Remove;
typedef void (*Eon_Theme_Element_Clear)(Eon_Theme_Element *e, Eon_Theme_Property *ep, void *data);

EAPI Eon_Theme_Element * eon_theme_element_new_with_namespace(const char *name, const char *ns_name, int version);
EAPI Eon_Theme_Element * eon_theme_element_ref(Eon_Theme_Element *e);
EAPI Eon_Theme_Element * eon_theme_element_unref(Eon_Theme_Element *e);
EAPI void * eon_theme_element_data_set(Eon_Theme_Element *e, const char *key, void *data);
EAPI void * eon_theme_element_data_get(Eon_Theme_Element *e, const char *key);
EAPI const char * eon_theme_element_name_get(Eon_Theme_Element *e);
EAPI Eon_Theme_Descriptor * eon_theme_element_descriptor_get(Eon_Theme_Element *e);

EAPI Eon_Theme_Property * eon_theme_element_property_add(Eon_Theme_Element *e, const char *name,
		Eon_Theme_Container *ec,
		Eon_Theme_Element_Getter get,
		Eon_Theme_Element_Setter set,
		Eon_Theme_Element_Add add,
		Eon_Theme_Element_Remove remove,
		Eon_Theme_Element_Clear clear,
		Eon_Theme_Element_Is_Set is_set,
		Eina_Bool relative, void *data);
EAPI Eon_Theme_Property * eon_theme_element_property_get(Eon_Theme_Element *e, const char *name);
EAPI void eon_theme_element_property_list(Eon_Theme_Element *e, Eon_Theme_Property_List_Callback cb, void *data);
EAPI void eon_theme_element_property_value_set_valist(Eon_Theme_Element *e, Eon_Theme_Property *prop, va_list va_args);
EAPI void eon_theme_element_property_value_set(Eon_Theme_Element *e, Eon_Theme_Property *prop, ...);
EAPI void eon_theme_element_property_value_set_simple(Eon_Theme_Element *e, Eon_Theme_Property *prop, Eon_Theme_Value *value);

EAPI void eon_theme_element_property_value_get_valist(Eon_Theme_Element *e, Eon_Theme_Property *prop, va_list va_args);
EAPI void eon_theme_element_property_value_get(Eon_Theme_Element *e, Eon_Theme_Property *prop, ...);
EAPI void eon_theme_element_property_value_get_simple(Eon_Theme_Element *e, Eon_Theme_Property *prop, Eon_Theme_Value **value);

EAPI void eon_theme_element_property_value_add_valist(Eon_Theme_Element *e, Eon_Theme_Property *prop, va_list var_args);
EAPI void eon_theme_element_property_value_add(Eon_Theme_Element *e, Eon_Theme_Property *prop, ...);
EAPI void eon_theme_element_property_value_add_simple(Eon_Theme_Element *e, Eon_Theme_Property *prop, Eon_Theme_Value *value);

EAPI void eon_theme_element_property_value_remove_valist(Eon_Theme_Element *e, Eon_Theme_Property *prop, va_list var_args);
EAPI void eon_theme_element_property_value_remove(Eon_Theme_Element *e, Eon_Theme_Property *prop, ...);
EAPI void eon_theme_element_property_value_remove_simple(Eon_Theme_Element *e, Eon_Theme_Property *prop, Eon_Theme_Value *value);

EAPI Eina_Bool eon_theme_element_property_value_is_set(Eon_Theme_Element *e, Eon_Theme_Property *prop);

EAPI void eon_theme_element_value_get(Eon_Theme_Element *e, const char *name, ...);
EAPI void eon_theme_element_value_get_valist(Eon_Theme_Element *e, const char *name, va_list var_args);
EAPI void eon_theme_element_value_get_simple(Eon_Theme_Element *e, const char *name, Eon_Theme_Value **value);

EAPI void eon_theme_element_value_set(Eon_Theme_Element *e, const char *name, ...);
EAPI void eon_theme_element_value_set_valist(Eon_Theme_Element *e, const char *name, va_list var_args);
EAPI void eon_theme_element_value_set_simple(Eon_Theme_Element *e, const char *name, Eon_Theme_Value *value);

EAPI void eon_theme_element_value_add(Eon_Theme_Element *e, const char *name, ...);
EAPI void eon_theme_element_value_add_valist(Eon_Theme_Element *e, const char *name, va_list var_args);
EAPI void eon_theme_element_value_add_simple(Eon_Theme_Element *e, const char *name, Eon_Theme_Value *value);

EAPI void eon_theme_element_value_remove(Eon_Theme_Element *e, const char *name, ...);
EAPI void eon_theme_element_value_remove_valist(Eon_Theme_Element *e, const char *name, va_list var_args);
EAPI void eon_theme_element_value_remove_simple(Eon_Theme_Element *e, const char *name, Eon_Theme_Value *value);

EAPI void eon_theme_element_value_clear(Eon_Theme_Element *e, const char *name);

EAPI Eina_Bool eon_theme_element_value_is_set(Eon_Theme_Element *e, const char *name);

EAPI void * eon_theme_element_object_get(Eon_Theme_Element *e);

EAPI Eon_Theme_Element * eon_theme_element_parent_get(Eon_Theme_Element *e);

EAPI Eina_Bool eon_theme_element_call_valist(Eon_Theme_Element *e, const char *name, void *ret, va_list va_args);
EAPI Eina_Bool eon_theme_element_call(Eon_Theme_Element *e, const char *name, void *ret, ...);
EAPI Eina_Bool eon_theme_element_function_call_valist(Eon_Theme_Element *e, Eon_Theme_Function *f, void *ret, va_list va_args);
EAPI Eina_Bool eon_theme_element_function_call(Eon_Theme_Element *e, Eon_Theme_Function *f, void *ret, ...);
EAPI Eina_Bool eon_theme_element_function_call_simple(Eon_Theme_Element *e,
		Eon_Theme_Function *f, Eon_Theme_Value *ret, Eina_List *args);

EAPI void * eon_theme_element_marshal(Eon_Theme_Element *e, unsigned int *len);

/**
 * @}
 * @defgroup Eon_Theme_Property_Group Property
 * @{
 */
EAPI Eon_Theme_Value_Type eon_theme_property_type_get(Eon_Theme_Property *p);
EAPI Eon_Theme_Container * eon_theme_property_container_get(Eon_Theme_Property *p);
EAPI Eina_Bool eon_theme_property_is_relative(Eon_Theme_Property *p);
EAPI const char * eon_theme_property_name_get(Eon_Theme_Property *p);
EAPI Eon_Theme_Property_Flag eon_theme_property_flags_get(Eon_Theme_Property *p);

/**
 * @}
 * @defgroup Eon_Theme_Constraint_Group Constraint
 * @{
 */

typedef enum _Eon_Theme_Constraint_Type
{
	ENDER_CONSTRAINT_RANGE,
	ENDER_CONSTRAINT_ENUM,
	ENDER_CONSTRAINT_DESCRIPTOR,
	ENDER_CONSTRAINTS,
} Eon_Theme_Constraint_Type;

EAPI Eon_Theme_Constraint_Type eon_theme_constraint_type_get(const Eon_Theme_Constraint *thiz);

EAPI Eon_Theme_Constraint * eon_theme_constraint_range_new(Eon_Theme_Value_Type type);

EAPI Eon_Theme_Constraint * eon_theme_constraint_enum_new(void);
EAPI void eon_theme_constraint_enum_append(Eon_Theme_Constraint *thiz, const char *name, int value);

EAPI Eon_Theme_Constraint * eon_theme_constraint_descriptor_new(
	Eon_Theme_Descriptor *descriptor);
EAPI Eon_Theme_Descriptor * eon_theme_constraint_descriptor_descriptor_get(
	const Eon_Theme_Constraint *thiz);

/**
 * @}
 * @defgroup Eon_Theme_Other_Group Other
 * @{
 */

typedef enum _Eon_Theme_Event_Mutation_Type
{
	ENDER_EVENT_MUTATION_SET,
	ENDER_EVENT_MUTATION_ADD,
	ENDER_EVENT_MUTATION_REMOVE,
	ENDER_EVENT_MUTATION_CLEAR,
} Eon_Theme_Event_Mutation_Type;

typedef struct _Eon_Theme_Event_Mutation
{
	const char *name;
	const Eon_Theme_Value *value;
	Eon_Theme_Event_Mutation_Type type;
} Eon_Theme_Event_Mutation;

typedef struct _Eon_Theme_Event_Mutation_Property
{
	const Eon_Theme_Value *value;
	Eon_Theme_Event_Mutation_Type type;
} Eon_Theme_Event_Mutation_Property;

typedef struct _Eon_Theme_Listener Eon_Theme_Listener;

EAPI Eon_Theme_Listener * eon_theme_event_listener_add(Eon_Theme_Element *e, const char *event_name, Eon_Theme_Event_Callback cb, void *data);
EAPI void eon_theme_event_listener_remove(Eon_Theme_Listener *l);
EAPI void eon_theme_event_listener_remove_full(Eon_Theme_Element *e, const char *event_name, Eon_Theme_Event_Callback cb, void *data);
EAPI void eon_theme_event_dispatch(Eon_Theme_Element *e, const char *event_name, void *event_data);

/**
 * @}
 * @defgroup Eon_Theme_Helper_Group Helper
 * @{
 */
EAPI const char * eon_theme_value_type_string_to(Eon_Theme_Value_Type type);
EAPI const char * eon_theme_descriptor_type_string_to(Eon_Theme_Descriptor_Type type);
EAPI Eina_Bool eon_theme_descriptor_type_value_type_to(Eon_Theme_Descriptor_Type d,
		Eon_Theme_Value_Type *v);

/**
 * @}
 * @defgroup Eon_Theme_Loader_Group Loader
 * @{
 */

typedef void (*Eon_Theme_Loader_Registry_Callback)(void *data);

EAPI void eon_theme_loader_load(const char *in);
EAPI void eon_theme_loader_registry_callback_add(Eon_Theme_Loader_Registry_Callback cb, void *data);

/**
 * @}
 * @defgroup Eon_Theme_Parser_Group Parser
 * @{
 */

typedef struct _Eon_Theme_Parser_Definition
{
	const char *name;
	const char *alias;
} Eon_Theme_Parser_Definition;

typedef struct _Eon_Theme_Parser_Container
{
	Eon_Theme_Value_Type type;
	Eina_List *subcontainers;
	char *defined;
} Eon_Theme_Parser_Container;

typedef struct _Eon_Theme_Parser_Property
{
	Eon_Theme_Parser_Definition def;
	Eon_Theme_Parser_Container *container;
	Eina_Bool rel;
} Eon_Theme_Parser_Property;

typedef struct _Eon_Theme_Parser_Function
{
	Eon_Theme_Parser_Definition def;
	Eon_Theme_Parser_Container *ret;
	Eina_List *args;
} Eon_Theme_Parser_Function;

/* FIXME this functions will be evolving until all of them
 * are similar to the add_property
 */
typedef void (*Eon_Theme_Parser_Add_Using)(void *data, const char *name);
typedef void (*Eon_Theme_Parser_Add_Namespace)(void *data, const char *name,
		int version);
typedef void (*Eon_Theme_Parser_Add_Native)(void *data, const char *name,
		const char *alias, Eon_Theme_Descriptor_Type type, const char *parent);
typedef void (*Eon_Theme_Parser_Add_Property)(void *data, Eon_Theme_Parser_Property *p);
typedef void (*Eon_Theme_Parser_Add_Function)(void *data, Eon_Theme_Parser_Function *f);

typedef struct _Eon_Theme_Parser_Descriptor
{
	Eon_Theme_Parser_Add_Using add_using;
	Eon_Theme_Parser_Add_Namespace add_namespace;
	Eon_Theme_Parser_Add_Native add_native;
	Eon_Theme_Parser_Add_Property add_property;
	Eon_Theme_Parser_Add_Function add_function;
} Eon_Theme_Parser_Descriptor;

EAPI Eina_Bool eon_theme_parser_parse(const char *file, Eon_Theme_Parser_Descriptor *descriptor, void *data);

/**
 * @}
 * @defgroup Eina_Ordered_Hash_Group Eina Ordered Hash
 * @{
 */

typedef struct _Eina_Ordered_Hash
{
	Eina_List *order;
	Eina_Hash *hash;
} Eina_Ordered_Hash;

EAPI Eina_Ordered_Hash * eina_ordered_hash_new(Eina_Free_Cb cb);
EAPI void eina_ordered_hash_free(Eina_Ordered_Hash *thiz);
EAPI int eina_ordered_hash_count(Eina_Ordered_Hash *thiz);
EAPI void * eina_ordered_hash_nth_get(Eina_Ordered_Hash *thiz, int nth);
EAPI void * eina_ordered_hash_find(Eina_Ordered_Hash *thiz, const char *key);
EAPI void eina_ordered_hash_add(Eina_Ordered_Hash *thiz, const char *name, void *data);
EAPI void eina_ordered_hash_del(Eina_Ordered_Hash *thiz, const char *name, void *data);
#endif
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*_ENDER_H*/
