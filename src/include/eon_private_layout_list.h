#ifndef _EON_PRIVATE_LAYOUT_LIST_H
#define _EON_PRIVATE_LAYOUT_LIST_H

typedef Eon_Orientation (*Eon_Layout_List_Orientation_Get)(void *ref);
typedef Eina_Bool (*Eon_Layout_List_Is_Homogeneous)(void *ref);
typedef void (*Eon_Layout_List_Min_Length_Set)(void *ref, double min);
typedef void (*Eon_Layout_List_Min_Length_Get)(void *ref, double *min);;
typedef int (*Eon_Layout_List_Child_Weight_Get)(void *ref, void *child);

typedef int (*Eon_Layout_List_Child_Count_Get)(void *ref);
typedef void (*Eon_Layout_List_Child_Count_Set)(void *ref, int count);

typedef struct _Eon_Layout_List_Descriptor
{
	Eon_Layout_List_Orientation_Get orientation_get;
	Eon_Layout_List_Child_Count_Get child_count_get;
	Eon_Layout_List_Child_Count_Set child_count_set;
	Eon_Layout_Child_Foreach child_foreach;
	Eon_Layout_Child_Hints_Get child_hints_get;
	Eon_Layout_Child_Geometry_Set child_geometry_set;
} Eon_Layout_List_Descriptor;

extern Eon_Layout eon_layout_list;

#endif

