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
/**
 * @todo
 * Add our own image loader for escen based files, which should use a common
 * implementation found on eon_image_loader.c?
 * For some reason the eon_init is being called twice, this might be related
 * to escen loading eon and initializing it twice :-/
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init_count = 0;

#if 0
/* for later */
static void _destructor_callback(Ender_Element *e, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (!eon_is_element(r))
		return;
}
#endif

static void _constructor_callback(Ender_Element *e, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_renderer_get(e);
	if (!eon_is_element(r))
		return;
	eon_element_initialize(e);
}

static void _register_enders(void *data)
{
	/* register the dependency */
	ender_loader_load("enesim");
	/* core */
	eon_element_init();
	/* element inheritance */
	eon_widget_init();
	eon_layout_init();
	eon_wrapper_init();
	eon_image_init();
	/* widget inheritance */
	eon_container_init();
	eon_label_init();
	eon_scrollbar_init();
	eon_progressbar_init();
	eon_entry_init();
	eon_spin_init();
	/* layout inheritance */
	eon_canvas_init();
	eon_stack_init();
	/* container inheritance */
	eon_button_base_init();
	eon_splitter_init();
	eon_scrollview_init();
	/* basic button inheritance */
	eon_button_init();
	eon_radio_init();
	eon_checkbox_init();
	eon_toggle_init();
	eon_color_init();
	eon_frame_init();
	/* extensions */
	eon_svg_init();
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_log = -1;

Ender_Namespace * eon_namespace_get(void)
{
	static Ender_Namespace *namespace = NULL;

	if (!namespace)
	{
		namespace = ender_namespace_new("eon");
	}
	return namespace;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
EAPI int eon_init(void)
{
	_init_count++;
	if (_init_count == 1)
	{
		eina_init();
		eon_log = eina_log_domain_register("eon", NULL);
		enesim_init();
		ender_loader_registry_callback_add(_register_enders, NULL);
		ender_init(NULL, NULL);
		ender_element_new_listener_add(_constructor_callback, NULL);
		escen_init();
		/* initialize the theme */
		if (!eon_theme_init())
		{
			escen_shutdown();
			ender_element_new_listener_remove(_constructor_callback, NULL);
			ender_shutdown();
			enesim_shutdown();
			eina_log_domain_unregister(eon_log);
			eina_shutdown();

			return --_init_count;
		}
	}
	return _init_count;
}

/**
 *
 */
EAPI void eon_shutdown(void)
{
	if (_init_count == 1)
	{
		escen_shutdown();
		ender_element_new_listener_remove(_constructor_callback, NULL);
		ender_shutdown();
		enesim_shutdown();
		eina_log_domain_unregister(eon_log);
		eina_shutdown();
	}
	_init_count--;
}

/**
 *
 */
EAPI void eon_version(unsigned int *major, unsigned int *minor, unsigned int *micro)
{
	if (major) *major = VERSION_MAJOR;
	if (minor) *minor = VERSION_MINOR;
	if (micro) *micro = VERSION_MICRO;
}

