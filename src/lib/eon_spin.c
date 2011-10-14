/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
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
#include "Eon.h"
#include "eon_private.h"
/* the etex span always strdup the string, we should add a way to share
 * a strbuffer between the internal state of the span and the API this way
 * we avoid useless copies
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Spin
{
	/* properties */
	double min;
	double max;
	double step_increment;
	/* private */
} Eon_Spin;
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
#include "eon_generated_spin.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_spin_new(void)
{
	return ender_element_new_with_namespace("spin", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_sping_max_range_set(Ender_Element *e, double max_range)
{
	ender_element_value_set(e, "max_range", max_range, NULL);

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_sping_max_range_get(Ender_Element *e, double *max_range)
{
	ender_element_value_get(e, "max_range", max_range, NULL);

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_sping_min_range_set(Ender_Element *e, double min_range)
{
	ender_element_value_set(e, "min_range", min_range, NULL);

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_sping_min_range_get(Ender_Element *e, double *min_range)
{
	ender_element_value_get(e, "min_range", min_range, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_step_increment_set(Ender_Element *e, double step_increment)
{
	ender_element_value_set(e, "step_increment", step_increment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_scrollbar_step_increment_get(Ender_Element *e, double *step_increment)
{
	ender_element_value_get(e, "step_increment", step_increment, NULL);
}
