#ifndef _EON_ECORE_REMOTE_H_
#define _EON_ECORE_REMOTE_H_

typedef enum _Eon_Ecore_Remote_Data_Type
{
	EON_ECORE_REMOTE_DATA_UINT32,
	EON_ECORE_REMOTE_DATA_DOUBLE,
	EON_ECORE_REMOTE_DATA_STRING,
	EON_ECORE_REMOTE_DATA_LIST,
	EON_ECORE_REMOTE_DATA_TYPES
} Eon_Ecore_Remote_Data_Type;

typedef struct _Eon_Ecore_Remote_Data_Uint32
{
	uint32_t u32;
} Eon_Ecore_Remote_Data_Uint32;

typedef struct _Eon_Ecore_Remote_Data_Double
{
	double d;
} Eon_Ecore_Remote_Data_Double;

typedef struct _Eon_Ecore_Remote_Data_String
{
	char *s;
} Eon_Ecore_Remote_Data_String;

typedef struct _Eon_Ecore_Remote_Data_List
{
	Eina_List *list;
} Eon_Ecore_Remote_Data_List;

typedef union _Eon_Ecore_Remote_Data_Value
{
	Eon_Ecore_Remote_Data_Uint32 duint32;
	Eon_Ecore_Remote_Data_Double ddouble;
	Eon_Ecore_Remote_Data_String dstring;
	Eon_Ecore_Remote_Data_List dlist;
} Eon_Ecore_Remote_Data_Value;

typedef struct _Eon_Ecore_Remote_Data
{
	Eon_Ecore_Remote_Data_Type type;
	Eon_Ecore_Remote_Data_Value value;
} Eon_Ecore_Remote_Data;

typedef enum _Eon_Ecore_Remote_Message
{
	EON_ECORE_REMOTE_CLIENT_SIZE = 0xe0400001,
	EON_ECORE_REMOTE_ELEMENT_NEW,
	EON_ECORE_REMOTE_PROPERTY_SET,
	EON_ECORE_REMOTE_PROPERTY_ADD,
	EON_ECORE_REMOTE_PROPERTY_REMOVE,
	EON_ECORE_REMOTE_PROPERTY_CLEAR,
} Eon_Ecore_Remote_Message;

typedef struct _Eon_Ecore_Remote_Client_New
{
	unsigned int width;
	unsigned int height;
	unsigned int layout_id;
} Eon_Ecore_Remote_Client_New;

typedef struct _Eon_Ecore_Remote_Element_New
{
	unsigned int id;
	const char *name;
} Eon_Ecore_Remote_Element_New;

typedef struct _Eon_Ecore_Remote_Property_Set
{
	unsigned int id;
	const char *name;
	Eon_Ecore_Remote_Data *value;
} Eon_Ecore_Remote_Property_Set;

typedef struct _Eon_Ecore_Remote_Property_Add
{
	unsigned int id;
	const char *name;
	Eon_Ecore_Remote_Data *value;
} Eon_Ecore_Remote_Property_Add;

typedef struct _Eon_Ecore_Remote_Property_Remove
{
	unsigned int id;
	const char *name;
	Eon_Ecore_Remote_Data *value;
} Eon_Ecore_Remote_Property_Remove;

typedef struct _Eon_Ecore_Remote_Property_Clear
{
	unsigned int id;
	const char *name;
} Eon_Ecore_Remote_Property_Clear;

EAPI void eon_ecore_remote_init(void);
EAPI Eon_Backend * eon_ecore_remote_new(void);
EAPI void eon_ecore_remote_server_setup(void *srv);
EAPI Eina_Bool eon_ecore_remote_data_from_value(Eon_Ecore_Remote_Data *data, const Ender_Value *v);

#endif

