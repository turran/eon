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

#ifndef EON_FEATURE_THEMABLE_PRIVATE_H_
#define EON_FEATURE_THEMABLE_PRIVATE_H_

Egueb_Dom_Feature * eon_feature_themable_add(Egueb_Dom_Node *n);
Egueb_Dom_Node * eon_feature_themable_load(Egueb_Dom_Feature *f);
void eon_feature_themable_event_propagate(Egueb_Dom_Feature *f,
		Egueb_Dom_String *ev);

Egueb_Dom_Node * eon_feature_themable_element_get(Egueb_Dom_Feature *f);

#endif

