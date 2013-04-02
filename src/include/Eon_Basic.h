#ifndef _EON_BASIC_H_
#define _EON_BASIC_H_

#include "Enesim.h"

/* checkbox */
EAPI Enesim_Renderer * eon_basic_checkbox_new(void);
EAPI void eon_basic_checkbox_size_set(Enesim_Renderer *r, unsigned int size);
EAPI void eon_basic_checkbox_selected_set(Enesim_Renderer *r, Eina_Bool selected);

/* controls */
typedef struct _Eon_Basic_Bar Eon_Basic_Bar;
Eon_Basic_Bar * eon_basic_bar_new(void);
void eon_basic_bar_size_set(Eon_Basic_Bar *thiz, double size, Eon_Orientation orientation);
double eon_basic_bar_max_size_get(Eon_Basic_Bar *thiz);
double eon_basic_bar_min_size_get(Eon_Basic_Bar *thiz);
void eon_basic_bar_position_set(Eon_Basic_Bar *thiz, double x, double y, Eon_Orientation orientation);
void eon_basic_bar_free(Eon_Basic_Bar *thiz);
Enesim_Renderer * eon_basic_bar_renderer_get(Eon_Basic_Bar *thiz);

#endif
