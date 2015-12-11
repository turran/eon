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
#ifndef _EON_THEME_DOCUMENT_PRIVATE_H
#define _EON_THEME_DOCUMENT_PRIVATE_H

Egueb_Dom_Node * eon_theme_document_new(void);
Egueb_Dom_Node * eon_theme_document_instance_new(Egueb_Dom_Node *n,
		const char *id, Eina_Error *err);

void eon_theme_document_init(void);
void eon_theme_document_shutdown(void);
Egueb_Dom_Node * eon_theme_document_load(Egueb_Dom_String *name);

#endif
