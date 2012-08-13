#ifndef _EON_PRIVATE_LAYOUT_H
#define _EON_PRIVATE_LAYOUT_H

/* common functions for the possible descriptors */
typedef void (*Eon_Layout_Child_Foreach_Cb)(void *ref, void *child, void *data);
typedef void (*Eon_Layout_Child_Foreach)(void *ref, Eon_Layout_Child_Foreach_Cb cb, void *data);
typedef void (*Eon_Layout_Child_Geometry_Set)(void *ref, void *child, Eon_Geometry *g);
typedef void (*Eon_Layout_Child_Hints_Get)(void *ref, void *child, Eon_Size *min, Eon_Size *max, Eon_Size *preferred);

/* the layout interface definition */
typedef void (*Eon_Layout_Hints_Get)(void *descriptor, void *data, Eon_Size *min, Eon_Size *max, Eon_Size *preferred);
typedef void (*Eon_Layout_Geometry_Set)(void *descriptor, void *data, Eon_Geometry *g);

typedef struct _Eon_Layout
{
	Eon_Layout_Geometry_Set geometry_set;
	Eon_Layout_Hints_Get hints_get;
} Eon_Layout;

/* useful struct in case you want to add layouts into other layouts */
typedef struct _Eon_Layout_Child_Data
{
	Eon_Layout *layout;
	void *descriptor;
} Eon_Layout_Child_Data;

static inline void eon_layout_geometry_set(Eon_Layout *thiz, void *descriptor, void *data, Eon_Geometry *g)
{
	thiz->geometry_set(descriptor, data, g);
}

static inline void eon_layout_hints_get(Eon_Layout *thiz, void *descriptor, void *data, Eon_Size *min, Eon_Size *max, Eon_Size *preferred)
{
	thiz->hints_get(descriptor, data, min, max, preferred);
}

#endif
