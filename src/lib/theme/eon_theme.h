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
#ifndef _EON_THEME_H
#define _EON_THEME_H

#include <Eina.h>
#include <Egueb_Css.h>
#include <Egueb_Dom.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EON_THEME_BUILD
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

EAPI void eon_theme_init(void);
EAPI void eon_theme_shutdown(void);

EAPI Egueb_Dom_Node * eon_theme_document_new(void);
EAPI Egueb_Dom_Node * eon_theme_document_instance_new(Egueb_Dom_Node *n,
		const char *id, Eina_Error *err);

EAPI Egueb_Dom_Node * eon_theme_element_eon_theme_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_state_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_scene_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_states_new(void);
EAPI Egueb_Dom_Node * eon_theme_element_object_new(void);

EAPI void * eon_theme_element_instance_object_get(Egueb_Dom_Node *n);
EAPI Eina_Bool eon_theme_element_instance_state_set(Egueb_Dom_Node *n,
		const char *state, Eina_Error *err);

/* Shared strings */
EAPI extern Egueb_Dom_String *EON_THEME_ELEMENT_EON_THEME;
EAPI extern Egueb_Dom_String *EON_THEME_ELEMENT_INSTANCE;
EAPI extern Egueb_Dom_String *EON_THEME_ELEMENT_OBJECT;
EAPI extern Egueb_Dom_String *EON_THEME_ELEMENT_SCENE;
EAPI extern Egueb_Dom_String *EON_THEME_ELEMENT_STATES;
EAPI extern Egueb_Dom_String *EON_THEME_ELEMENT_STATE;

#include "eon_theme_attr_int.h"
#include "eon_theme_attr_double.h"
#include "eon_theme_attr_enesim_color.h"
#include "eon_theme_attr_font.h"
#include "eon_theme_attr_primitive.h"
#include "eon_theme_instance.h"
#include "eon_theme_namespace.h"


#ifdef __cplusplus
}
#endif

#endif /*_EON_THEME_H*/
