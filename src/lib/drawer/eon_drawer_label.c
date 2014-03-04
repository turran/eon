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
#include "eon_drawer_private.h"

#include "eon_drawer_widget.h"
#include "eon_drawer_label.h"
#include "eon_drawer_widget_private.h"
#include "eon_drawer_label_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_DRAWER_LABEL_DESCRIPTOR eon_drawer_label_descriptor_get()
#define EON_DRAWER_LABEL(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Eon_Drawer_Label, EON_DRAWER_LABEL_DESCRIPTOR)

typedef struct _Eon_Drawer_Label
{
	Eon_Drawer_Widget base;
	const Eon_Drawer_Label_Descriptor *d;
	void *data;
	/* attributes set from the control */
	Enesim_Color control_color;
	Eina_Bool control_color_set;
	/* attributes populated */
	Enesim_Color color;
	Egueb_Css_Font font;
	/* our generated attributes */
	Enesim_Text_Font *tf;
	Enesim_Color final_color;
	Eina_Bool font_changed;
	Eina_Bool color_changed;
} Eon_Drawer_Label;

typedef struct _Eon_Drawer_Label_Class
{
	Eon_Drawer_Widget_Class base;
} Eon_Drawer_Label_Class;


static Eina_Bool _eon_drawer_label_generate_font(Eon_Drawer_Label *thiz)
{
	if (thiz->font_changed)
	{
		Enesim_Text_Engine *e;
		Enesim_Renderer *r;
		char *f;
		Eina_List *l;

		r = eon_drawer_label_text_renderer_get(EON_DRAWER_WIDGET(thiz));
		if (!r)
		{
			return EINA_FALSE;
		}

		/* TODO generate a text font based on the attributes */
		e = enesim_text_engine_default_get();
		EINA_LIST_FOREACH(thiz->font.families, l, f)
		{
			/* TODO still missing the font size, convert the variants into fontconfig information, etc */
			thiz->tf = enesim_text_font_new_description_from(e, f, 16);
			if (thiz->tf)
				break;
		}
		if (!thiz->tf)
		{
			printf("Impossible to load any font, using default one");
			thiz->tf = enesim_text_font_new_description_from(e, "Sans:style=Regular", 16);
		}
		enesim_text_engine_unref(e);
		thiz->font_changed = EINA_FALSE;

		enesim_renderer_text_span_font_set(r, thiz->tf);
		enesim_renderer_unref(r);
	}
	return EINA_TRUE;
}

static void _eon_drawer_label_generate_color(Eon_Drawer_Label *thiz)
{
	if (thiz->color_changed)
	{
		if (thiz->control_color_set)
			thiz->final_color = thiz->control_color;
		else
			thiz->final_color = thiz->color;
		if (thiz->d->text_color_set)
			thiz->d->text_color_set(EON_DRAWER_WIDGET(thiz), thiz->data, thiz->final_color);
		thiz->color_changed = EINA_FALSE;
	}
}

/*----------------------------------------------------------------------------*
 *                         Attribute setters/getters                           *
 *----------------------------------------------------------------------------*/
static void _eon_drawer_label_color_set(Eon_Drawer_Widget *w, Enesim_Color color)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	thiz->color = color;
	thiz->color_changed = EINA_TRUE;
	_eon_drawer_label_generate_color(thiz);
}

static Enesim_Color _eon_drawer_label_color_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	return thiz->final_color;
}

static void _eon_drawer_label_font_set(Eon_Drawer_Widget *w, Egueb_Css_Font *font)
{
	Eon_Drawer_Label *thiz;
	char *f;

	thiz = EON_DRAWER_LABEL(w);
	EINA_LIST_FREE(thiz->font.families, f)
		//free(f);
	thiz->font.families = NULL;
	thiz->font = *font;
	thiz->font_changed = EINA_TRUE;
}

static void _eon_drawer_label_font_get(Eon_Drawer_Widget *w, Egueb_Css_Font *font)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	*font = thiz->font;
}
/*----------------------------------------------------------------------------*
 *                              Widget interface                              *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_drawer_label_renderer_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	if (thiz->d->renderer_get)
		return thiz->d->renderer_get(w, thiz->data);
	return NULL;
}

static void _eon_drawer_label_geometry_set(Eon_Drawer_Widget *w, Eina_Rectangle *geom)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	if (thiz->d->geometry_set)
		thiz->d->geometry_set(w, thiz->data, geom);
}

static void _eon_drawer_label_ender_populate(Eon_Drawer_Widget *w, Egueb_Dom_Node *n)
{
	Eon_Drawer_Label *thiz;
	Egueb_Dom_Node *attr;
	Egueb_Dom_String *s;

	thiz = EON_DRAWER_LABEL(w);
	/* add the font attributes */
	attr = ender_attr_enesim_color_new("color",
			ENDER_ATTR_ENESIM_COLOR_GET(_eon_drawer_label_color_get),
			ENDER_ATTR_ENESIM_COLOR_SET(_eon_drawer_label_color_set));
	egueb_dom_element_attribute_add(n, attr, NULL);

	attr = ender_attr_font_new("font",
			ENDER_ATTR_FONT_GET(_eon_drawer_label_font_get),
			ENDER_ATTR_FONT_SET(_eon_drawer_label_font_set));
	/* set the default font */
	egueb_dom_element_attribute_add(n, attr, NULL);

	if (thiz->d->ender_populate)
		thiz->d->ender_populate(w, thiz->data, n);
}

