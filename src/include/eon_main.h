#ifndef _EON_MAIN_H_
#define _EON_MAIN_H_

typedef struct _Eon_Margin
{
	double left;
	double top;
	double right;
	double bottom;
} Eon_Margin;

typedef struct _Eon_Position
{
	double x;
	double y;
} Eon_Position;

typedef struct _Eon_Size
{
	double width;
	double height;
} Eon_Size;

typedef struct _Eon_Geometry
{
	double x;
	double y;
	double width;
	double height;
} Eon_Geometry;

typedef enum _Eon_Direction
{
	EON_DIRECTION_HORIZONTAL,
	EON_DIRECTION_VERTICAL,
} Eon_Direction;

typedef enum _Eon_Horizontal_Alignment
{
	EON_HORIZONTAL_ALIGNMENT_LEFT,
	EON_HORIZONTAL_ALIGNMENT_RIGHT,
	EON_HORIZONTAL_ALIGNMENT_CENTER,
} Eon_Horizontal_Alignment;

typedef enum _Eon_Vertical_Alignment
{
	EON_VERTICAL_ALIGNMENT_TOP,
	EON_VERTICAL_ALIGNMENT_BOTTOM,
	EON_VERTICAL_ALIGNMENT_CENTER,
} Eon_Vertical_Alignment;


static inline void eon_size_values_set(Eon_Size *thiz, double w, double h)
{
	thiz->width = w;
	thiz->height = h;
}

static inline void eon_size_values_get(Eon_Size *thiz, double *w, double *h)
{
	if (w) *w = thiz->width;
	if (h) *h = thiz->height;
}

static inline void eon_geometry_coords_from(Eon_Geometry *g, double x, double y, double w, double h)
{
	g->x = x;
	g->y = y;
	g->width = w;
	g->height = h;
}

/**
 * @{
 */

EAPI int eon_init(void);
EAPI void eon_shutdown(void);
EAPI void eon_version(unsigned int *major, unsigned int *minor, unsigned int *micro);

/**
 * @}
 */

#endif


