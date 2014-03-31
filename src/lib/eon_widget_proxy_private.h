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

#ifndef EON_WIDGET_PROXY_PRIVATE_H_
#define EON_WIDGET_PROXY_PRIVATE_H_

#include "eon_widget_private.h"

#define EON_WIDGET_PROXY_DESCRIPTOR eon_widget_proxy_descriptor_get()
Enesim_Object_Descriptor * eon_widget_proxy_descriptor_get(void);

#define EON_WIDGET_PROXY_CLASS_GET(o) EON_WIDGET_PROXY_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EON_WIDGET_PROXY_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Eon_Widget_Proxy_Class, EON_WIDGET_PROXY_DESCRIPTOR)
#define EON_WIDGET_PROXY(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Widget_Proxy, EON_WIDGET_PROXY_DESCRIPTOR)

typedef struct _Eon_Widget_Proxy
{
	Eon_Widget base;
	/* attributes */
	/* private */
	Egueb_Dom_Node *proxy;
	/* Whenever the parent's theme attr changed or the
	 * widget's theme attr changed this flag will be set
	 */
	Eina_Bool theme_changed;
	Eina_Bool adopted;
} Eon_Widget_Proxy;

typedef void (*Eon_Widget_Proxy_Init)(Eon_Widget_Proxy *thiz);
typedef void (*Eon_Widget_Proxy_Geometry_Propagate)(Eon_Widget_Proxy *thiz);
typedef Egueb_Dom_Node * (*Eon_Widget_Proxy_Proxied_Get)(Eon_Widget_Proxy *thiz);

typedef struct _Eon_Widget_Proxy_Class
{
	Eon_Widget_Class base;
	Eon_Widget_Proxy_Init init;
	Eon_Widget_Proxy_Geometry_Propagate geometry_propagate;
	Eon_Widget_Proxy_Proxied_Get proxied_get;
} Eon_Widget_Proxy_Class;

#endif
