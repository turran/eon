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

/*
 * A layout must be a generic interface to create layout algorithms.
 */
#include "eon_private.h"
#include "eon_main.h"

#include "eon_renderable_private.h"
#include "eon_layout_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(ENESIM_OBJECT_DESCRIPTOR,
		Eon_Layout, Eon_Layout_Class, eon_layout);

static void _eon_layout_class_init(void *k)
{
}

static void _eon_layout_instance_init(void *o)
{
}

static void _eon_layout_instance_deinit(void *o)
{
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_layout_size_hints_get(Eon_Layout *thiz, Eon_Renderable_Size *size)
{
	Eon_Layout_Class *klass;

	klass = EON_LAYOUT_CLASS_GET(thiz);
	return klass->size_hints_get(thiz, size);
}

void eon_layout_geometry_set(Eon_Layout *thiz, Eina_Rectangle *area)
{
	Eon_Layout_Class *klass;

	klass = EON_LAYOUT_CLASS_GET(thiz);
	klass->geometry_set(thiz, area);
}

void eon_layout_free(Eon_Layout *thiz)
{
	enesim_object_instance_free(ENESIM_OBJECT_INSTANCE(thiz));
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

