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
#include "Eon_Theme.h"
#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *EON_ENTRY_ALIGNMENT;
static Ender_Property *EON_ENTRY_MAX_LENGTH;
static Ender_Property *EON_ENTRY_TEXT;

typedef struct _Eon_Entry
{
	/* properties */
	int max_length;
	Eon_Horizontal_Alignment alignment;
	Eina_Bool enabled;
	/* private */
	Ender_Element *label;
	Etex_Buffer *buffer;
	int num_entries;
} Eon_Entry;

static inline Eon_Entry * _eon_entry_get(Eon_Element *ee)
{
	Eon_Entry *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}

static void _eon_entry_click(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Event_Mouse_Click *ev = event_data;

	eon_input_focus_set(ev->input, e);
}

static void _eon_entry_key_down(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Eon_Entry *thiz;
	Eon_Element *ee;
	Eon_Event_Key_Down *ev = event_data;
	Enesim_Renderer *theme_r;

	ee = ender_element_object_get(e);
	thiz = _eon_entry_get(ee);

	printf("entry key down! %s\n", ev->key->name);
	theme_r = eon_widget_theme_renderer_get(ee);
	/* TODO get the position of the cursor */
	etex_buffer_string_insert(thiz->buffer, ev->key->name, -1, 0);
	/* TODO inform the position of the cursor */
	eon_theme_entry_buffer_needs_setup(theme_r);
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_entry_initialize(Ender_Element *e)
{
	Eon_Entry *thiz;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;

	ee = ender_element_object_get(e);
	thiz = _eon_entry_get(ee);

	/* the events */
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_MOUSE_CLICK],
			_eon_entry_click, NULL);
	ender_event_listener_add(e,
			eon_input_event_names[EON_INPUT_EVENT_KEY_DOWN],
			_eon_entry_key_down, NULL);
	/* get the buffer from the theme renderer */
	theme_r = eon_widget_theme_renderer_get(ee);
	thiz->buffer = eon_theme_entry_buffer_get(theme_r);
}

static Eina_Bool _eon_entry_setup(Ender_Element *e,
		const Eon_Element_State *state,
		Enesim_Surface *s, Enesim_Error **err)
{
	Eon_Entry *thiz;
	Eon_Element *ee;
	Enesim_Renderer *theme_r;

	ee = ender_element_object_get(e);
	thiz = _eon_entry_get(ee);
	theme_r = eon_widget_theme_renderer_get(ee);

	return EINA_TRUE;
}

static void _eon_entry_free(Eon_Element *ee)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(ee);
	free(thiz);
}

static double _eon_entry_min_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_entry_min_width_get(theme_r, &v);

	return v;
}

static double _eon_entry_min_height_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	double v;

	eon_theme_entry_min_height_get(theme_r, &v);

	return v;
}

static double _eon_entry_preferred_width_get(Ender_Element *e, Enesim_Renderer *theme_r)
{
	return -1;
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
static Eon_Element * _eon_entry_new(void)
{
	Eon_Entry *thiz;
	Eon_Element *ee;
	Eon_Keyboard_Proxy *kp;

	thiz = calloc(1, sizeof(Eon_Entry));
	if (!thiz) return NULL;
	/* default values */
	thiz->max_length = 255;
	thiz->enabled = EINA_TRUE;

	ee = eon_widget_new(&_eon_entry_widget_descriptor, thiz);
	if (!ee) goto renderer_err;

	kp = eon_keyboard_proxy_focus_new();
        eon_element_keyboard_proxy_set(ee, kp);

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_entry_max_length_set(Eon_Element *ee, int max_length)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(ee);
	thiz->max_length = max_length;
}

static void _eon_entry_max_length_get(Eon_Element *ee, int *max_length)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(ee);
	*max_length = thiz->max_length;
}

static void _eon_entry_alignment_set(Eon_Element *ee, Eon_Horizontal_Alignment alignment)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(ee);
	thiz->alignment = alignment;
	eon_widget_property_set(ee, "alignment", alignment, NULL);
}

static void _eon_entry_alignment_get(Eon_Element *ee, Eon_Horizontal_Alignment *alignment)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(ee);
	*alignment = thiz->alignment;
}

static void _eon_entry_text_set(Eon_Element *ee, char *text)
{
	Eon_Entry *thiz;
	Enesim_Renderer *theme_r;

	thiz = _eon_entry_get(ee);
	etex_buffer_string_set(thiz->buffer, text, -1);
	theme_r = eon_widget_theme_renderer_get(ee);
	eon_theme_entry_buffer_needs_setup(theme_r);
}

static void _eon_entry_text_get(Eon_Element *ee, char **text)
{
	Eon_Entry *thiz;

	thiz = _eon_entry_get(ee);
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
