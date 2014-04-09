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
#ifndef _EON_ELEMENT_OBJECT_H_
#define _EON_ELEMENT_OBJECT_H_

EAPI Egueb_Dom_Node * eon_element_object_new(void);
EAPI void eon_element_object_xlink_href_set(Egueb_Dom_Node *n,
		Egueb_Dom_String * xlink_href);
EAPI Egueb_Dom_String * eon_element_object_xlink_href_get(Egueb_Dom_Node *n);
EAPI Egueb_Dom_Node * eon_element_object_document_get(Egueb_Dom_Node *n);

#endif
