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
#include "eon_stock.h"
#include "eon_document.h"
#include "eon_element_eon.h"
#include "eon_element_button.h"
#include "eon_element_button_stock.h"
#include "eon_element_button_toggle.h"
#include "eon_element_label.h"
#include "eon_element_label_stock.h"
#include "eon_element_stack.h"
#include "eon_element_object.h"
#include "eon_element_radio.h"
#include "eon_element_checkbox.h"
#include "eon_element_switch.h"
#include "eon_element_frame.h"
#include "eon_element_separator.h"
#include "eon_element_image.h"
#include "eon_element_entry.h"

#include "eon_theme_element_eot_private.h"

#include "eon_renderable_private.h"
#include "eon_element_eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Document
{
} Eon_Document;

/*----------------------------------------------------------------------------*
 *                     The exernal document interface                         *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_Node * _eon_document_element_create(Egueb_Dom_Node *n,
		void *data, const char *ns, const char *name)
{
	Egueb_Dom_Node *ret = NULL;

	DBG("Creating element '%s'", name);
	if (!strcmp(name, "eon"))
		ret = eon_element_eon_new();
	else if (!strcmp(name, "eot"))
		ret = eon_theme_element_eot_new();
	else if (!strcmp(name, "button"))
		ret = eon_element_button_new();
	else if (!strcmp(name, "entry"))
		ret = eon_element_entry_new();
	else if (!strcmp(name, "label"))
		ret = eon_element_label_new();
	else if (!strcmp(name, "stack"))
		ret = eon_element_stack_new();
	else if (!strcmp(name, "stockLabel"))
		ret = eon_element_label_stock_new();
	else if (!strcmp(name, "stockButton"))
		ret = eon_element_button_stock_new();
	else if (!strcmp(name, "object"))
		ret = eon_element_object_new();
	else if (!strcmp(name, "checkbox"))
		ret = eon_element_checkbox_new();
	else if (!strcmp(name, "switch"))
		ret = eon_element_switch_new();
	else if (!strcmp(name, "toggleButton"))
		ret = eon_element_button_toggle_new();
	else if (!strcmp(name, "radio"))
		ret = eon_element_radio_new();
	else if (!strcmp(name, "frame"))
		ret = eon_element_frame_new();
	else if (!strcmp(name, "separator"))
		ret = eon_element_separator_new();
	else if (!strcmp(name, "image"))
		ret = eon_element_image_new();
	return ret;
}

static Eina_Bool _eon_document_child_appendable(Egueb_Dom_Node *n,
		void *data, Egueb_Dom_Node *child)
{
	Egueb_Dom_String *name;
	Eina_Bool ret;

	name = egueb_dom_node_name_get(child);
	ret = egueb_dom_string_is_equal(name, EON_NAME_ELEMENT_EON);
	egueb_dom_string_unref(name);
	return ret;
}

static void _eon_document_init(Egueb_Dom_Node *n, void *data)
{
}

static void _eon_document_deinit(Egueb_Dom_Node *n, void *data)
{
	Eon_Document *thiz = data;
	free(thiz);
}

static Egueb_Dom_Document_External_Descriptor _descriptor = {
	/* version 		= */ EGUEB_DOM_DOCUMENT_EXTERNAL_DESCRIPTOR_VERSION,
	/* init 		= */ _eon_document_init,
	/* deinit 		= */ _eon_document_deinit,
	/* element_create	= */ _eon_document_element_create,
	/* child_appendable	= */ _eon_document_child_appendable,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_document_new(void)
{
	Eon_Document *thiz;

	thiz = calloc(1, sizeof(Eon_Document));
	return egueb_dom_document_external_new(&_descriptor, thiz);
}
