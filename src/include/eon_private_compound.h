#ifndef _EON_COMPOUND_PRIVATE_H
#define _EON_COMPOUND_PRIVATE_H

typedef struct _Eon_Compound_Descriptor
{
	Eon_Element_Initialize initialize;
	Eon_Element_Free free;
	const char *name;
} Eon_Compound_Descriptor;

Eon_Element * eon_compound_new(Eon_Compound_Descriptor *descriptor,
		Ender_Element *relative,
		void *data);
void * eon_compound_data_get(Eon_Element *ee);

#endif
