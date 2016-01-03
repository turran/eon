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
#include "eon_private.h"
#include "eon_main.h"
#include "eon_element_expander.h"

#include "eon_feature_themable_private.h"
#include "eon_renderable_private.h"
#include "eon_theme_renderable_private.h"

/*
 * TODO:
 * + Add an "expanded" = "true|false" attribute
 * + Add an "orientation" attribute to expand to the right
 *   or down
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_ELEMENT_EXPANDER_DESCRIPTOR eon_element_expander_descriptor_get()
#define EON_ELEMENT_EXPANDER(o) ENESIM_OBJECT_INSTANCE_CHECK(o,		\
		Eon_Element_Expander, EON_ELEMENT_EXPANDER_DESCRIPTOR)

typedef struct _Eon_Element_Expander
{
	Eon_Renderable base;
	/* attributes */
	Egueb_Dom_Node *expanded;
	/* private */
	Egueb_Dom_Feature *theme_feature;
	Enesim_Renderer *proxy;
} Eon_Element_Expander;

typedef struct _Eon_Element_Expander_Class
{
	Eon_Renderable_Class base;
} Eon_Element_Expander_Class;

static void _eon_element_expander_calculate_child_hints(Eon_Renderable_Size *size,
		int *hints, Eon_Renderable_Size *chs, int chsm,
		Eon_Box *padding)
{
	if (chsm & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (chs->min_width >= 0)
			size->min_width = MAX(chs->min_width + padding->top + padding->bottom, size->min_width);
		if (chs->min_height >= 0)
			size->min_height += chs->min_height + padding->left + padding->right;
	}

	if (chsm & EON_RENDERABLE_HINT_PREFERRED)
	{
		ERR("TODO: Missing preferred calcs");
	}
}

static Eina_Bool _eon_element_expander_calculate_child_size(Eina_Rectangle *size,
		Eina_Rectangle *chsize, Eon_Renderable_Size *chs, int chsm, Eon_Box *chp)
{
	Eina_Bool done = EINA_FALSE;

	chsize->x = size->x;
	chsize->y = size->y;
	chsize->w = size->w;
	chsize->h = 0;
	if (chsm & EON_RENDERABLE_HINT_MIN_MAX)
	{
		if (chs->min_height >= 0)
		{
			chsize->h = chs->min_height + chp->top + chp->bottom;
			done = EINA_TRUE;
		}
	}

	if (!done && (chsm & EON_RENDERABLE_HINT_PREFERRED))
	{
		if (chs->pref_height >= 0)
		{
			chsize->h = chs->pref_height + chp->top + chp->bottom;
			done = EINA_TRUE;
		}
	}
	return done;
}
/*----------------------------------------------------------------------------*
 *                           Renderable interface                             *
 *----------------------------------------------------------------------------*/
static void _eon_element_expander_init(Eon_Renderable *r)
{
	Eon_Element_Expander *thiz;
	Eon_Element *e;
	Egueb_Dom_Node *n;
	Egueb_Dom_Event_Target *et;

	thiz = EON_ELEMENT_EXPANDER(r);
	/* attributes */
	thiz->expanded = egueb_dom_attr_boolean_new(
			egueb_dom_string_ref(EON_NAME_ATTR_ACTIVATED),
			EINA_TRUE, EINA_TRUE, EINA_FALSE);
	egueb_dom_attr_set(thiz->expanded, EGUEB_DOM_ATTR_TYPE_DEFAULT, EINA_FALSE);
	n = (EON_ELEMENT(r))->n;
	egueb_dom_element_attribute_node_set(n,
			egueb_dom_node_ref(thiz->expanded), NULL);
	/* events */
	et = EGUEB_DOM_EVENT_TARGET(n);
	/* private */
	thiz->proxy = enesim_renderer_proxy_new();
	thiz->theme_feature = eon_feature_themable_add(n);
	e = EON_ELEMENT(r);
	egueb_dom_attr_string_list_append(e->theme_id, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			egueb_dom_string_ref(EON_NAME_ELEMENT_EXPANDER));
}

static Egueb_Dom_Node * _eon_element_expander_element_at(Eon_Renderable *r,
		Eina_Rectangle *cursor)
{
	return NULL;
}

static Enesim_Renderer * _eon_element_expander_renderer_get(Eon_Renderable *r)
{
	Eon_Element_Expander *thiz;

	thiz = EON_ELEMENT_EXPANDER(r);
	return enesim_renderer_ref(thiz->proxy);
}

