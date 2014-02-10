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
#include "eon_element_eon.h"
#include "eon_element_button.h"
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
		void *data, const char *name)
{
	Egueb_Dom_Node *ret = NULL;

	printf("creating element %s\n", name);
	if (!strcmp(name, "eon"))
		ret = eon_element_eon_new();
	else if (!strcmp(name, "button"))
		ret = eon_element_button_new();
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
	/* init 		= */ _eon_document_init,
	/* deinit 		= */ _eon_document_deinit,
	/* element_create	= */ _eon_document_element_create,
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
