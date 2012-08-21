#ifndef _EON_PRIVATE_LAYOUT_GRID_H
#define _EON_PRIVATE_LAYOUT_GRID_H

typedef int (*Eon_Layout_Grid_Num_Columns_Get)(void *ref);

typedef void (*Eon_Layout_Grid_Rows_Columns_Get)(void *ref, int *rows, int *cols);
typedef void (*Eon_Layout_Grid_Rows_Columns_Set)(void *ref, int rows, int cols);

typedef struct _Eon_Layout_Grid_Descriptor
{
	Eon_Layout_Grid_Rows_Columns_Get rows_columns_get;
	Eon_Layout_Grid_Rows_Columns_Set rows_columns_set;
	Eon_Layout_Grid_Num_Columns_Get num_columns_get;
	Eon_Layout_Child_Foreach child_foreach;
	Eon_Layout_Child_Hints_Get child_hints_get;
	Eon_Layout_Child_Geometry_Set child_geometry_set;
} Eon_Layout_Grid_Descriptor;

extern Eon_Layout eon_layout_grid;

#endif


