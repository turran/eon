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
#include "eon_theme_private.h"
#include "eon_theme_main_private.h"
#include "eon_theme_widget_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_THEME_RENDERABLE_DESCRIPTOR, Eon_Theme_Widget,
		Eon_Theme_Widget_Class, eon_theme_widget);

static void _eon_theme_widget_class_init(void *k)
{
}

static void _eon_theme_widget_instance_init(void *o)
{
}

static void _eon_theme_widget_instance_deinit(void *o)
{
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_widget_enabled_set(Egueb_Dom_Node *n, Eina_Bool enabled)
{
	Eon_Theme_Widget *thiz;

	thiz = EON_THEME_WIDGET(egueb_dom_element_external_data_get(n));
	thiz->enabled = enabled;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eina_Bool eon_theme_widget_enabled_get(Egueb_Dom_Node *n)
{
	Eon_Theme_Widget *thiz;

	thiz = EON_THEME_WIDGET(egueb_dom_element_external_data_get(n));
	return thiz->enabled;
}
