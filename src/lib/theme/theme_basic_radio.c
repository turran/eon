#include "Eon_Basic.h"
#include "eon_basic_private.h"

typedef struct _Radio
{
	Enesim_Renderer *compound;
	Enesim_Renderer_Sw_Fill fill;
} Radio;

static inline Radio * _radio_get(Enesim_Renderer *r)
{
	Radio *rad;

	rad = enesim_renderer_data_get(r);
	return rad;
}

static void _radio_draw(Enesim_Renderer *r, int x, int y, unsigned int len, uint32_t *dst)
{
	Radio *rad;

	rad = _radio_get(r);
	rad->fill(rad->compound, x, y, len, dst);
}

static Eina_Bool _radio_setup(Enesim_Renderer *r, Enesim_Renderer_Sw_Fill *fill)
{
	Radio *rad;

	rad = _radio_get(r);
	rad->fill = enesim_renderer_sw_fill_get(rad->compound);
	if (!rad->fill) return EINA_FALSE;

	*fill = _radio_draw;

	return EINA_TRUE;
}

static void _radio_cleanup(Enesim_Renderer *r)
{
	Radio *rad;

	rad = _radio_get(r);
	enesim_renderer_sw_cleanup(rad->compound);
}

static void _radio_free(Enesim_Renderer *r)
{
	Radio *rad;

	rad = _radio_get(r);
	if (rad->compound)
		enesim_renderer_delete(rad->compound);
	free(rad);
}

static Enesim_Renderer_Descriptor _descriptor = {
	.sw_setup = _radio_setup,
	.sw_cleanup = _radio_cleanup,
	.free = _radio_free,
};


EAPI Enesim_Renderer * eon_basic_radio_new(void)
{
	Enesim_Renderer *thiz, *r;
	Enesim_Renderer_Flag flags;
	Radio *rad;

	rad = calloc(1, sizeof(Radio));
	if (!rad) return NULL;

	r = enesim_renderer_compound_new();
	if (!r) goto compound_err;
	rad->compound = r;

	thiz = enesim_renderer_new(&_descriptor, flags, rad);
	if (!thiz) goto renderer_err;

	return thiz;
renderer_err:
	enesim_renderer_delete(rad->compound);
compound_err:
	free(rad);
	return NULL;
}
