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
#include "Eon.h"
#include "eon_private.h"
/* the etex span always strdup the string, we should add a way to share
 * a strbuffer between the internal state of the span and the API this way
 * we avoid useless copies
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Entry
{
	/* properties */
	int max_length;
	Eon_Horizontal_Alignment alignment;
	/* private */
	Ender_Element *label;
	Etex_Buffer *buffer;
	int num_entries;
} Eon_Entry;

static inline Eon_Entry * _eon_entry_get(Enesim_Renderer *r)
{
	Eon_Entry *thiz;

	thiz = eon_widget_data_get(r);
	return thiz;
}

static void _eon_entry_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Entry *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_entry_get(r);

	/* FIXME just an example for now, given that the key evenst are now managed yet */
	{
		static int offset = 0;
		etex_buffer_string_insert(thiz->buffer, "l>", -1, offset);
		offset += 2;
		eon_element_changed_set(e, EINA_TRUE);
	}
}

static void _eon_entry_key_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Entry *thiz;
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	thiz = _eon_entry_get(r);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_entry_initialize(Ender_Element *e)
{
	Eon_Entry *thiz;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;

	r = ender_element_renderer_get(e);
	thiz = _eon_entry_get(r);

	/* the events */
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
			_eon_entry_click, NULL);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_DOWN],
			_eon_entry_key_down, NULL);
	/* get the buffer from the theme renderer */
	theme_r = eon_widget_theme_renderer_get(r);
	thiz->buffer = eon_theme_entry_buffer_get(theme_r);
}

static Eina_Bool _eon_entry_setup(Ender_Element *e)
{
	Eon_Entry *thiz;
	Eon_Size size;
	Enesim_Renderer *r;
	Enesim_Renderer *theme_r;

	r = ender_element_renderer_get(e);
	thiz = _eon_entry_get(r);
	theme_r = eon_widget_theme_renderer_get(r);

	eon_element_actual_size_get(r, &size);

	return EINA_TRUE;
}

static void _eon_entry_free(Enesim_Renderer *r)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
	free(thiz);
}

static double _eon_entry_min_width_get(Ender_Element *e)
{
	Eon_Entry *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_entry_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_entry_min_width_get(theme_r, &v);

	return v;
}

static double _eon_entry_min_height_get(Ender_Element *e)
{
	Eon_Entry *thiz;
	Enesim_Renderer *theme_r;
	Enesim_Renderer *r;
	double v;

	r = ender_element_renderer_get(e);
	thiz = _eon_entry_get(r);

	theme_r = eon_widget_theme_renderer_get(r);
	eon_theme_entry_min_height_get(theme_r, &v);

	return v;
}

static double _eon_entry_preferred_width_get(Ender_Element *e)
{
	return DBL_MAX;
}

static Eon_Widget_Descriptor _eon_entry_widget_descriptor = {
	.name = "entry",
	.initialize = _eon_entry_initialize,
	.free = _eon_entry_free,
	.setup = _eon_entry_setup,
	.min_width_get = _eon_entry_min_width_get,
	.min_height_get = _eon_entry_min_height_get,
	.max_height_get = _eon_entry_min_height_get,
	.preferred_width_get = _eon_entry_preferred_width_get,
	.preferred_height_get = _eon_entry_min_height_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _eon_entry_new(void)
{
	Eon_Entry *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Entry));
	if (!thiz) return NULL;
	/* default values */
	thiz->max_length = 255;

	r = eon_widget_new(&_eon_entry_widget_descriptor, thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_entry_max_length_set(Enesim_Renderer *r, int max_length)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
	thiz->max_length = max_length;
}

static void _eon_entry_max_length_get(Enesim_Renderer *r, int *max_length)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
	*max_length = thiz->max_length;
}

static void _eon_entry_alignment_set(Enesim_Renderer *r, Eon_Horizontal_Alignment alignment)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
	thiz->alignment = alignment;
}

static void _eon_entry_alignment_get(Enesim_Renderer *r, Eon_Horizontal_Alignment *alignment)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
	*alignment = thiz->alignment;
}

static void _eon_entry_text_set(Enesim_Renderer *r, char *text)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
}

static void _eon_entry_text_get(Enesim_Renderer *r, char **text)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(r);
}

#include "eon_generated_entry.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_entry_new(void)
{
	return ender_element_new_with_namespace("entry", "eon");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_entry_max_length_set(Ender_Element *e, int max_length)
{
	ender_element_value_set(e, "max_length", max_length, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_entry_max_length_get(Ender_Element *e, int *max_length)
{
	ender_element_value_get(e, "max_length", max_length, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_entry_alignment_set(Ender_Element *e, Eon_Horizontal_Alignment alignment)
{
	ender_element_value_set(e, "alignment", alignment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_entry_alignment_get(Ender_Element *e, Eon_Horizontal_Alignment *alignment)
{
	ender_element_value_get(e, "alignment", alignment, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_entry_text_set(Ender_Element *e, const char *text)
{
	ender_element_value_set(e, "text", text, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_entry_text_get(Ender_Element *e, const char **text)
{
	ender_element_value_get(e, "text", text, NULL);
}
