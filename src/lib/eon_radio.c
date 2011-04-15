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
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Radio
{
	/* properties */
	char *group_name;
} Eon_Radio;

static inline Eon_Radio * _eon_radio_get(Enesim_Renderer *r)
{
	Eon_Radio *thiz;

	thiz = eon_container_data_get(r);
	return thiz;
}

static Eina_Hash *_groups = NULL;
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
EAPI Enesim_Renderer * eon_radio_new(void)
{
	Eon_Radio *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Eon_Radio));
	if (!thiz) return NULL;

	r = eon_container_new("radio", thiz);
	if (!r) goto renderer_err;

	return r;

renderer_err:
	free(thiz);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_group_name_set(Enesim_Renderer *r, const char *group)
{
	Eon_Radio *thiz;
	Eina_List *radios;
	Eina_List *l;

	thiz = _eon_radio_get(r);
	if (!thiz) return;

	if (!_groups)
	{
		_groups = eina_hash_string_superfast_new(NULL);
	}
	/* remove from the previous group */
	if (thiz->group_name)
	{
		radios = l = eina_hash_find(_groups, thiz->group_name);
		radios = eina_list_remove(radios, r);
		if (!radios)
			eina_hash_del(_groups, thiz->group_name, l);
		
	}
	/* append to the new group */
	thiz->group_name = strdup(group);
	radios = l = eina_hash_find(_groups, thiz->group_name);
	radios = eina_list_append(radios, r);
	if (!l)
	{
		eina_hash_add(_groups, thiz->group_name, radios);	
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_radio_group_name_get(Enesim_Renderer *r, const char **group)
{
	Eon_Radio *thiz;

	thiz = _eon_radio_get(r);
	if (!thiz) return;

	*group = thiz->group_name;
}
