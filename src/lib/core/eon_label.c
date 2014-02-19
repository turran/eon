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
#include "eon_backend.h"
#include "eon_input.h"
#include "eon_element.h"
#include "eon_widget.h"
#include "eon_label.h"

#include "eon_private_input.h"
#include "eon_private_element.h"
#include "eon_private_theme.h"
#include "eon_private_widget.h"

#include "theme/eon_theme_widget.h"
#include "theme/eon_theme_label.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_LOG_DEFAULT _eon_label_log

static int _eon_label_log = -1;

static Ender_Property *EON_LABEL_ELLIPSIZE;
static Ender_Property *EON_LABEL_TEXT;

static inline Eon_Label * _eon_label_get(Eon_Element *ee)
{
	Eon_Label *thiz;

	thiz = eon_widget_data_get(ee);
	return thiz;
}
/*----------------------------------------------------------------------------*
 *                         The Eon's widget interface                         *
 *----------------------------------------------------------------------------*/
static void _eon_label_free(Eon_Element *e)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(e);
	free(thiz);
}

static Eon_Widget_Descriptor _eon_label_descriptor = {
	/* .initialize 		= */ NULL,
	/* .backend_added 	= */ NULL,
	/* .backend_removed 	= */ NULL,
	/* .geometry_set 	= */ NULL,
	/* .free		= */ _eon_label_free,
	/* .name 		= */ "label",
	/* .hints_get 		= */ _eon_label_hints_get,
};
/*----------------------------------------------------------------------------*
 *                       The Ender descriptor functions                       *
 *----------------------------------------------------------------------------*/
static Eon_Element * _eon_label_new(void)
{
	Eon_Label *thiz;
	Eon_Element *ee;
	Eon_Theme_Instance *theme;

	theme = eon_theme_instance_new("label", EINA_TRUE);
	if (!theme) return NULL;

	thiz = calloc(1, sizeof(Eon_Label));
	thiz->ellipsize = EINA_FALSE;

	ee = eon_widget_new(theme, &_eon_label_descriptor, thiz);
	if (!ee) goto renderer_err;

	return ee;

renderer_err:
	free(thiz);
	return NULL;
}

static void _eon_label_text_set(Eon_Element *e, const char *text)
{
	Eon_Theme_Instance *theme;

	theme = eon_widget_theme_instance_get(e);
	eon_theme_label_text_set(theme->object, text);
}

static void _eon_label_text_get(Eon_Element *e, const char **text)
{
	Eon_Theme_Instance *theme;

	theme = eon_widget_theme_instance_get(e);
	eon_theme_label_text_get(theme->object, text);
}

static void _eon_label_ellipsize_set(Eon_Element *ee, Eina_Bool enable)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
	thiz->ellipsize = enable;
}

static void _eon_label_ellipsize_get(Eon_Element *ee, Eina_Bool *enabled)
{
	Eon_Label *thiz;

	if (!enabled) return;

	thiz = _eon_label_get(ee);
	*enabled = thiz->ellipsize;
}

#define _eon_label_delete NULL
#include "eon_generated_label.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_label_init(void)
{
	_eon_label_log = eina_log_domain_register("eon_label", EON_LOG_COLOR_DEFAULT);
	if (_eon_label_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_eon_label_init();
}

void eon_label_shutdown(void)
{
	if (_eon_label_log < 0)
		return;
	_eon_label_shutdown();
	eina_log_domain_unregister(_eon_label_log);
	_eon_label_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_label_new(void)
{
	return EON_ELEMENT_NEW("label");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * eon_label_new_with_text(const char *text)
{
	Ender_Element *e;

	e = eon_label_new();
	if (!e) return NULL;

	eon_label_text_set(e, text);
	return e;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_set(Ender_Element *e, const char *text)
{
	ender_element_property_value_set(e, EON_LABEL_TEXT, text, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_get(Ender_Element *e, const char **text)
{
	Eon_Element *ee;

	ee = ender_element_object_get(e);
	_eon_label_text_get(ee, text);
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_ellipsize_set(Ender_Element *e, Eina_Bool enable)
{
	ender_element_value_set(e, "ellipsize", enable, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_ellipsize_get(Ender_Element *e, Eina_Bool *enable)
{
	ender_element_value_get(e, "ellipsize", enable, NULL);
}

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_orientation_set(Ender_Element *e, Enesim_Text_Direction orientation)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
	eon_widget_property_set(ee, "orientation", orientation);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_label_text_orientation_get(Ender_Element *e, Enesim_Text_Direction *orientation)
{
	Eon_Label *thiz;

	thiz = _eon_label_get(ee);
	eon_widget_property_get(ee, "orientation", orientation);
}
#endif
