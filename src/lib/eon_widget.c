#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Widget
{
	void *data; /* the data provided by the widget types */
	Escen_Ender *escen_ender;
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = enesim_renderer_data_get(r);
	return e;
}
/*----------------------------------------------------------------------------*
 *                      The Enesim's renderer interface                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _eon_widget_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	/* get the ender from the escen ender and get the fill function */
	return EINA_TRUE;
}

static void _eon_widget_cleanup(Enesim_Renderer *r)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);

}

static void _eon_widget_free(Enesim_Renderer *r)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);

}

static void _eon_widget_boundings(Enesim_Renderer *r, Eina_Rectangle *rect)
{
	Eon_Widget *ew;

	ew = _eon_widget_get(r);
}

static Enesim_Renderer_Descriptor _eon_widget_descriptor = {
	.sw_setup = _eon_widget_setup,
	.sw_cleanup = _eon_widget_cleanup,
	.boundings = _eon_widget_boundings,
	.free = _eon_widget_free,
};

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
EAPI Enesim_Renderer * eon_widget_new(const char *name, void *data)
{
	Eon_Widget *e;
	Enesim_Renderer *thiz;
	Enesim_Renderer_Descriptor descriptor;
	Enesim_Renderer_Flags flags;
	Escen *escen;
	Escen_Ender *escen_ender;
	Enesim_Renderer *escen_renderer;

	e = calloc(1, sizeof(Eon_Widget));
	e->data = data;

	/* get the flags from the theme */
	escen = escen_parser_load(PACKAGE_DATA_DIR "/theme.escen");
	escen_ender = escen_ender_get(escen, name);
	if (!escen_ender) goto renderer_err;
	escen_renderer = ender_renderer_get(escen_ender_ender_get(escen_ender));
	enesim_renderer_flags(escen_renderer, &flags);
	thiz = enesim_renderer_new(descriptor, flags, e);
	if (!thiz) goto renderer_err;

	return thiz;

renderer_err:
	free(e);
	return NULL;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool eon_is_widget(Enesim_Renderer *r)
{

}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void eon_widget_theme_set(Enesim_Renderer *r, const char *file)
{
	/* remove the theme already associated with the widget
	 * and set this, get the correct escen_ender and set
	 * the current state
	 */

}

EAPI void * eon_widget_data_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

	e = _eon_widget_get(r);
	if (!e) return NULL;

	return e->data;
}
