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
#include "eon_private_main.h"

#include "eon_main.h"
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

void eon_types_init(void);

#if 0
/* for later */
static void _destructor_callback(Ender_Element *e, void *data)
{
	Enesim_Renderer *r;

	r = ender_element_object_get(e);
	if (!eon_is_element(r))
		return;
}
#endif

static void _constructor_callback(Ender_Element *e, void *data)
{
	if (!eon_is_element(e))
		return;
	eon_element_initialize(e);
}

static void _register_enders(void)
{
	/* register the dependency */
	ender_loader_load("enesim");
	/* core */
	eon_types_init();
	eon_element_init();
	/* element inheritance */
	eon_widget_init();
	eon_wrapper_init();
	eon_surface_init();
	/* widget inheritance */
	eon_container_init();
	eon_image_init();
	eon_label_init();
	eon_scrollbar_init();
	/* container inheritance */
	eon_bin_init();
	eon_stack_init();
	eon_list_init();
	eon_grid_init();
	eon_canvas_init();
	/* bin inheritance */
	eon_button_base_init();
	eon_scrollview_init();
	/* basic button inheritance */
	eon_button_init();
	eon_radio_init();
	eon_checkbox_init();
	/* extensions */
#if BUILD_EXTENSION_ESVG
	eon_svg_init();
#endif
#if 0
	/* widget inheritance */
	eon_compound_init();
	eon_container_init();
	eon_progressbar_init();
	eon_entry_init();
	eon_spin_init();
	/* layout inheritance */
	/* container inheritance */
	eon_splitter_init();
	/* basic button inheritance */
	eon_toggle_init();
	eon_color_init();
	eon_frame_init();
#endif
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eon_log = -1;

void eon_hints_geometry_align(Eon_Hints *hints, Eon_Geometry *g, Eon_Horizontal_Alignment halign, Eon_Vertical_Alignment valign)
{
	/* check the geometry against the last hints */
	if (g->width > hints->max.width)
	{
		double w = hints->preferred.width;
		double x = 0;

		printf("aligning width %g %g %g\n", g->width, hints->max.width, w);
		if (w < 0) w = hints->max.width;
		switch (halign)
		{
			case EON_HORIZONTAL_ALIGNMENT_RIGHT:
			x = g->width - w;
			break;

			case EON_HORIZONTAL_ALIGNMENT_CENTER:
			x = (g->width / 2) - (w / 2);
			break;

			default:
			break;
		}
		g->x += x;
		g->width = w;
	}
	if (g->height > hints->max.height)
	{
		double h = hints->preferred.height;
		double y = 0;

		printf("aligning height %g %g %g\n", g->height, hints->max.height, h);
		if (h < 0) h = hints->max.height;
		switch (valign)
		{
			case EON_VERTICAL_ALIGNMENT_BOTTOM:
			y = g->height - h;
			break;

			case EON_VERTICAL_ALIGNMENT_CENTER:
			y = (g->height / 2) - (h / 2);
			break;

			default:
			break;
		}
		g->y += y;
		g->height = h;
	}
}

Ender_Namespace * eon_namespace_get(void)
{
	static Ender_Namespace *namespace = NULL;

	if (!namespace)
	{
		namespace = ender_namespace_new("eon", 0);
	}
	return namespace;
}

#include "eon_generated_types.c"
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
		ender_init(NULL, NULL);
		_register_enders();
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

