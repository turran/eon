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

#ifndef EON_H_
#define EON_H_

#include "Egueb_Dom.h"
#include "Egueb_Css.h"
#include "Egueb_Xlink.h"

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EGUEB_DOM_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "eon_main.h"

/* types */
#include "eon_stock.h"

/* elements */
#include "eon_document.h"
#include "eon_element_eon.h"
#include "eon_element_eon.h"
#include "eon_element_button.h"
#include "eon_element_checkbox.h"
#include "eon_element_button_stock.h"
#include "eon_element_button_toggle.h"
#include "eon_element_frame.h"
#include "eon_element_label.h"
#include "eon_element_object.h"
#include "eon_element_radio.h"
#include "eon_element_stack.h"
#include "eon_element_separator.h"
#include "eon_element_switch.h"
#include "eon_renderable.h"
#include "eon_event_geometry.h"

/* theme */
#include "eon_theme_namespace.h"
#include "eon_theme_element.h"
#include "eon_theme_renderable.h"
#include "eon_theme_widget.h"
#include "eon_theme_element_button.h"
#include "eon_theme_element_eon.h"
#include "eon_theme_element_frame.h"
#include "eon_theme_element_separator.h"
#include "eon_theme_element_switch.h"

#ifdef __cplusplus
}
#endif

#endif
