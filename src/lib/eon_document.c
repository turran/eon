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
	return NULL;
}

static void * _eon_document_init(Egueb_Dom_Node *n)
{
	Eon_Document *thiz;

	thiz = calloc(1, sizeof(Eon_Document));

	return thiz;
}

static void _eon_document_deinit(Egueb_Dom_Node *n, void *data)
{
	Eon_Document *thiz = data;
	free(thiz);
}

static void _eon_document_process(Egueb_Dom_Node *n, void *data)
{

}

static Egueb_Dom_Document_External_Descriptor _descriptor = {
	/* init 		= */ _eon_document_init,
	/* deinit 		= */ _eon_document_deinit,
	/* element_create	= */ _eon_document_element_create,
	/* process 		= */ _eon_document_process,
	/* needs process 	= */ NULL,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_document_new(void)
{
	return egueb_dom_document_external_new(&_descriptor);
}
