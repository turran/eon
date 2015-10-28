/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2014 Jorge Luis Zapata
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
#include "eon_document.h"

#include "eon_main_private.h"

#include "eon_theme_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#if BUILD_STATIC_MODULE_DRAWER_BASIC
Eina_Bool eon_drawer_basic_init(void);
void eon_drawer_basic_shutdown(void);
#endif

static Egueb_Dom_String * _eon_mime;
static int _init_count = 0;

/*----------------------------------------------------------------------------*
 *                      Implementation interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_List * _impl_mime_get(void)
{
	Egueb_Dom_List *ret;

	ret = egueb_dom_list_new(egueb_dom_value_dom_string_descriptor_get());
	egueb_dom_list_item_append(ret, egueb_dom_string_ref(_eon_mime));
	return ret;
}

static Egueb_Dom_Node * _impl_document_create(void)
{
	return eon_document_new();
}

static Egueb_Dom_Implementation_Descriptor _impl_descriptor = {
	/* .version		= */ EGUEB_DOM_IMPLEMENTATION_DESCRIPTOR_VERSION,
	/* .mime_get 		= */ _impl_mime_get,
	/* .document_create 	= */ _impl_document_create,
};

static void _strings_init(void)
{
	/* mime */
	_eon_mime = egueb_dom_string_new_with_static_string("application/eon+xml");
	/* attributes */
	EON_NAME_ID = egueb_dom_string_new_with_static_string("id");
	EON_MIN_WIDTH = egueb_dom_string_new_with_static_string("min-width");
	EON_MIN_HEIGHT = egueb_dom_string_new_with_static_string("min-height");
	EON_MAX_WIDTH = egueb_dom_string_new_with_static_string("max-width");
	EON_MAX_HEIGHT = egueb_dom_string_new_with_static_string("max-height");
	EON_NAME_HEXPAND = egueb_dom_string_new_with_static_string("hexpand");
	EON_NAME_VEXPAND = egueb_dom_string_new_with_static_string("vexpand");
	EON_THEME = egueb_dom_string_new_with_static_string("theme");
	EON_NAME_THEME_ID = egueb_dom_string_new_with_static_string("theme-id");
	EON_ATTR_ELLIPSIZE = egueb_dom_string_new_with_static_string("ellipsize");
	EON_ATTR_VALIGN = egueb_dom_string_new_with_static_string("valign");
	EON_ATTR_HALIGN = egueb_dom_string_new_with_static_string("halign");
	EON_ATTR_WEIGHT = egueb_dom_string_new_with_static_string("weight");
	EON_ATTR_ORIENTATION = egueb_dom_string_new_with_static_string("orientation");
	EON_ATTR_ENABLED = egueb_dom_string_new_with_static_string("enabled");
	EON_STOCK = egueb_dom_string_new_with_static_string("stock");
	/* elements */
	EON_NAME_ELEMENT_EON = egueb_dom_string_new_with_static_string("eon");
	EON_NAME_ELEMENT_BUTTON = egueb_dom_string_new_with_static_string("button");
	EON_NAME_ELEMENT_BUTTON_STOCK = egueb_dom_string_new_with_static_string("stockButton");
	EON_ELEMENT_LABEL = egueb_dom_string_new_with_static_string("label");
	EON_ELEMENT_LABEL_STOCK = egueb_dom_string_new_with_static_string("stockLabel");
	EON_ELEMENT_STACK = egueb_dom_string_new_with_static_string("stack");
	EON_ELEMENT_OBJECT = egueb_dom_string_new_with_static_string("object");
	/* events */
	EON_EVENT_GEOMETRY_REQUEST = egueb_dom_string_new_with_static_string("EONGeometryRequest");
	EON_EVENT_GEOMETRY_INVALIDATE = egueb_dom_string_new_with_static_string("EONGeometryInvalidate");
}