static Eina_Bool _eon_drawer_label_ender_process(Eon_Drawer_Widget *w, Egueb_Dom_Node *n)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	if (!_eon_drawer_label_generate_font(thiz))
		return EINA_FALSE;
	_eon_drawer_label_generate_color(thiz);

	if (thiz->d->ender_process)
		return thiz->d->ender_process(w, thiz->data, n);
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_DRAWER_WIDGET_DESCRIPTOR,
		Eon_Drawer_Label, Eon_Drawer_Label_Class, eon_drawer_label);

static void _eon_drawer_label_class_init(void *k)
{
	Eon_Drawer_Widget_Class *klass;

	klass = EON_DRAWER_WIDGET_CLASS(k);
	klass->renderer_get = _eon_drawer_label_renderer_get;
	klass->geometry_set = _eon_drawer_label_geometry_set;
	klass->ender_populate = _eon_drawer_label_ender_populate;
	klass->ender_process = _eon_drawer_label_ender_process;
}

static void _eon_drawer_label_instance_init(void *o)
{
}

static void _eon_drawer_label_instance_deinit(void *o)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(o);
	if (thiz->d->free)
		thiz->d->free(EON_DRAWER_WIDGET(o), thiz->data);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Enesim_Text_Font * eon_drawer_label_text_font_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	if (!_eon_drawer_label_generate_font(thiz))
		return NULL;
	return enesim_text_font_ref(thiz->tf);
}

Enesim_Renderer * eon_drawer_label_text_renderer_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	if (thiz->d->text_renderer_get)
		return thiz->d->text_renderer_get(w, thiz->data);
	return NULL;
}

void eon_drawer_label_text_color_set(Eon_Drawer_Widget *w, Enesim_Color color)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	thiz->control_color = color;
	thiz->control_color_set = EINA_TRUE;
	thiz->color_changed = EINA_TRUE;
}

void eon_drawer_label_text_color_unset(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	thiz->control_color_set = EINA_FALSE;
	thiz->color_changed = EINA_TRUE;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new label drawer
 * @param d The set of functions the drawer implemenation must implement
 * @param data The private data passed in on the descriptor functions 
 */
EAPI Eon_Drawer_Widget * eon_drawer_label_new(
		const Eon_Drawer_Label_Descriptor *d, void *data)
{
	Eon_Drawer_Label *thiz;
	Eon_Drawer_Widget *w;

	w = ENESIM_OBJECT_INSTANCE_NEW(eon_drawer_label);
	thiz = EON_DRAWER_LABEL(w);
	thiz->d = d;
	thiz->data = data;
	thiz->font_changed = EINA_TRUE;

	return w;	
}

/**
 * Gets the private data of a label
 * @param w The label to get the data from
 * @return The private data
 */
EAPI void * eon_drawer_label_data_get(Eon_Drawer_Widget *w)
{
	Eon_Drawer_Label *thiz;

	thiz = EON_DRAWER_LABEL(w);
	return thiz->data;
}

/**
 * @brief Register the label implementation on the ender namespace
 * @param ns The ender namespace registered on the drawer imeplementation
 * @param ctor The function used to create a drawer implementation label
 */
EAPI void eon_drawer_label_ender_register(Ender_Namespace *ns,
		Ender_Instance_Descriptor_Ctor ctor)
{
	Ender_Instance_Descriptor d;

	d.ctor = ctor;
	d.dtor = ENDER_INSTANCE_DESCRIPTOR_DTOR(eon_drawer_widget_free);
	d.populate = ENDER_INSTANCE_DESCRIPTOR_POPULATE(eon_drawer_widget_ender_populate);
	d.process = ENDER_INSTANCE_DESCRIPTOR_PROCESS(eon_drawer_widget_ender_process);
	d.child_appendable = NULL;
	d.child_added = NULL;
	d.child_removed = NULL;

	ender_namespace_instance_register(ns, &d, "label");
}

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_drawer_label(Eon_Drawer_Widget *t)
{
	Eon_Drawer_Label *thiz;
	return EINA_TRUE;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * eon_drawer_label_data_get(Eon_Drawer_Widget *t)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_drawer_label_font_get(Eon_Drawer_Widget *t, const char **str)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.font_get)
		thiz->descriptor.font_get(t, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_drawer_label_font_set(Eon_Drawer_Widget *t, const char *str)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.font_set)
		thiz->descriptor.font_set(t, str);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_drawer_label_font_size_get(Eon_Drawer_Widget *t, int *size)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.font_size_get)
		thiz->descriptor.font_size_get(t, size);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_drawer_label_font_size_set(Eon_Drawer_Widget *t, int size)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.font_size_set)
		thiz->descriptor.font_size_set(t, size);
}

EAPI void eon_drawer_label_text_get(Eon_Drawer_Widget *t, const char **str)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.text_get)
		thiz->descriptor.text_get(t, str);
}

EAPI void eon_drawer_label_text_set(Eon_Drawer_Widget *t, const char *str)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.text_set)
		thiz->descriptor.text_set(t, str);
}

EAPI void eon_drawer_label_color_get(Eon_Drawer_Widget *t, Enesim_Color *color)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.color_get)
		thiz->descriptor.color_get(t, color);
}

EAPI void eon_drawer_label_color_set(Eon_Drawer_Widget *t, Enesim_Color color)
{
	Eon_Drawer_Label *thiz;

	thiz = _eon_drawer_label_get(t);
	if (thiz->descriptor.color_set)
		thiz->descriptor.color_set(t, color);
}
#endif
