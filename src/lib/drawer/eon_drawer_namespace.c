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
#include "eon_drawer_private.h"
#include "eon_drawer_instance.h"
#include "eon_drawer_namespace.h"
#include "eon_drawer_namespace_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Eon_Drawer_Namespace
{
	char *name;
	Eina_Hash *descriptors;
};

static Eina_Hash *_namespaces = NULL;

static void _eon_drawer_namespace_free(void *data)
{
	Eon_Drawer_Namespace *thiz = data;

	if (thiz->name)
		free(thiz->name);
	eina_hash_free(thiz->descriptors);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_drawer_namespace_init(void)
{
	_namespaces = eina_hash_string_superfast_new(_eon_drawer_namespace_free);
}

void eon_drawer_namespace_shutdown(void)
{
	eina_hash_free(_namespaces);
}

const Eon_Drawer_Instance_Descriptor * eon_drawer_namespace_instance_find(
		const Eon_Drawer_Namespace *thiz, const char *name)
{
	if (!thiz) return NULL;
	DBG("Looking for instance '%s' on namespace '%s'", name, thiz->name);
	return eina_hash_find(thiz->descriptors, name); 
}

Eina_Bool eon_drawer_namespace_instance_register(Eon_Drawer_Namespace *thiz,
		const Eon_Drawer_Instance_Descriptor *descriptor, const char *name)
{
	Eon_Drawer_Instance_Descriptor *d;

	if (!thiz) return EINA_FALSE;
	if (!descriptor) return EINA_FALSE;
	if (!name) return EINA_FALSE;

	DBG("Registering instance '%s' on namespace '%s'", name, thiz->name);
	d = eina_hash_find(thiz->descriptors, name);
	if (d) return EINA_FALSE;

	d = calloc(1, sizeof(Eon_Drawer_Instance_Descriptor));
	*d = *descriptor;

	eina_hash_add(thiz->descriptors, name, d);
	return EINA_TRUE;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eon_Drawer_Namespace * eon_drawer_namespace_register(const char *name)
{
	Eon_Drawer_Namespace *thiz;

	DBG("Registering namespace '%s'", name);
	if (!name) return NULL;

	thiz = eina_hash_find(_namespaces, name);
	if (thiz) return thiz;

	thiz = calloc(1, sizeof(Eon_Drawer_Namespace));
	thiz->name = strdup(name);
	thiz->descriptors = eina_hash_string_superfast_new(EINA_FREE_CB(free));

	/* add it */
	eina_hash_add(_namespaces, name, thiz);

	return thiz;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_drawer_namespace_unregister(Eon_Drawer_Namespace *thiz)
{
	if (!thiz) return;
	DBG("Unregistering namespace '%s'", thiz->name);
	eina_hash_del(_namespaces, thiz->name, thiz);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI const Eon_Drawer_Namespace * eon_drawer_namespace_find(const char *name)
{
	DBG("Looking for namespace '%s'", name);
	if (!name) return NULL;
	return eina_hash_find(_namespaces, name);
}


