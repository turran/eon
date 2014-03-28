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

#ifndef EON_WIDGET_PRIVATE_H_
#define EON_WIDGET_PRIVATE_H_

#include "eon_renderable_private.h"
#include "eon_drawer_widget.h"

#define EON_WIDGET_DESCRIPTOR eon_widget_descriptor_get()
Enesim_Object_Descriptor * eon_widget_descriptor_get(void);

#define EON_WIDGET_CLASS_GET(o) EON_WIDGET_CLASS(				\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_WIDGET_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Widget_Class, EON_WIDGET_DESCRIPTOR)
#define EON_WIDGET(o) ENESIM_OBJECT_INSTANCE_CHECK(o,				\
		Eon_Widget, EON_WIDGET_DESCRIPTOR)

typedef struct _Eon_Widget
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *enabled;
	/* private */
	Egueb_Dom_String *last_theme;
	Egueb_Dom_String *last_parent_theme;
} Eon_Widget;

typedef void (*Eon_Widget_Init)(Eon_Widget *thiz);
typedef Eina_Bool (*Eon_Widget_Pre_Process)(Eon_Widget *thiz);
typedef void (*Eon_Widget_State_Set)(Eon_Widget *thiz, Egueb_Dom_String *state);
typedef void (*Eon_Widget_Theme_Changed)(Eon_Widget *thiz);

typedef struct _Eon_Widget_Class
{
	Eon_Renderable_Class base;
	Eon_Widget_Init init;
	Eon_Widget_State_Set state_set;
	Eon_Widget_Pre_Process pre_process;
	Eon_Widget_Theme_Changed theme_changed;
} Eon_Widget_Class;

void eon_widget_state_set(Egueb_Dom_Node *n, Egueb_Dom_String *state);

#endif
