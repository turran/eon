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
/*----------------------------------------------------------------------------*
 *                             Value interface                                *
 *----------------------------------------------------------------------------*/
static Eina_Bool eon_stock_string_from(Eon_Stock *thiz, const char *str)
{
	if (!strcmp(str, "ok"))
		*thiz = EON_STOCK_OK;
	else if (!strcmp(str, "cancel"))
		*thiz = EON_STOCK_CANCEL;
	else if (!strcmp(str, "apply"))
		*thiz = EON_STOCK_APPLY;
	else if (!strcmp(str, "help"))
		*thiz = EON_STOCK_HELP;
	else if (!strcmp(str, "next"))
		*thiz = EON_STOCK_NEXT;
	else if (!strcmp(str, "prev"))
		*thiz = EON_STOCK_PREV;
	else
		return EINA_FALSE;
	return EINA_TRUE;
}

static char * eon_stock_string_to(Eon_Stock thiz)
{
	switch (thiz)
	{
		case EON_STOCK_OK:
		return strdup("ok");

		case EON_STOCK_CANCEL:
		return strdup("cancel");

		case EON_STOCK_APPLY:
		return strdup("apply");

		case EON_STOCK_HELP:
		return strdup("help");

		case EON_STOCK_NEXT:
		return strdup("next");

		case EON_STOCK_PREV:
		return strdup("prev");

		default:
		return NULL;
	}
}

EGUEB_DOM_VALUE_ENUM_BOILERPLATE(eon_stock, Eon_Stock);
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_stock_attr_new(void)
{
	return egueb_dom_attr_enum_new(egueb_dom_string_ref(EON_STOCK),
			EINA_FALSE, EINA_FALSE, EINA_FALSE,
			&_eon_stock_descriptor);
}

