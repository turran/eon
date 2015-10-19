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
#include "eon_theme_private.h"
#include "eon_theme_namespace.h"
#include "eon_theme_main_private.h"
#include "eon_theme_namespace_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#if BUILD_STATIC_MODULE_THEME_MARS
Eina_Bool eon_theme_mars_init(void);
void eon_theme_mars_shutdown(void);
#endif

static Eina_Hash *_namespaces = NULL;
static Eina_Array *_modules = NULL;

struct _Eon_Theme_Namespace
{
	Eina_Hash *elements;
};

static void _eon_theme_namespace_free(Eon_Theme_Namespace *thiz)
{
	eina_hash_free(thiz->elements);
	free(thiz);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eon_theme_namespace_init(void)
{
	_namespaces = eina_hash_string_superfast_new(
			EINA_FREE_CB(_eon_theme_namespace_free));
	_modules = eina_module_list_get(_modules, PACKAGE_LIB_DIR"/eon/theme/", 1, NULL, NULL);
	eina_module_list_load(_modules);
#if BUILD_STATIC_MODULE_THEME_MARS
	eon_theme_mars_init();
#endif
}

void eon_theme_namespace_shutdown(void)
{
#if BUILD_STATIC_MODULE_THEME_MARS
	eon_theme_mars_shutdown();
#endif
	eina_hash_free(_namespaces);
	eina_module_list_free(_modules);
	eina_array_free(_modules);
}

Eon_Theme_Namespace * eon_theme_namespace_find(const char *name)
{
	return eina_hash_find(_namespaces, name);
}

Egueb_Dom_Node * eon_theme_namespace_element_ctor(Eon_Theme_Namespace *thiz,
		const char *name)
{
	Eon_Theme_Namespace_Element_Ctor ctor;
	Egueb_Dom_Node *ret;

	ctor = eina_hash_find(thiz->elements, name);
	if (!ctor)
		return NULL;
	ret = ctor();
	return ret;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eon_Theme_Namespace * eon_theme_namespace_register(const char *ns_uri)
{
	Eon_Theme_Namespace *thiz;

	thiz = eina_hash_find(_namespaces, ns_uri);
	if (thiz)
	{
		WRN("Namespace '%s' already found, using it", ns_uri);
		return thiz;
	}

	INF("Registering namespace '%s'", ns_uri);
	thiz = calloc(1, sizeof(Eon_Theme_Namespace));
	thiz->elements = eina_hash_string_superfast_new(NULL);
	eina_hash_add(_namespaces, ns_uri, thiz);
	return thiz;
}

EAPI void eon_theme_namespace_element_add(Eon_Theme_Namespace *thiz,
		const char *name, Eon_Theme_Namespace_Element_Ctor ctor)
{
	if (eina_hash_find(thiz->elements, name))
	{
		WRN("Element with name '%s' already found, not adding", name);
		return;
	}
	eina_hash_add(thiz->elements, name, ctor);
}
