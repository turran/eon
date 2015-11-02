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
#include "eon_element_checkbox.h"

#include "eon_feature_themable_private.h"
#include "eon_widget_private.h"
#include "eon_theme_renderable_private.h"
//#include "eon_theme_element_checkbox_private.h"
#include "eon_layout_frame_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_CHECKBOX_DESCRIPTOR eon_element_checkbox_descriptor_get()
#define EON_ELEMENT_CHECKBOX(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Element_Checkbox, EON_ELEMENT_CHECKBOX_DESCRIPTOR)

typedef struct _Eon_Element_Checkbox
{
	Eon_Widget base;
	/* attributes */
	Egueb_Dom_Node *selected;
	/* private */
	Egueb_Dom_Feature *theme_feature;
} Eon_Element_Checkbox;

typedef struct _Eon_Element_Checkbox_Class
{
	Eon_Widget_Class base;
} Eon_Element_Checkbox_Class;

/*----------------------------------------------------------------------------*
 *                             Widget interface                               *
 *----------------------------------------------------------------------------*/
static void _eon_element_checkbox_init(Eon_Widget *w)
{
	Eon_Element_Checkbox *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;

	n = (EON_ELEMENT(w))->n;
	thiz = EON_ELEMENT_CHECKBOX(w);

	/* private */
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(w);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_CHECKBOX));

	/* attributes */
	thiz->selected = egueb_dom_attr_boolean_new(egueb_dom_string_ref(EON_ATTR_ENABLED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->selected, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->selected), NULL);

}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_element_checkbox_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Checkbox *thiz;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;

	thiz = EON_ELEMENT_CHECKBOX(r);
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

static int _eon_element_checkbox_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Checkbox *thiz;
	Eon_Box padding;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	int ret;

	n = (EON_ELEMENT(r))->n;

	ret = eon_layout_frame_size_hints_get(n, size);

	/* finally add our padding */
	thiz = EON_ELEMENT_CHECKBOX(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	//eon_theme_element_checkbox_padding_get(theme_element, &padding);
	/* a checkbox can be of any size */
	ret |= EON_RENDERABLE_HINT_MIN_MAX;
	if (size->min_width > 0)
		size->min_width += padding.left + padding.right;
	if (size->min_height > 0)
		size->min_height += padding.top + padding.bottom;
	size->max_width = -1;
	size->max_height = -1;

	if (ret & EON_RENDERABLE_HINT_PREFERRED)
	{
		if (size->pref_width > 0)
			size->pref_width += padding.left + padding.right;
		if (size->pref_height > 0)
			size->pref_height += padding.top + padding.bottom;
	}

	return ret;
}

static Eina_Bool _eon_element_checkbox_process(Eon_Renderable *r)
{
	Eon_Element_Checkbox *thiz;
	Eon_Widget *w;
	Eon_Box padding = { 0, 0, 0, 0 };
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *child;
	Egueb_Dom_Node *theme_element;
	Eina_Rectangle geometry;
	Eina_Rectangle free_space;
	int enabled;

	n = (EON_ELEMENT(r))->n;
	thiz = EON_ELEMENT_CHECKBOX(r);

	free_space = r->geometry;

	/* get the theme */
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		goto done;
	}

	/* Set the geometry on the child */
	//eon_theme_element_checkbox_padding_get(theme_element, &padding);
	//eon_theme_renderable_geometry_set(theme_element, &r->geometry);
	/* Set the enabled */
	w = EON_WIDGET(r);
	egueb_dom_attr_final_get(w->enabled, &enabled);
	eon_theme_widget_enabled_set(theme_element, enabled);

	free_space.x += padding.left;
	free_space.y += padding.top;
	free_space.w -= padding.left + padding.right;
	free_space.h -= padding.bottom + padding.top;

	DBG_ELEMENT(n, "Free space %" EINA_RECTANGLE_FORMAT, EINA_RECTANGLE_ARGS(&free_space));
	/* Set the content renderer */
	child = egueb_dom_element_child_first_get(n);
	if (child)
	{
		Enesim_Renderer *ren;

		ren = eon_renderable_renderer_get(child);
		//eon_theme_element_checkbox_content_set(theme_element, ren);
		egueb_dom_node_unref(child);
	}
	else
	{
		//eon_theme_element_checkbox_content_set(theme_element, NULL);
	}

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

done:
	/* Our basic frame layout algorithm */
	eon_layout_frame_size_geometry_set(n, &free_space);
	
	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_checkbox_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_CHECKBOX);
}

static Eina_Bool _eon_element_checkbox_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *our_child;

	/* only accept one child */
	if (!eon_is_renderable(child))
		return EINA_FALSE;

	/* check if we already have one child */
	n = e->n;
	our_child = egueb_dom_element_child_first_get(n);
	if (our_child)
	{
		WARN("Only one child supported");
		egueb_dom_node_unref(our_child);
		return EINA_FALSE;
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_WIDGET_DESCRIPTOR, Eon_Element_Checkbox,
		Eon_Element_Checkbox_Class, eon_element_checkbox);

