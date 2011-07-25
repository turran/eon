#ifndef _EON_ECORE_REMOTE_H_
#define _EON_ECORE_REMOTE_H_

typedef enum _Eon_Ecore_Remote_Data_Type
{
	EON_ECORE_REMOTE_DATA_UINT32,
	EON_ECORE_REMOTE_DATA_DOUBLE,
	EON_ECORE_REMOTE_DATA_PTR,
	EON_ECORE_REMOTE_DATA_TYPES
} Eon_Ecore_Remote_Data_Type;

typedef union _Eon_Ecore_Remote_Data_Value
{
	uint32_t u32;
	double d;
	void *ptr;
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

typedef struct _Eon_Ecore_Remote_Client_Size
{
	unsigned int width;
	unsigned int height;
} Eon_Ecore_Remote_Client_Size;

typedef struct _Eon_Ecore_Remote_Element_New
{
	unsigned int id;
	const char *name;
} Eon_Ecore_Remote_Element_New;

typedef struct _Eon_Ecore_Remote_Property_Set
{
	unsigned int id;
	const char *name;
	Ender_Value *value;
} Eon_Ecore_Remote_Property_Set;

typedef struct _Eon_Ecore_Remote_Property_Add
{
	unsigned int id;
	const char *name;
	Ender_Value *value;
} Eon_Ecore_Remote_Property_Add;

typedef struct _Eon_Ecore_Remote_Property_Remove
{
	unsigned int id;
	const char *name;
	Ender_Value *value;
} Eon_Ecore_Remote_Property_Remove;

typedef struct _Eon_Ecore_Remote_Property_Clear
{
	unsigned int id;
	const char *name;
} Eon_Ecore_Remote_Property_Clear;

EAPI void eon_ecore_remote_init(void);
EAPI Eon_Backend * eon_ecore_remote_new(void);
EAPI void eon_ecore_remote_server_setup(void *srv);

#endif

