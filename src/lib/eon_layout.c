#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eon_Layout
{
	Eina_Tiler *tiler;
	Eina_List *obscure;
	Eina_List *damage;
	void *data;
} Eon_Layout;

static inline Eon_Layout * _eon_layout_get(Enesim_Renderer *r)
{
	Eon_Layout *e;

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
EAPI Enesim_Renderer * eon_layout_new(Enesim_Renderer_Descriptor *descriptor,
		Enesim_Renderer_Flag flags,
		void *data)
{
	Eon_Layout *e;
	Enesim_Renderer *thiz;

	e = calloc(1, sizeof(Eon_Layout));
	e->data = data;

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
void * eon_layout_data_get(Enesim_Renderer *r)
{
	Eon_Layout *l;

	l = _eon_layout_get(r);
	return l->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void eon_layout_obscure_add(Enesim_Renderer *r, Eina_Rectangle *obscure)
{
	Eon_Layout *l;

	l = _eon_layout_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void eon_layout_damage_add(Enesim_Renderer *r, Eina_Rectangle *damage)
{
	Eon_Layout *l;

	l = _eon_layout_get(r);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
void eon_layout_render(Enesim_Renderer *r, Enesim_Surface *dst)
{
	/* iterate over the list of damages */
	/* remove the obscures */
	/* draw the layout */
	/* cleanup the damages */
}