static void _eon_element_checkbox_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;
	Eon_Widget_Class *w_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_checkbox_tag_name_get;
	klass->child_appendable = _eon_element_checkbox_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->renderer_get = _eon_element_checkbox_renderer_get;
	r_klass->size_hints_get = _eon_element_checkbox_size_hints_get;
	r_klass->process = _eon_element_checkbox_process;

	w_klass = EON_WIDGET_CLASS(k);
	w_klass->init = _eon_element_checkbox_init;
}

static void _eon_element_checkbox_instance_init(void *o)
{
}

static void _eon_element_checkbox_instance_deinit(void *o)
{
	Eon_Element_Checkbox *thiz;

	thiz = EON_ELEMENT_CHECKBOX(o);
	/* attributes */
	egueb_dom_node_unref(thiz->selected);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_checkbox_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_checkbox);
	return n;
}

EAPI void eon_element_checkbox_selected_set(Egueb_Dom_Node *n, Eina_Bool selected)
{
	Eon_Element_Checkbox *thiz;
	int s = selected;

	thiz = EON_ELEMENT_CHECKBOX(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_set(thiz->selected, EGUEB_DOM_ATTR_TYPE_BASE, s);
}

EAPI Eina_Bool eon_element_checkbox_selected_get(Egueb_Dom_Node *n)
{
	Eon_Element_Checkbox *thiz;
	int s;

	thiz = EON_ELEMENT_CHECKBOX(egueb_dom_element_external_data_get(n));
	egueb_dom_attr_final_get(thiz->selected, &s);
	return s ? EINA_TRUE : EINA_FALSE;
}

#if 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Checkbox
{
	/* properties */
	/* private */
	Ender_Element *e;
	/* our own checkbox control */
	Eon_Theme_Instance *control;
	/* layout */
	double min_length;
} Eon_Checkbox;

static inline Eon_Checkbox * _eon_checkbox_get(Eon_Element *ee)
{
	Eon_Checkbox *thiz;

	thiz = eon_checkbox_base_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                             The checkbox layout                               *
 *----------------------------------------------------------------------------*/
static void _checkbox_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 10;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _checkbox_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Checkbox *thiz;
	Eon_Element *e = ref;

	thiz = _eon_checkbox_get(e);
	eon_theme_instance_property_set(thiz->control, "x", g->x, NULL);
	eon_theme_instance_property_set(thiz->control, "y", g->y, NULL);
	eon_theme_instance_property_set(thiz->control, "width", g->width, NULL);
	eon_theme_instance_property_set(thiz->control, "height", g->height, NULL);
}

static void _checkbox_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Checkbox *thiz;
	Eon_Element *e = ref;
	Eon_Size size;

	thiz = _eon_checkbox_get(e);
	eon_theme_control_checkbox_size_get(thiz->control->object, &size);
	eon_hints_sizes_values_set(hints, size.width, size.height,
			size.width, size.height,
			size.width, size.height);
}

static void _checkbox_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	cb(ref, NULL, data);
}

static Eon_Layout_Frame_Descriptor _checkbox_layout = {
	/* .child_padding_get 	= */ _checkbox_layout_child_padding_get,
	/* .child_foreach 	= */ _checkbox_layout_child_foreach,
	/* .child_hints_get 	= */ _checkbox_layout_child_hints_get,
	/* .child_geometry_set 	= */ _checkbox_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                           The bin child layout                             *
 *----------------------------------------------------------------------------*/
static void _child_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 10;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _child_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Element *e = child;
	eon_element_geometry_set(e, g);
}

static void _child_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Element *e = child;
	eon_element_hints_get(e, hints);
}

static void _child_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Element *e = ref;
	Ender_Element *child;

	child = eon_bin_internal_child_get(e);
	if (!child) return;

	e = ender_element_object_get(child);
	cb(ref, e, data);
}

static Eon_Layout_Frame_Descriptor _child_layout = {
	/* .child_padding_get 	= */ _child_layout_child_padding_get,
	/* .child_foreach 	= */ _child_layout_child_foreach,
	/* .child_hints_get 	= */ _child_layout_child_hints_get,
	/* .child_geometry_set 	= */ _child_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                     The main stack layout descriptors                      *
 *----------------------------------------------------------------------------*/
static void _checkbox_stack_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	Eon_Layout_Child_Data *lchild = child;
	eon_layout_geometry_set(lchild->layout, lchild->descriptor, ref, g);
}

static void _checkbox_stack_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	Eon_Layout_Child_Data *lchild = child;
	eon_layout_hints_get(lchild->layout, lchild->descriptor, ref, hints);
}

static void _checkbox_stack_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	Eon_Layout_Child_Data lchild;

	/* the only children of this main stack are the two frames */
	lchild.layout = &eon_layout_frame;
	lchild.descriptor = &_checkbox_layout;
	cb(ref, &lchild, data);

	lchild.layout = &eon_layout_frame;
	lchild.descriptor = &_child_layout;
	cb(ref, &lchild, data);
}

static Eon_Orientation _checkbox_stack_layout_orientation_get(void *ref)
{
	return EON_ORIENTATION_HORIZONTAL;
}

