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
static Egueb_Dom_Node * _impl_document_create(void)
{
	return eon_document_new();
}

static Egueb_Dom_Implementation_Descriptor _impl_descriptor = {
	/* .document_create 	= */ _impl_document_create,
};

/*----------------------------------------------------------------------------*
 *                    Implementation source interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Implementation * _impl_source_implementation_get(void)
{
	return egueb_dom_implementation_new(&_impl_descriptor);
}

static Egueb_Dom_String * _impl_source_mime_get(void)
{
	return egueb_dom_string_ref(_eon_mime);
}

static Egueb_Dom_Implementation_Source_Descriptor _impl_source_descriptor = {
	/* .implementation_get 	= */ _impl_source_implementation_get,
	/* .mime_get 		= */ _impl_source_mime_get,
};

static void _strings_init(void)
{
	/* mime */
	_eon_mime = egueb_dom_string_new_with_static_string("application/eon+xml");
	/* attributes */
	EON_MIN_WIDTH = egueb_dom_string_new_with_static_string("min-width");
	EON_MIN_HEIGHT = egueb_dom_string_new_with_static_string("min-height");
	EON_MAX_WIDTH = egueb_dom_string_new_with_static_string("max-width");
	EON_MAX_HEIGHT = egueb_dom_string_new_with_static_string("max-height");
	EON_WIDTH = egueb_dom_string_new_with_static_string("width");
	EON_HEIGHT = egueb_dom_string_new_with_static_string("height");
	EON_THEME = egueb_dom_string_new_with_static_string("theme");
	EON_ATTR_ELLIPSIZE = egueb_dom_string_new_with_static_string("ellipsize");
	EON_ATTR_VALIGN = egueb_dom_string_new_with_static_string("valign");
	EON_ATTR_HALIGN = egueb_dom_string_new_with_static_string("halign");
	EON_ATTR_WEIGHT = egueb_dom_string_new_with_static_string("weight");
	EON_ATTR_ORIENTATION = egueb_dom_string_new_with_static_string("orientation");
	EON_ATTR_ENABLED = egueb_dom_string_new_with_static_string("enabled");
	EON_STOCK = egueb_dom_string_new_with_static_string("stock");
	/* elements */
	EON_ELEMENT_EON = egueb_dom_string_new_with_static_string("eon");
	EON_ELEMENT_BUTTON = egueb_dom_string_new_with_static_string("button");
	EON_ELEMENT_BUTTON_STOCK = egueb_dom_string_new_with_static_string("stockButton");
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
	egueb_dom_string_unref(EON_MIN_WIDTH);
	egueb_dom_string_unref(EON_MIN_HEIGHT);
	egueb_dom_string_unref(EON_MAX_WIDTH);
	egueb_dom_string_unref(EON_MAX_HEIGHT);
	egueb_dom_string_unref(EON_WIDTH);
	egueb_dom_string_unref(EON_HEIGHT);
	egueb_dom_string_unref(EON_THEME);
	egueb_dom_string_unref(EON_ATTR_ELLIPSIZE);
	egueb_dom_string_unref(EON_ATTR_VALIGN);
	egueb_dom_string_unref(EON_ATTR_HALIGN);
	egueb_dom_string_unref(EON_ATTR_WEIGHT);
	egueb_dom_string_unref(EON_ATTR_ORIENTATION);
	egueb_dom_string_unref(EON_ATTR_ENABLED);
	egueb_dom_string_unref(EON_STOCK);
	/* elements */
	egueb_dom_string_unref(EON_ELEMENT_EON);
	egueb_dom_string_unref(EON_ELEMENT_BUTTON);
	egueb_dom_string_unref(EON_ELEMENT_BUTTON_STOCK);
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
Egueb_Dom_String *EON_MIN_WIDTH;
Egueb_Dom_String *EON_MIN_HEIGHT;
Egueb_Dom_String *EON_MAX_WIDTH;
Egueb_Dom_String *EON_MAX_HEIGHT;
Egueb_Dom_String *EON_WIDTH;
Egueb_Dom_String *EON_HEIGHT;
Egueb_Dom_String *EON_THEME;
Egueb_Dom_String *EON_ATTR_ELLIPSIZE;
Egueb_Dom_String *EON_ATTR_VALIGN;
Egueb_Dom_String *EON_ATTR_HALIGN;
Egueb_Dom_String *EON_ATTR_WEIGHT;
Egueb_Dom_String *EON_ATTR_ORIENTATION;
Egueb_Dom_String *EON_ATTR_ENABLED;
Egueb_Dom_String *EON_STOCK;
/* elements */
Egueb_Dom_String *EON_ELEMENT_EON;
Egueb_Dom_String *EON_ELEMENT_BUTTON;
Egueb_Dom_String *EON_ELEMENT_BUTTON_STOCK;
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
	Egueb_Dom_Implementation_Source *s;

	_init_count++;
	if (_init_count == 1)
	{
		eina_init();
		eon_main_log = eina_log_domain_register("eon", NULL);
		ender_init();
		egueb_dom_init();
		_strings_init();
#if BUILD_STATIC_MODULE_DRAWER_BASIC
		eon_drawer_basic_init();
#endif
		/* register our own source */
		s = egueb_dom_implementation_source_new(&_impl_source_descriptor);
		egueb_dom_registry_source_add(s);
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
	egueb_dom_shutdown();
	ender_shutdown();
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
