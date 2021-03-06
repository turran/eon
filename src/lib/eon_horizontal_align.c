/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2015 Jorge Luis Zapata
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "eon_private.h"
#include "eon_main.h"
/*----------------------------------------------------------------------------*
 *                             Value interface                                *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Value_Descriptor _descriptor;

static void _eon_horizontal_align_data_from(Egueb_Dom_Value *v, Egueb_Dom_Value_Data *data)
{
	EINA_SAFETY_ON_FALSE_RETURN(v->descriptor == &_descriptor);
	egueb_dom_value_i32_data_from(v, data);
}

static void _eon_horizontal_align_data_to(Egueb_Dom_Value *v, Egueb_Dom_Value_Data *data)
{
	EINA_SAFETY_ON_FALSE_RETURN(v->descriptor == &_descriptor);
	egueb_dom_value_i32_data_to(v, data);
}

static char * _eon_horizontal_align_string_to(const Egueb_Dom_Value *v)
{
	EINA_SAFETY_ON_FALSE_RETURN_VAL(v->descriptor == &_descriptor, NULL);
	switch (v->data.i32)
	{
		case EON_HORIZONTAL_ALIGN_LEFT:
		return strdup("left");

		case EON_HORIZONTAL_ALIGN_CENTER:
		return strdup("center");

		case EON_HORIZONTAL_ALIGN_RIGHT:
		return strdup("right");

		default:
		return NULL;
	}
}

static Eina_Bool _eon_horizontal_align_string_from(Egueb_Dom_Value *v, const char *str)
{
	EINA_SAFETY_ON_FALSE_RETURN_VAL(v->descriptor == &_descriptor, EINA_FALSE);
	if (!strcmp(str, "left"))
		v->data.i32 = EON_HORIZONTAL_ALIGN_LEFT;
	else if (!strcmp(str, "center"))
		v->data.i32 = EON_HORIZONTAL_ALIGN_CENTER;
	else if (!strcmp(str, "right"))
		v->data.i32 = EON_HORIZONTAL_ALIGN_RIGHT;
	else
		return EINA_FALSE;
	return EINA_TRUE;
}

static void _eon_horizontal_align_interpolate(Egueb_Dom_Value *v,
		Egueb_Dom_Value *a, Egueb_Dom_Value *b, double m,
		Egueb_Dom_Value *add, Egueb_Dom_Value *acc, int mul)
{
	egueb_dom_value_enum_interpolate(v, a, b, m, add, acc, mul);
}

static Egueb_Dom_Value_Descriptor _descriptor = {
	/* .data_from 		= */ _eon_horizontal_align_data_from,
	/* .data_from_type 	= */ EGUEB_DOM_VALUE_DATA_TYPE_INT32,
	/* .data_to 		= */ _eon_horizontal_align_data_to,
	/* .data_to_type 	= */ EGUEB_DOM_VALUE_DATA_TYPE_PTR,
	/* .init 		= */ NULL,
	/* .free 		= */ NULL,
	/* .copy 		= */ NULL,
	/* .string_to 		= */ _eon_horizontal_align_string_to,
	/* .string_from 	= */ _eon_horizontal_align_string_from,
	/* .interpolate 	= */ _eon_horizontal_align_interpolate,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_horizontal_align_attr_new(void)
{
	return egueb_dom_attr_enum_new(egueb_dom_string_ref(EON_NAME_ATTR_HALIGN),
			EINA_FALSE, EINA_FALSE, EINA_FALSE,
			&_descriptor);
}

