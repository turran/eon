#ifndef _EON_BASIC_H_
#define _EON_BASIC_H_

#include "Enesim.h"
#include "Etex.h"

/* checkbox */
EAPI Enesim_Renderer * eon_basic_checkbox_new(void);
EAPI void eon_basic_checkbox_size_set(Enesim_Renderer *r, unsigned int size);
EAPI void eon_basic_checkbox_selected_set(Enesim_Renderer *r, Eina_Bool selected);

#endif
