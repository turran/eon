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
#include "eon_theme.h"

#include "eon_theme_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*
 * <string>, currentDocument, environment
 */
Eina_Bool eon_theme_string_from(Eon_Theme *paint, const char *attr)
{
	/* currentDocument */
	if (!strcmp(attr, "currentDocument"))
	{
		paint->type = EON_THEME_TYPE_CURRENT_DOCUMENT;
	}
	/* currentColor */
	else if (!strcmp(attr, "environment"))
	{
		paint->type = EON_THEME_TYPE_ENVIRONMENT;
	}
	/* custom */
	else
	{
		paint->type = EON_THEME_TYPE_CUSTOM;
		paint->custom = egueb_dom_string_new_with_string(attr);
	}

	return EINA_TRUE;
}

char * eon_theme_string_to(Eon_Theme *thiz)
{
	switch (thiz->type)
	{
		case EON_THEME_TYPE_CURRENT_DOCUMENT:
		return strdup("currentDocument");
		break;

		case EON_THEME_TYPE_ENVIRONMENT:
		return strdup("environment");
		break;

		case EON_THEME_TYPE_CUSTOM:
		return strdup(egueb_dom_string_string_get(thiz->custom));
		break;

		default:
		break;
	}
	return NULL;
}

void eon_theme_interpolate(Eon_Theme *v,
		Eon_Theme *a, Eon_Theme *b, double m,
		Eon_Theme *add, Eon_Theme *acc, int mul)
{
}

Eina_Bool eon_theme_is_equal(const Eon_Theme *p1,
		const Eon_Theme *p2)
{
	/* sanity checks */
	if (p1 == p2) return EINA_TRUE;
	if (!p1) return EINA_FALSE;
	if (!p2) return EINA_FALSE;

	if (p1->type != p2->type) return EINA_FALSE;
	/* ok, we have values and same types, now compare each type */
	switch (p1->type)
	{
		case EON_THEME_TYPE_CURRENT_DOCUMENT:
		case EON_THEME_TYPE_ENVIRONMENT:
		return EINA_TRUE;

		case EON_THEME_TYPE_CUSTOM:
		return egueb_dom_string_is_equal(p1->custom, p2->custom);

		default:
		return EINA_FALSE;
	}
}

void eon_theme_copy(const Eon_Theme *thiz, Eon_Theme *copy, Eina_Bool full)
{
	eon_theme_reset(copy);

	/* common */
	copy->type = thiz->type;
	if (thiz->type == EON_THEME_TYPE_CUSTOM)
	{
		if (thiz->custom)
		{
			if (full)
				copy->custom = egueb_dom_string_dup(thiz->custom);
			else
				copy->custom = egueb_dom_string_ref(thiz->custom);
		}
	}
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void eon_theme_reset(Eon_Theme *thiz)
{
	if (thiz->type == EON_THEME_TYPE_CUSTOM)
	{
		if (thiz->custom)
		{
			egueb_dom_string_unref(thiz->custom);
			thiz->custom = NULL;
		}
	}
}

