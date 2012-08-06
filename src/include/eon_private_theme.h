#ifndef _EON_PRIVATE_THEME_H_
#define _EON_PRIVATE_THEME_H_

Eina_Bool eon_theme_init(void);
void eon_theme_shutdown(void);

/*
 * This is a wrapper of Escen, a simple interface to interact with the
 * underlying theme system
 */
typedef struct _Eon_Theme_File
{
	Escen *escen;
	char *file;
} Eon_Theme_File;

Eon_Theme_File * eon_theme_file_get(void);
Eon_Theme_File * eon_theme_file_get_from_file(const char *file);

typedef struct _Eon_Theme_Instance
{
	Eon_Theme_File *theme;
	Escen_Ender *ender;
	Escen_Instance *instance;
	Ender_Element *element;
	void *object;
	Enesim_Renderer *renderer;
} Eon_Theme_Instance;

Eon_Theme_Instance * eon_theme_instance_new(const char *name);
Eon_Theme_Instance * eon_theme_instance_new_from_file(Eon_Theme_File *file, const char *name);
void eon_theme_instance_property_set(Eon_Theme_Instance *thiz, const char *name, ...);
void eon_theme_instance_property_get(Eon_Theme_Instance *thiz, const char *name, ...);
void eon_theme_instance_property_add(Eon_Theme_Instance *thiz, const char *name, ...);
void eon_theme_instance_property_remove(Eon_Theme_Instance *thiz, const char *name, ...);
void eon_theme_instance_property_clear(Eon_Theme_Instance *thiz, const char *name);
void eon_theme_instance_state_set(Eon_Theme_Instance *thiz, const char *name, Eina_Bool be_finalized);
Escen_State * eon_theme_instance_state_get(Eon_Theme_Instance *thiz, const char *name);
Escen_Ender * eon_theme_instance_ender_get(Eon_Theme_Instance *thiz);
Enesim_Renderer * eon_theme_instance_renderer_get(Eon_Theme_Instance *thiz);
Ender_Element * eon_theme_instance_element_get(Eon_Theme_Instance *thiz);
Escen_Instance * eon_theme_instance_instance_get(Eon_Theme_Instance *thiz);

#endif