static void _checkbox_stack_layout_min_length_get(void *ref, double *min)
{
	Eon_Element *e = ref;
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(e);
	*min = thiz->min_length;
}

static void _checkbox_stack_layout_min_length_set(void *ref, double min)
{
	Eon_Element *e = ref;
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(e);
	thiz->min_length = min;
}

static int _checkbox_stack_layout_child_weight_get(void *ref, void *child)
{
	Eon_Layout_Child_Data *lchild = child;
	

	/* the child frame always has priority */
	if (lchild->descriptor == &_child_layout)
		return 1;
	else
		return 0;
}

static Eon_Layout_Stack_Descriptor _stack_layout = {
	/* .orientation_get 	= */ _checkbox_stack_layout_orientation_get,
	/* .min_length_get 	= */ _checkbox_stack_layout_min_length_get,
	/* .min_length_get 	= */ _checkbox_stack_layout_min_length_set,
	/* .child_weight_get 	= */ _checkbox_stack_layout_child_weight_get,
	/* .child_foreach 	= */ _checkbox_stack_layout_child_foreach,
	/* .child_hints_get 	= */ _checkbox_stack_layout_child_hints_get,
	/* .child_geometry_set 	= */ _checkbox_stack_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                     The main frame layout descriptors                      *
 *----------------------------------------------------------------------------*/
static void _main_layout_child_padding_get(void *ref, void *child,
		Eon_Margin *margin)
{
	margin->left = margin->top = margin->right = margin->bottom = 10;
	/* TODO
	*margin = thiz->padding;
	*/
}

static void _main_layout_child_geometry_set(void *ref, void *child,
		Eon_Geometry *g)
{
	eon_layout_geometry_set(&eon_layout_stack, &_stack_layout, ref, g);
}

static void _main_layout_child_hints_get(void *ref, void *child,
		Eon_Hints *hints)
{
	eon_layout_hints_get(&eon_layout_stack, &_stack_layout, ref, hints);
}

static void _main_layout_child_foreach(void *ref, Eon_Layout_Child_Foreach_Cb cb,
		 void *data)
{
	cb(ref, NULL, data);
}

static Eon_Layout_Frame_Descriptor _main_layout = {
	/* .child_padding_get 	= */ _main_layout_child_padding_get,
	/* .child_foreach 	= */ _main_layout_child_foreach,
	/* .child_hints_get 	= */ _main_layout_child_hints_get,
	/* .child_geometry_set 	= */ _main_layout_child_geometry_set,
};
/*----------------------------------------------------------------------------*
 *                                Event handlers                              *
 *----------------------------------------------------------------------------*/
static void _eon_checkbox_mouse_click(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eina_Bool selected;

	eon_checkbox_selected_get(e, &selected);
	eon_checkbox_selected_set(e, !selected);
}

static void _eon_checkbox_key_up(Ender_Element *e,
		const char *event_name,
		void *event_data,
		void *data)
{
	Eon_Event_Key_Up *ev = event_data;
	Eon_Navigation_Key nkey;
	Eina_Bool enabled;
	Eina_Bool selected;

	/* check if the key is an enter key */
	if (!eon_input_navigation_key_get(ev->input, ev->key, &nkey))
		return;

	if (nkey != EON_NAVIGATION_KEY_OK)
		return;

	eon_widget_enabled_get(e, &enabled);
	if (!enabled)
		return;

	eon_checkbox_selected_get(e, &selected);
	eon_checkbox_selected_set(e, !selected);
}
/*----------------------------------------------------------------------------*
 *                      The Eon's checkbox_base interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_checkbox_initialize(Ender_Element *e)
{
	Eon_Checkbox *thiz;
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	thiz = _eon_checkbox_get(ee);
	thiz->e = e;
	/* register every needed callback */
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
			_eon_checkbox_mouse_click, NULL);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_UP],
			_eon_checkbox_key_up, NULL);
}

static void _eon_checkbox_geometry_set(Eon_Element *e, Eon_Geometry *g)
{
	eon_layout_geometry_set(&eon_layout_frame, &_main_layout, e, g);
}

static void _eon_checkbox_free(Eon_Element *e)
{
	Eon_Checkbox *thiz;

	thiz = _eon_checkbox_get(e);
	free(thiz);
}

static void _eon_checkbox_hints_get(Eon_Element *e, Eon_Theme_Instance *theme,
		Eon_Hints *hints)
{
	eon_layout_hints_get(&eon_layout_frame, &_main_layout, e, hints);
	hints->max.width = DBL_MAX;
	hints->max.height = DBL_MAX;
}


static Eon_Checkbox_Base_Descriptor _descriptor = {
	/* .initialize 		= */ _eon_checkbox_initialize,
	/* .geometry_set 	= */ _eon_checkbox_geometry_set,
	/* .free		= */ _eon_checkbox_free,
	/* .name 		= */ "checkbox",
	/* .hints_get 		= */ _eon_checkbox_hints_get,
	/* .child_at 		= */ NULL,
	/* .pass_events		= */ EINA_FALSE,
};

#endif
