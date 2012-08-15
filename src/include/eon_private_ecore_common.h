#ifndef _EON_ECORE_PRIVATE_H
#define _EON_ECORE_PRIVATE_H

typedef struct _Eon_Ecore_Backend_Descriptor
{
	/* window related functions */
	Eon_Backend_Window_New window_new;
	Eon_Backend_Window_Delete window_delete;
} Eon_Ecore_Backend_Descriptor;

#endif

