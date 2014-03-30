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
#ifndef _EON_ELEMENT_BUTTON_STOCK_H_
#define _EON_ELEMENT_BUTTON_STOCK_H_

typedef enum _Eon_Element_Button_Stock_Type
{
	EON_ELEMENT_BUTTON_STOCK_TYPE_OK,
	EON_ELEMENT_BUTTON_STOCK_TYPE_CANCEL,
	EON_ELEMENT_BUTTON_STOCK_TYPE_APPLY,
	EON_ELEMENT_BUTTON_STOCK_TYPE_HELP,
	EON_ELEMENT_BUTTON_STOCK_TYPE_NEXT,
	EON_ELEMENT_BUTTON_STOCK_TYPE_PREV,
} Eon_Element_Button_Stock_Type;

EAPI Egueb_Dom_Node * eon_element_button_stock_new(void);
EAPI void eon_element_button_stock_type_set(Egueb_Dom_Node *n,
		Eon_Element_Button_Stock_Type type);
EAPI Eon_Element_Button_Stock_Type
eon_element_button_stock_type_get(Egueb_Dom_Node *n);

#endif