static int _eon_element_expander_size_hints_get(Eon_Renderable *r,
		Eon_Renderable_Size *size)
{
	Eon_Element_Expander *thiz;
	Egueb_Dom_Node *ch1;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *theme_element;
	Enesim_Renderer *ren;
	int ch1sm;
	int ret = 0;

	thiz = EON_ELEMENT_EXPANDER(r);
	theme_element = eon_feature_themable_load(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return 0;
	}

	/* Get the children and its hints */
	n = (EON_ELEMENT(r))->n;
	ch1 = egueb_dom_element_child_first_get(n);
	if (ch1)
	{
		Eon_Renderable_Size ch1s;
		Eon_Box ch1p;
		Egueb_Dom_Node *ch2;

		/* Once we do have a child, we can be of any max size */
		ret |= EON_RENDERABLE_HINT_MIN_MAX;
		size->max_width = size->max_height = -1;

		/* Get the related theme information */
		eon_theme_element_expander_first_padding_get(theme_element, &ch1p);

		/* Add the padding on each hint */
		ch1sm = eon_renderable_size_hints_get(ch1, &ch1s);
		_eon_element_expander_calculate_child_hints(size, &ret, &ch1s,
				ch1sm, &ch1p);

		ch2 = egueb_dom_element_sibling_next_get(ch1);
		if (ch2)
		{
			Eon_Renderable_Size ch2s;
			Eon_Box ch2p;
			int ch2sm;

			/* Get the related theme information */
			eon_theme_element_expander_second_padding_get(theme_element, &ch2p);
			ch2sm = eon_renderable_size_hints_get(ch2, &ch2s);
			_eon_element_expander_calculate_child_hints(size, &ret,
					&ch2s, ch2sm, &ch2p);
			egueb_dom_node_unref(ch2);
		}
		egueb_dom_node_unref(ch1);
	}

	/* set the proxied renderer */
	ren = eon_theme_renderable_renderer_get(theme_element);
	enesim_renderer_proxy_proxied_set(thiz->proxy, ren);
	egueb_dom_node_unref(theme_element);

	return ret;
}

