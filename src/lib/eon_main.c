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
#include "eon_document.h"

#include "eon_main_private.h"

#include "eon_theme_main_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
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
	EON_NAME_COLOR = egueb_dom_string_new_with_static_string("color");
	EON_NAME_ATTR_THEME_NAME = egueb_dom_string_new_with_static_string("theme-name");
	EON_NAME_THEME_ID = egueb_dom_string_new_with_static_string("theme-id");
	EON_ATTR_ELLIPSIZE = egueb_dom_string_new_with_static_string("ellipsize");
	EON_ATTR_VALIGN = egueb_dom_string_new_with_static_string("valign");
	EON_ATTR_HALIGN = egueb_dom_string_new_with_static_string("halign");
	EON_ATTR_WEIGHT = egueb_dom_string_new_with_static_string("weight");
	EON_ATTR_ORIENTATION = egueb_dom_string_new_with_static_string("orientation");
	EON_ATTR_ENABLED = egueb_dom_string_new_with_static_string("enabled");
	EON_NAME_ATTR_ACTIVE = egueb_dom_string_new_with_static_string("active");
	EON_NAME_ATTR_INACTIVE = egueb_dom_string_new_with_static_string("inactive");
	EON_NAME_ATTR_ACTIVATED = egueb_dom_string_new_with_static_string("activated");
	EON_NAME_ATTR_TITLE = egueb_dom_string_new_with_static_string("title");
	EON_NAME_ATTR_SCALABLE = egueb_dom_string_new_with_static_string("scalable");
	EON_NAME_ATTR_DEFAULT_VALUE = egueb_dom_string_new_with_static_string("default-value");
	EON_NAME_ATTR_PROGRESSION = egueb_dom_string_new_with_static_string("progression");
	EON_STOCK = egueb_dom_string_new_with_static_string("stock");
	EON_NAME_ATTR_EXPANDED = egueb_dom_string_new_with_static_string("expanded");
	/* elements */
	EON_NAME_ELEMENT_EON = egueb_dom_string_new_with_static_string("eon");
	EON_NAME_ELEMENT_BUTTON = egueb_dom_string_new_with_static_string("button");
	EON_NAME_ELEMENT_BUTTON_STOCK = egueb_dom_string_new_with_static_string("stockButton");
	EON_NAME_ELEMENT_BUTTON_TOGGLE = egueb_dom_string_new_with_static_string("toggleButton");
	EON_NAME_ELEMENT_LABEL = egueb_dom_string_new_with_static_string("label");
	EON_NAME_ELEMENT_LABEL_STOCK = egueb_dom_string_new_with_static_string("stockLabel");
	EON_ELEMENT_STACK = egueb_dom_string_new_with_static_string("stack");
	EON_NAME_ELEMENT_OBJECT = egueb_dom_string_new_with_static_string("object");
	EON_NAME_ELEMENT_RADIO = egueb_dom_string_new_with_static_string("radio");
	EON_NAME_ELEMENT_CHECKBOX = egueb_dom_string_new_with_static_string("checkbox");
	EON_NAME_ELEMENT_SWITCH = egueb_dom_string_new_with_static_string("switch");
	EON_NAME_ELEMENT_FRAME = egueb_dom_string_new_with_static_string("frame");
	EON_NAME_ELEMENT_SEPARATOR = egueb_dom_string_new_with_static_string("separator");
	EON_NAME_ELEMENT_IMAGE = egueb_dom_string_new_with_static_string("image");
	EON_NAME_ELEMENT_ENTRY = egueb_dom_string_new_with_static_string("entry");
	EON_NAME_ELEMENT_PROGRESS_BAR = egueb_dom_string_new_with_static_string("progressBar");
	EON_NAME_ELEMENT_PANED = egueb_dom_string_new_with_static_string("paned");
	EON_NAME_ELEMENT_EXPANDER = egueb_dom_string_new_with_static_string("expander");
	/* generic */
	EON_NAME_ON = egueb_dom_string_new_with_static_string("On");
	EON_NAME_OFF = egueb_dom_string_new_with_static_string("Off");
	/* events */
	EON_EVENT_GEOMETRY_REQUEST = egueb_dom_string_new_with_static_string("EONGeometryRequest");
	EON_EVENT_GEOMETRY_INVALIDATE = egueb_dom_string_new_with_static_string("EONGeometryInvalidate");
	EON_NAME_EVENT_ACTIVATE = egueb_dom_string_new_with_static_string("activate");
	EON_NAME_EVENT_DEACTIVATE = egueb_dom_string_new_with_static_string("deactivate");
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
	egueb_dom_string_unref(EON_NAME_COLOR);
	egueb_dom_string_unref(EON_NAME_ATTR_THEME_NAME);
	egueb_dom_string_unref(EON_NAME_THEME_ID);
	egueb_dom_string_unref(EON_ATTR_ELLIPSIZE);
	egueb_dom_string_unref(EON_ATTR_VALIGN);
	egueb_dom_string_unref(EON_ATTR_HALIGN);
	egueb_dom_string_unref(EON_ATTR_WEIGHT);
	egueb_dom_string_unref(EON_ATTR_ORIENTATION);
	egueb_dom_string_unref(EON_ATTR_ENABLED);
	egueb_dom_string_unref(EON_NAME_ATTR_ACTIVE);
	egueb_dom_string_unref(EON_NAME_ATTR_INACTIVE);
	egueb_dom_string_unref(EON_NAME_ATTR_ACTIVATED);
	egueb_dom_string_unref(EON_NAME_ATTR_TITLE);
	egueb_dom_string_unref(EON_NAME_ATTR_SCALABLE);
	egueb_dom_string_unref(EON_NAME_ATTR_DEFAULT_VALUE);
	egueb_dom_string_unref(EON_NAME_ATTR_PROGRESSION);
	egueb_dom_string_unref(EON_STOCK);
	egueb_dom_string_unref(EON_NAME_ATTR_EXPANDED);
	/* elements */
	egueb_dom_string_unref(EON_NAME_ELEMENT_EON);
	egueb_dom_string_unref(EON_NAME_ELEMENT_BUTTON);
	egueb_dom_string_unref(EON_NAME_ELEMENT_BUTTON_STOCK);
	egueb_dom_string_unref(EON_NAME_ELEMENT_BUTTON_TOGGLE);
	egueb_dom_string_unref(EON_NAME_ELEMENT_LABEL);
	egueb_dom_string_unref(EON_NAME_ELEMENT_LABEL_STOCK);
	egueb_dom_string_unref(EON_ELEMENT_STACK);
	egueb_dom_string_unref(EON_NAME_ELEMENT_OBJECT);
	egueb_dom_string_unref(EON_NAME_ELEMENT_RADIO);
	egueb_dom_string_unref(EON_NAME_ELEMENT_CHECKBOX);
	egueb_dom_string_unref(EON_NAME_ELEMENT_SWITCH);
	egueb_dom_string_unref(EON_NAME_ELEMENT_FRAME);
	egueb_dom_string_unref(EON_NAME_ELEMENT_SEPARATOR);
	egueb_dom_string_unref(EON_NAME_ELEMENT_IMAGE);
	egueb_dom_string_unref(EON_NAME_ELEMENT_ENTRY);
	egueb_dom_string_unref(EON_NAME_ELEMENT_PROGRESS_BAR);
	egueb_dom_string_unref(EON_NAME_ELEMENT_PANED);
	egueb_dom_string_unref(EON_NAME_ELEMENT_EXPANDER);
	/* generic */
	egueb_dom_string_unref(EON_NAME_ON);
	egueb_dom_string_unref(EON_NAME_OFF);
	/* events */
	egueb_dom_string_unref(EON_EVENT_GEOMETRY_REQUEST);
	egueb_dom_string_unref(EON_EVENT_GEOMETRY_INVALIDATE);
	egueb_dom_string_unref(EON_NAME_EVENT_ACTIVATE);
	egueb_dom_string_unref(EON_NAME_EVENT_DEACTIVATE);
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
Egueb_Dom_String *EON_NAME_COLOR;
Egueb_Dom_String *EON_NAME_ATTR_THEME_NAME;
Egueb_Dom_String *EON_NAME_THEME_ID;
Egueb_Dom_String *EON_ATTR_ELLIPSIZE;
Egueb_Dom_String *EON_ATTR_VALIGN;
Egueb_Dom_String *EON_ATTR_HALIGN;
Egueb_Dom_String *EON_ATTR_WEIGHT;
Egueb_Dom_String *EON_ATTR_ORIENTATION;
Egueb_Dom_String *EON_ATTR_ENABLED;
Egueb_Dom_String *EON_NAME_ATTR_ACTIVE;
Egueb_Dom_String *EON_NAME_ATTR_INACTIVE;
Egueb_Dom_String *EON_NAME_ATTR_ACTIVATED;
Egueb_Dom_String *EON_NAME_ATTR_TITLE;
Egueb_Dom_String *EON_NAME_ATTR_SCALABLE;
Egueb_Dom_String *EON_NAME_ATTR_DEFAULT_VALUE;
Egueb_Dom_String *EON_NAME_ATTR_PROGRESSION;
Egueb_Dom_String *EON_STOCK;
Egueb_Dom_String *EON_NAME_ATTR_EXPANDED;
/* elements */
Egueb_Dom_String *EON_NAME_ELEMENT_EON;
Egueb_Dom_String *EON_NAME_ELEMENT_BUTTON;
Egueb_Dom_String *EON_NAME_ELEMENT_BUTTON_STOCK;
Egueb_Dom_String *EON_NAME_ELEMENT_BUTTON_TOGGLE;
Egueb_Dom_String *EON_NAME_ELEMENT_LABEL;
Egueb_Dom_String *EON_NAME_ELEMENT_LABEL_STOCK;
Egueb_Dom_String *EON_ELEMENT_STACK;
Egueb_Dom_String *EON_NAME_ELEMENT_OBJECT;
Egueb_Dom_String *EON_NAME_ELEMENT_RADIO;
Egueb_Dom_String *EON_NAME_ELEMENT_CHECKBOX;
Egueb_Dom_String *EON_NAME_ELEMENT_SWITCH;
Egueb_Dom_String *EON_NAME_ELEMENT_FRAME;
Egueb_Dom_String *EON_NAME_ELEMENT_SEPARATOR;
Egueb_Dom_String *EON_NAME_ELEMENT_IMAGE;
Egueb_Dom_String *EON_NAME_ELEMENT_ENTRY;
Egueb_Dom_String *EON_NAME_ELEMENT_PROGRESS_BAR;
Egueb_Dom_String *EON_NAME_ELEMENT_PANED;
Egueb_Dom_String *EON_NAME_ELEMENT_EXPANDER;
/* generic */
Egueb_Dom_String *EON_NAME_ON;
Egueb_Dom_String *EON_NAME_OFF;
/* events */
Egueb_Dom_String *EON_EVENT_GEOMETRY_REQUEST;
Egueb_Dom_String *EON_EVENT_GEOMETRY_INVALIDATE;
Egueb_Dom_String *EON_NAME_EVENT_ACTIVATE;
Egueb_Dom_String *EON_NAME_EVENT_DEACTIVATE;

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
		egueb_xlink_init();
		egueb_css_init();
		egueb_smil_init();
		_strings_init();
		eon_theme_init();
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

	eon_theme_shutdown();
	_strings_shutdown();
	egueb_smil_shutdown();
	egueb_css_shutdown();
	egueb_xlink_shutdown();
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
