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

	e = enesim_renderer_data_get(r);
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