static Eina_Bool _eon_element_expander_process(Eon_Renderable *r)
{
	Eon_Element_Expander *thiz;
	Eon_Renderable_Size ch1s;
	Egueb_Dom_Node *theme_element;
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *ch1;
	int ch1sm;

	thiz = EON_ELEMENT_EXPANDER(r);
	/* get the theme */
	theme_element = eon_feature_themable_element_get(thiz->theme_feature);
	if (!theme_element)
	{
		WARN("No theme element found");
		return EINA_FALSE;
	}

	n = (EON_ELEMENT(r))->n;

	/* Set the geometry on the theme */
	eon_theme_renderable_geometry_set(theme_element, &r->geometry);

	/* Set the geometry on every child */
	ch1 = egueb_dom_element_child_first_get(n);
	if (ch1)
	{
		Eon_Renderable_Size ch2s;
		Eon_Box ch1p = { 0, 0, 0, 0 };
		Eon_Box ch2p = { 0, 0, 0, 0 };
		Egueb_Dom_Node *ch2;
		Enesim_Renderer *ren;
		Eina_Rectangle ch1fs, ch2fs;
		Eina_Rectangle free_space;
		Eina_Bool ch1done, ch2done = EINA_FALSE;
		int notdone = 0;
		int ch2sm;

		eon_theme_element_expander_first_padding_get(theme_element, &ch1p);

		/* Calculate the size */
		ch1sm = eon_renderable_size_hints_get(ch1, &ch1s);

		free_space = r->geometry;
		ch1done = _eon_element_expander_calculate_child_size(&free_space, &ch1fs,
				&ch1s, ch1sm, &ch1p);
		/* In case it did not suceeded (no hints at all, or infinite), save for later
		 * othersize, advance */
		if (ch1done)
		{
			Eina_Bool vexpand;

			free_space.h -= ch1fs.h;
			free_space.y += ch1fs.h;

			vexpand = eon_renderable_vexpand_get(ch1);
			/* in case it can grow, go ahead and mark it for later */
			if (vexpand)
				ch1done = EINA_FALSE;
		}

		if (!ch1done)
			notdone++;

		ch2 = egueb_dom_element_sibling_next_get(ch1);
		if (ch2)
		{
			eon_theme_element_expander_second_padding_get(theme_element, &ch2p);

			/* Calculate the size */
			ch2sm = eon_renderable_size_hints_get(ch2, &ch2s);

			ch2done = _eon_element_expander_calculate_child_size(&free_space, &ch2fs,
					&ch2s, ch2sm, &ch2p);

			/* In case it did not suceeded (no hints at all, or infinite), save for later
			 * othersize, advance */
			if (ch2done)
			{
				Eina_Bool vexpand;

				free_space.h -= ch1fs.h;

				vexpand = eon_renderable_vexpand_get(ch2);
				/* in case it can grow, go ahead and mark it for later */
				if (vexpand)
					ch2done = EINA_FALSE;
			}
			if (!ch2done)
				notdone++;
		}
		else
		{
			eon_theme_element_expander_second_content_set(theme_element, NULL, NULL);
		}

		/* Further calculation if we need to */
		if (notdone)
		{
			int h = free_space.h / notdone;

			ERR_ELEMENT(n, "We have %d of free space for %d child", free_space.h, notdone);
			if (!ch1done)
			{
				ch1fs.h += h;
				if (ch2)
				{
					ch2fs.y += h;
				}
			}

			if (ch2 && !ch2done)
			{
				ch2fs.h += h;
			}
			free_space.h = 0;
		}

		if (free_space.h)
		{
			int h = free_space.h / 2;

			ch1fs.h += h;
			if (ch2)
			{
				ch2fs.y += h;
				ch2fs.h += h;
			}
		}

		/* finally process */
		if (ch2)
		{
			/* Set the renderer */
			ren = eon_renderable_renderer_get(ch2);
			eon_theme_element_expander_second_content_set(theme_element, ren, &ch2fs);

			/* finally process */
			ch2fs.x += ch2p.left;
			ch2fs.y += ch2p.top;
			ch2fs.w -= ch2p.left + ch2p.right;
			ch2fs.h -= ch2p.top + ch2p.bottom;
			eon_renderable_geometry_set(ch2, &ch2fs);
			egueb_dom_element_process(ch2);
			egueb_dom_node_unref(ch2);
		}

		/* Set the renderer */
		ren = eon_renderable_renderer_get(ch1);
		eon_theme_element_expander_first_content_set(theme_element, ren, &ch1fs);

		ch1fs.x += ch1p.left;
		ch1fs.y += ch1p.top;
		ch1fs.w -= ch1p.left + ch1p.right;
		ch1fs.h -= ch1p.top + ch1p.bottom;
		eon_renderable_geometry_set(ch1, &ch1fs);
		egueb_dom_element_process(ch1);
		egueb_dom_node_unref(ch1);
	}
	else
	{
		eon_theme_element_expander_first_content_set(theme_element, NULL, NULL);
		eon_theme_element_expander_second_content_set(theme_element, NULL, NULL);
	}

	/* Finally process our theme */
	egueb_dom_element_process(theme_element);
	egueb_dom_node_unref(theme_element);

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _eon_element_expander_tag_name_get(Eon_Element *e)
{
	return egueb_dom_string_ref(EON_NAME_ELEMENT_EXPANDER);
}

static Eina_Bool _eon_element_expander_child_appendable(Eon_Element *e, Egueb_Dom_Node *child)
{
	Egueb_Dom_Node *n;
	Egueb_Dom_Node *curr_child;
	int count = 0;

	/* only accept one child */
	if (!eon_is_renderable(child))
		return EINA_FALSE;

	/* only two children */
	n = e->n;
	curr_child = egueb_dom_element_child_first_get(n);
	while (curr_child)
	{
		Egueb_Dom_Node *tmp;

		count++;
		tmp = egueb_dom_element_sibling_next_get(curr_child);
		egueb_dom_node_unref(curr_child);
		curr_child = tmp;
	}
	if (count >= 2)
	{
		WARN("Only two child supported");
		return EINA_FALSE;
	}

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EON_RENDERABLE_DESCRIPTOR, Eon_Element_Expander,
		Eon_Element_Expander_Class, eon_element_expander);

static void _eon_element_expander_class_init(void *k)
{
	Eon_Element_Class *klass;
	Eon_Renderable_Class *r_klass;

	klass = EON_ELEMENT_CLASS(k);
	klass->tag_name_get = _eon_element_expander_tag_name_get;
	klass->child_appendable = _eon_element_expander_child_appendable;

	r_klass = EON_RENDERABLE_CLASS(k);
	r_klass->init = _eon_element_expander_init;
	r_klass->element_at = _eon_element_expander_element_at;
	r_klass->renderer_get = _eon_element_expander_renderer_get;
	r_klass->size_hints_get = _eon_element_expander_size_hints_get;
	r_klass->process = _eon_element_expander_process;
	r_klass->child_size_dependant = EINA_TRUE;
}

static void _eon_element_expander_instance_init(void *o)
{
}

static void _eon_element_expander_instance_deinit(void *o)
{
	Eon_Element_Expander *thiz;

	thiz = EON_ELEMENT_EXPANDER(o);
	/* attributes */
	egueb_dom_node_unref(thiz->expanded);
	/* private */
	enesim_renderer_unref(thiz->proxy);
	egueb_dom_feature_unref(thiz->theme_feature);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * eon_element_expander_new(void)
{
	Egueb_Dom_Node *n;
	n = EON_ELEMENT_NEW(eon_element_expander);
	return n;
}
