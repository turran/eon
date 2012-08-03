#ifndef _EON_PRIVATE_LAYOUT_FRAME_H
#define _EON_PRIVATE_LAYOUT_FRAME_H

typedef void (*Eon_Layout_Frame_Child_Padding_Get)(void *ref, void *child, Eon_Margin *margin);

typedef struct _Eon_Layout_Frame_Descriptor
{
	Eon_Layout_Frame_Child_Padding_Get child_padding_get;
	Eon_Layout_Child_Foreach child_foreach;
	Eon_Layout_Child_Hints_Get child_hints_get;
	Eon_Layout_Child_Geometry_Set child_geometry_set;
} Eon_Layout_Frame_Descriptor;

extern Eon_Layout eon_layout_frame;

#endif
