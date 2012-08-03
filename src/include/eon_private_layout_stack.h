#ifndef _EON_PRIVATE_LAYOUT_STACK_H
#define _EON_PRIVATE_LAYOUT_STACK_H

typedef Eon_Vertical_Alignment (*Eon_Layout_Stack_Child_Vertical_Alignment_Get)(void *ref, void *child);
typedef Eon_Horizontal_Alignment (*Eon_Layout_Stack_Child_Horizontal_Alignment_Get)(void *ref, void *child);

typedef struct _Eon_Layout_Stack_Descriptor
{
	Eon_Layout_Stack_Child_Vertical_Alignment_Get vertical_alignment_get;
	Eon_Layout_Stack_Child_Horizontal_Alignment_Get horizontal_alignment_get;
	Eon_Layout_Child_Foreach child_foreach;
	Eon_Layout_Child_Hints_Get child_hints_get;
	Eon_Layout_Child_Geometry_Set child_geometry_set;
} Eon_Layout_Stack_Descriptor;

extern Eon_Layout eon_layout_stack;

#endif

