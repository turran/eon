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
#ifndef _EON_THEME_PRIVATE_H
#define _EON_THEME_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Egueb_Dom.h>
#include <Egueb_Css.h>
#include <Egueb_Smil.h>
#include "Eon.h"

#define ERR(...) EINA_LOG_DOM_ERR(eon_theme_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eon_theme_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(eon_theme_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eon_theme_log_dom, __VA_ARGS__)

#endif
