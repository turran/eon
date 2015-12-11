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
#include "eon_theme_name.h"

#include "eon_theme_name_private.h"
#include "eon_value_theme_name_private.h"
#include "eon_attr_theme_name_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Attr_Theme_Name
{
	Eon_Theme_Name styled;
	Eon_Theme_Name value;
	Eon_Theme_Name def;
} Eon_Attr_Theme_Name;
/*----------------------------------------------------------------------------*
 *                              Attr interface                                *
 *----------------------------------------------------------------------------*/
static void * _eon_attr_theme_name_init(Egueb_Dom_Node *node)
{
	Eon_Attr_Theme_Name *thiz;

	thiz = calloc(1, sizeof(Eon_Attr_Theme_Name));
	return thiz;
}

static void _eon_attr_theme_name_deinit(Egueb_Dom_Node *node, void *data)
{
	Eon_Attr_Theme_Name *thiz = data;

	eon_theme_name_reset(&thiz->styled);
	eon_theme_name_reset(&thiz->value);
	eon_theme_name_reset(&thiz->def);
	free(thiz);
}

static Eina_Bool _eon_attr_theme_name_value_get(Egueb_Dom_Node *n, void *data,
		Egueb_Dom_Attr_Type type, Egueb_Dom_Value *value)
{
	Eon_Attr_Theme_Name *thiz = data;

	switch (type)
	{
		default:
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

static Eina_Bool _eon_attr_theme_name_value_set(Egueb_Dom_Node *n, void *data,
		Egueb_Dom_Attr_Type type, Egueb_Dom_Value *value)
{
	Eon_Attr_Theme_Name *thiz = data;

	switch (type)
	{
		default:
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

static const Egueb_Dom_Value_Descriptor * _eon_attr_theme_name_value_descriptor_get(
		Egueb_Dom_Node *n, void *data)
{
	return eon_value_theme_name_descriptor_get();
}

static Egueb_Dom_Attr_External_Descriptor _descriptor = {
	/* .version			= */ EGUEB_DOM_ATTR_EXTERNAL_DESCRIPTOR_VERSION,
	/* .init			= */ _eon_attr_theme_name_init,
	/* .deinit			= */ _eon_attr_theme_name_deinit,
	/* .value_descriptor_get 	= */ _eon_attr_theme_name_value_descriptor_get,
	/* .value_get 			= */ _eon_attr_theme_name_value_get,
	/* .value_set 			= */ _eon_attr_theme_name_value_set
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_attr_theme_name_new(void)
{
	Egueb_Dom_Node *n;

	n = egueb_dom_attr_external_new(&_descriptor);
	egueb_dom_attr_init(n, egueb_dom_string_ref(EON_NAME_ATTR_THEME_NAME), NULL,
			EINA_FALSE, EINA_TRUE, EINA_TRUE);
	egueb_dom_attr_inherit(n, EGUEB_DOM_ATTR_TYPE_DEFAULT);

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
