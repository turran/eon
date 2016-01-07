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

#ifndef _EON_THEME_MARS_MAIN_PRIVATE_H_
#define _EON_THEME_MARS_MAIN_PRIVATE_H_

/* Some helpful defines */
#define EON_THEME_MARS_MARGIN 5
#define EON_THEME_MARS_BORDER 2

/* The constructors for the registration */
Egueb_Dom_Node * eon_theme_mars_eon_new(void);
Egueb_Dom_Node * eon_theme_mars_entry_new(void);
Egueb_Dom_Node * eon_theme_mars_button_new(void);
Egueb_Dom_Node * eon_theme_mars_switch_new(void);
Egueb_Dom_Node * eon_theme_mars_radio_new(void);
Egueb_Dom_Node * eon_theme_mars_frame_new(void);
Egueb_Dom_Node * eon_theme_mars_image_new(void);
Egueb_Dom_Node * eon_theme_mars_separator_new(void);
Egueb_Dom_Node * eon_theme_mars_progress_bar_new(void);
Egueb_Dom_Node * eon_theme_mars_paned_new(void);
Egueb_Dom_Node * eon_theme_mars_expander_new(void);
Egueb_Dom_Node * eon_theme_mars_scale_new(void);

#endif
