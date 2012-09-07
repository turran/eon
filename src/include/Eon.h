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

#ifndef _EON_H
#define _EON_H

/**
 * @mainpage Eon
 * @section intro Introduction
 * Eon is a framework for building rich graphical applications
 *
 * @section dependencies Dependencies
 * - Eina
 * - Enesim
 * - Ender
 * - Escen
 * - Emage
 * @section optional_dependencies Optional Dependencies
 * @section backends Backends
 * - Ecore_SDL
 * @section Design
 */

#include <Enesim.h>
#include <Ender.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EON_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EON_BUILD */
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

#include "eon_main.h"
#include "eon_element.h"
#include "eon_image.h"
#include "eon_container.h"
#include "eon_bin.h"
#include "eon_grid.h"
#include "eon_canvas.h"
#include "eon_entry.h"
#include "eon_stack.h"
#include "eon_input.h"
#include "eon_progressbar.h"
#include "eon_scrollbar.h"
#include "eon_scrollview.h"
#include "eon_splitter.h"
#include "eon_radio.h"
#include "eon_checkbox.h"
#include "eon_button.h"
#include "eon_color.h"
#include "eon_frame.h"
#include "eon_label.h"
#include "eon_list.h"
#include "eon_spin.h"
#include "eon_surface.h"
#include "eon_tab.h"
#include "eon_toggle.h"
#include "eon_widget.h"
#include "eon_backend.h"
#include "eon_wrapper.h"
/* the backends */
#include "eon_ecore_sdl.h"
#include "eon_ecore_remote.h"

/** @} */

#endif
