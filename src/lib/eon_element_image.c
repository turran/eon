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
#include "eon_private.h"
#include "eon_main.h"
#include "eon_element_image.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
//#include "eon_theme_element_image_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_IMAGE_DESCRIPTOR eon_element_image_descriptor_get()
#define EON_ELEMENT_IMAGE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Image, EON_ELEMENT_IMAGE_DESCRIPTOR)

typedef struct _Eon_Element_Image
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *scalable;
	Egueb_Dom_Node *xlink_href;
	/* private */
	Egueb_Dom_Feature *theme_feature;
} Eon_Element_Image;

typedef struct _Eon_Element_Image_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Image_Class;

/* Inform of a geometry change */
static void _eon_element_image_attr_modified_cb(Egueb_Dom_Event *e,
		void *data)
{
	Eon_Element_Image *thiz = data;
	Egueb_Dom_Node *attr;

	if (egueb_dom_event_phase_get(e) != EGUEB_DOM_EVENT_PHASE_AT_TARGET)
		return;

	attr = egueb_dom_event_mutation_related_get(e);
	if (thiz->scalable == attr || thiz->xlink_href == attr)
	{
		DBG_ELEMENT((EON_ELEMENT(thiz))->n, "Geometry related attr changed, "
				"invalidating geometry");
		eon_renderable_invalidate_geometry((EON_ELEMENT(thiz))->n);
	}
	egueb_dom_node_unref(attr);
}
/*----------------------------------------------------------------------------*
 *                             Renderable interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_image_init(Eon_Renderable *w)
{
	Eon_Element_Image *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_IMAGE(w);

	/* attributes */
	thiz->scalable = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_ATTR_SCALABLE),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	thiz->xlink_href = egueb_xlink_attr_href_new(
			egueb_dom_string_ref(EGUEB_XLINK_NAME_HREF),
			NULL);
	n = (EON_ELEMENT(w))->n;
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->scalable), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->xlink_href), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	egueb_dom_event_target_event_listener_add(et,
			EGUEB_DOM_EVENT_MUTATION_ATTR_MODIFIED,
			_eon_element_image_attr_modified_cb,
			EINA_FALSE, thiz);
	/* private */
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_IMAGE));
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_image_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Image *thiz;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;

	thiz = EON_ELEMENT_IMAGE(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return NULL;
	}
	ren = eon_theme_renderable_renderer_get(theme_element);
	egueb_dom_node_unref(theme_element);
	return ren;
}

static int _eon_element_image_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Image *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	int ret;

	thiz = EON_ELEMENT_IMAGE(r);
	return ret;
}

static Eina_Bool _eon_element_image_process(Eon_Renderable *r)
{
	Eon_Element_Image *thiz;
	Eon_Renderable *w;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_IMAGE(r);

	/* get the theme */
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}

	/* Set the geometry on the child */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_image_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_IMAGE);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Image,
		Eon_Element_Image_Class, eon_element_image);

static void _eon_element_image_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_image_tag_name_get;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_image_renderer_get;
	r_klass->size_hints_get = _eon_element_image_size_hints_get;
	r_klass->process = _eon_element_image_process;
	r_klass->init = _eon_element_image_init;
}

static void _eon_element_image_instance_init(void *o)
{
}

static void _eon_element_image_instance_deinit(void *o)
{
	Eon_Element_Image *thiz;

	thiz = EON_ELEMENT_IMAGE(o);
	egueb_dom_node_unref(thiz->scalable);
	egueb_dom_node_unref(thiz->xlink_href);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_image_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_image);
	return n;
}

