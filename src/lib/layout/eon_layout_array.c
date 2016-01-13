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

#include "eon_renderable_private.h"
#include "eon_layout_private.h"
#include "eon_layout_array_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LAYOUT_ARRAY_DESCRIPTOR eon_layout_array_descriptor_get()
#define EON_LAYOUT_ARRAY(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Layout_Array, EON_LAYOUT_ARRAY_DESCRIPTOR)

typedef struct _Eon_Layout_Array
{
	Eon_Layout base;
	/* properties */
	int spacing;
	Eon_Box padding;
	Eon_Orientation orientation;
	/* the external interface */
	Eon_Layout_Array_Descriptor *descriptor;
	void *data;
} Eon_Layout_Array;

typedef struct _Eon_Layout_Array_Class
{
	Eon_Layout_Class base;
} Eon_Layout_Array_Class;
/*----------------------------------------------------------------------------*
 *                              Layout interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_layout_array_size_hints_get(Eon_Layout *l, Eon_Renderable_Size *size)
{

}

static void _eon_layout_array_geometry_set(Eon_Layout *l, Eina_Rectangle *area)
{

}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_LAYOUT_DESCRIPTOR,
		Eon_Layout_Array, Eon_Layout_Array_Class,
		eon_layout_array);

static void _eon_layout_array_class_init(void *k)
{
	Eon_Layout_Class *klass;

	klass = EON_LAYOUT_CLASS(k);
	klass->size_hints_get = _eon_layout_array_size_hints_get;
	klass->geometry_set = _eon_layout_array_geometry_set;
}

static void _eon_layout_array_instance_init(void *o)
{
}

static void _eon_layout_array_instance_deinit(void *o)
{
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Layout * eon_layout_array_new(Eon_Layout_Array_Descriptor *d, void *data)
{
	Eon_Layout_Array *thiz;
	Eon_Layout *l;

	l = ENESIM_OBJECT_INSTANCE_NEW(eon_layout_array);
	thiz = EON_LAYOUT_ARRAY(l);
	thiz->descriptor = d;
	thiz->data = data;

	return l;
}

void eon_layout_array_inner_spacing_set(Eon_Layout *l, int spacing)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->spacing = spacing;
}

void eon_layout_array_padding_set(Eon_Layout *l, Eon_Box *padding)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->padding = *padding;
}

void eon_layout_array_orientation_set(Eon_Layout *l, Eon_Orientation orientation)
{
	Eon_Layout_Array *thiz;

	thiz = EON_LAYOUT_ARRAY(l);
	thiz->orientation = orientation;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

