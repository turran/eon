/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2011 Jorge Luis Zapata
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

#ifndef _EON_LABEL_H_
#define _EON_LABEL_H_

/**
 * @defgroup Eon_Label Label
 * @ingroup Eon_Widget
 * @{
 */

EAPI Ender_Element * eon_label_new(void);
EAPI void eon_label_text_set(Ender_Element *e, const char *text);
EAPI void eon_label_text_get(Ender_Element *e, const char **text);

/**
 * @}
 */

#endif