static void _strings_shutdown(void)
{
	/* mime */
	egueb_dom_string_unref(_eon_mime);
	/* attributes */
	egueb_dom_string_unref(EON_NAME_ID);
	egueb_dom_string_unref(EON_MIN_WIDTH);
	egueb_dom_string_unref(EON_MIN_HEIGHT);
	egueb_dom_string_unref(EON_MAX_WIDTH);
	egueb_dom_string_unref(EON_MAX_HEIGHT);
	egueb_dom_string_unref(EON_NAME_HEXPAND);
	egueb_dom_string_unref(EON_NAME_VEXPAND);
	egueb_dom_string_unref(EON_THEME);
	egueb_dom_string_unref(EON_NAME_THEME_ID);
	egueb_dom_string_unref(EON_ATTR_ELLIPSIZE);
	egueb_dom_string_unref(EON_ATTR_VALIGN);
	egueb_dom_string_unref(EON_ATTR_HALIGN);
	egueb_dom_string_unref(EON_ATTR_WEIGHT);
	egueb_dom_string_unref(EON_ATTR_ORIENTATION);
	egueb_dom_string_unref(EON_ATTR_ENABLED);
	egueb_dom_string_unref(EON_STOCK);
	/* elements */
	egueb_dom_string_unref(EON_NAME_ELEMENT_EON);
	egueb_dom_string_unref(EON_NAME_ELEMENT_BUTTON);
	egueb_dom_string_unref(EON_NAME_ELEMENT_BUTTON_STOCK);
	egueb_dom_string_unref(EON_ELEMENT_LABEL);
	egueb_dom_string_unref(EON_ELEMENT_LABEL_STOCK);
	egueb_dom_string_unref(EON_ELEMENT_STACK);
	egueb_dom_string_unref(EON_ELEMENT_OBJECT);
	/* events */
	egueb_dom_string_unref(EON_EVENT_GEOMETRY_REQUEST);
	egueb_dom_string_unref(EON_EVENT_GEOMETRY_INVALIDATE);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_main_log = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* attributes */
Egueb_Dom_String *EON_NAME_ID;
Egueb_Dom_String *EON_MIN_WIDTH;
Egueb_Dom_String *EON_MIN_HEIGHT;
Egueb_Dom_String *EON_MAX_WIDTH;
Egueb_Dom_String *EON_MAX_HEIGHT;
Egueb_Dom_String *EON_NAME_HEXPAND;
Egueb_Dom_String *EON_NAME_VEXPAND;
Egueb_Dom_String *EON_THEME;
Egueb_Dom_String *EON_NAME_THEME_ID;
Egueb_Dom_String *EON_ATTR_ELLIPSIZE;
Egueb_Dom_String *EON_ATTR_VALIGN;
Egueb_Dom_String *EON_ATTR_HALIGN;
Egueb_Dom_String *EON_ATTR_WEIGHT;
Egueb_Dom_String *EON_ATTR_ORIENTATION;
Egueb_Dom_String *EON_ATTR_ENABLED;
Egueb_Dom_String *EON_STOCK;
/* elements */
Egueb_Dom_String *EON_NAME_ELEMENT_EON;
Egueb_Dom_String *EON_NAME_ELEMENT_BUTTON;
Egueb_Dom_String *EON_NAME_ELEMENT_BUTTON_STOCK;
Egueb_Dom_String *EON_ELEMENT_LABEL;
Egueb_Dom_String *EON_ELEMENT_LABEL_STOCK;
Egueb_Dom_String *EON_ELEMENT_STACK;
Egueb_Dom_String *EON_ELEMENT_OBJECT;
/* events */
Egueb_Dom_String *EON_EVENT_GEOMETRY_REQUEST;
Egueb_Dom_String *EON_EVENT_GEOMETRY_INVALIDATE;

/**
 *
 */
EAPI int eon_init(void)
{
	_init_count++;
	if (_init_count == 1)
	{
		Egueb_Dom_Implementation *i;

		eina_init();
		eon_main_log = eina_log_domain_register("eon", NULL);
		egueb_dom_init();
		egueb_smil_init();
		eon_theme_init();
		_strings_init();
#if BUILD_STATIC_MODULE_DRAWER_BASIC
		eon_drawer_basic_init();
#endif
		/* register our own source */
		i = egueb_dom_implementation_new(&_impl_descriptor);
		egueb_dom_registry_implementation_add(i);
	}
	return _init_count;
}

/**
 *
 */
EAPI int eon_shutdown(void)
{
	if (--_init_count != 0)
		return _init_count;

#if BUILD_STATIC_MODULE_DRAWER_BASIC
	eon_drawer_basic_shutdown();
#endif
	_strings_shutdown();
	eon_theme_shutdown();
	egueb_smil_shutdown();
	egueb_dom_shutdown();
	eina_log_domain_unregister(eon_main_log);
	eon_main_log = -1;
	eina_shutdown();
	return _init_count;
}

/**
 *
 */
EAPI void eon_version(unsigned int *major, unsigned int *minor, unsigned int *micro)
{
	if (major) *major = VERSION_MAJOR;
	if (minor) *minor = VERSION_MINOR;
	if (micro) *micro = VERSION_MICRO;
}
