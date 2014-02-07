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

#ifndef EON_PRIVATE_H_
#define EON_PRIVATE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Egueb_Dom.h"
#include "Ender.h"

#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(eon_main_log, __VA_ARGS__)

#ifdef WARN
# undef WARN
#endif
#define WARN(...) EINA_LOG_DOM_WARN(eon_main_log, __VA_ARGS__)

#ifdef INFO
# undef INFO
#endif
#define INFO(...) EINA_LOG_DOM_INFO(eon_main_log, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(eon_main_log, __VA_ARGS__)

extern int eon_main_log;

#endif

