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


