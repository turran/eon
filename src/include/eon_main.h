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

typedef enum _Eon_Orientation
{
	EON_ORIENTATION_HORIZONTAL,
	EON_ORIENTATION_VERTICAL,
} Eon_Orientation;

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


