#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Button
{
	char *text;
	Ender *external;
} Eon_Button;

static inline Eon_Button * _eon_button_get(Enesim_Renderer *r)
{
	Eon_Button *e;

	e = eon_widget_data_getr(r);
	return e;
}
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
EAPI Enesim_Renderer * eon_button_new(void)
{
	Eon_Button *e;
	Enesim_Renderer *thiz;

	e = calloc(1, sizeof(Eon_Button));
	if (!e) return NULL;

	thiz = eon_widget_new("button", e);
	if (!thiz) goto renderer_err;

	return thiz;

renderer_err:
	free(e);
	return NULL;
}

EAPI void eon_button_text_set(Enesim_Renderer *r, const char *text)
{
	Eon_Button *button;

	button = eon_button_get(r);
	if (!button) return;

	/* we should set the property to the escen */
}

EAPI void eon_button_image_set(Enesim_Renderer *r, const char *file)
{
	Eon_Button *button;

	button = eon_button_get(r);
	if (!button) return;

	/* we should set the property to the escen */
}

