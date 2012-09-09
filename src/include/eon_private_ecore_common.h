#ifndef _EON_ECORE_PRIVATE_H
#define _EON_ECORE_PRIVATE_H

typedef struct _Eon_Ecore_Backend_Descriptor
{
	/* window related functions */
	Eon_Backend_Window_New window_new;
	Eon_Backend_Window_Delete window_delete;
	/* element related functions */
	Eon_Backend_Element_Add element_add;
	Eon_Backend_Element_Remove element_remove;
} Eon_Ecore_Backend_Descriptor;

Eon_Backend * eon_ecore_backend_new(Eon_Ecore_Backend_Descriptor *descriptor,
		void *data);

#endif

