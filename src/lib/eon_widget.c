#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Widget
{
	void *data;
} Eon_Widget;

static inline Eon_Widget * _eon_widget_get(Enesim_Renderer *r)
{
	Eon_Widget *e;

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
EAPI Enesim_Renderer * eon_widget_new(const char *name, void *data)
{
	Eon_Widget *e;
	Enesim_Renderer *thiz;
	Enesim_Renderer_Descriptor descriptor;

	e = calloc(1, sizeof(Eon_Widget));
	e->data = data;

#if 0
	thiz = enesim_renderer_new(descriptor, flags, e);
	if (!thiz) goto renderer_err;
#endif
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
