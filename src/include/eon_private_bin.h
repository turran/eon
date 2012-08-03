#ifndef _EON_PRIVATE_BIN_H
#define _EON_PRIVATE_BIN_H

typedef struct _Eon_Bin_State
{
	Ender_Element *content;
} Eon_Bin_State;

typedef Eina_Bool (*Eon_Bin_Setup)(Ender_Element *e,
		const Eon_Element_State *state,
		const Eon_Bin_State *cstate,
		Enesim_Surface *s, Enesim_Error **error);
typedef double (*Eon_Bin_Min_Width_Get)(Ender_Element *e, double cmw);
typedef double (*Eon_Bin_Max_Width_Get)(Ender_Element *e, double cmw);
typedef double (*Eon_Bin_Min_Height_Get)(Ender_Element *e, double cmh);
typedef double (*Eon_Bin_Max_Height_Get)(Ender_Element *e, double cmh);
typedef double (*Eon_Bin_Preferred_Height_Get)(Ender_Element *e, double cmh);
typedef double (*Eon_Bin_Preferred_Width_Get)(Ender_Element *e, double cmw);
typedef Ender_Element * (*Eon_Bin_Element_At)(Ender_Element *e, double x, double y);

typedef struct _Eon_Bin_Descriptor
{
	Eon_Bin_Setup setup;
	Eon_Bin_Min_Width_Get min_width_get;
	Eon_Bin_Max_Width_Get max_width_get;
	Eon_Bin_Min_Height_Get min_height_get;
	Eon_Bin_Max_Height_Get max_height_get;
	Eon_Bin_Preferred_Width_Get preferred_width_get;
	Eon_Bin_Preferred_Height_Get preferred_height_get;
	Eon_Bin_Element_At element_at;
	Eon_Element_Initialize initialize;
	Eon_Element_Needs_Setup needs_setup;
	Eon_Element_Free free;
	const char *name;
	Eina_Bool pass_events;
} Eon_Bin_Descriptor;

Eon_Element * eon_container_new(Eon_Bin_Descriptor *descriptor, void *data);
void * eon_container_data_get(Eon_Element *ee);

#endif
