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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_widget_proxy_init(Eon_Renderable *r)
{
	Eon_Widget_Proxy *thiz;
	Eon_Widget_Proxy_Class *klass;
	Egueb_Dom_Node *n;

	thiz = EON_WIDGET_PROXY(r);
	n = (EON_ELEMENT(r))->n;
	klass = EON_WIDGET_PROXY_CLASS_GET(thiz);
	if (klass->init)
		klass->init(thiz);
}

static Enesim_Renderer * _eon_widget_proxy_renderer_get(Eon_Renderable *r)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	return enesim_renderer_ref(thiz->proxy_r);
}

static void _eon_widget_proxy_geometry_set(Eon_Renderable *r, Eina_Rectangle *geometry)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	if (!thiz->proxy)
		return;
	eon_renderable_geometry_set(thiz->proxy, geometry);
}

static int _eon_widget_proxy_size_hints_get(Eon_Renderable *r, Eon_Renderable_Size *size)
{
	Eon_Widget_Proxy *thiz;
	Eon_Widget_Proxy_Class *klass;

	thiz = EON_WIDGET_PROXY(r);
	return eon_renderable_size_hints_get(thiz->proxy, size);
}

static Eina_Bool _eon_widget_proxy_pre_process(Eon_Renderable *r)
{
	Eon_Widget_Proxy_Class *klass;
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(r);
	klass = EON_WIDGET_PROXY_CLASS_GET(r);
	if (klass->pre_process)
	{
		if (!klass->pre_process(thiz))
			return EINA_FALSE;
	}
	return EINA_TRUE;
}

static Eina_Bool _eon_widget_proxy_process(Eon_Renderable *r)
{
	Eon_Widget_Proxy *thiz;
	Eon_Widget_Proxy_Class *klass;

	thiz = EON_WIDGET_PROXY(r);
	return egueb_dom_element_process(thiz->proxy);
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_ABSTRACT_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Widget_Proxy,
		Eon_Widget_Proxy_Class, eon_widget_proxy);

static void _eon_widget_proxy_class_init(void *k)
{
	Eon_Element_Class *klass = EON_ELEMENT_CLASS(k);
	Eon_Renderable_Class *r_klass;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_widget_proxy_init;
	r_klass->renderer_get = _eon_widget_proxy_renderer_get;
	r_klass->size_hints_get = _eon_widget_proxy_size_hints_get;
	r_klass->geometry_set = _eon_widget_proxy_geometry_set;
	r_klass->pre_process = _eon_widget_proxy_pre_process;
	r_klass->process = _eon_widget_proxy_process;
}

static void _eon_widget_proxy_instance_init(void *o)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(o);
	/* instantiate the theme element */
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_changed = EINA_TRUE;
}

static void _eon_widget_proxy_instance_deinit(void *o)
{
	Eon_Widget_Proxy *thiz;

	thiz = EON_WIDGET_PROXY(o);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

